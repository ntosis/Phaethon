  #ifndef RTW_HEADER_Ctrl_Subsystem_h_
  #define RTW_HEADER_Ctrl_Subsystem_h_

#include <stddef.h>
    #ifndef Ctrl_Subsystem_COMMON_INCLUDES_
  # define Ctrl_Subsystem_COMMON_INCLUDES_
    #include "rtwtypes.h"
  #endif /* Ctrl_Subsystem_COMMON_INCLUDES_ */
  

    #include "Ctrl_Subsystem_types.h"    
	#include "pid.h"




    /* Macros for accessing real-time model data structure */
    

    #ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm) ((void*) 0)
#endif


  
   
    
      






    
          /* Block states (auto storage) for system '<Root>' */
              
        
  
     typedef struct  {
  
            
                    
          
                int32_T Filter_DSTATE; /* '<S6>/Filter' */
                          
        

          
          
                    
          
                int16_T Integrator_DSTATE; /* '<S6>/Integrator' */
                          
        

          

  
    } DW_Ctrl_Subsystem;
  

      
        




          
        /* Block states (auto storage) */
                extern DW_Ctrl_Subsystem Ctrl_Subsystem_DW;



          
      /*
      * Exported Global Signals
      *
      * Note: Exported global signals are block signals with an exported global
      * storage class designation.  Code generation will declare the memory for 
      * these signals and export their symbols.
      *
      */
                
    
                  extern  boolean_T stateOfProgram; /* '<S5>/XOR1' */

    
          
    
                  extern  boolean_T selectProgram; /* '<S2>/XOR1' */

    
          
    
                  extern  boolean_T stateOfRelay; /* '<S1>/Logical Operator' */

    
    
    
 



      /* Model entry point functions */
            extern void Ctrl_Subsystem_initialize(void);
                        extern void Ctrl_Subsystem_step(void);
            













  
    /*-
     * These blocks were eliminated from the model due to optimizations:
     *
     * Block '<S5>/Scope' : Unused code path elimination
     */
      
  /*-
   * The generated code includes comments that allow you to trace directly 
   * back to the appropriate location in the model.  The basic format
   * is <system>/block_name, where system is the system number (uniquely
   * assigned by Simulink) and block_name is the name of the block.
   *
   * Note that this particular code originates from a subsystem build,
   * and has its own system numbers different from the parent model.
   * Refer to the system hierarchy for this subsystem below, and use the
   * MATLAB hilite_system command to trace the generated code back
   * to the parent model.  For example,
   *
   * hilite_system('training2/Ctrl_Subsystem')    - opens subsystem training2/Ctrl_Subsystem
   * hilite_system('training2/Ctrl_Subsystem/Kp') - opens and selects block Kp
   *
   * Here is the system hierarchy for this model
   *
    * '<Root>' : 'training2'
      * '<S1>'   : 'training2/Ctrl_Subsystem'
      * '<S2>'   : 'training2/Ctrl_Subsystem/Auto//Man selection '
      * '<S3>'   : 'training2/Ctrl_Subsystem/If Action Auto program'
      * '<S4>'   : 'training2/Ctrl_Subsystem/If Action Manual program'
      * '<S5>'   : 'training2/Ctrl_Subsystem/On//Off'
      * '<S6>'   : 'training2/Ctrl_Subsystem/If Action Auto program/PID Controller'
   */



  #endif /* RTW_HEADER_Ctrl_Subsystem_h_ */

/*======================== TOOL VERSION INFORMATION ==========================*
 * MATLAB 8.3 (R2014a)27-Dec-2013                                             *
 * Simulink 8.3 (R2014a)27-Dec-2013                                           *
 * Simulink Coder 8.6 (R2014a)27-Dec-2013                                     *
 * Embedded Coder 6.6 (R2014a)27-Dec-2013                                     *
 * Stateflow 8.3 (R2014a)27-Dec-2013                                          *
 * Fixed-Point Designer 4.2 (R2014a)27-Dec-2013                               *
 *============================================================================*/

/*======================= LICENSE IN USE INFORMATION =========================*
 * matlab                                                                     *
 * matlab_coder                                                               *
 * real-time_workshop                                                         *
 * rtw_embedded_coder                                                         *
 * simulink                                                                   *
 *============================================================================*/
