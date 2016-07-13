#include "Ctrl_Subsystem.h"

/* Exported block signals */
boolean_T isCoolingOn;                 /* '<S5>/Compare' */
boolean_T selectProgram;               /* '<S2>/XOR1' */
boolean_T stateOfProgram;              /* '<S4>/XOR1' */
boolean_T isHeatingOn;                 /* '<S3>/NOT' */
int8_t SOLLtemperature = 22;
int16_t inputValue_Htng;
int16_t inputValue_Coolg;
struct PID_DATA pidData_Htng;
struct PID_DATA pidData_Coolg;
/* Exported data definition */

/* Const memory section */
/* Definition for custom storage class: Const */
const boolean_T setOffSecondLvlinManual = 0;

/* Block states (auto storage) */
DW_Ctrl_Subsystem Ctrl_Subsystem_DW;

/* External outputs (root outports fed by signals with auto storage) */
ExtY_Ctrl_Subsystem Ctrl_Subsystem_Y;

/*
 * Output and update for action system:
 *    '<S19>/Heating PID 2 levels'
 *    '<S11>/Coooling PID 2 levels'
 */
void Ctrl_Subsystem_HeatingPID2levels(int16_t rtu_In1, boolean_T *rty_Out1,
  boolean_T *rty_Out2)
{
  /* Logic: '<S22>/time AND1' incorporates:
   *  Constant: '<S22>/reference'
   *  Constant: '<S22>/reference1'
   *  RelationalOperator: '<S22>/Relational Operator'
   *  RelationalOperator: '<S22>/Relational Operator1'
   */
  *rty_Out1 = ((rtu_In1 < (rtcP_reference_Value << 13)) && (rtu_In1 >=
    (rtcP_reference1_Value << 13)));

  /* Logic: '<S22>/time AND2' incorporates:
   *  Constant: '<S22>/reference2'
   *  Constant: '<S22>/reference3'
   *  RelationalOperator: '<S22>/Relational Operator2'
   *  RelationalOperator: '<S22>/Relational Operator3'
   */
  *rty_Out2 = ((rtu_In1 < (rtcP_reference2_Value << 13)) && (rtu_In1 <
    (rtcP_reference3_Value << 13)));
}

/*
 * Output and update for action system:
 *    '<S19>/Heating PID 1 level'
 *    '<S11>/Cooling PID 1 level'
 */
void Ctrl_Subsystem_HeatingPID1level(int16_t rtu_In1, boolean_T *rty_Out1,
  boolean_T *rty_Out2)
{
  /* RelationalOperator: '<S23>/Compare' incorporates:
   *  Constant: '<S23>/Constant'
   */
  *rty_Out1 = (rtu_In1 < rtcP_Constant_Value);

  /* Constant: '<S21>/Constant' */
  *rty_Out2 = rtcP_Constant_Value_ldvs;
}

/* Model step function */
void Ctrl_Subsystem_step(void)
{
  /* local block i/o variables */
  boolean_T rtb_Merge1_ev30;
  boolean_T rtb_Merge_jq0e;
  boolean_T rtb_Merge1_hmtq;
  boolean_T rtb_Merge_a1e2;
  boolean_T rtb_Compare;
  int8_T rtb_Solltemperature_intern;
  boolean_T rtb_Merge;
  boolean_T rtb_Merge1;
  int32_T rtb_FilterCoefficient;

  /* RelationalOperator: '<S5>/Compare' incorporates:
   *  Constant: '<S5>/Constant'
   *  Inport: '<Root>/In2'
   */
  isCoolingOn = (SOLLtemperature <= rtcP_Constant_Value_psko);

  /* Logic: '<S2>/XOR1' incorporates:
   *  Inport: '<Root>/In3'
   *  UnitDelay: '<S2>/Unit Delay1'
   */
  selectProgram = doubleClicked ^ Ctrl_Subsystem_DW.UnitDelay1_DSTATE;

  /* Logic: '<S4>/XOR1' incorporates:
   *  Inport: '<Root>/In4'
   *  UnitDelay: '<S4>/Unit Delay1'
   */
  stateOfProgram = clicked ^ Ctrl_Subsystem_DW.UnitDelay1_DSTATE_fbw3;

  /* If: '<S3>/If' */
  if (!isCoolingOn) {
    /* Outputs for IfAction SubSystem: '<S3>/Heating' incorporates:
     *  ActionPort: '<S7>/Heating'
     */
    /* RelationalOperator: '<S16>/Compare' incorporates:
     *  Constant: '<S16>/Constant'
     *  Inport: '<Root>/In2'
     */
    rtb_Compare = (SOLLtemperature > rtcP_Constant_Value_km13);

    /* Saturate: '<S7>/Saturation' incorporates:
     *  Inport: '<Root>/In2'
     */
    if (SOLLtemperature > rtcP_Saturation_UpperSat) {
      rtb_Solltemperature_intern = rtcP_Saturation_UpperSat;
    } else if (SOLLtemperature < rtcP_Saturation_LowerSat) {
      rtb_Solltemperature_intern = rtcP_Saturation_LowerSat;
    } else {
      rtb_Solltemperature_intern = SOLLtemperature;
    }

    /* End of Saturate: '<S7>/Saturation' */

    /* If: '<S7>/If' incorporates:
     *  Constant: '<S18>/Constant'
     */
    if (!selectProgram) {
      /* Outputs for IfAction SubSystem: '<S7>/If Action Manual program' incorporates:
       *  ActionPort: '<S18>/Manual Subsystem'
       */
      /* RelationalOperator: '<S18>/Relational Operator' incorporates:
       *  Inport: '<Root>/In1'
       */
      rtb_Merge = (Temperature < rtb_Solltemperature_intern);
      rtb_Merge1 = setOffSecondLvlinManual;

      /* End of Outputs for SubSystem: '<S7>/If Action Manual program' */
    } else {
      /* Outputs for IfAction SubSystem: '<S7>/If Action Auto program' incorporates:
       *  ActionPort: '<S17>/Auto programm subsystem'*/

      inputValue_Htng = pid_Controller(rtb_Solltemperature_intern, Temperature, &pidData_Htng);

      /* If: '<S19>/If' incorporates:
       *  Constant: '<S17>/Constant1'
       */
      if (!oneLevelSystem_C) {
        /* Outputs for IfAction SubSystem: '<S19>/Heating PID 2 levels' incorporates:
         *  ActionPort: '<S22>/Action Port'
         */
        Ctrl_Subsystem_HeatingPID2levels(inputValue_Htng,
          &rtb_Merge_a1e2, &rtb_Merge1_hmtq);

        /* End of Outputs for SubSystem: '<S19>/Heating PID 2 levels' */
      } else {
        /* Outputs for IfAction SubSystem: '<S19>/Heating PID 1 level' incorporates:
         *  ActionPort: '<S21>/Action Port'
         */
        Ctrl_Subsystem_HeatingPID1level(inputValue_Htng,
          &rtb_Merge_a1e2, &rtb_Merge1_hmtq);

        /* End of Outputs for SubSystem: '<S19>/Heating PID 1 level' */
      }

      /* End of If: '<S19>/If' */

      /* Logic: '<S19>/time AND' incorporates:
       *  Inport: '<Root>/In5'
       */
      rtb_Merge1 = (rtb_Merge1_hmtq && autoProgramTimeEnabled);

      /* Logic: '<S19>/time AND1' incorporates:
       *  Inport: '<Root>/In5'
       */
      rtb_Merge = (rtb_Merge_a1e2 && autoProgramTimeEnabled);
      /* End of Outputs for SubSystem: '<S7>/If Action Auto program' */
    }

    /* End of If: '<S7>/If' */

    /* Outport: '<Root>/Out2' incorporates:
     *  Logic: '<S7>/Logical Operator1'
     *  Logic: '<S7>/Logical Operator2'
     */
    Ctrl_Subsystem_Y.Out2 = (rtb_Compare && (stateOfProgram && rtb_Merge1));

    /* Outport: '<Root>/Out1' incorporates:
     *  Logic: '<S7>/Logical Operator'
     *  Logic: '<S7>/Logical Operator3'
     */
    Ctrl_Subsystem_Y.Out1 = ((rtb_Merge && stateOfProgram) && rtb_Compare);

    /* End of Outputs for SubSystem: '<S3>/Heating' */
  } else {
    /* Outputs for IfAction SubSystem: '<S3>/Cooling' incorporates:
     *  ActionPort: '<S6>/Cooling'
     */
    /* RelationalOperator: '<S8>/Compare' incorporates:
     *  Constant: '<S8>/Constant'
     *  Inport: '<Root>/In2'
     */
    rtb_Compare = (SOLLtemperature <= rtcP_Constant_Value_dj10);

    /* Gain: '<S6>/Gain' incorporates:
     *  Inport: '<Root>/In2'
     */
    rtb_Solltemperature_intern = (int8_T)(rtcP_Gain_Gain * SOLLtemperature);

    /* Saturate: '<S6>/Saturation' */
    if (rtb_Solltemperature_intern > rtcP_Saturation_UpperSat_gwlq) {
      rtb_Solltemperature_intern = rtcP_Saturation_UpperSat_gwlq;
    } else {
      if (rtb_Solltemperature_intern < rtcP_Saturation_LowerSat_hjhp) {
        rtb_Solltemperature_intern = rtcP_Saturation_LowerSat_hjhp;
      }
    }

    /* End of Saturate: '<S6>/Saturation' */

    /* If: '<S6>/If' incorporates:
     *  Constant: '<S10>/Constant'
     */
    if (!selectProgram) {
      /* Outputs for IfAction SubSystem: '<S6>/If Action Manual program' incorporates:
       *  ActionPort: '<S10>/Manual Subsystem'
       */
      /* RelationalOperator: '<S10>/Relational Operator' incorporates:
       *  Inport: '<Root>/In1'
       */
      rtb_Merge = (Temperature > rtb_Solltemperature_intern);
      rtb_Merge1 = setOffSecondLvlinManual;

      /* End of Outputs for SubSystem: '<S6>/If Action Manual program' */
    } else {
      /* Outputs for IfAction SubSystem: '<S6>/If Action Auto program' incorporates:
       *  ActionPort: '<S9>/Auto programm subsystem'
       */
        inputValue_Coolg = pid_Controller(rtb_Solltemperature_intern, Temperature, &pidData_Coolg);


      /* If: '<S11>/If' incorporates:
       *  Constant: '<S9>/Constant1'
       */
      if (!oneLevelSystem_C) {
        /* Outputs for IfAction SubSystem: '<S11>/Coooling PID 2 levels' incorporates:
         *  ActionPort: '<S14>/Action Port'
         */
        Ctrl_Subsystem_HeatingPID2levels(inputValue_Coolg, &rtb_Merge_jq0e,
          &rtb_Merge1_ev30);

        /* End of Outputs for SubSystem: '<S11>/Coooling PID 2 levels' */
      } else {
        /* Outputs for IfAction SubSystem: '<S11>/Cooling PID 1 level' incorporates:
         *  ActionPort: '<S13>/Action Port'
         */
        Ctrl_Subsystem_HeatingPID1level(inputValue_Coolg, &rtb_Merge_jq0e,
          &rtb_Merge1_ev30);

        /* End of Outputs for SubSystem: '<S11>/Cooling PID 1 level' */
      }

      /* End of If: '<S11>/If' */

      /* Logic: '<S11>/time AND' */
      rtb_Merge1 = (rtb_Merge1_ev30 && stateOfProgram);

      /* Logic: '<S11>/time AND1' */
      rtb_Merge = (rtb_Merge_jq0e && stateOfProgram);

      /* End of Outputs for SubSystem: '<S6>/If Action Auto program' */
    }

    /* End of If: '<S6>/If' */

    /* Outport: '<Root>/Out3' incorporates:
     *  Inport: '<Root>/In5'
     *  Logic: '<S6>/Logical Operator'
     *  Logic: '<S6>/Logical Operator2'
     */
    Ctrl_Subsystem_Y.Out3 = ((rtb_Merge && autoProgramTimeEnabled) &&
      rtb_Compare);

    /* Outport: '<Root>/Out4' incorporates:
     *  Inport: '<Root>/In5'
     *  Logic: '<S6>/Logical Operator1'
     *  Logic: '<S6>/Logical Operator3'
     */
    Ctrl_Subsystem_Y.Out4 = (rtb_Compare && (autoProgramTimeEnabled &&
      rtb_Merge1));

    /* End of Outputs for SubSystem: '<S3>/Cooling' */
  }

  /* End of If: '<S3>/If' */

  /* Logic: '<S3>/NOT' */
  isHeatingOn = !isCoolingOn;

  /* Update for UnitDelay: '<S2>/Unit Delay1' */
  Ctrl_Subsystem_DW.UnitDelay1_DSTATE = selectProgram;

  /* Update for UnitDelay: '<S4>/Unit Delay1' */
  Ctrl_Subsystem_DW.UnitDelay1_DSTATE_fbw3 = stateOfProgram;
}

