// generated using template: cop_main.template---------------------------------------------
/******************************************************************************************
**
**  Module Name: cop_main.c
**  NOTE: Automatically generated file. DO NOT MODIFY!
**  Description:
**            Main file
**
******************************************************************************************/
// generated using template: arm/custom_include.template-----------------------------------

#include "math.h"

// x86 libraries:
#include "../include/sp_functions_dev0.h"

// H files from Advanced C Function components
//#include "example_dll.h"

// Header files from additional sources (Advanced C Function)
// ----------------------------------------------------------------------------------------
// generated using template: VirtualHIL/custom_defines.template----------------------------

typedef unsigned char X_UnInt8;
typedef char X_Int8;
typedef signed short X_Int16;
typedef unsigned short X_UnInt16;
typedef int X_Int32;
typedef unsigned int X_UnInt32;
typedef unsigned int uint;
typedef double real;

// ----------------------------------------------------------------------------------------
// generated using template: custom_consts.template----------------------------------------

// arithmetic constants
#define C_SQRT_2                    1.4142135623730950488016887242097f
#define C_SQRT_3                    1.7320508075688772935274463415059f
#define C_PI                        3.1415926535897932384626433832795f
#define C_E                         2.7182818284590452353602874713527f
#define C_2PI                       6.283185307179586476925286766559f

//@cmp.def.start
//component defines
























































































#define _l4_constant_power_l4_sg_ctrl_sine__no_of_points 1000
#define _l4_constant_power_l4_sg_ctrl_sine__max_int40 (1099511627775)
#define _l4_constant_power_l4_sg_ctrl_sine__max_int32 (4294967295)








#define _l5_constant_power_l5_sg_ctrl_sine__no_of_points 1000
#define _l5_constant_power_l5_sg_ctrl_sine__max_int40 (1099511627775)
#define _l5_constant_power_l5_sg_ctrl_sine__max_int32 (4294967295)








#define _l6_constant_power_l6_sg_ctrl_sine__no_of_points 1000
#define _l6_constant_power_l6_sg_ctrl_sine__max_int40 (1099511627775)
#define _l6_constant_power_l6_sg_ctrl_sine__max_int32 (4294967295)




//@cmp.def.end


//-----------------------------------------------------------------------------------------
// generated using template: common_variables.template-------------------------------------
// true global variables


//@cmp.var.start
// variables
float _l4_constant_power_l4_ia__out;
float _l4_constant_power_l4_rsnb_val__out = 2222.222222222222;
float _l4_constant_power_l4_vab__out;
float _l4_constant_power_l4_cpu_tr1_output__out;
float _l4_constant_power_l4_cpu_tr2_output__out;
float _l4_constant_power_l4_phase__out = 0.0;
float _l5_constant_power_l5_ia__out;
float _l5_constant_power_l5_rsnb_val__out = 2222.222222222222;
float _l5_constant_power_l5_vab__out;
float _l5_constant_power_l5_cpu_tr1_output__out;
float _l5_constant_power_l5_cpu_tr2_output__out;
float _l5_constant_power_l5_phase__out = 0.0;
float _l6_constant_power_l6_ia__out;
float _l6_constant_power_l6_rsnb_val__out = 2222.222222222222;
float _l6_constant_power_l6_vab__out;
float _l6_constant_power_l6_cpu_tr1_output__out;
float _l6_constant_power_l6_cpu_tr2_output__out;
float _l6_constant_power_l6_phase__out = 0.0;
float _l4_constant_power_l4_power_src_fast__v_eff_s;
float _l4_constant_power_l4_power_src_fast__v_zc;
float _l4_constant_power_l4_power_src_fast__v_ic;
float _l4_constant_power_l4_power_src_fast__i_zc;
X_UnInt32 _l4_constant_power_l4_power_src_fast__v_zc_event;
float _l4_constant_power_l4_power_src_fast__v_filt;
float _l4_constant_power_l4_power_src_fast__i_filt;
float _l4_constant_power_l4_power_src_fast__v_interp;
float _l4_constant_power_l4_power_src_fast__i_interp;
float _l4_constant_power_l4_qscale__out;
float _l4_constant_power_l4_calc_v_params__v_rms;
float _l4_constant_power_l4_calc_v_params__v_f_in;
float _l5_constant_power_l5_power_src_fast__v_eff_s;
float _l5_constant_power_l5_power_src_fast__v_zc;
float _l5_constant_power_l5_power_src_fast__v_ic;
float _l5_constant_power_l5_power_src_fast__i_zc;
X_UnInt32 _l5_constant_power_l5_power_src_fast__v_zc_event;
float _l5_constant_power_l5_power_src_fast__v_filt;
float _l5_constant_power_l5_power_src_fast__i_filt;
float _l5_constant_power_l5_power_src_fast__v_interp;
float _l5_constant_power_l5_power_src_fast__i_interp;
float _l5_constant_power_l5_qscale__out;
float _l5_constant_power_l5_calc_v_params__v_rms;
float _l5_constant_power_l5_calc_v_params__v_f_in;
float _l6_constant_power_l6_power_src_fast__v_eff_s;
float _l6_constant_power_l6_power_src_fast__v_zc;
float _l6_constant_power_l6_power_src_fast__v_ic;
float _l6_constant_power_l6_power_src_fast__i_zc;
X_UnInt32 _l6_constant_power_l6_power_src_fast__v_zc_event;
float _l6_constant_power_l6_power_src_fast__v_filt;
float _l6_constant_power_l6_power_src_fast__i_filt;
float _l6_constant_power_l6_power_src_fast__v_interp;
float _l6_constant_power_l6_power_src_fast__i_interp;
float _l6_constant_power_l6_qscale__out;
float _l6_constant_power_l6_calc_v_params__v_rms;
float _l6_constant_power_l6_calc_v_params__v_f_in;
float _l4_constant_power_l4_qscale1__out;
float _l4_constant_power_l4_vscale__out;
float _l5_constant_power_l5_qscale1__out;
float _l5_constant_power_l5_vscale__out;
float _l6_constant_power_l6_qscale1__out;
float _l6_constant_power_l6_vscale__out;
float _l4_constant_power_l4_snb_pwr__out;
float _l5_constant_power_l5_snb_pwr__out;
float _l6_constant_power_l6_snb_pwr__out;
float _l4_constant_power_l4_pref_sum__out;
float _l5_constant_power_l5_pref_sum__out;
float _l6_constant_power_l6_pref_sum__out;
float _l4_constant_power_l4_pscale__out;
float _l5_constant_power_l5_pscale__out;
float _l6_constant_power_l6_pscale__out;
float _l4_constant_power_l4_pscale1__out;
float _l4_constant_power_l4_power_src_slow__i_rms;
float _l4_constant_power_l4_power_src_slow__i_rms_old;
float _l4_constant_power_l4_power_src_slow__i_f;
float _l4_constant_power_l4_power_src_slow__i_init_phase;
float _l4_constant_power_l4_power_src_slow__int_err;
float _l4_constant_power_l4_power_src_slow__curr_err;
float _l4_constant_power_l4_power_src_slow__i_f_init;
float _l4_constant_power_l4_power_src_slow__v_f_old;
float _l4_constant_power_l4_power_src_slow__active;
X_UnInt32 _l4_constant_power_l4_power_src_slow__lock_cnt;
X_UnInt32 _l4_constant_power_l4_power_src_slow__state;

float _l4_constant_power_l4_power_src_slow__abs_err;
float _l4_constant_power_l4_power_src_slow__phase_shift_ref_abs;
float _l4_constant_power_l4_power_src_slow__s_ref;
float _l4_constant_power_l4_power_src_slow__angle_ref;
float _l4_constant_power_l4_power_src_slow__i_f_delta;

float _l4_constant_power_l4_power_src_slow__c_p;
float _l4_constant_power_l4_power_src_slow__c_i;
float _l5_constant_power_l5_pscale1__out;
float _l5_constant_power_l5_power_src_slow__i_rms;
float _l5_constant_power_l5_power_src_slow__i_rms_old;
float _l5_constant_power_l5_power_src_slow__i_f;
float _l5_constant_power_l5_power_src_slow__i_init_phase;
float _l5_constant_power_l5_power_src_slow__int_err;
float _l5_constant_power_l5_power_src_slow__curr_err;
float _l5_constant_power_l5_power_src_slow__i_f_init;
float _l5_constant_power_l5_power_src_slow__v_f_old;
float _l5_constant_power_l5_power_src_slow__active;
X_UnInt32 _l5_constant_power_l5_power_src_slow__lock_cnt;
X_UnInt32 _l5_constant_power_l5_power_src_slow__state;

float _l5_constant_power_l5_power_src_slow__abs_err;
float _l5_constant_power_l5_power_src_slow__phase_shift_ref_abs;
float _l5_constant_power_l5_power_src_slow__s_ref;
float _l5_constant_power_l5_power_src_slow__angle_ref;
float _l5_constant_power_l5_power_src_slow__i_f_delta;

float _l5_constant_power_l5_power_src_slow__c_p;
float _l5_constant_power_l5_power_src_slow__c_i;
float _l6_constant_power_l6_pscale1__out;
float _l6_constant_power_l6_power_src_slow__i_rms;
float _l6_constant_power_l6_power_src_slow__i_rms_old;
float _l6_constant_power_l6_power_src_slow__i_f;
float _l6_constant_power_l6_power_src_slow__i_init_phase;
float _l6_constant_power_l6_power_src_slow__int_err;
float _l6_constant_power_l6_power_src_slow__curr_err;
float _l6_constant_power_l6_power_src_slow__i_f_init;
float _l6_constant_power_l6_power_src_slow__v_f_old;
float _l6_constant_power_l6_power_src_slow__active;
X_UnInt32 _l6_constant_power_l6_power_src_slow__lock_cnt;
X_UnInt32 _l6_constant_power_l6_power_src_slow__state;

float _l6_constant_power_l6_power_src_slow__abs_err;
float _l6_constant_power_l6_power_src_slow__phase_shift_ref_abs;
float _l6_constant_power_l6_power_src_slow__s_ref;
float _l6_constant_power_l6_power_src_slow__angle_ref;
float _l6_constant_power_l6_power_src_slow__i_f_delta;

float _l6_constant_power_l6_power_src_slow__c_p;
float _l6_constant_power_l6_power_src_slow__c_i;
float _l4_constant_power_l4_active__tmp;
X_UnInt32 _l4_constant_power_l4_sg_ctrl_sine__channels[2] = {0, 1};
float _l4_constant_power_l4_sg_ctrl_sine__gain;
float _l4_constant_power_l4_sg_ctrl_sine__period;
float _l4_constant_power_l4_sg_ctrl_sine__shift;
float _l4_constant_power_l4_sg_ctrl_sine__phase_pu;
float _l4_constant_power_l4_sg_ctrl_sine__offset;
X_UnInt32 _l4_constant_power_l4_sg_ctrl_sine__offset_uint;
X_UnInt32 _l4_constant_power_l4_sg_ctrl_sine__sample_cnt_inc_lo;
X_UnInt32 _l4_constant_power_l4_sg_ctrl_sine__sample_cnt_inc_hi;
float _l4_constant_power_l4_sg_ctrl_sine__sample_cnt_inc_real;
X_UnInt32 _l4_constant_power_l4_sg_ctrl_sine__sg_update_value;
float _l4_constant_power_l4_sg_ctrl_sine__xn;
float _l4_constant_power_l4_sg_ctrl_sine__xn_1;
float _l4_constant_power_l4_sg_ctrl_sine__yn;
float _l4_constant_power_l4_sg_ctrl_sine__yn_1;
X_UnInt32 _l4_constant_power_l4_sg_ctrl_sine__i;
X_UnInt32 _l4_constant_power_l4_sg_ctrl_sine__j;
float _l4_constant_power_l4_sg_ctrl_sine__k;
float _l5_constant_power_l5_active__tmp;
X_UnInt32 _l5_constant_power_l5_sg_ctrl_sine__channels[2] = {4, 5};
float _l5_constant_power_l5_sg_ctrl_sine__gain;
float _l5_constant_power_l5_sg_ctrl_sine__period;
float _l5_constant_power_l5_sg_ctrl_sine__shift;
float _l5_constant_power_l5_sg_ctrl_sine__phase_pu;
float _l5_constant_power_l5_sg_ctrl_sine__offset;
X_UnInt32 _l5_constant_power_l5_sg_ctrl_sine__offset_uint;
X_UnInt32 _l5_constant_power_l5_sg_ctrl_sine__sample_cnt_inc_lo;
X_UnInt32 _l5_constant_power_l5_sg_ctrl_sine__sample_cnt_inc_hi;
float _l5_constant_power_l5_sg_ctrl_sine__sample_cnt_inc_real;
X_UnInt32 _l5_constant_power_l5_sg_ctrl_sine__sg_update_value;
float _l5_constant_power_l5_sg_ctrl_sine__xn;
float _l5_constant_power_l5_sg_ctrl_sine__xn_1;
float _l5_constant_power_l5_sg_ctrl_sine__yn;
float _l5_constant_power_l5_sg_ctrl_sine__yn_1;
X_UnInt32 _l5_constant_power_l5_sg_ctrl_sine__i;
X_UnInt32 _l5_constant_power_l5_sg_ctrl_sine__j;
float _l5_constant_power_l5_sg_ctrl_sine__k;
float _l6_constant_power_l6_active__tmp;
X_UnInt32 _l6_constant_power_l6_sg_ctrl_sine__channels[2] = {2, 3};
float _l6_constant_power_l6_sg_ctrl_sine__gain;
float _l6_constant_power_l6_sg_ctrl_sine__period;
float _l6_constant_power_l6_sg_ctrl_sine__shift;
float _l6_constant_power_l6_sg_ctrl_sine__phase_pu;
float _l6_constant_power_l6_sg_ctrl_sine__offset;
X_UnInt32 _l6_constant_power_l6_sg_ctrl_sine__offset_uint;
X_UnInt32 _l6_constant_power_l6_sg_ctrl_sine__sample_cnt_inc_lo;
X_UnInt32 _l6_constant_power_l6_sg_ctrl_sine__sample_cnt_inc_hi;
float _l6_constant_power_l6_sg_ctrl_sine__sample_cnt_inc_real;
X_UnInt32 _l6_constant_power_l6_sg_ctrl_sine__sg_update_value;
float _l6_constant_power_l6_sg_ctrl_sine__xn;
float _l6_constant_power_l6_sg_ctrl_sine__xn_1;
float _l6_constant_power_l6_sg_ctrl_sine__yn;
float _l6_constant_power_l6_sg_ctrl_sine__yn_1;
X_UnInt32 _l6_constant_power_l6_sg_ctrl_sine__i;
X_UnInt32 _l6_constant_power_l6_sg_ctrl_sine__j;
float _l6_constant_power_l6_sg_ctrl_sine__k;
//@cmp.var.end

//@cmp.svar.start
// state variables
float _l4_constant_power_l4_rt1_output__out =  0.0;

float _l4_constant_power_l4_rt2_output__out =  0.0;

float _l4_constant_power_l4_rt3_output__out =  0.0;

float _l4_constant_power_l4_rt4_output__out =  0.0;

float _l5_constant_power_l5_rt1_output__out =  0.0;

float _l5_constant_power_l5_rt2_output__out =  0.0;

float _l5_constant_power_l5_rt3_output__out =  0.0;

float _l5_constant_power_l5_rt4_output__out =  0.0;

float _l6_constant_power_l6_rt1_output__out =  0.0;

float _l6_constant_power_l6_rt2_output__out =  0.0;

float _l6_constant_power_l6_rt3_output__out =  0.0;

float _l6_constant_power_l6_rt4_output__out =  0.0;

float _l4_constant_power_l4_power_src_fast__v_sq_sum_state;
X_UnInt32 _l4_constant_power_l4_power_src_fast__pc_cnt_state;
X_UnInt32 _l4_constant_power_l4_power_src_fast__v_zc_event_cnt;
float _l4_constant_power_l4_power_src_fast__v_filt_old;
float _l4_constant_power_l4_power_src_fast__i_filt_old;
float _l4_constant_power_l4_power_src_fast__v_period;
float _l4_constant_power_l4_power_src_fast__v_interp_old;
float _l4_constant_power_l4_calc_v_params__v_f;
float _l5_constant_power_l5_power_src_fast__v_sq_sum_state;
X_UnInt32 _l5_constant_power_l5_power_src_fast__pc_cnt_state;
X_UnInt32 _l5_constant_power_l5_power_src_fast__v_zc_event_cnt;
float _l5_constant_power_l5_power_src_fast__v_filt_old;
float _l5_constant_power_l5_power_src_fast__i_filt_old;
float _l5_constant_power_l5_power_src_fast__v_period;
float _l5_constant_power_l5_power_src_fast__v_interp_old;
float _l5_constant_power_l5_calc_v_params__v_f;
float _l6_constant_power_l6_power_src_fast__v_sq_sum_state;
X_UnInt32 _l6_constant_power_l6_power_src_fast__pc_cnt_state;
X_UnInt32 _l6_constant_power_l6_power_src_fast__v_zc_event_cnt;
float _l6_constant_power_l6_power_src_fast__v_filt_old;
float _l6_constant_power_l6_power_src_fast__i_filt_old;
float _l6_constant_power_l6_power_src_fast__v_period;
float _l6_constant_power_l6_power_src_fast__v_interp_old;
float _l6_constant_power_l6_calc_v_params__v_f;
//@cmp.svar.end


// generated using template: virtual_hil/custom_functions.template---------------------------------
void ReInit_sys_sp_cpu_dev0() {
#if DEBUG_MODE
    printf("\n\rReInitTimer");
#endif
    //@cmp.init.block.start
    _l4_constant_power_l4_rt1_output__out =  0.0;
    _l4_constant_power_l4_rt2_output__out =  0.0;
    _l4_constant_power_l4_rt3_output__out =  0.0;
    _l4_constant_power_l4_rt4_output__out =  0.0;
    _l5_constant_power_l5_rt1_output__out =  0.0;
    _l5_constant_power_l5_rt2_output__out =  0.0;
    _l5_constant_power_l5_rt3_output__out =  0.0;
    _l5_constant_power_l5_rt4_output__out =  0.0;
    _l6_constant_power_l6_rt1_output__out =  0.0;
    _l6_constant_power_l6_rt2_output__out =  0.0;
    _l6_constant_power_l6_rt3_output__out =  0.0;
    _l6_constant_power_l6_rt4_output__out =  0.0;
    _l4_constant_power_l4_power_src_fast__v_zc_event_cnt = 0;
    _l4_constant_power_l4_power_src_fast__v_sq_sum_state = 0.0f;
    _l4_constant_power_l4_power_src_fast__pc_cnt_state = 0;
    _l4_constant_power_l4_power_src_fast__v_filt_old = 0.0f;
    _l4_constant_power_l4_power_src_fast__i_filt_old = 0.0f;
    _l4_constant_power_l4_power_src_fast__v_period = 0.0f;
    _l4_constant_power_l4_calc_v_params__v_f = 0.0f;
    _l5_constant_power_l5_power_src_fast__v_zc_event_cnt = 0;
    _l5_constant_power_l5_power_src_fast__v_sq_sum_state = 0.0f;
    _l5_constant_power_l5_power_src_fast__pc_cnt_state = 0;
    _l5_constant_power_l5_power_src_fast__v_filt_old = 0.0f;
    _l5_constant_power_l5_power_src_fast__i_filt_old = 0.0f;
    _l5_constant_power_l5_power_src_fast__v_period = 0.0f;
    _l5_constant_power_l5_calc_v_params__v_f = 0.0f;
    _l6_constant_power_l6_power_src_fast__v_zc_event_cnt = 0;
    _l6_constant_power_l6_power_src_fast__v_sq_sum_state = 0.0f;
    _l6_constant_power_l6_power_src_fast__pc_cnt_state = 0;
    _l6_constant_power_l6_power_src_fast__v_filt_old = 0.0f;
    _l6_constant_power_l6_power_src_fast__i_filt_old = 0.0f;
    _l6_constant_power_l6_power_src_fast__v_period = 0.0f;
    _l6_constant_power_l6_calc_v_params__v_f = 0.0f;
    XIo_OutFloat(0x55000124, 0.0f);
    XIo_OutFloat(0x55000108, 0.0f);
    XIo_OutFloat(0x5500012c, 0.0f);
    XIo_OutFloat(0x55000114, 0.0f);
    XIo_OutFloat(0x55000134, 0.0f);
    XIo_OutFloat(0x55000120, 0.0f);
    XIo_OutFloat(0x55000104, 0.0f);
    XIo_OutFloat(0x55000110, 0.0f);
    XIo_OutFloat(0x5500011c, 0.0f);
    _l4_constant_power_l4_power_src_slow__i_rms = 0.0f;
    _l4_constant_power_l4_power_src_slow__i_rms_old = 0.0f;
    _l4_constant_power_l4_power_src_slow__i_f = 50.0f;
    _l4_constant_power_l4_power_src_slow__int_err = 0.0f;
    _l4_constant_power_l4_power_src_slow__i_f_init = 50.0f;
    _l4_constant_power_l4_power_src_slow__v_f_old = 0.0f;
    _l4_constant_power_l4_power_src_slow__lock_cnt = 0;
    _l4_constant_power_l4_power_src_slow__state = 0;
    _l4_constant_power_l4_power_src_slow__active = 0.0f;
    _l4_constant_power_l4_power_src_slow__curr_err = 0.0f;
    _l5_constant_power_l5_power_src_slow__i_rms = 0.0f;
    _l5_constant_power_l5_power_src_slow__i_rms_old = 0.0f;
    _l5_constant_power_l5_power_src_slow__i_f = 50.0f;
    _l5_constant_power_l5_power_src_slow__int_err = 0.0f;
    _l5_constant_power_l5_power_src_slow__i_f_init = 50.0f;
    _l5_constant_power_l5_power_src_slow__v_f_old = 0.0f;
    _l5_constant_power_l5_power_src_slow__lock_cnt = 0;
    _l5_constant_power_l5_power_src_slow__state = 0;
    _l5_constant_power_l5_power_src_slow__active = 0.0f;
    _l5_constant_power_l5_power_src_slow__curr_err = 0.0f;
    _l6_constant_power_l6_power_src_slow__i_rms = 0.0f;
    _l6_constant_power_l6_power_src_slow__i_rms_old = 0.0f;
    _l6_constant_power_l6_power_src_slow__i_f = 50.0f;
    _l6_constant_power_l6_power_src_slow__int_err = 0.0f;
    _l6_constant_power_l6_power_src_slow__i_f_init = 50.0f;
    _l6_constant_power_l6_power_src_slow__v_f_old = 0.0f;
    _l6_constant_power_l6_power_src_slow__lock_cnt = 0;
    _l6_constant_power_l6_power_src_slow__state = 0;
    _l6_constant_power_l6_power_src_slow__active = 0.0f;
    _l6_constant_power_l6_power_src_slow__curr_err = 0.0f;
    XIo_OutFloat(0x55000100, 0.0f);
    XIo_OutFloat(0x55000128, 0.0f);
    // write initial SG parameters
    for (_l4_constant_power_l4_sg_ctrl_sine__i = 0; _l4_constant_power_l4_sg_ctrl_sine__i < 2; _l4_constant_power_l4_sg_ctrl_sine__i++) {
        HIL_OutFloat(0x400200 + _l4_constant_power_l4_sg_ctrl_sine__channels[_l4_constant_power_l4_sg_ctrl_sine__i], 0.0f);  // set gain to 0 (initial value)
        HIL_OutInt32(0x400180 + _l4_constant_power_l4_sg_ctrl_sine__channels[_l4_constant_power_l4_sg_ctrl_sine__i], 0);      // set offset to 0 (initial value)
        HIL_OutInt32(0x400000 + _l4_constant_power_l4_sg_ctrl_sine__channels[_l4_constant_power_l4_sg_ctrl_sine__i], 1);
        HIL_OutInt32(0x400280 + _l4_constant_power_l4_sg_ctrl_sine__channels[_l4_constant_power_l4_sg_ctrl_sine__i], 0);
        HIL_OutFloat(0x400100 + _l4_constant_power_l4_sg_ctrl_sine__channels[_l4_constant_power_l4_sg_ctrl_sine__i], 2.0f);
        HIL_OutInt32(0x400300 + _l4_constant_power_l4_sg_ctrl_sine__channels[_l4_constant_power_l4_sg_ctrl_sine__i], 1);   // enable wave update
        _l4_constant_power_l4_sg_ctrl_sine__sg_update_value += 1 << _l4_constant_power_l4_sg_ctrl_sine__channels[_l4_constant_power_l4_sg_ctrl_sine__i];
    }
    // calculate and write waveform data
    _l4_constant_power_l4_sg_ctrl_sine__k = 2.0f * M_PI / _l4_constant_power_l4_sg_ctrl_sine__no_of_points;
    // oscillator based sine implementation
    // oscillator init
    _l4_constant_power_l4_sg_ctrl_sine__xn_1 = sinf(0.0f);
    _l4_constant_power_l4_sg_ctrl_sine__yn_1 = cosf(0.0f);
    // oscillator
    for (_l4_constant_power_l4_sg_ctrl_sine__i = 0; _l4_constant_power_l4_sg_ctrl_sine__i < _l4_constant_power_l4_sg_ctrl_sine__no_of_points; _l4_constant_power_l4_sg_ctrl_sine__i++) {
        _l4_constant_power_l4_sg_ctrl_sine__xn = _l4_constant_power_l4_sg_ctrl_sine__xn_1 + _l4_constant_power_l4_sg_ctrl_sine__k * _l4_constant_power_l4_sg_ctrl_sine__yn_1;
        _l4_constant_power_l4_sg_ctrl_sine__yn = _l4_constant_power_l4_sg_ctrl_sine__yn_1 - _l4_constant_power_l4_sg_ctrl_sine__k * _l4_constant_power_l4_sg_ctrl_sine__xn;
        // write the same data to all the channels
        for (_l4_constant_power_l4_sg_ctrl_sine__j = 0; _l4_constant_power_l4_sg_ctrl_sine__j < 2; _l4_constant_power_l4_sg_ctrl_sine__j++) {
            HIL_OutFloat(0x600000 + _l4_constant_power_l4_sg_ctrl_sine__channels[_l4_constant_power_l4_sg_ctrl_sine__j] * 0x800 + _l4_constant_power_l4_sg_ctrl_sine__i, (float) _l4_constant_power_l4_sg_ctrl_sine__xn_1);
        }
        _l4_constant_power_l4_sg_ctrl_sine__xn_1 = _l4_constant_power_l4_sg_ctrl_sine__xn;
        _l4_constant_power_l4_sg_ctrl_sine__yn_1 = _l4_constant_power_l4_sg_ctrl_sine__yn;
    }
    HIL_OutInt32(0x400080, _l4_constant_power_l4_sg_ctrl_sine__sg_update_value);  // update both parameters and waveforms on all channels
    XIo_OutFloat(0x5500010c, 0.0f);
    XIo_OutFloat(0x55000130, 0.0f);
    // write initial SG parameters
    for (_l5_constant_power_l5_sg_ctrl_sine__i = 0; _l5_constant_power_l5_sg_ctrl_sine__i < 2; _l5_constant_power_l5_sg_ctrl_sine__i++) {
        HIL_OutFloat(0x400200 + _l5_constant_power_l5_sg_ctrl_sine__channels[_l5_constant_power_l5_sg_ctrl_sine__i], 0.0f);  // set gain to 0 (initial value)
        HIL_OutInt32(0x400180 + _l5_constant_power_l5_sg_ctrl_sine__channels[_l5_constant_power_l5_sg_ctrl_sine__i], 0);      // set offset to 0 (initial value)
        HIL_OutInt32(0x400000 + _l5_constant_power_l5_sg_ctrl_sine__channels[_l5_constant_power_l5_sg_ctrl_sine__i], 1);
        HIL_OutInt32(0x400280 + _l5_constant_power_l5_sg_ctrl_sine__channels[_l5_constant_power_l5_sg_ctrl_sine__i], 0);
        HIL_OutFloat(0x400100 + _l5_constant_power_l5_sg_ctrl_sine__channels[_l5_constant_power_l5_sg_ctrl_sine__i], 2.0f);
        HIL_OutInt32(0x400300 + _l5_constant_power_l5_sg_ctrl_sine__channels[_l5_constant_power_l5_sg_ctrl_sine__i], 1);   // enable wave update
        _l5_constant_power_l5_sg_ctrl_sine__sg_update_value += 1 << _l5_constant_power_l5_sg_ctrl_sine__channels[_l5_constant_power_l5_sg_ctrl_sine__i];
    }
    // calculate and write waveform data
    _l5_constant_power_l5_sg_ctrl_sine__k = 2.0f * M_PI / _l5_constant_power_l5_sg_ctrl_sine__no_of_points;
    // oscillator based sine implementation
    // oscillator init
    _l5_constant_power_l5_sg_ctrl_sine__xn_1 = sinf(0.0f);
    _l5_constant_power_l5_sg_ctrl_sine__yn_1 = cosf(0.0f);
    // oscillator
    for (_l5_constant_power_l5_sg_ctrl_sine__i = 0; _l5_constant_power_l5_sg_ctrl_sine__i < _l5_constant_power_l5_sg_ctrl_sine__no_of_points; _l5_constant_power_l5_sg_ctrl_sine__i++) {
        _l5_constant_power_l5_sg_ctrl_sine__xn = _l5_constant_power_l5_sg_ctrl_sine__xn_1 + _l5_constant_power_l5_sg_ctrl_sine__k * _l5_constant_power_l5_sg_ctrl_sine__yn_1;
        _l5_constant_power_l5_sg_ctrl_sine__yn = _l5_constant_power_l5_sg_ctrl_sine__yn_1 - _l5_constant_power_l5_sg_ctrl_sine__k * _l5_constant_power_l5_sg_ctrl_sine__xn;
        // write the same data to all the channels
        for (_l5_constant_power_l5_sg_ctrl_sine__j = 0; _l5_constant_power_l5_sg_ctrl_sine__j < 2; _l5_constant_power_l5_sg_ctrl_sine__j++) {
            HIL_OutFloat(0x600000 + _l5_constant_power_l5_sg_ctrl_sine__channels[_l5_constant_power_l5_sg_ctrl_sine__j] * 0x800 + _l5_constant_power_l5_sg_ctrl_sine__i, (float) _l5_constant_power_l5_sg_ctrl_sine__xn_1);
        }
        _l5_constant_power_l5_sg_ctrl_sine__xn_1 = _l5_constant_power_l5_sg_ctrl_sine__xn;
        _l5_constant_power_l5_sg_ctrl_sine__yn_1 = _l5_constant_power_l5_sg_ctrl_sine__yn;
    }
    HIL_OutInt32(0x400080, _l5_constant_power_l5_sg_ctrl_sine__sg_update_value);  // update both parameters and waveforms on all channels
    XIo_OutFloat(0x55000118, 0.0f);
    XIo_OutFloat(0x55000138, 0.0f);
    // write initial SG parameters
    for (_l6_constant_power_l6_sg_ctrl_sine__i = 0; _l6_constant_power_l6_sg_ctrl_sine__i < 2; _l6_constant_power_l6_sg_ctrl_sine__i++) {
        HIL_OutFloat(0x400200 + _l6_constant_power_l6_sg_ctrl_sine__channels[_l6_constant_power_l6_sg_ctrl_sine__i], 0.0f);  // set gain to 0 (initial value)
        HIL_OutInt32(0x400180 + _l6_constant_power_l6_sg_ctrl_sine__channels[_l6_constant_power_l6_sg_ctrl_sine__i], 0);      // set offset to 0 (initial value)
        HIL_OutInt32(0x400000 + _l6_constant_power_l6_sg_ctrl_sine__channels[_l6_constant_power_l6_sg_ctrl_sine__i], 1);
        HIL_OutInt32(0x400280 + _l6_constant_power_l6_sg_ctrl_sine__channels[_l6_constant_power_l6_sg_ctrl_sine__i], 0);
        HIL_OutFloat(0x400100 + _l6_constant_power_l6_sg_ctrl_sine__channels[_l6_constant_power_l6_sg_ctrl_sine__i], 2.0f);
        HIL_OutInt32(0x400300 + _l6_constant_power_l6_sg_ctrl_sine__channels[_l6_constant_power_l6_sg_ctrl_sine__i], 1);   // enable wave update
        _l6_constant_power_l6_sg_ctrl_sine__sg_update_value += 1 << _l6_constant_power_l6_sg_ctrl_sine__channels[_l6_constant_power_l6_sg_ctrl_sine__i];
    }
    // calculate and write waveform data
    _l6_constant_power_l6_sg_ctrl_sine__k = 2.0f * M_PI / _l6_constant_power_l6_sg_ctrl_sine__no_of_points;
    // oscillator based sine implementation
    // oscillator init
    _l6_constant_power_l6_sg_ctrl_sine__xn_1 = sinf(0.0f);
    _l6_constant_power_l6_sg_ctrl_sine__yn_1 = cosf(0.0f);
    // oscillator
    for (_l6_constant_power_l6_sg_ctrl_sine__i = 0; _l6_constant_power_l6_sg_ctrl_sine__i < _l6_constant_power_l6_sg_ctrl_sine__no_of_points; _l6_constant_power_l6_sg_ctrl_sine__i++) {
        _l6_constant_power_l6_sg_ctrl_sine__xn = _l6_constant_power_l6_sg_ctrl_sine__xn_1 + _l6_constant_power_l6_sg_ctrl_sine__k * _l6_constant_power_l6_sg_ctrl_sine__yn_1;
        _l6_constant_power_l6_sg_ctrl_sine__yn = _l6_constant_power_l6_sg_ctrl_sine__yn_1 - _l6_constant_power_l6_sg_ctrl_sine__k * _l6_constant_power_l6_sg_ctrl_sine__xn;
        // write the same data to all the channels
        for (_l6_constant_power_l6_sg_ctrl_sine__j = 0; _l6_constant_power_l6_sg_ctrl_sine__j < 2; _l6_constant_power_l6_sg_ctrl_sine__j++) {
            HIL_OutFloat(0x600000 + _l6_constant_power_l6_sg_ctrl_sine__channels[_l6_constant_power_l6_sg_ctrl_sine__j] * 0x800 + _l6_constant_power_l6_sg_ctrl_sine__i, (float) _l6_constant_power_l6_sg_ctrl_sine__xn_1);
        }
        _l6_constant_power_l6_sg_ctrl_sine__xn_1 = _l6_constant_power_l6_sg_ctrl_sine__xn;
        _l6_constant_power_l6_sg_ctrl_sine__yn_1 = _l6_constant_power_l6_sg_ctrl_sine__yn;
    }
    HIL_OutInt32(0x400080, _l6_constant_power_l6_sg_ctrl_sine__sg_update_value);  // update both parameters and waveforms on all channels
    //@cmp.init.block.end
}

void ReInit_sp_scope_sys_sp_cpu_dev0() {
    // initialise SP Scope buffer pointer
}


// generated using template: common_timer_counter_handler.template-------------------------

/*****************************************************************************************/
/**
* This function is the handler which performs processing for the timer counter.
* It is called from an interrupt context such that the amount of processing
* performed should be minimized.  It is called when the timer counter expires
* if interrupts are enabled.
*
*
* @param    None
*
* @return   None
*
* @note     None
*
*****************************************************************************************/

void TimerCounterHandler_0_sys_sp_cpu_dev0() {
#if DEBUG_MODE
    printf("\n\rTimerCounterHandler_0");
#endif
    //////////////////////////////////////////////////////////////////////////
    // Output block
    //////////////////////////////////////////////////////////////////////////
    //@cmp.out.block.start
    // Generated from the component: L4.Constant Power L4.Ia
    _l4_constant_power_l4_ia__out = (HIL_InFloat(0xc80000 + 0x1c));
    // Generated from the component: L4.Constant Power L4.Vab
    _l4_constant_power_l4_vab__out = (HIL_InFloat(0xc80000 + 0x12));
    // Generated from the component: L5.Constant Power L5.Ia
    _l5_constant_power_l5_ia__out = (HIL_InFloat(0xc80000 + 0x416));
    // Generated from the component: L5.Constant Power L5.Vab
    _l5_constant_power_l5_vab__out = (HIL_InFloat(0xc80000 + 0x40c));
    // Generated from the component: L6.Constant Power L6.Ia
    _l6_constant_power_l6_ia__out = (HIL_InFloat(0xc80000 + 0x218));
    // Generated from the component: L6.Constant Power L6.Vab
    _l6_constant_power_l6_vab__out = (HIL_InFloat(0xc80000 + 0x20e));
    // Generated from the component: L4.Constant Power L4.m1
    // Generated from the component: L4.Constant Power L4.power_src_fast
    // voltage filter
    _l4_constant_power_l4_power_src_fast__v_filt = (_l4_constant_power_l4_vab__out * 0.3f) + (_l4_constant_power_l4_power_src_fast__v_filt_old * 0.7f);
    _l4_constant_power_l4_power_src_fast__i_filt = (_l4_constant_power_l4_ia__out * 0.3f) + (_l4_constant_power_l4_power_src_fast__i_filt_old * 0.7f);
    // voltage square sum - used for RMS measurement
    _l4_constant_power_l4_power_src_fast__v_sq_sum_state = _l4_constant_power_l4_power_src_fast__v_sq_sum_state + _l4_constant_power_l4_vab__out * _l4_constant_power_l4_vab__out;
    // voltage zc detection
    if ((_l4_constant_power_l4_power_src_fast__v_filt >= 0.0f) && (_l4_constant_power_l4_power_src_fast__v_filt_old < 0.0f)) {
        _l4_constant_power_l4_power_src_fast__v_interp = _l4_constant_power_l4_power_src_fast__v_filt_old / (_l4_constant_power_l4_power_src_fast__v_filt_old - _l4_constant_power_l4_power_src_fast__v_filt);
        _l4_constant_power_l4_power_src_fast__v_zc = _l4_constant_power_l4_power_src_fast__pc_cnt_state + _l4_constant_power_l4_power_src_fast__v_interp;
        _l4_constant_power_l4_power_src_fast__v_zc_event = 1;
        _l4_constant_power_l4_power_src_fast__v_zc_event_cnt = _l4_constant_power_l4_power_src_fast__v_zc_event_cnt + 1;
    }
    else {
        _l4_constant_power_l4_power_src_fast__v_zc_event = 0;
    }
    //current zc detection
    if ((_l4_constant_power_l4_power_src_fast__i_filt > 0.0f) && (_l4_constant_power_l4_power_src_fast__i_filt_old <= 0.0f)) {
        _l4_constant_power_l4_power_src_fast__i_interp = _l4_constant_power_l4_power_src_fast__i_filt_old / (_l4_constant_power_l4_power_src_fast__i_filt_old - _l4_constant_power_l4_power_src_fast__i_filt);
        _l4_constant_power_l4_power_src_fast__i_zc = _l4_constant_power_l4_power_src_fast__pc_cnt_state + _l4_constant_power_l4_power_src_fast__i_interp;
    }
    // update on current zero cross
    if ((_l4_constant_power_l4_power_src_fast__v_zc_event == 1) || (_l4_constant_power_l4_power_src_fast__pc_cnt_state > 50000)) {
        _l4_constant_power_l4_power_src_fast__v_eff_s = _l4_constant_power_l4_power_src_fast__v_sq_sum_state;
        _l4_constant_power_l4_power_src_fast__v_sq_sum_state = 0.0f;
        if (_l4_constant_power_l4_power_src_fast__v_zc_event_cnt > 3) {
            _l4_constant_power_l4_power_src_fast__v_period = _l4_constant_power_l4_power_src_fast__v_zc - _l4_constant_power_l4_power_src_fast__v_interp_old + 1;
        }
    }
    // Generated from the component: L5.Constant Power L5.m1
    // Generated from the component: L5.Constant Power L5.power_src_fast
    // voltage filter
    _l5_constant_power_l5_power_src_fast__v_filt = (_l5_constant_power_l5_vab__out * 0.3f) + (_l5_constant_power_l5_power_src_fast__v_filt_old * 0.7f);
    _l5_constant_power_l5_power_src_fast__i_filt = (_l5_constant_power_l5_ia__out * 0.3f) + (_l5_constant_power_l5_power_src_fast__i_filt_old * 0.7f);
    // voltage square sum - used for RMS measurement
    _l5_constant_power_l5_power_src_fast__v_sq_sum_state = _l5_constant_power_l5_power_src_fast__v_sq_sum_state + _l5_constant_power_l5_vab__out * _l5_constant_power_l5_vab__out;
    // voltage zc detection
    if ((_l5_constant_power_l5_power_src_fast__v_filt >= 0.0f) && (_l5_constant_power_l5_power_src_fast__v_filt_old < 0.0f)) {
        _l5_constant_power_l5_power_src_fast__v_interp = _l5_constant_power_l5_power_src_fast__v_filt_old / (_l5_constant_power_l5_power_src_fast__v_filt_old - _l5_constant_power_l5_power_src_fast__v_filt);
        _l5_constant_power_l5_power_src_fast__v_zc = _l5_constant_power_l5_power_src_fast__pc_cnt_state + _l5_constant_power_l5_power_src_fast__v_interp;
        _l5_constant_power_l5_power_src_fast__v_zc_event = 1;
        _l5_constant_power_l5_power_src_fast__v_zc_event_cnt = _l5_constant_power_l5_power_src_fast__v_zc_event_cnt + 1;
    }
    else {
        _l5_constant_power_l5_power_src_fast__v_zc_event = 0;
    }
    //current zc detection
    if ((_l5_constant_power_l5_power_src_fast__i_filt > 0.0f) && (_l5_constant_power_l5_power_src_fast__i_filt_old <= 0.0f)) {
        _l5_constant_power_l5_power_src_fast__i_interp = _l5_constant_power_l5_power_src_fast__i_filt_old / (_l5_constant_power_l5_power_src_fast__i_filt_old - _l5_constant_power_l5_power_src_fast__i_filt);
        _l5_constant_power_l5_power_src_fast__i_zc = _l5_constant_power_l5_power_src_fast__pc_cnt_state + _l5_constant_power_l5_power_src_fast__i_interp;
    }
    // update on current zero cross
    if ((_l5_constant_power_l5_power_src_fast__v_zc_event == 1) || (_l5_constant_power_l5_power_src_fast__pc_cnt_state > 50000)) {
        _l5_constant_power_l5_power_src_fast__v_eff_s = _l5_constant_power_l5_power_src_fast__v_sq_sum_state;
        _l5_constant_power_l5_power_src_fast__v_sq_sum_state = 0.0f;
        if (_l5_constant_power_l5_power_src_fast__v_zc_event_cnt > 3) {
            _l5_constant_power_l5_power_src_fast__v_period = _l5_constant_power_l5_power_src_fast__v_zc - _l5_constant_power_l5_power_src_fast__v_interp_old + 1;
        }
    }
    // Generated from the component: L6.Constant Power L6.m1
    // Generated from the component: L6.Constant Power L6.power_src_fast
    // voltage filter
    _l6_constant_power_l6_power_src_fast__v_filt = (_l6_constant_power_l6_vab__out * 0.3f) + (_l6_constant_power_l6_power_src_fast__v_filt_old * 0.7f);
    _l6_constant_power_l6_power_src_fast__i_filt = (_l6_constant_power_l6_ia__out * 0.3f) + (_l6_constant_power_l6_power_src_fast__i_filt_old * 0.7f);
    // voltage square sum - used for RMS measurement
    _l6_constant_power_l6_power_src_fast__v_sq_sum_state = _l6_constant_power_l6_power_src_fast__v_sq_sum_state + _l6_constant_power_l6_vab__out * _l6_constant_power_l6_vab__out;
    // voltage zc detection
    if ((_l6_constant_power_l6_power_src_fast__v_filt >= 0.0f) && (_l6_constant_power_l6_power_src_fast__v_filt_old < 0.0f)) {
        _l6_constant_power_l6_power_src_fast__v_interp = _l6_constant_power_l6_power_src_fast__v_filt_old / (_l6_constant_power_l6_power_src_fast__v_filt_old - _l6_constant_power_l6_power_src_fast__v_filt);
        _l6_constant_power_l6_power_src_fast__v_zc = _l6_constant_power_l6_power_src_fast__pc_cnt_state + _l6_constant_power_l6_power_src_fast__v_interp;
        _l6_constant_power_l6_power_src_fast__v_zc_event = 1;
        _l6_constant_power_l6_power_src_fast__v_zc_event_cnt = _l6_constant_power_l6_power_src_fast__v_zc_event_cnt + 1;
    }
    else {
        _l6_constant_power_l6_power_src_fast__v_zc_event = 0;
    }
    //current zc detection
    if ((_l6_constant_power_l6_power_src_fast__i_filt > 0.0f) && (_l6_constant_power_l6_power_src_fast__i_filt_old <= 0.0f)) {
        _l6_constant_power_l6_power_src_fast__i_interp = _l6_constant_power_l6_power_src_fast__i_filt_old / (_l6_constant_power_l6_power_src_fast__i_filt_old - _l6_constant_power_l6_power_src_fast__i_filt);
        _l6_constant_power_l6_power_src_fast__i_zc = _l6_constant_power_l6_power_src_fast__pc_cnt_state + _l6_constant_power_l6_power_src_fast__i_interp;
    }
    // update on current zero cross
    if ((_l6_constant_power_l6_power_src_fast__v_zc_event == 1) || (_l6_constant_power_l6_power_src_fast__pc_cnt_state > 50000)) {
        _l6_constant_power_l6_power_src_fast__v_eff_s = _l6_constant_power_l6_power_src_fast__v_sq_sum_state;
        _l6_constant_power_l6_power_src_fast__v_sq_sum_state = 0.0f;
        if (_l6_constant_power_l6_power_src_fast__v_zc_event_cnt > 3) {
            _l6_constant_power_l6_power_src_fast__v_period = _l6_constant_power_l6_power_src_fast__v_zc - _l6_constant_power_l6_power_src_fast__v_interp_old + 1;
        }
    }
    // Generated from the component: L4.Constant Power L4.rt1.Input
    _l4_constant_power_l4_rt1_output__out = _l4_constant_power_l4_power_src_fast__v_eff_s;
    // Generated from the component: L4.Constant Power L4.rt2.Input
    _l4_constant_power_l4_rt2_output__out = _l4_constant_power_l4_power_src_fast__v_zc;
    // Generated from the component: L4.Constant Power L4.rt3.Input
    _l4_constant_power_l4_rt3_output__out = _l4_constant_power_l4_power_src_fast__i_zc;
    // Generated from the component: L4.Constant Power L4.rt4.Input
    _l4_constant_power_l4_rt4_output__out = _l4_constant_power_l4_power_src_fast__v_period;
    // Generated from the component: L5.Constant Power L5.rt1.Input
    _l5_constant_power_l5_rt1_output__out = _l5_constant_power_l5_power_src_fast__v_eff_s;
    // Generated from the component: L5.Constant Power L5.rt2.Input
    _l5_constant_power_l5_rt2_output__out = _l5_constant_power_l5_power_src_fast__v_zc;
    // Generated from the component: L5.Constant Power L5.rt3.Input
    _l5_constant_power_l5_rt3_output__out = _l5_constant_power_l5_power_src_fast__i_zc;
    // Generated from the component: L5.Constant Power L5.rt4.Input
    _l5_constant_power_l5_rt4_output__out = _l5_constant_power_l5_power_src_fast__v_period;
    // Generated from the component: L6.Constant Power L6.rt1.Input
    _l6_constant_power_l6_rt1_output__out = _l6_constant_power_l6_power_src_fast__v_eff_s;
    // Generated from the component: L6.Constant Power L6.rt2.Input
    _l6_constant_power_l6_rt2_output__out = _l6_constant_power_l6_power_src_fast__v_zc;
    // Generated from the component: L6.Constant Power L6.rt3.Input
    _l6_constant_power_l6_rt3_output__out = _l6_constant_power_l6_power_src_fast__i_zc;
    // Generated from the component: L6.Constant Power L6.rt4.Input
    _l6_constant_power_l6_rt4_output__out = _l6_constant_power_l6_power_src_fast__v_period;
    //@cmp.out.block.end
    //////////////////////////////////////////////////////////////////////////
    // Update block
    //////////////////////////////////////////////////////////////////////////
    //@cmp.update.block.start
    // Generated from the component: L4.Constant Power L4.power_src_fast
    if ((_l4_constant_power_l4_power_src_fast__v_zc_event == 1) || (_l4_constant_power_l4_power_src_fast__pc_cnt_state > 50000)) {
        _l4_constant_power_l4_power_src_fast__pc_cnt_state = 0;
        _l4_constant_power_l4_power_src_fast__v_interp_old = _l4_constant_power_l4_power_src_fast__v_interp;
    }
    else {
        _l4_constant_power_l4_power_src_fast__pc_cnt_state ++;
    }
    _l4_constant_power_l4_power_src_fast__v_filt_old = _l4_constant_power_l4_power_src_fast__v_filt;
    _l4_constant_power_l4_power_src_fast__i_filt_old = _l4_constant_power_l4_power_src_fast__i_filt;
    // Generated from the component: L5.Constant Power L5.power_src_fast
    if ((_l5_constant_power_l5_power_src_fast__v_zc_event == 1) || (_l5_constant_power_l5_power_src_fast__pc_cnt_state > 50000)) {
        _l5_constant_power_l5_power_src_fast__pc_cnt_state = 0;
        _l5_constant_power_l5_power_src_fast__v_interp_old = _l5_constant_power_l5_power_src_fast__v_interp;
    }
    else {
        _l5_constant_power_l5_power_src_fast__pc_cnt_state ++;
    }
    _l5_constant_power_l5_power_src_fast__v_filt_old = _l5_constant_power_l5_power_src_fast__v_filt;
    _l5_constant_power_l5_power_src_fast__i_filt_old = _l5_constant_power_l5_power_src_fast__i_filt;
    // Generated from the component: L6.Constant Power L6.power_src_fast
    if ((_l6_constant_power_l6_power_src_fast__v_zc_event == 1) || (_l6_constant_power_l6_power_src_fast__pc_cnt_state > 50000)) {
        _l6_constant_power_l6_power_src_fast__pc_cnt_state = 0;
        _l6_constant_power_l6_power_src_fast__v_interp_old = _l6_constant_power_l6_power_src_fast__v_interp;
    }
    else {
        _l6_constant_power_l6_power_src_fast__pc_cnt_state ++;
    }
    _l6_constant_power_l6_power_src_fast__v_filt_old = _l6_constant_power_l6_power_src_fast__v_filt;
    _l6_constant_power_l6_power_src_fast__i_filt_old = _l6_constant_power_l6_power_src_fast__i_filt;
    //@cmp.update.block.end
}
void TimerCounterHandler_1_sys_sp_cpu_dev0() {
#if DEBUG_MODE
    printf("\n\rTimerCounterHandler_1");
#endif
    //////////////////////////////////////////////////////////////////////////
    // Output block
    //////////////////////////////////////////////////////////////////////////
    //@cmp.out.block.start
    // Generated from the component: L4.Constant Power L4.Rsnb_val
    // Generated from the component: L4.Constant Power L4.cpu_tr1.Output
    _l4_constant_power_l4_cpu_tr1_output__out = XIo_InFloat(0x5500013c);
    // Generated from the component: L4.Constant Power L4.cpu_tr2.Output
    _l4_constant_power_l4_cpu_tr2_output__out = XIo_InFloat(0x55000140);
    // Generated from the component: L4.Constant Power L4.phase
    // Generated from the component: L4.Constant Power L4.rt1.Output
    // Generated from the component: L4.Constant Power L4.rt2.Output
    // Generated from the component: L4.Constant Power L4.rt3.Output
    // Generated from the component: L4.Constant Power L4.rt4.Output
    // Generated from the component: L5.Constant Power L5.Rsnb_val
    // Generated from the component: L5.Constant Power L5.cpu_tr1.Output
    _l5_constant_power_l5_cpu_tr1_output__out = XIo_InFloat(0x55000144);
    // Generated from the component: L5.Constant Power L5.cpu_tr2.Output
    _l5_constant_power_l5_cpu_tr2_output__out = XIo_InFloat(0x55000148);
    // Generated from the component: L5.Constant Power L5.phase
    // Generated from the component: L5.Constant Power L5.rt1.Output
    // Generated from the component: L5.Constant Power L5.rt2.Output
    // Generated from the component: L5.Constant Power L5.rt3.Output
    // Generated from the component: L5.Constant Power L5.rt4.Output
    // Generated from the component: L6.Constant Power L6.Rsnb_val
    // Generated from the component: L6.Constant Power L6.cpu_tr1.Output
    _l6_constant_power_l6_cpu_tr1_output__out = XIo_InFloat(0x5500014c);
    // Generated from the component: L6.Constant Power L6.cpu_tr2.Output
    _l6_constant_power_l6_cpu_tr2_output__out = XIo_InFloat(0x55000150);
    // Generated from the component: L6.Constant Power L6.phase
    // Generated from the component: L6.Constant Power L6.rt1.Output
    // Generated from the component: L6.Constant Power L6.rt2.Output
    // Generated from the component: L6.Constant Power L6.rt3.Output
    // Generated from the component: L6.Constant Power L6.rt4.Output
    // Generated from the component: L4.Constant Power L4.Qscale
    _l4_constant_power_l4_qscale__out = 0.3333333333333333 * _l4_constant_power_l4_cpu_tr2_output__out;
    // Generated from the component: L4.Constant Power L4.calc_v_params
    if (_l4_constant_power_l4_rt4_output__out > 0.0f) {
        _l4_constant_power_l4_calc_v_params__v_rms =  sqrtf(_l4_constant_power_l4_rt1_output__out / _l4_constant_power_l4_rt4_output__out);
        _l4_constant_power_l4_calc_v_params__v_f_in =  1.0f / (_l4_constant_power_l4_rt4_output__out * 0.0001);
        if (fabsf(_l4_constant_power_l4_calc_v_params__v_f - _l4_constant_power_l4_calc_v_params__v_f_in) < 5.0f) {
            _l4_constant_power_l4_calc_v_params__v_f = 0.02f * _l4_constant_power_l4_calc_v_params__v_f_in + 0.98f * _l4_constant_power_l4_calc_v_params__v_f;
        }
        else {
            _l4_constant_power_l4_calc_v_params__v_f = _l4_constant_power_l4_calc_v_params__v_f_in;
        }
    }
    // Generated from the component: L5.Constant Power L5.Qscale
    _l5_constant_power_l5_qscale__out = 0.3333333333333333 * _l5_constant_power_l5_cpu_tr2_output__out;
    // Generated from the component: L5.Constant Power L5.calc_v_params
    if (_l5_constant_power_l5_rt4_output__out > 0.0f) {
        _l5_constant_power_l5_calc_v_params__v_rms =  sqrtf(_l5_constant_power_l5_rt1_output__out / _l5_constant_power_l5_rt4_output__out);
        _l5_constant_power_l5_calc_v_params__v_f_in =  1.0f / (_l5_constant_power_l5_rt4_output__out * 0.0001);
        if (fabsf(_l5_constant_power_l5_calc_v_params__v_f - _l5_constant_power_l5_calc_v_params__v_f_in) < 5.0f) {
            _l5_constant_power_l5_calc_v_params__v_f = 0.02f * _l5_constant_power_l5_calc_v_params__v_f_in + 0.98f * _l5_constant_power_l5_calc_v_params__v_f;
        }
        else {
            _l5_constant_power_l5_calc_v_params__v_f = _l5_constant_power_l5_calc_v_params__v_f_in;
        }
    }
    // Generated from the component: L6.Constant Power L6.Qscale
    _l6_constant_power_l6_qscale__out = 0.3333333333333333 * _l6_constant_power_l6_cpu_tr2_output__out;
    // Generated from the component: L6.Constant Power L6.calc_v_params
    if (_l6_constant_power_l6_rt4_output__out > 0.0f) {
        _l6_constant_power_l6_calc_v_params__v_rms =  sqrtf(_l6_constant_power_l6_rt1_output__out / _l6_constant_power_l6_rt4_output__out);
        _l6_constant_power_l6_calc_v_params__v_f_in =  1.0f / (_l6_constant_power_l6_rt4_output__out * 0.0001);
        if (fabsf(_l6_constant_power_l6_calc_v_params__v_f - _l6_constant_power_l6_calc_v_params__v_f_in) < 5.0f) {
            _l6_constant_power_l6_calc_v_params__v_f = 0.02f * _l6_constant_power_l6_calc_v_params__v_f_in + 0.98f * _l6_constant_power_l6_calc_v_params__v_f;
        }
        else {
            _l6_constant_power_l6_calc_v_params__v_f = _l6_constant_power_l6_calc_v_params__v_f_in;
        }
    }
    // Generated from the component: L4.Constant Power L4.Qscale1
    _l4_constant_power_l4_qscale1__out = 3.0 * _l4_constant_power_l4_qscale__out;
    // Generated from the component: L4.Constant Power L4.Vscale
    _l4_constant_power_l4_vscale__out = 0.5773502691896258 * _l4_constant_power_l4_calc_v_params__v_rms;
    // Generated from the component: L4.Constant Power L4.cpu_tr3.Input
    XIo_OutFloat(0x55000124, _l4_constant_power_l4_calc_v_params__v_f);
    // Generated from the component: L4.Constant Power L4.f
    XIo_OutFloat(0x55000108, _l4_constant_power_l4_calc_v_params__v_f);
    // Generated from the component: L5.Constant Power L5.Qscale1
    _l5_constant_power_l5_qscale1__out = 3.0 * _l5_constant_power_l5_qscale__out;
    // Generated from the component: L5.Constant Power L5.Vscale
    _l5_constant_power_l5_vscale__out = 0.5773502691896258 * _l5_constant_power_l5_calc_v_params__v_rms;
    // Generated from the component: L5.Constant Power L5.cpu_tr3.Input
    XIo_OutFloat(0x5500012c, _l5_constant_power_l5_calc_v_params__v_f);
    // Generated from the component: L5.Constant Power L5.f
    XIo_OutFloat(0x55000114, _l5_constant_power_l5_calc_v_params__v_f);
    // Generated from the component: L6.Constant Power L6.Qscale1
    _l6_constant_power_l6_qscale1__out = 3.0 * _l6_constant_power_l6_qscale__out;
    // Generated from the component: L6.Constant Power L6.Vscale
    _l6_constant_power_l6_vscale__out = 0.5773502691896258 * _l6_constant_power_l6_calc_v_params__v_rms;
    // Generated from the component: L6.Constant Power L6.cpu_tr3.Input
    XIo_OutFloat(0x55000134, _l6_constant_power_l6_calc_v_params__v_f);
    // Generated from the component: L6.Constant Power L6.f
    XIo_OutFloat(0x55000120, _l6_constant_power_l6_calc_v_params__v_f);
    // Generated from the component: L4.Constant Power L4.Q
    XIo_OutFloat(0x55000104, _l4_constant_power_l4_qscale1__out);
    // Generated from the component: L4.Constant Power L4.Snb_pwr
    _l4_constant_power_l4_snb_pwr__out = (_l4_constant_power_l4_vscale__out * _l4_constant_power_l4_vscale__out) * 1.0 / (_l4_constant_power_l4_rsnb_val__out);
    // Generated from the component: L5.Constant Power L5.Q
    XIo_OutFloat(0x55000110, _l5_constant_power_l5_qscale1__out);
    // Generated from the component: L5.Constant Power L5.Snb_pwr
    _l5_constant_power_l5_snb_pwr__out = (_l5_constant_power_l5_vscale__out * _l5_constant_power_l5_vscale__out) * 1.0 / (_l5_constant_power_l5_rsnb_val__out);
    // Generated from the component: L6.Constant Power L6.Q
    XIo_OutFloat(0x5500011c, _l6_constant_power_l6_qscale1__out);
    // Generated from the component: L6.Constant Power L6.Snb_pwr
    _l6_constant_power_l6_snb_pwr__out = (_l6_constant_power_l6_vscale__out * _l6_constant_power_l6_vscale__out) * 1.0 / (_l6_constant_power_l6_rsnb_val__out);
    // Generated from the component: L4.Constant Power L4.Pref_sum
    _l4_constant_power_l4_pref_sum__out = _l4_constant_power_l4_cpu_tr1_output__out - _l4_constant_power_l4_snb_pwr__out;
    // Generated from the component: L5.Constant Power L5.Pref_sum
    _l5_constant_power_l5_pref_sum__out = _l5_constant_power_l5_cpu_tr1_output__out - _l5_constant_power_l5_snb_pwr__out;
    // Generated from the component: L6.Constant Power L6.Pref_sum
    _l6_constant_power_l6_pref_sum__out = _l6_constant_power_l6_cpu_tr1_output__out - _l6_constant_power_l6_snb_pwr__out;
    // Generated from the component: L4.Constant Power L4.Pscale
    _l4_constant_power_l4_pscale__out = 0.3333333333333333 * _l4_constant_power_l4_pref_sum__out;
    // Generated from the component: L5.Constant Power L5.Pscale
    _l5_constant_power_l5_pscale__out = 0.3333333333333333 * _l5_constant_power_l5_pref_sum__out;
    // Generated from the component: L6.Constant Power L6.Pscale
    _l6_constant_power_l6_pscale__out = 0.3333333333333333 * _l6_constant_power_l6_pref_sum__out;
    // Generated from the component: L4.Constant Power L4.Pscale1
    _l4_constant_power_l4_pscale1__out = 3.0 * _l4_constant_power_l4_pscale__out;
    // Generated from the component: L4.Constant Power L4.power_src_slow
    _l4_constant_power_l4_power_src_slow__c_p = 10.0;
    _l4_constant_power_l4_power_src_slow__c_i = 2.0;
    _l4_constant_power_l4_power_src_slow__active = 0.0f;
    _l4_constant_power_l4_power_src_slow__s_ref = sqrtf((_l4_constant_power_l4_pscale__out * _l4_constant_power_l4_pscale__out) + (_l4_constant_power_l4_qscale__out * _l4_constant_power_l4_qscale__out));
    if (_l4_constant_power_l4_pscale__out != 0.0f) {
        _l4_constant_power_l4_power_src_slow__angle_ref = atanf(_l4_constant_power_l4_qscale__out / _l4_constant_power_l4_pscale__out);
    }
    else if (_l4_constant_power_l4_qscale__out < 0.0f) {
        _l4_constant_power_l4_power_src_slow__angle_ref = -M_PI / 2.0f;
    }
    else {
        _l4_constant_power_l4_power_src_slow__angle_ref = M_PI / 2.0f;
    }
    _l4_constant_power_l4_power_src_slow__angle_ref = _l4_constant_power_l4_power_src_slow__angle_ref + 0.5235987755982988; // compensate for line voltage phase shift
    if (_l4_constant_power_l4_pscale__out < 0) {
        _l4_constant_power_l4_power_src_slow__angle_ref = _l4_constant_power_l4_power_src_slow__angle_ref + M_PI; // power source mode
    }
    _l4_constant_power_l4_power_src_slow__i_init_phase = -_l4_constant_power_l4_power_src_slow__angle_ref;
    switch ( _l4_constant_power_l4_power_src_slow__state ) {
    case 0 :
        if ((_l4_constant_power_l4_rt4_output__out > 0.0f) && (_l4_constant_power_l4_calc_v_params__v_f >= 15.0) && (_l4_constant_power_l4_calc_v_params__v_f <= 100.0)) {
            _l4_constant_power_l4_power_src_slow__i_f_init = _l4_constant_power_l4_calc_v_params__v_f;
            _l4_constant_power_l4_power_src_slow__i_f = _l4_constant_power_l4_power_src_slow__i_f_init;
        }
        _l4_constant_power_l4_power_src_slow__i_rms = 1e-06;
        _l4_constant_power_l4_power_src_slow__int_err = 0.0f;
        break;
    case 1 :
        _l4_constant_power_l4_power_src_slow__i_rms = 1e-06;
        break;
    case 2 :
        _l4_constant_power_l4_power_src_slow__i_rms = 1e-06;
        _l4_constant_power_l4_power_src_slow__c_p = _l4_constant_power_l4_power_src_slow__c_p / 5.0f;
        _l4_constant_power_l4_power_src_slow__c_i = _l4_constant_power_l4_power_src_slow__c_i / 5.0f;
        break;
    default :
        if ((_l4_constant_power_l4_calc_v_params__v_f < 15.0) || (_l4_constant_power_l4_calc_v_params__v_f > 100.0) || (fabsf(_l4_constant_power_l4_power_src_slow__curr_err) > 0.25f) || ((_l4_constant_power_l4_calc_v_params__v_f - _l4_constant_power_l4_power_src_slow__v_f_old) > 3.0f)) {
            _l4_constant_power_l4_power_src_slow__i_rms = 1e-06;
        }
        else if (_l4_constant_power_l4_power_src_slow__s_ref == 0.0f) {
            _l4_constant_power_l4_power_src_slow__i_rms = 1e-06;
        }
        else if (_l4_constant_power_l4_vscale__out == 0.0f) {
            _l4_constant_power_l4_power_src_slow__i_rms = 100000.0;
        }
        else {
            _l4_constant_power_l4_power_src_slow__i_rms = _l4_constant_power_l4_power_src_slow__s_ref / _l4_constant_power_l4_vscale__out;
            if (_l4_constant_power_l4_power_src_slow__i_rms > 100000.0) {
                _l4_constant_power_l4_power_src_slow__i_rms = 100000.0;
            }
        }
        _l4_constant_power_l4_power_src_slow__active = 1.0f;
    }
    // current filter
    _l4_constant_power_l4_power_src_slow__i_rms = (_l4_constant_power_l4_power_src_slow__i_rms * 0.2f) + (_l4_constant_power_l4_power_src_slow__i_rms_old * 0.8f);
    _l4_constant_power_l4_power_src_slow__i_rms_old = _l4_constant_power_l4_power_src_slow__i_rms;
    if ((_l4_constant_power_l4_power_src_slow__state == 2) || (_l4_constant_power_l4_power_src_slow__state == 3)) {
        // phase regulation
        _l4_constant_power_l4_power_src_slow__phase_shift_ref_abs = (_l4_constant_power_l4_power_src_slow__angle_ref / (2.0f * M_PI)) * _l4_constant_power_l4_rt4_output__out;
        _l4_constant_power_l4_power_src_slow__abs_err = ((float)(_l4_constant_power_l4_rt3_output__out - _l4_constant_power_l4_rt2_output__out)) - _l4_constant_power_l4_power_src_slow__phase_shift_ref_abs;
        _l4_constant_power_l4_power_src_slow__abs_err = fmodf(_l4_constant_power_l4_power_src_slow__abs_err, _l4_constant_power_l4_rt4_output__out);
        if (_l4_constant_power_l4_rt4_output__out != 0) {
            _l4_constant_power_l4_power_src_slow__curr_err = _l4_constant_power_l4_power_src_slow__abs_err / _l4_constant_power_l4_rt4_output__out;
        }
        if (_l4_constant_power_l4_power_src_slow__curr_err > (0.5f)) {
            _l4_constant_power_l4_power_src_slow__curr_err = _l4_constant_power_l4_power_src_slow__curr_err - 1.0f;
        }
        if (_l4_constant_power_l4_power_src_slow__curr_err < (-0.5f)) {
            _l4_constant_power_l4_power_src_slow__curr_err = _l4_constant_power_l4_power_src_slow__curr_err + 1.0f;
        }
        // PI regulation
        _l4_constant_power_l4_power_src_slow__i_f_delta = 10.0 * _l4_constant_power_l4_power_src_slow__curr_err + 2.0 * _l4_constant_power_l4_power_src_slow__int_err;
        _l4_constant_power_l4_power_src_slow__int_err = _l4_constant_power_l4_power_src_slow__int_err + _l4_constant_power_l4_power_src_slow__curr_err;
        _l4_constant_power_l4_power_src_slow__i_f = _l4_constant_power_l4_power_src_slow__i_f_init + _l4_constant_power_l4_power_src_slow__i_f_delta;
    }
    _l4_constant_power_l4_power_src_slow__v_f_old = _l4_constant_power_l4_calc_v_params__v_f;
    // Generated from the component: L5.Constant Power L5.Pscale1
    _l5_constant_power_l5_pscale1__out = 3.0 * _l5_constant_power_l5_pscale__out;
    // Generated from the component: L5.Constant Power L5.power_src_slow
    _l5_constant_power_l5_power_src_slow__c_p = 10.0;
    _l5_constant_power_l5_power_src_slow__c_i = 2.0;
    _l5_constant_power_l5_power_src_slow__active = 0.0f;
    _l5_constant_power_l5_power_src_slow__s_ref = sqrtf((_l5_constant_power_l5_pscale__out * _l5_constant_power_l5_pscale__out) + (_l5_constant_power_l5_qscale__out * _l5_constant_power_l5_qscale__out));
    if (_l5_constant_power_l5_pscale__out != 0.0f) {
        _l5_constant_power_l5_power_src_slow__angle_ref = atanf(_l5_constant_power_l5_qscale__out / _l5_constant_power_l5_pscale__out);
    }
    else if (_l5_constant_power_l5_qscale__out < 0.0f) {
        _l5_constant_power_l5_power_src_slow__angle_ref = -M_PI / 2.0f;
    }
    else {
        _l5_constant_power_l5_power_src_slow__angle_ref = M_PI / 2.0f;
    }
    _l5_constant_power_l5_power_src_slow__angle_ref = _l5_constant_power_l5_power_src_slow__angle_ref + 0.5235987755982988; // compensate for line voltage phase shift
    if (_l5_constant_power_l5_pscale__out < 0) {
        _l5_constant_power_l5_power_src_slow__angle_ref = _l5_constant_power_l5_power_src_slow__angle_ref + M_PI; // power source mode
    }
    _l5_constant_power_l5_power_src_slow__i_init_phase = -_l5_constant_power_l5_power_src_slow__angle_ref;
    switch ( _l5_constant_power_l5_power_src_slow__state ) {
    case 0 :
        if ((_l5_constant_power_l5_rt4_output__out > 0.0f) && (_l5_constant_power_l5_calc_v_params__v_f >= 15.0) && (_l5_constant_power_l5_calc_v_params__v_f <= 100.0)) {
            _l5_constant_power_l5_power_src_slow__i_f_init = _l5_constant_power_l5_calc_v_params__v_f;
            _l5_constant_power_l5_power_src_slow__i_f = _l5_constant_power_l5_power_src_slow__i_f_init;
        }
        _l5_constant_power_l5_power_src_slow__i_rms = 1e-06;
        _l5_constant_power_l5_power_src_slow__int_err = 0.0f;
        break;
    case 1 :
        _l5_constant_power_l5_power_src_slow__i_rms = 1e-06;
        break;
    case 2 :
        _l5_constant_power_l5_power_src_slow__i_rms = 1e-06;
        _l5_constant_power_l5_power_src_slow__c_p = _l5_constant_power_l5_power_src_slow__c_p / 5.0f;
        _l5_constant_power_l5_power_src_slow__c_i = _l5_constant_power_l5_power_src_slow__c_i / 5.0f;
        break;
    default :
        if ((_l5_constant_power_l5_calc_v_params__v_f < 15.0) || (_l5_constant_power_l5_calc_v_params__v_f > 100.0) || (fabsf(_l5_constant_power_l5_power_src_slow__curr_err) > 0.25f) || ((_l5_constant_power_l5_calc_v_params__v_f - _l5_constant_power_l5_power_src_slow__v_f_old) > 3.0f)) {
            _l5_constant_power_l5_power_src_slow__i_rms = 1e-06;
        }
        else if (_l5_constant_power_l5_power_src_slow__s_ref == 0.0f) {
            _l5_constant_power_l5_power_src_slow__i_rms = 1e-06;
        }
        else if (_l5_constant_power_l5_vscale__out == 0.0f) {
            _l5_constant_power_l5_power_src_slow__i_rms = 100000.0;
        }
        else {
            _l5_constant_power_l5_power_src_slow__i_rms = _l5_constant_power_l5_power_src_slow__s_ref / _l5_constant_power_l5_vscale__out;
            if (_l5_constant_power_l5_power_src_slow__i_rms > 100000.0) {
                _l5_constant_power_l5_power_src_slow__i_rms = 100000.0;
            }
        }
        _l5_constant_power_l5_power_src_slow__active = 1.0f;
    }
    // current filter
    _l5_constant_power_l5_power_src_slow__i_rms = (_l5_constant_power_l5_power_src_slow__i_rms * 0.2f) + (_l5_constant_power_l5_power_src_slow__i_rms_old * 0.8f);
    _l5_constant_power_l5_power_src_slow__i_rms_old = _l5_constant_power_l5_power_src_slow__i_rms;
    if ((_l5_constant_power_l5_power_src_slow__state == 2) || (_l5_constant_power_l5_power_src_slow__state == 3)) {
        // phase regulation
        _l5_constant_power_l5_power_src_slow__phase_shift_ref_abs = (_l5_constant_power_l5_power_src_slow__angle_ref / (2.0f * M_PI)) * _l5_constant_power_l5_rt4_output__out;
        _l5_constant_power_l5_power_src_slow__abs_err = ((float)(_l5_constant_power_l5_rt3_output__out - _l5_constant_power_l5_rt2_output__out)) - _l5_constant_power_l5_power_src_slow__phase_shift_ref_abs;
        _l5_constant_power_l5_power_src_slow__abs_err = fmodf(_l5_constant_power_l5_power_src_slow__abs_err, _l5_constant_power_l5_rt4_output__out);
        if (_l5_constant_power_l5_rt4_output__out != 0) {
            _l5_constant_power_l5_power_src_slow__curr_err = _l5_constant_power_l5_power_src_slow__abs_err / _l5_constant_power_l5_rt4_output__out;
        }
        if (_l5_constant_power_l5_power_src_slow__curr_err > (0.5f)) {
            _l5_constant_power_l5_power_src_slow__curr_err = _l5_constant_power_l5_power_src_slow__curr_err - 1.0f;
        }
        if (_l5_constant_power_l5_power_src_slow__curr_err < (-0.5f)) {
            _l5_constant_power_l5_power_src_slow__curr_err = _l5_constant_power_l5_power_src_slow__curr_err + 1.0f;
        }
        // PI regulation
        _l5_constant_power_l5_power_src_slow__i_f_delta = 10.0 * _l5_constant_power_l5_power_src_slow__curr_err + 2.0 * _l5_constant_power_l5_power_src_slow__int_err;
        _l5_constant_power_l5_power_src_slow__int_err = _l5_constant_power_l5_power_src_slow__int_err + _l5_constant_power_l5_power_src_slow__curr_err;
        _l5_constant_power_l5_power_src_slow__i_f = _l5_constant_power_l5_power_src_slow__i_f_init + _l5_constant_power_l5_power_src_slow__i_f_delta;
    }
    _l5_constant_power_l5_power_src_slow__v_f_old = _l5_constant_power_l5_calc_v_params__v_f;
    // Generated from the component: L6.Constant Power L6.Pscale1
    _l6_constant_power_l6_pscale1__out = 3.0 * _l6_constant_power_l6_pscale__out;
    // Generated from the component: L6.Constant Power L6.power_src_slow
    _l6_constant_power_l6_power_src_slow__c_p = 10.0;
    _l6_constant_power_l6_power_src_slow__c_i = 2.0;
    _l6_constant_power_l6_power_src_slow__active = 0.0f;
    _l6_constant_power_l6_power_src_slow__s_ref = sqrtf((_l6_constant_power_l6_pscale__out * _l6_constant_power_l6_pscale__out) + (_l6_constant_power_l6_qscale__out * _l6_constant_power_l6_qscale__out));
    if (_l6_constant_power_l6_pscale__out != 0.0f) {
        _l6_constant_power_l6_power_src_slow__angle_ref = atanf(_l6_constant_power_l6_qscale__out / _l6_constant_power_l6_pscale__out);
    }
    else if (_l6_constant_power_l6_qscale__out < 0.0f) {
        _l6_constant_power_l6_power_src_slow__angle_ref = -M_PI / 2.0f;
    }
    else {
        _l6_constant_power_l6_power_src_slow__angle_ref = M_PI / 2.0f;
    }
    _l6_constant_power_l6_power_src_slow__angle_ref = _l6_constant_power_l6_power_src_slow__angle_ref + 0.5235987755982988; // compensate for line voltage phase shift
    if (_l6_constant_power_l6_pscale__out < 0) {
        _l6_constant_power_l6_power_src_slow__angle_ref = _l6_constant_power_l6_power_src_slow__angle_ref + M_PI; // power source mode
    }
    _l6_constant_power_l6_power_src_slow__i_init_phase = -_l6_constant_power_l6_power_src_slow__angle_ref;
    switch ( _l6_constant_power_l6_power_src_slow__state ) {
    case 0 :
        if ((_l6_constant_power_l6_rt4_output__out > 0.0f) && (_l6_constant_power_l6_calc_v_params__v_f >= 15.0) && (_l6_constant_power_l6_calc_v_params__v_f <= 100.0)) {
            _l6_constant_power_l6_power_src_slow__i_f_init = _l6_constant_power_l6_calc_v_params__v_f;
            _l6_constant_power_l6_power_src_slow__i_f = _l6_constant_power_l6_power_src_slow__i_f_init;
        }
        _l6_constant_power_l6_power_src_slow__i_rms = 1e-06;
        _l6_constant_power_l6_power_src_slow__int_err = 0.0f;
        break;
    case 1 :
        _l6_constant_power_l6_power_src_slow__i_rms = 1e-06;
        break;
    case 2 :
        _l6_constant_power_l6_power_src_slow__i_rms = 1e-06;
        _l6_constant_power_l6_power_src_slow__c_p = _l6_constant_power_l6_power_src_slow__c_p / 5.0f;
        _l6_constant_power_l6_power_src_slow__c_i = _l6_constant_power_l6_power_src_slow__c_i / 5.0f;
        break;
    default :
        if ((_l6_constant_power_l6_calc_v_params__v_f < 15.0) || (_l6_constant_power_l6_calc_v_params__v_f > 100.0) || (fabsf(_l6_constant_power_l6_power_src_slow__curr_err) > 0.25f) || ((_l6_constant_power_l6_calc_v_params__v_f - _l6_constant_power_l6_power_src_slow__v_f_old) > 3.0f)) {
            _l6_constant_power_l6_power_src_slow__i_rms = 1e-06;
        }
        else if (_l6_constant_power_l6_power_src_slow__s_ref == 0.0f) {
            _l6_constant_power_l6_power_src_slow__i_rms = 1e-06;
        }
        else if (_l6_constant_power_l6_vscale__out == 0.0f) {
            _l6_constant_power_l6_power_src_slow__i_rms = 100000.0;
        }
        else {
            _l6_constant_power_l6_power_src_slow__i_rms = _l6_constant_power_l6_power_src_slow__s_ref / _l6_constant_power_l6_vscale__out;
            if (_l6_constant_power_l6_power_src_slow__i_rms > 100000.0) {
                _l6_constant_power_l6_power_src_slow__i_rms = 100000.0;
            }
        }
        _l6_constant_power_l6_power_src_slow__active = 1.0f;
    }
    // current filter
    _l6_constant_power_l6_power_src_slow__i_rms = (_l6_constant_power_l6_power_src_slow__i_rms * 0.2f) + (_l6_constant_power_l6_power_src_slow__i_rms_old * 0.8f);
    _l6_constant_power_l6_power_src_slow__i_rms_old = _l6_constant_power_l6_power_src_slow__i_rms;
    if ((_l6_constant_power_l6_power_src_slow__state == 2) || (_l6_constant_power_l6_power_src_slow__state == 3)) {
        // phase regulation
        _l6_constant_power_l6_power_src_slow__phase_shift_ref_abs = (_l6_constant_power_l6_power_src_slow__angle_ref / (2.0f * M_PI)) * _l6_constant_power_l6_rt4_output__out;
        _l6_constant_power_l6_power_src_slow__abs_err = ((float)(_l6_constant_power_l6_rt3_output__out - _l6_constant_power_l6_rt2_output__out)) - _l6_constant_power_l6_power_src_slow__phase_shift_ref_abs;
        _l6_constant_power_l6_power_src_slow__abs_err = fmodf(_l6_constant_power_l6_power_src_slow__abs_err, _l6_constant_power_l6_rt4_output__out);
        if (_l6_constant_power_l6_rt4_output__out != 0) {
            _l6_constant_power_l6_power_src_slow__curr_err = _l6_constant_power_l6_power_src_slow__abs_err / _l6_constant_power_l6_rt4_output__out;
        }
        if (_l6_constant_power_l6_power_src_slow__curr_err > (0.5f)) {
            _l6_constant_power_l6_power_src_slow__curr_err = _l6_constant_power_l6_power_src_slow__curr_err - 1.0f;
        }
        if (_l6_constant_power_l6_power_src_slow__curr_err < (-0.5f)) {
            _l6_constant_power_l6_power_src_slow__curr_err = _l6_constant_power_l6_power_src_slow__curr_err + 1.0f;
        }
        // PI regulation
        _l6_constant_power_l6_power_src_slow__i_f_delta = 10.0 * _l6_constant_power_l6_power_src_slow__curr_err + 2.0 * _l6_constant_power_l6_power_src_slow__int_err;
        _l6_constant_power_l6_power_src_slow__int_err = _l6_constant_power_l6_power_src_slow__int_err + _l6_constant_power_l6_power_src_slow__curr_err;
        _l6_constant_power_l6_power_src_slow__i_f = _l6_constant_power_l6_power_src_slow__i_f_init + _l6_constant_power_l6_power_src_slow__i_f_delta;
    }
    _l6_constant_power_l6_power_src_slow__v_f_old = _l6_constant_power_l6_calc_v_params__v_f;
    // Generated from the component: L4.Constant Power L4.P
    XIo_OutFloat(0x55000100, _l4_constant_power_l4_pscale1__out);
    // Generated from the component: L4.Constant Power L4.active
    HIL_OutInt32(0xf00400, _l4_constant_power_l4_power_src_slow__active != 0x0);
    // Generated from the component: L4.Constant Power L4.cpu_tr4.Input
    XIo_OutFloat(0x55000128, _l4_constant_power_l4_power_src_slow__active);
    // Generated from the component: L4.Constant Power L4.m2
    // Generated from the component: L4.Constant Power L4.sg_ctrl_sine
    // calculate and write SG parameters
    // calculate amplitude
    _l4_constant_power_l4_sg_ctrl_sine__gain = _l4_constant_power_l4_power_src_slow__i_rms * M_SQRT2;
    // calculate period
    _l4_constant_power_l4_sg_ctrl_sine__period = 1.0f / _l4_constant_power_l4_power_src_slow__i_f;
    // calculate sample_cnt increment value
    _l4_constant_power_l4_sg_ctrl_sine__sample_cnt_inc_real = _l4_constant_power_l4_sg_ctrl_sine__max_int40 / (_l4_constant_power_l4_sg_ctrl_sine__period / 2e-06);
    _l4_constant_power_l4_sg_ctrl_sine__sample_cnt_inc_lo = (X_UnInt32)(fmodf(_l4_constant_power_l4_sg_ctrl_sine__sample_cnt_inc_real, _l4_constant_power_l4_sg_ctrl_sine__max_int32));
    _l4_constant_power_l4_sg_ctrl_sine__sample_cnt_inc_hi = (X_UnInt32)(_l4_constant_power_l4_sg_ctrl_sine__sample_cnt_inc_real / _l4_constant_power_l4_sg_ctrl_sine__max_int32);
    _l4_constant_power_l4_sg_ctrl_sine__sg_update_value = 0;
    for (_l4_constant_power_l4_sg_ctrl_sine__i = 0; _l4_constant_power_l4_sg_ctrl_sine__i < 2; _l4_constant_power_l4_sg_ctrl_sine__i++) {
        //calculate offset for each channel
        _l4_constant_power_l4_sg_ctrl_sine__shift = (float)(_l4_constant_power_l4_sg_ctrl_sine__i) * -2.0943951023931953;
        _l4_constant_power_l4_sg_ctrl_sine__phase_pu = fmodf((_l4_constant_power_l4_phase__out / 360.0f + _l4_constant_power_l4_sg_ctrl_sine__shift / (2.0f * M_PI)), 1.0f);
        if (_l4_constant_power_l4_sg_ctrl_sine__phase_pu < 0.0f) {
            _l4_constant_power_l4_sg_ctrl_sine__phase_pu = _l4_constant_power_l4_sg_ctrl_sine__phase_pu + 1.0f;
        }
        _l4_constant_power_l4_sg_ctrl_sine__offset = (_l4_constant_power_l4_sg_ctrl_sine__max_int32 * _l4_constant_power_l4_sg_ctrl_sine__phase_pu);  // offset in SP
        _l4_constant_power_l4_sg_ctrl_sine__offset_uint = (X_UnInt32)_l4_constant_power_l4_sg_ctrl_sine__offset;  // offset in fp32
        // write parameters
        HIL_OutFloat(0x400200 + _l4_constant_power_l4_sg_ctrl_sine__channels[_l4_constant_power_l4_sg_ctrl_sine__i], (float) _l4_constant_power_l4_sg_ctrl_sine__gain);
        HIL_OutInt32(0x400180 + _l4_constant_power_l4_sg_ctrl_sine__channels[_l4_constant_power_l4_sg_ctrl_sine__i], _l4_constant_power_l4_sg_ctrl_sine__offset_uint); // offset in fp32
        HIL_OutInt32(0x400000 + _l4_constant_power_l4_sg_ctrl_sine__channels[_l4_constant_power_l4_sg_ctrl_sine__i], _l4_constant_power_l4_sg_ctrl_sine__sample_cnt_inc_lo);
        HIL_OutInt32(0x400280 + _l4_constant_power_l4_sg_ctrl_sine__channels[_l4_constant_power_l4_sg_ctrl_sine__i], _l4_constant_power_l4_sg_ctrl_sine__sample_cnt_inc_hi);
        HIL_OutFloat(0x400100 + _l4_constant_power_l4_sg_ctrl_sine__channels[_l4_constant_power_l4_sg_ctrl_sine__i], (float) _l4_constant_power_l4_sg_ctrl_sine__no_of_points);
        _l4_constant_power_l4_sg_ctrl_sine__sg_update_value += 1 << _l4_constant_power_l4_sg_ctrl_sine__channels[_l4_constant_power_l4_sg_ctrl_sine__i];
    }
    HIL_OutInt32(0x400080, _l4_constant_power_l4_sg_ctrl_sine__sg_update_value);  // update SG parameters for all channels
    // Generated from the component: L4.Constant Power L4.t1
    // Generated from the component: L4.Constant Power L4.t2
    // Generated from the component: L4.Constant Power L4.t3
    // Generated from the component: L5.Constant Power L5.P
    XIo_OutFloat(0x5500010c, _l5_constant_power_l5_pscale1__out);
    // Generated from the component: L5.Constant Power L5.active
    HIL_OutInt32(0xf00401, _l5_constant_power_l5_power_src_slow__active != 0x0);
    // Generated from the component: L5.Constant Power L5.cpu_tr4.Input
    XIo_OutFloat(0x55000130, _l5_constant_power_l5_power_src_slow__active);
    // Generated from the component: L5.Constant Power L5.m2
    // Generated from the component: L5.Constant Power L5.sg_ctrl_sine
    // calculate and write SG parameters
    // calculate amplitude
    _l5_constant_power_l5_sg_ctrl_sine__gain = _l5_constant_power_l5_power_src_slow__i_rms * M_SQRT2;
    // calculate period
    _l5_constant_power_l5_sg_ctrl_sine__period = 1.0f / _l5_constant_power_l5_power_src_slow__i_f;
    // calculate sample_cnt increment value
    _l5_constant_power_l5_sg_ctrl_sine__sample_cnt_inc_real = _l5_constant_power_l5_sg_ctrl_sine__max_int40 / (_l5_constant_power_l5_sg_ctrl_sine__period / 2e-06);
    _l5_constant_power_l5_sg_ctrl_sine__sample_cnt_inc_lo = (X_UnInt32)(fmodf(_l5_constant_power_l5_sg_ctrl_sine__sample_cnt_inc_real, _l5_constant_power_l5_sg_ctrl_sine__max_int32));
    _l5_constant_power_l5_sg_ctrl_sine__sample_cnt_inc_hi = (X_UnInt32)(_l5_constant_power_l5_sg_ctrl_sine__sample_cnt_inc_real / _l5_constant_power_l5_sg_ctrl_sine__max_int32);
    _l5_constant_power_l5_sg_ctrl_sine__sg_update_value = 0;
    for (_l5_constant_power_l5_sg_ctrl_sine__i = 0; _l5_constant_power_l5_sg_ctrl_sine__i < 2; _l5_constant_power_l5_sg_ctrl_sine__i++) {
        //calculate offset for each channel
        _l5_constant_power_l5_sg_ctrl_sine__shift = (float)(_l5_constant_power_l5_sg_ctrl_sine__i) * -2.0943951023931953;
        _l5_constant_power_l5_sg_ctrl_sine__phase_pu = fmodf((_l5_constant_power_l5_phase__out / 360.0f + _l5_constant_power_l5_sg_ctrl_sine__shift / (2.0f * M_PI)), 1.0f);
        if (_l5_constant_power_l5_sg_ctrl_sine__phase_pu < 0.0f) {
            _l5_constant_power_l5_sg_ctrl_sine__phase_pu = _l5_constant_power_l5_sg_ctrl_sine__phase_pu + 1.0f;
        }
        _l5_constant_power_l5_sg_ctrl_sine__offset = (_l5_constant_power_l5_sg_ctrl_sine__max_int32 * _l5_constant_power_l5_sg_ctrl_sine__phase_pu);  // offset in SP
        _l5_constant_power_l5_sg_ctrl_sine__offset_uint = (X_UnInt32)_l5_constant_power_l5_sg_ctrl_sine__offset;  // offset in fp32
        // write parameters
        HIL_OutFloat(0x400200 + _l5_constant_power_l5_sg_ctrl_sine__channels[_l5_constant_power_l5_sg_ctrl_sine__i], (float) _l5_constant_power_l5_sg_ctrl_sine__gain);
        HIL_OutInt32(0x400180 + _l5_constant_power_l5_sg_ctrl_sine__channels[_l5_constant_power_l5_sg_ctrl_sine__i], _l5_constant_power_l5_sg_ctrl_sine__offset_uint); // offset in fp32
        HIL_OutInt32(0x400000 + _l5_constant_power_l5_sg_ctrl_sine__channels[_l5_constant_power_l5_sg_ctrl_sine__i], _l5_constant_power_l5_sg_ctrl_sine__sample_cnt_inc_lo);
        HIL_OutInt32(0x400280 + _l5_constant_power_l5_sg_ctrl_sine__channels[_l5_constant_power_l5_sg_ctrl_sine__i], _l5_constant_power_l5_sg_ctrl_sine__sample_cnt_inc_hi);
        HIL_OutFloat(0x400100 + _l5_constant_power_l5_sg_ctrl_sine__channels[_l5_constant_power_l5_sg_ctrl_sine__i], (float) _l5_constant_power_l5_sg_ctrl_sine__no_of_points);
        _l5_constant_power_l5_sg_ctrl_sine__sg_update_value += 1 << _l5_constant_power_l5_sg_ctrl_sine__channels[_l5_constant_power_l5_sg_ctrl_sine__i];
    }
    HIL_OutInt32(0x400080, _l5_constant_power_l5_sg_ctrl_sine__sg_update_value);  // update SG parameters for all channels
    // Generated from the component: L5.Constant Power L5.t1
    // Generated from the component: L5.Constant Power L5.t2
    // Generated from the component: L5.Constant Power L5.t3
    // Generated from the component: L6.Constant Power L6.P
    XIo_OutFloat(0x55000118, _l6_constant_power_l6_pscale1__out);
    // Generated from the component: L6.Constant Power L6.active
    HIL_OutInt32(0xf00402, _l6_constant_power_l6_power_src_slow__active != 0x0);
    // Generated from the component: L6.Constant Power L6.cpu_tr4.Input
    XIo_OutFloat(0x55000138, _l6_constant_power_l6_power_src_slow__active);
    // Generated from the component: L6.Constant Power L6.m2
    // Generated from the component: L6.Constant Power L6.sg_ctrl_sine
    // calculate and write SG parameters
    // calculate amplitude
    _l6_constant_power_l6_sg_ctrl_sine__gain = _l6_constant_power_l6_power_src_slow__i_rms * M_SQRT2;
    // calculate period
    _l6_constant_power_l6_sg_ctrl_sine__period = 1.0f / _l6_constant_power_l6_power_src_slow__i_f;
    // calculate sample_cnt increment value
    _l6_constant_power_l6_sg_ctrl_sine__sample_cnt_inc_real = _l6_constant_power_l6_sg_ctrl_sine__max_int40 / (_l6_constant_power_l6_sg_ctrl_sine__period / 2e-06);
    _l6_constant_power_l6_sg_ctrl_sine__sample_cnt_inc_lo = (X_UnInt32)(fmodf(_l6_constant_power_l6_sg_ctrl_sine__sample_cnt_inc_real, _l6_constant_power_l6_sg_ctrl_sine__max_int32));
    _l6_constant_power_l6_sg_ctrl_sine__sample_cnt_inc_hi = (X_UnInt32)(_l6_constant_power_l6_sg_ctrl_sine__sample_cnt_inc_real / _l6_constant_power_l6_sg_ctrl_sine__max_int32);
    _l6_constant_power_l6_sg_ctrl_sine__sg_update_value = 0;
    for (_l6_constant_power_l6_sg_ctrl_sine__i = 0; _l6_constant_power_l6_sg_ctrl_sine__i < 2; _l6_constant_power_l6_sg_ctrl_sine__i++) {
        //calculate offset for each channel
        _l6_constant_power_l6_sg_ctrl_sine__shift = (float)(_l6_constant_power_l6_sg_ctrl_sine__i) * -2.0943951023931953;
        _l6_constant_power_l6_sg_ctrl_sine__phase_pu = fmodf((_l6_constant_power_l6_phase__out / 360.0f + _l6_constant_power_l6_sg_ctrl_sine__shift / (2.0f * M_PI)), 1.0f);
        if (_l6_constant_power_l6_sg_ctrl_sine__phase_pu < 0.0f) {
            _l6_constant_power_l6_sg_ctrl_sine__phase_pu = _l6_constant_power_l6_sg_ctrl_sine__phase_pu + 1.0f;
        }
        _l6_constant_power_l6_sg_ctrl_sine__offset = (_l6_constant_power_l6_sg_ctrl_sine__max_int32 * _l6_constant_power_l6_sg_ctrl_sine__phase_pu);  // offset in SP
        _l6_constant_power_l6_sg_ctrl_sine__offset_uint = (X_UnInt32)_l6_constant_power_l6_sg_ctrl_sine__offset;  // offset in fp32
        // write parameters
        HIL_OutFloat(0x400200 + _l6_constant_power_l6_sg_ctrl_sine__channels[_l6_constant_power_l6_sg_ctrl_sine__i], (float) _l6_constant_power_l6_sg_ctrl_sine__gain);
        HIL_OutInt32(0x400180 + _l6_constant_power_l6_sg_ctrl_sine__channels[_l6_constant_power_l6_sg_ctrl_sine__i], _l6_constant_power_l6_sg_ctrl_sine__offset_uint); // offset in fp32
        HIL_OutInt32(0x400000 + _l6_constant_power_l6_sg_ctrl_sine__channels[_l6_constant_power_l6_sg_ctrl_sine__i], _l6_constant_power_l6_sg_ctrl_sine__sample_cnt_inc_lo);
        HIL_OutInt32(0x400280 + _l6_constant_power_l6_sg_ctrl_sine__channels[_l6_constant_power_l6_sg_ctrl_sine__i], _l6_constant_power_l6_sg_ctrl_sine__sample_cnt_inc_hi);
        HIL_OutFloat(0x400100 + _l6_constant_power_l6_sg_ctrl_sine__channels[_l6_constant_power_l6_sg_ctrl_sine__i], (float) _l6_constant_power_l6_sg_ctrl_sine__no_of_points);
        _l6_constant_power_l6_sg_ctrl_sine__sg_update_value += 1 << _l6_constant_power_l6_sg_ctrl_sine__channels[_l6_constant_power_l6_sg_ctrl_sine__i];
    }
    HIL_OutInt32(0x400080, _l6_constant_power_l6_sg_ctrl_sine__sg_update_value);  // update SG parameters for all channels
    // Generated from the component: L6.Constant Power L6.t1
    // Generated from the component: L6.Constant Power L6.t2
    // Generated from the component: L6.Constant Power L6.t3
    //@cmp.out.block.end
    //////////////////////////////////////////////////////////////////////////
    // Update block
    //////////////////////////////////////////////////////////////////////////
    //@cmp.update.block.start
    // Generated from the component: L4.Constant Power L4.power_src_slow
    switch ( _l4_constant_power_l4_power_src_slow__state ) {
    case 0 :
        if ((_l4_constant_power_l4_rt4_output__out > 0.0f) && (_l4_constant_power_l4_calc_v_params__v_f >= 15.0) && (_l4_constant_power_l4_calc_v_params__v_f <= 100.0)) {
            _l4_constant_power_l4_power_src_slow__state = 1;
        }
        _l4_constant_power_l4_power_src_slow__lock_cnt = 0;
        break;
    case 1 :
        if (_l4_constant_power_l4_rt3_output__out > 0) {
            _l4_constant_power_l4_power_src_slow__state = 2;
        }
        break;
    case 2 :
        if ((_l4_constant_power_l4_calc_v_params__v_f < 15.0) || (_l4_constant_power_l4_calc_v_params__v_f > 100.0)) {
            _l4_constant_power_l4_power_src_slow__state = 0;
        }
        else if (_l4_constant_power_l4_power_src_slow__lock_cnt >= 30) {
            _l4_constant_power_l4_power_src_slow__state = 3;
        }
        _l4_constant_power_l4_power_src_slow__lock_cnt ++;
        break;
    default :
        if ((_l4_constant_power_l4_calc_v_params__v_f < 15.0) || (_l4_constant_power_l4_calc_v_params__v_f > 100.0) || (fabsf(_l4_constant_power_l4_power_src_slow__curr_err) > 0.25f) || ((_l4_constant_power_l4_calc_v_params__v_f - _l4_constant_power_l4_power_src_slow__v_f_old) > 3.0f)) {
            _l4_constant_power_l4_power_src_slow__state = 0;
        }
    }
    // Generated from the component: L5.Constant Power L5.power_src_slow
    switch ( _l5_constant_power_l5_power_src_slow__state ) {
    case 0 :
        if ((_l5_constant_power_l5_rt4_output__out > 0.0f) && (_l5_constant_power_l5_calc_v_params__v_f >= 15.0) && (_l5_constant_power_l5_calc_v_params__v_f <= 100.0)) {
            _l5_constant_power_l5_power_src_slow__state = 1;
        }
        _l5_constant_power_l5_power_src_slow__lock_cnt = 0;
        break;
    case 1 :
        if (_l5_constant_power_l5_rt3_output__out > 0) {
            _l5_constant_power_l5_power_src_slow__state = 2;
        }
        break;
    case 2 :
        if ((_l5_constant_power_l5_calc_v_params__v_f < 15.0) || (_l5_constant_power_l5_calc_v_params__v_f > 100.0)) {
            _l5_constant_power_l5_power_src_slow__state = 0;
        }
        else if (_l5_constant_power_l5_power_src_slow__lock_cnt >= 30) {
            _l5_constant_power_l5_power_src_slow__state = 3;
        }
        _l5_constant_power_l5_power_src_slow__lock_cnt ++;
        break;
    default :
        if ((_l5_constant_power_l5_calc_v_params__v_f < 15.0) || (_l5_constant_power_l5_calc_v_params__v_f > 100.0) || (fabsf(_l5_constant_power_l5_power_src_slow__curr_err) > 0.25f) || ((_l5_constant_power_l5_calc_v_params__v_f - _l5_constant_power_l5_power_src_slow__v_f_old) > 3.0f)) {
            _l5_constant_power_l5_power_src_slow__state = 0;
        }
    }
    // Generated from the component: L6.Constant Power L6.power_src_slow
    switch ( _l6_constant_power_l6_power_src_slow__state ) {
    case 0 :
        if ((_l6_constant_power_l6_rt4_output__out > 0.0f) && (_l6_constant_power_l6_calc_v_params__v_f >= 15.0) && (_l6_constant_power_l6_calc_v_params__v_f <= 100.0)) {
            _l6_constant_power_l6_power_src_slow__state = 1;
        }
        _l6_constant_power_l6_power_src_slow__lock_cnt = 0;
        break;
    case 1 :
        if (_l6_constant_power_l6_rt3_output__out > 0) {
            _l6_constant_power_l6_power_src_slow__state = 2;
        }
        break;
    case 2 :
        if ((_l6_constant_power_l6_calc_v_params__v_f < 15.0) || (_l6_constant_power_l6_calc_v_params__v_f > 100.0)) {
            _l6_constant_power_l6_power_src_slow__state = 0;
        }
        else if (_l6_constant_power_l6_power_src_slow__lock_cnt >= 30) {
            _l6_constant_power_l6_power_src_slow__state = 3;
        }
        _l6_constant_power_l6_power_src_slow__lock_cnt ++;
        break;
    default :
        if ((_l6_constant_power_l6_calc_v_params__v_f < 15.0) || (_l6_constant_power_l6_calc_v_params__v_f > 100.0) || (fabsf(_l6_constant_power_l6_power_src_slow__curr_err) > 0.25f) || ((_l6_constant_power_l6_calc_v_params__v_f - _l6_constant_power_l6_power_src_slow__v_f_old) > 3.0f)) {
            _l6_constant_power_l6_power_src_slow__state = 0;
        }
    }
    //@cmp.update.block.end
}
// ----------------------------------------------------------------------------------------  //-----------------------------------------------------------------------------------------