#include <Wire.h>
#include <Time.h>
#include <DS1307RTC.h>
#include <SPI.h>
#include <TM1637Display.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/power.h>

extern "C" {
#include <pid.h>
#include <stdint.h>
}

#define CLK 7
#define DIO 8
#define RED_LED A1
#define FAST_RED_LED C,1
#define FAST_GREEN_LED C,2
#define BLUE_LED A0
#define GREEN_LED A2
// MACROS FOR EASY PIN HANDLING FOR ATMEL GCC-AVR
//these macros are used indirectly by other macros , mainly for string concatination
#define _SET(type,name,bit)          type ## name  |= _BV(bit)    
#define _CLEAR(type,name,bit)        type ## name  &= ~ _BV(bit)        
#define _TOGGLE(type,name,bit)       type ## name  ^= _BV(bit)    
#define _GET(type,name,bit)          ((type ## name >> bit) &  1)
#define _PUT(type,name,bit,value)    type ## name = ( type ## name & ( ~ _BV(bit)) ) | ( ( 1 & (unsigned char)value ) << bit )

//these macros are used by end user
#define FAST_OUTPUT(pin)         _SET(DDR,pin)    
#define FAST_INPUT(pin)          _CLEAR(DDR,pin)    
#define FAST_HIGH(pin)           _SET(PORT,pin)
#define FAST_LOW(pin)            _CLEAR(PORT,pin)    
#define FAST_TOGGLE(pin)         _TOGGLE(PORT,pin)    
#define FAST_READ(pin)           _GET(PIN,pin)

/*! \brief P, I and D parameter values
*
* The K_P, K_I and K_D values (P, I and D gains)
* need to be modified to adapt to the application at hand
*/
#define   INT16_MAX   0x7fff
 
#define   INT16_MIN   (-INT16_MAX - 1)
//! \xrefitem todo "Todo" "Todo list"
#define K_P     10
//! \xrefitem todo "Todo" "Todo list"
#define K_I     0.5
//! \xrefitem todo "Todo" "Todo list"
#define K_D     2
/*! \brief Flags for status information */
boolean manualProgramDelay=false;
// Time struct
tmElements_t dateStruct;
struct GLOBAL_FLAGS {
  //! True when PID control loop should run one time
  uint8_t pidTimer:1;
  uint8_t dummy:7;
} gFlags = {0, 0};
//! Parameters for regulator
struct PID_DATA pidData;
#define TIME_INTERVAL   1000U
static uint16_t i_t = 0;
  static uint16_t j_t = 0;
/*! \brief Timer interrupt to control the sampling interval
 */
ISR(TIMER2_OVF_vect) {
  TCNT2 =6;
   
  if(i_t < TIME_INTERVAL)

    i_t++;

  else{
    
    gFlags.pidTimer = TRUE;

    i_t = 0;

  }
  
  if(j_t < 60000U)

    j_t++;

  else{
    
     manualProgramDelay= TRUE;

    j_t = 0;

  }

}
// Module connection pins (Digital Pins)
TM1637Display display(CLK, DIO);
// the Button
const int PinSW = 3;
volatile int pnt=0;
//Summer Time factor
uint8_t summerTime =0;
typedef struct {
long int timeOfClick=0;
boolean clicked=false;
} click;

static click *ArrayOfClicks;
static boolean clicked=false;
static boolean dualclicked=false;
//Relay
const int relayPin =6;
//SPI tempSensor pin to select the device
const int chipSelectPin = 5;
static int SOLLtemperature = 22;
static int PreviousTemperature = 0;
static boolean stateOfHeatingSystem = false;
// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
static unsigned long previousMillis = 0;
static long int prvsConvertedTime = 0;

// onOff state
boolean onStateofProgram=false;
// Auto Programm
boolean autoProgramState = false;
static long int prvsLEDms=0;
//Rotary function
volatile boolean TurnDetected;
volatile boolean up;
const int PinCLK=2;                   // Used for generating interrupts using CLK signal
const int PinDT=4;                    // Used for reading DT signal
 // A as array
  const uint8_t SEG_0A[] = {
  SEG_A|SEG_B|SEG_C|SEG_G|SEG_E|SEG_F   // A
  };
// On/Off time Array
// Sunday->Saturday ontime/offtime
  const uint16_t onTimesAM[7] PROGMEM = 
  //ontime
  {845,615,615,615,615,615,845};
  const uint16_t offTimesAM[7] PROGMEM = 
  //offtime
  {1200,705,705,705,705,705,1200};
  
  const uint16_t onTimesPM[7] PROGMEM = 
  //ontime
  {1300,1630,1630,1630,1630,1630,1400} ;
  
  const uint16_t offTimesPM[7] PROGMEM = 
  //offtime
  {2130,2230,2230,2230,2230,2330,2200};

void isr ()  {                    // Interrupt service routine is executed when a HIGH to LOW transition is detected on CLK
  if (digitalRead(PinCLK))
    up = digitalRead(PinDT);
  else
    up = !digitalRead(PinDT);
  TurnDetected = true;
}
void isr1 ()  {   
  
    //Serial.println(millis());      // Using the word RESET instead of COUNT here to find out a buggy encoder
    
    (ArrayOfClicks+pnt)->timeOfClick=millis();
    (ArrayOfClicks+pnt)->clicked=true;
     pnt++;
         // Using the word RESET instead of COUNT here to find out a buggy encoder
    if(pnt>5) pnt=0;
}

uint16_t referenceValue, measurementValue, inputValue;

void setup() {
    
  Init();
  sei();
  ArrayOfClicks = (click*)malloc(5*sizeof(click)); 
  
  pinMode(PinSW,INPUT);
  digitalWrite(PinSW, LOW);  // Turn off internal Pull-Up Resistor
  attachInterrupt (0,isr,FALLING);   // interrupt 0 is always connected to pin 2 on Arduino UNO
  attachInterrupt (1,isr1,FALLING);   // interrupt 0 is always connected to pin 2 on Arduino UNO


  Serial.begin(9600);
    // start the SPI library:
  SPI.begin();
    // initalize the  data ready and chip select pins:
  pinMode(chipSelectPin, OUTPUT);  
  display.setBrightness(0x0f);

  //LED configure
  pinMode(RED_LED,OUTPUT);
  pinMode(GREEN_LED,OUTPUT);
  //FAST_OUTPUT(FAST_GREEN_LED);
  //Relay gonfigure
  pinMode(relayPin,OUTPUT);
  clearDisplay(0,2); 
  //Set REDLED
  if(!onStateofProgram)  {
    digitalWrite(relayPin,HIGH); 
    digitalWrite(RED_LED,HIGH);
  }  
  
  // init the date
  tmElements_t temp = checkTheDate();
  prvsConvertedTime = temp.Hour*100 + temp.Minute;
  
 
}

void loop() {
    
    checkStruct();
    if (TurnDetected)  updateSollTemperature();       // do this only if rotation was detected
    checkIfButtonIsPressed();
    unsigned long currentMillis = millis();
    setOnOffState(onStateofProgram);
     if(onStateofProgram && dualclicked) SetAutoProgram();
      
    if(autoProgramState) {if((currentMillis-prvsLEDms)>300) { FAST_TOGGLE(FAST_GREEN_LED); prvsLEDms=currentMillis;}}
      
   
    long int dt = currentMillis - previousMillis;
    //Serial.println(dt);
    if (dt < 600) {
    // Update the room Temperature every 3 seconds.
    displayTheActualTemperature();
    
    }
    else if(4800<dt&&dt<5200) {
    
    tmElements_t temp = checkTheDate();
    clearDisplay(2,2);
    display.showNumberDec(temp.Hour, false, 2,0);
      if(temp.Minute<10) {
      display.showNumberDec(0, false, 1,2);
      display.showNumberDec(temp.Minute, false, 1,3);
      }
      else{
      display.showNumberDec(temp.Minute, false, 2,2);
      }  

    }
    else if(9800<dt&&dt<10200) {
        if(autoProgramState){    
            clearDisplay(0,4);
            display.setSegments(SEG_0A,1,1);
        }    
    }
    //reset the Period
    if(dt>12000) {
    previousMillis = currentMillis;
    }
    
    delay(500);
       
}

  tmElements_t checkTheDate() {
  tmElements_t temp;
  
  if(RTC.read(temp)) {
    //Check summertime or windertime.
  if((temp.Month==10)&&(temp.Wday==1)&&(temp.Hour==3)&&(temp.Minute==00))
  {
     if((31-temp.Day)<7) {summerTime=0; }
  }
   else if((temp.Month==3)&&(temp.Wday==1)&&(temp.Hour==2)&&(temp.Minute==00))
  {
     if((31-temp.Day)<7) {summerTime=1; }
  }
   temp.Hour=temp.Hour + summerTime; 
   return temp;
   
  } 
 else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
  
 }

}

float actualTemperature() {
  
    float RoomTemperature,ADC_Temp_Code_dec;
    int TempVal =0;
    byte LSB =0 ,MSB=0;

    digitalWrite(chipSelectPin,LOW);
    delayMicroseconds(10);
    //Read the temperature data
    MSB =  SPI.transfer(0x00);
    LSB =  SPI.transfer(0x00);
    delayMicroseconds(10);
    digitalWrite(chipSelectPin,HIGH);


    // convert the temperature to celsius and display it:
    TempVal = (MSB << 8) + LSB;
    ADC_Temp_Code_dec = (float)TempVal ;
    return (RoomTemperature = TempVal / 32.0);
}

void displayTheActualTemperature() {
  
  clearDisplay(0,4);
  int T;
  T = round(actualTemperature());
  display.showNumberDec(T, false, 2, 2);

}

void clearDisplay(int start,int length) {
  
  uint8_t clearDisplay[] = { 0x00, 0x00, 0x00, 0x00 };
  display.setSegments(clearDisplay, length, start);
}

boolean checkIfButtonIsPressed() {
  
  if(clicked) 
  {    
   if(!onStateofProgram) { 
   onStateofProgram=true;                
   return true;
      } 
   else {
   onStateofProgram=false;                
   return false;
      }     
   }
}

void setOnOffState(boolean a) {
if(a){
      // ON      
      // autoProgramm
        if(autoProgramState) {        
        // call the Auto Program
        autoProgram();
        }
      // manual Program
        else
        {
          FAST_HIGH(FAST_GREEN_LED);
          FAST_LOW(FAST_RED_LED);
              int tempTemperature = round(actualTemperature());
              if(manualProgramDelay) {
                if((SOLLtemperature - tempTemperature)>0) { setStateOfHeatingSystem(true); }
                else { setStateOfHeatingSystem(false); }
                manualProgramDelay= FALSE;
              }
              
         }
    }
else {
      //OFF
        FAST_LOW(FAST_GREEN_LED); //set LOW
        FAST_HIGH(FAST_RED_LED); //set HIGH
        setStateOfHeatingSystem(false);
        autoProgramState=false;
               
      }
}


boolean SetAutoProgram() {
  
          if(!autoProgramState) { 
                autoProgramState=true;                
                return true;
            } else {
                autoProgramState=false;                
                return false;
              } 
}

boolean autoProgram() {
  tmElements_t temp = checkTheDate();
  
  
  long int convertedTime= temp.Hour*100 +temp.Minute;
  uint16_t onTimeAM= pgm_read_word_near(onTimesAM + (temp.Wday-1));
  uint16_t offTimeAM= pgm_read_word_near(offTimesAM + (temp.Wday-1));
  uint16_t onTimePM= pgm_read_word_near(onTimesPM + (temp.Wday-1));
  uint16_t offTimePM= pgm_read_word_near(offTimesPM + (temp.Wday-1));
  
  if((onTimeAM<convertedTime&&convertedTime<offTimeAM)||(onTimePM<convertedTime&&convertedTime<offTimePM)) {
      smartSystemControl(); 
  }
        
}

void smartSystemControl() {
  
  
 if(gFlags.pidTimer)
    {
      //digitalWrite(GREEN_LED,!digitalRead(GREEN_LED));
      referenceValue = Get_Reference();
      measurementValue = Get_Measurement();
      inputValue = pid_Controller(referenceValue, measurementValue, &pidData);
      Set_Input(inputValue);
      gFlags.pidTimer = FALSE;
     } 
}

void setStateOfHeatingSystem(boolean state) {
      digitalWrite(relayPin,!state);
      digitalWrite(BLUE_LED,state);
      stateOfHeatingSystem = state; 
    
}

void checkStruct() {
  noInterrupts();
 clicked=false;
 dualclicked=false;
  for(int i=0;i<5;i++) {
         if(i<4){
         if((ArrayOfClicks+i)->clicked==true)  {
               
             if((ArrayOfClicks+i+1)->clicked==true) {
               long int dt= ((ArrayOfClicks+i+1)->timeOfClick)-((ArrayOfClicks+i)->timeOfClick);
               if(dt<500) {
                 dualclicked=true;
                 //jump to next incoming signal.
                  i++;
                }
               else clicked=true;
               
             }
             else clicked=true;
         } 
       }
       else if(i==4) { 
         if((ArrayOfClicks+i)->clicked==true)  {
               
             if((ArrayOfClicks)->clicked==true) {
               long int dt= ((ArrayOfClicks)->timeOfClick)-((ArrayOfClicks+i)->timeOfClick);
               if(0<dt<500) {
               dualclicked=true;
               //i++;
             }
               else clicked=true;
               
             }
             else clicked=true;
         } 
       } 
  }
    for(int i=0;i<5;i++) {
      (ArrayOfClicks+i)->clicked=false;
      (ArrayOfClicks+i)->timeOfClick=0;
    }
   interrupts();
}
void updateSollTemperature() {
 
 if(onStateofProgram) {
  if (up)
      SOLLtemperature--;
    else
      SOLLtemperature++;
     if(SOLLtemperature<5) SOLLtemperature=5;
    if(SOLLtemperature>40) SOLLtemperature=40;
    clearDisplay(0,4);
    display.showNumberDec(SOLLtemperature, false, 2, 0);
 }
     TurnDetected = false;    // do NOT repeat IF loop until new rotation detected

}

/*! \brief Read reference value.

 *

 * This function must return the reference value.

 * May be constant or varying

 */

uint16_t Get_Reference()

{

  return SOLLtemperature;

}



/*! \brief Read system process value

 *

 * This function must return the measured data

 */

uint16_t Get_Measurement()

{

  return round(actualTemperature());
}



/*! \brief Set control input to system

 *

 * Set the output from the controller as input

 * to system.

 */

void Set_Input(int16_t inputValue)

{
  if(inputValue<=0) setStateOfHeatingSystem(false);
else if(inputValue>0) setStateOfHeatingSystem(true);

/*Serial.print("PID msg original : ");
Serial.println(inputValue);

Serial.print("PID msg with map: ");
Serial.println(inputValue);
Serial.print("Window Start: ");
Serial.println(PIDwindowStart);
Serial.print("Window Dt: ");
Serial.println(currentMilliss-PIDwindowStart);
Serial.print("State of Heating Sys: ");
Serial.println(stateOfHeatingSystem);
Serial.println("----------------"); */

}

void Init() {

  pid_Init(K_P*SCALING_FACTOR,K_I*SCALING_FACTOR,K_D*SCALING_FACTOR,&pidData);
  // Set up timer, enable timer/counte 2 overflow interrupt
  //Setup Timer2 to fire every 1ms
  TCCR2B = 0x00;        //Disbale Timer2 while we set it up
  TCNT2  = 130;         //Reset Timer Count to 130 out of 255
  TIFR2  = 0x00;        //Timer2 INT Flag Reg: Clear Timer Overflow Flag
  TIMSK2 = 0x01;        //Timer2 INT Reg: Timer2 Overflow Interrupt Enable
  TCCR2A = 0x00;        //Timer2 Control Reg A: Wave Gen Mode normal
  TCCR2B |= (1<<CS21)|(1<<CS20); //Timer2 Control Reg B: Timer Prescaler set to 128

  }
void enterSleep(void)
{  
 
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);
  
  sleep_enable();
  
  sleep_mode();
  
  /* The program will continue from here. */
  
  /* First thing to do is disable sleep. */
  sleep_disable(); 
}

//add some comment for git test
