  #ifndef RTW_HEADER_Ctrl_Subsystem_private_h_
  #define RTW_HEADER_Ctrl_Subsystem_private_h_
  #include "rtwtypes.h"

#ifndef UCHAR_MAX
#include <limits.h>
#endif

#if ( UCHAR_MAX != (0xFFU) ) || ( SCHAR_MAX != (0x7F) )
#error "Code was generated for compiler with different sized uchar/char. Consider adjusting Emulation Hardware word size settings on the Hardware Implementation pane to match your compiler word sizes as defined in the compiler's limits.h header file. Alternatively, you can select 'None' for Emulation Hardware and select the 'Enable portable word sizes' option for ERT based targets, which will disable the preprocessor word size checks."
#endif

#if ( USHRT_MAX != (0xFFFFU) ) || ( SHRT_MAX != (0x7FFF) )
#error "Code was generated for compiler with different sized ushort/short. Consider adjusting Emulation Hardware word size settings on the Hardware Implementation pane to match your compiler word sizes as defined in the compilers limits.h header file. Alternatively, you can select 'None' for Emulation Hardware and select the 'Enable portable word sizes' option for ERT based targets, this will disable the preprocessor word size checks."
#endif

#if ( UINT_MAX != (0xFFFFFFFFU) ) || ( INT_MAX != (0x7FFFFFFF) )
//#error "Code was generated for compiler with different sized uint/int. Consider adjusting Emulation Hardware word size settings on the Hardware Implementation pane to match your compiler word sizes as defined in the compilers limits.h header file. Alternatively, you can select 'None' for Emulation Hardware and select the 'Enable portable word sizes' option for ERT based targets, this will disable the preprocessor word size checks."
#endif

#if ( ULONG_MAX != (0xFFFFFFFFU) ) || ( LONG_MAX != (0x7FFFFFFF) )
#error "Code was generated for compiler with different sized ulong/long. Consider adjusting Emulation Hardware word size settings on the Hardware Implementation pane to match your compiler word sizes as defined in the compilers limits.h header file. Alternatively, you can select 'None' for Emulation Hardware and select the 'Enable portable word sizes' option for ERT based targets, this will disable the preprocessor word size checks."
#endif


#ifndef __RTWTYPES_H__
  #error This file requires rtwtypes.h to be included
  #endif /* __RTWTYPES_H__ */

/* Expression: 0
* Referenced by: '<S3>/reference'
*/
                #define rtcP_reference_Value (0.0)
            
/* Imported (extern) block signals */
extern  uint8_T Temperature; /* '<Root>/In1' */
extern  uint8_T SollTemperature; /* '<Root>/In2' */
extern  boolean_T doubleClicked; /* '<Root>/In3' */
extern  boolean_T clicked; /* '<Root>/In4' */
extern  boolean_T autoProgramTimeEnabled; /* '<Root>/In5' */


#endif /* RTW_HEADER_Ctrl_Subsystem_private_h_ */

