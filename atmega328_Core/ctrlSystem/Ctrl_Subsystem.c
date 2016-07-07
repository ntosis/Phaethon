#include "Ctrl_Subsystem.h"
#include "Ctrl_Subsystem_private.h"
#include "pid.h"
    /* Exported block signals */
boolean_T stateOfProgram; /* '<S5>/XOR1' */
boolean_T selectProgram; /* '<S2>/XOR1' */
boolean_T stateOfRelay; /* '<S1>/Logical Operator' */
/* Block states (auto storage) */
uint8_t SOLLtemperature = 22;
int16_t inputValue;
struct PID_DATA pidData;
/* Model step function */

void Ctrl_Subsystem_step(void)  {
	boolean_T rtb_Merge;

/* Logic: '<S5>/XOR1' incorporates:
 *  Inport: '<Root>/In4'
 *  UnitDelay: '<S5>/Unit Delay1'
 */
	stateOfProgram = clicked ^ stateOfProgram;
/* Logic: '<S2>/XOR1' incorporates:
 *  Inport: '<Root>/In3'
 *  UnitDelay: '<S2>/Unit Delay1'
 */
	selectProgram = doubleClicked ^ selectProgram;
/* If: '<S1>/If' */
	if (!selectProgram) {
    /* Outputs for IfAction SubSystem: '<S1>/If Action Manual program' incorporates:
 *  ActionPort: '<S4>/Manual Subsystem'
 */
    /* RelationalOperator: '<S4>/Relational Operator' incorporates:
 *  Inport: '<Root>/In1'
 *  Inport: '<Root>/In2'
 */
    rtb_Merge = (Temperature < SOLLtemperature);
    /* End of Outputs for SubSystem: '<S1>/If Action Manual program' */
	} else {
    /* Outputs for IfAction SubSystem: '<S1>/If Action Auto program' incorporates:
 /*/
		inputValue = pid_Controller(SOLLtemperature, Temperature, &pidData);
    // Merge Block
    rtb_Merge = (((inputValue) > rtcP_reference_Value) && autoProgramTimeEnabled);

}
/* End of If: '<S1>/If' */

/* Logic: '<S1>/Logical Operator' */
stateOfRelay = (rtb_Merge && stateOfProgram);      
 }
