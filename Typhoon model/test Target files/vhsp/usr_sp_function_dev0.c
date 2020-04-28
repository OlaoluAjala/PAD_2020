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






























































































































//@cmp.def.end


//-----------------------------------------------------------------------------------------
// generated using template: common_variables.template-------------------------------------
// true global variables


//@cmp.var.start
// variables
double _modbus1_c_u11__out = 1.0;
double _modbus1_c_u12__out = 1.0;
double _modbus1_c_u13__out = 50.0;
double _modbus1_constant1__out = 10000.0;
double _modbus1_constant2__out = 0.0001;
double _modbus1_constant3__out = 10000.0;
double _modbus1_constant4__out = 0.0001;
double _modbus1_constant5__out = 10000.0;
double _modbus1_constant6__out = 10000.0;
double _modbus1_modbus_device1__config1__comp_coil_out__out;
double _modbus1_modbus_device1__config1__comp_holding_out__out[4];
double _modbus2_c_u11__out = 1.0;
double _modbus2_c_u12__out = 1.0;
double _modbus2_c_u13__out = 50.0;
double _modbus2_constant1__out = 10000.0;
double _modbus2_constant2__out = 0.0001;
double _modbus2_constant3__out = 10000.0;
double _modbus2_constant4__out = 0.0001;
double _modbus2_constant5__out = 10000.0;
double _modbus2_constant6__out = 10000.0;
double _modbus2_modbus_device2__config2__comp_coil_out__out;
double _modbus2_modbus_device2__config2__comp_holding_out__out[4];
double _modbus3_c_u11__out = 1.0;
double _modbus3_c_u12__out = 1.0;
double _modbus3_c_u13__out = 50.0;
double _modbus3_constant1__out = 10000.0;
double _modbus3_constant2__out = 0.0001;
double _modbus3_constant3__out = 10000.0;
double _modbus3_constant4__out = 0.0001;
double _modbus3_constant5__out = 10000.0;
double _modbus3_constant6__out = 10000.0;
double _modbus3_modbus_device3__config3__comp_coil_out__out;
double _modbus3_modbus_device3__config3__comp_holding_out__out[4];
double _flow_flag1__out;
double _flow_flag2__out;
double _flow_flag3__out;
double _freq_error1__out;
double _freq_error2__out;
double _freq_error3__out;
double _q_level1__out;
double _q_level2__out;
double _q_level3__out;
double _volt_error1__out;
double _volt_error2__out;
double _volt_error3__out;
double _modbus1_bus_split1__out;
double _modbus1_bus_split1__out1;
double _modbus1_bus_split1__out2;
double _modbus1_bus_split1__out3;
double _modbus2_bus_split1__out;
double _modbus2_bus_split1__out1;
double _modbus2_bus_split1__out2;
double _modbus2_bus_split1__out3;
double _modbus3_bus_split1__out;
double _modbus3_bus_split1__out1;
double _modbus3_bus_split1__out2;
double _modbus3_bus_split1__out3;
double _modbus1_product5__out;
double _modbus2_product5__out;
double _modbus3_product5__out;
double _modbus1_product1__out;
double _modbus2_product1__out;
double _modbus3_product1__out;
double _modbus1_product6__out;
double _modbus2_product6__out;
double _modbus3_product6__out;
double _modbus1_product3__out;
double _modbus2_product3__out;
double _modbus3_product3__out;
double _modbus1_c_function2__in;
double _modbus1_c_function2__sign;


double _modbus1_c_function2__out;
double _modbus1_c_function4__in;
double _modbus1_c_function4__sign;


double _modbus1_c_function4__out;
double _modbus2_c_function2__in;
double _modbus2_c_function2__sign;


double _modbus2_c_function2__out;
double _modbus2_c_function4__in;
double _modbus2_c_function4__sign;


double _modbus2_c_function4__out;
double _modbus3_c_function2__in;
double _modbus3_c_function2__sign;


double _modbus3_c_function2__out;
double _modbus3_c_function4__in;
double _modbus3_c_function4__sign;


double _modbus3_c_function4__out;
X_Int32 _modbus1_c_function5__in;


X_Int32 _modbus1_c_function5__out;
X_Int32 _modbus1_c_function5__sign;
X_Int32 _modbus2_c_function5__in;


X_Int32 _modbus2_c_function5__out;
X_Int32 _modbus2_c_function5__sign;
X_Int32 _modbus3_c_function5__in;


X_Int32 _modbus3_c_function5__out;
X_Int32 _modbus3_c_function5__sign;
X_Int32 _modbus1_c_function1__in;


X_Int32 _modbus1_c_function1__out;
X_Int32 _modbus1_c_function1__sign;
X_Int32 _modbus2_c_function1__in;


X_Int32 _modbus2_c_function1__out;
X_Int32 _modbus2_c_function1__sign;
X_Int32 _modbus3_c_function1__in;


X_Int32 _modbus3_c_function1__out;
X_Int32 _modbus3_c_function1__sign;
X_Int32 _modbus1_c_function6__in;


X_Int32 _modbus1_c_function6__out;
X_Int32 _modbus1_c_function6__sign;
X_Int32 _modbus2_c_function6__in;


X_Int32 _modbus2_c_function6__out;
X_Int32 _modbus2_c_function6__sign;
X_Int32 _modbus3_c_function6__in;


X_Int32 _modbus3_c_function6__out;
X_Int32 _modbus3_c_function6__sign;
X_Int32 _modbus1_c_function3__in;


X_Int32 _modbus1_c_function3__out;
X_Int32 _modbus1_c_function3__sign;
X_Int32 _modbus2_c_function3__in;


X_Int32 _modbus2_c_function3__out;
X_Int32 _modbus2_c_function3__sign;
X_Int32 _modbus3_c_function3__in;


X_Int32 _modbus3_c_function3__out;
X_Int32 _modbus3_c_function3__sign;
double _modbus1_product2__out;
double _modbus1_product4__out;
double _modbus2_product2__out;
double _modbus2_product4__out;
double _modbus3_product2__out;
double _modbus3_product4__out;
X_Int32 _modbus1_bus_join2__out[8];
X_Int32 _modbus2_bus_join2__out[8];
X_Int32 _modbus3_bus_join2__out[8];
//@cmp.var.end

//@cmp.svar.start
// state variables
//@cmp.svar.end



// generated using template: virtual_hil/custom_functions.template---------------------------------
void ReInit_user_sp_cpu_dev0() {
#if DEBUG_MODE
    printf("\n\rReInitTimer");
#endif
    //@cmp.init.block.start
    XIo_OutFloat(0x55000130, 0.0);
    XIo_OutFloat(0x55000134, 0.0);
    XIo_OutFloat(0x55000138, 0.0);
    XIo_OutFloat(0x5500013c, 0.0);
    XIo_OutFloat(0x55000140, 0.0);
    XIo_OutFloat(0x55000144, 0.0);
    XIo_OutFloat(0x55000148, 0.0);
    XIo_OutFloat(0x5500014c, 0.0);
    XIo_OutFloat(0x55000150, 0.0);
    XIo_OutFloat(0x55000154, 0.0);
    XIo_OutFloat(0x55000158, 0.0);
    XIo_OutFloat(0x5500015c, 0.0);
    XIo_OutFloat(0x55000160, 0.0);
    XIo_OutFloat(0x55000164, 0.0);
    XIo_OutFloat(0x55000168, 0.0);
    XIo_OutFloat(0x5500016c, 0.0f);
    XIo_OutFloat(0x55000170, 0.0f);
    XIo_OutFloat(0x55000174, 0.0f);
    XIo_OutFloat(0x55000198, 0.0f);
    XIo_OutFloat(0x5500019c, 0.0f);
    XIo_OutFloat(0x550001a0, 0.0f);
    XIo_OutFloat(0x550001c4, 0.0f);
    XIo_OutFloat(0x550001c8, 0.0f);
    XIo_OutFloat(0x550001cc, 0.0f);
    HIL_OutAO(0x4001, 0.0f);
    HIL_OutAO(0x4002, 0.0f);
    HIL_OutAO(0x4005, 0.0f);
    HIL_OutAO(0x400b, 0.0f);
    HIL_OutAO(0x4011, 0.0f);
    HIL_OutAO(0x4000, 0.0f);
    HIL_OutAO(0x4006, 0.0f);
    HIL_OutAO(0x400c, 0.0f);
    HIL_OutAO(0x4003, 0.0f);
    HIL_OutAO(0x4009, 0.0f);
    HIL_OutAO(0x400f, 0.0f);
    HIL_OutAO(0x4004, 0.0f);
    HIL_OutAO(0x400a, 0.0f);
    HIL_OutAO(0x4010, 0.0f);
    HIL_OutAO(0x4007, 0.0f);
    HIL_OutAO(0x4008, 0.0f);
    HIL_OutAO(0x400d, 0.0f);
    HIL_OutAO(0x400e, 0.0f);
    HIL_OutAO(0x4012, 0.0f);
    HIL_OutAO(0x4015, 0.0f);
    HIL_OutAO(0x4013, 0.0f);
    HIL_OutAO(0x4016, 0.0f);
    HIL_OutAO(0x4014, 0.0f);
    HIL_OutAO(0x4017, 0.0f);
    XIo_OutInt32(0x55000178, 0);
    XIo_OutInt32(0x550001a4, 0);
    XIo_OutInt32(0x550001d0, 0);
    //@cmp.init.block.end
}

void ReInit_sp_scope_user_sp_cpu_dev0() {
    // initialise SP Scope buffer pointer
}

void init_fmu_objects_dev0(void) {
    return;
}


void terminate_fmu_objects_dev0(void) {
    return;
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

void TimerCounterHandler_0_user_sp_cpu_dev0() {
#if DEBUG_MODE
    printf("\n\rTimerCounterHandler_0");
#endif
    //////////////////////////////////////////////////////////////////////////
    // Output block
    //////////////////////////////////////////////////////////////////////////
    //@cmp.out.block.start
    // Generated from the component: Modbus1.C_u11
    // Generated from the component: Modbus1.C_u12
    // Generated from the component: Modbus1.C_u13
    // Generated from the component: Modbus1.Constant1
    // Generated from the component: Modbus1.Constant2
    // Generated from the component: Modbus1.Constant3
    // Generated from the component: Modbus1.Constant4
    // Generated from the component: Modbus1.Constant5
    // Generated from the component: Modbus1.Constant6
    // Generated from the component: Modbus1.ModBus Device1 (config1).comp_coil_out
    _modbus1_modbus_device1__config1__comp_coil_out__out = XIo_InFloat(0x55000130);
    // Generated from the component: Modbus1.ModBus Device1 (config1).comp_holding_out
    _modbus1_modbus_device1__config1__comp_holding_out__out[0] = XIo_InFloat(0x55000134);
    _modbus1_modbus_device1__config1__comp_holding_out__out[1] = XIo_InFloat(0x55000138);
    _modbus1_modbus_device1__config1__comp_holding_out__out[2] = XIo_InFloat(0x5500013c);
    _modbus1_modbus_device1__config1__comp_holding_out__out[3] = XIo_InFloat(0x55000140);
    // Generated from the component: Modbus2.C_u11
    // Generated from the component: Modbus2.C_u12
    // Generated from the component: Modbus2.C_u13
    // Generated from the component: Modbus2.Constant1
    // Generated from the component: Modbus2.Constant2
    // Generated from the component: Modbus2.Constant3
    // Generated from the component: Modbus2.Constant4
    // Generated from the component: Modbus2.Constant5
    // Generated from the component: Modbus2.Constant6
    // Generated from the component: Modbus2.ModBus Device2 (config2).comp_coil_out
    _modbus2_modbus_device2__config2__comp_coil_out__out = XIo_InFloat(0x55000144);
    // Generated from the component: Modbus2.ModBus Device2 (config2).comp_holding_out
    _modbus2_modbus_device2__config2__comp_holding_out__out[0] = XIo_InFloat(0x55000148);
    _modbus2_modbus_device2__config2__comp_holding_out__out[1] = XIo_InFloat(0x5500014c);
    _modbus2_modbus_device2__config2__comp_holding_out__out[2] = XIo_InFloat(0x55000150);
    _modbus2_modbus_device2__config2__comp_holding_out__out[3] = XIo_InFloat(0x55000154);
    // Generated from the component: Modbus3.C_u11
    // Generated from the component: Modbus3.C_u12
    // Generated from the component: Modbus3.C_u13
    // Generated from the component: Modbus3.Constant1
    // Generated from the component: Modbus3.Constant2
    // Generated from the component: Modbus3.Constant3
    // Generated from the component: Modbus3.Constant4
    // Generated from the component: Modbus3.Constant5
    // Generated from the component: Modbus3.Constant6
    // Generated from the component: Modbus3.ModBus Device3 (config3).comp_coil_out
    _modbus3_modbus_device3__config3__comp_coil_out__out = XIo_InFloat(0x55000158);
    // Generated from the component: Modbus3.ModBus Device3 (config3).comp_holding_out
    _modbus3_modbus_device3__config3__comp_holding_out__out[0] = XIo_InFloat(0x5500015c);
    _modbus3_modbus_device3__config3__comp_holding_out__out[1] = XIo_InFloat(0x55000160);
    _modbus3_modbus_device3__config3__comp_holding_out__out[2] = XIo_InFloat(0x55000164);
    _modbus3_modbus_device3__config3__comp_holding_out__out[3] = XIo_InFloat(0x55000168);
    // Generated from the component: flow_flag1
    _flow_flag1__out = XIo_InFloat(0x55000100);
    // Generated from the component: flow_flag2
    _flow_flag2__out = XIo_InFloat(0x55000104);
    // Generated from the component: flow_flag3
    _flow_flag3__out = XIo_InFloat(0x55000108);
    // Generated from the component: freq_error1
    _freq_error1__out = XIo_InFloat(0x5500010c);
    // Generated from the component: freq_error2
    _freq_error2__out = XIo_InFloat(0x55000110);
    // Generated from the component: freq_error3
    _freq_error3__out = XIo_InFloat(0x55000114);
    // Generated from the component: q_level1
    _q_level1__out = XIo_InFloat(0x55000118);
    // Generated from the component: q_level2
    _q_level2__out = XIo_InFloat(0x5500011c);
    // Generated from the component: q_level3
    _q_level3__out = XIo_InFloat(0x55000120);
    // Generated from the component: volt_error1
    _volt_error1__out = XIo_InFloat(0x55000124);
    // Generated from the component: volt_error2
    _volt_error2__out = XIo_InFloat(0x55000128);
    // Generated from the component: volt_error3
    _volt_error3__out = XIo_InFloat(0x5500012c);
    // Generated from the component: Modbus1.ModBus Device1 (config1).comp_coil_in
    XIo_OutFloat(0x5500016c, _modbus1_c_u11__out);
    // Generated from the component: Modbus1.ModBus Device1 (config1).comp_discrete_in
    XIo_OutFloat(0x55000170, _modbus1_c_u12__out);
    // Generated from the component: Modbus1.ModBus Device1 (config1).comp_holding_in
    XIo_OutFloat(0x55000174, _modbus1_c_u13__out);
    // Generated from the component: Modbus1.Termination1
    // Generated from the component: Modbus1.Bus Split1
    _modbus1_bus_split1__out = _modbus1_modbus_device1__config1__comp_holding_out__out[0];
    _modbus1_bus_split1__out1 = _modbus1_modbus_device1__config1__comp_holding_out__out[1];
    _modbus1_bus_split1__out2 = _modbus1_modbus_device1__config1__comp_holding_out__out[2];
    _modbus1_bus_split1__out3 = _modbus1_modbus_device1__config1__comp_holding_out__out[3];
    // Generated from the component: Modbus2.ModBus Device2 (config2).comp_coil_in
    XIo_OutFloat(0x55000198, _modbus2_c_u11__out);
    // Generated from the component: Modbus2.ModBus Device2 (config2).comp_discrete_in
    XIo_OutFloat(0x5500019c, _modbus2_c_u12__out);
    // Generated from the component: Modbus2.ModBus Device2 (config2).comp_holding_in
    XIo_OutFloat(0x550001a0, _modbus2_c_u13__out);
    // Generated from the component: Modbus2.Termination1
    // Generated from the component: Modbus2.Bus Split1
    _modbus2_bus_split1__out = _modbus2_modbus_device2__config2__comp_holding_out__out[0];
    _modbus2_bus_split1__out1 = _modbus2_modbus_device2__config2__comp_holding_out__out[1];
    _modbus2_bus_split1__out2 = _modbus2_modbus_device2__config2__comp_holding_out__out[2];
    _modbus2_bus_split1__out3 = _modbus2_modbus_device2__config2__comp_holding_out__out[3];
    // Generated from the component: Modbus3.ModBus Device3 (config3).comp_coil_in
    XIo_OutFloat(0x550001c4, _modbus3_c_u11__out);
    // Generated from the component: Modbus3.ModBus Device3 (config3).comp_discrete_in
    XIo_OutFloat(0x550001c8, _modbus3_c_u12__out);
    // Generated from the component: Modbus3.ModBus Device3 (config3).comp_holding_in
    XIo_OutFloat(0x550001cc, _modbus3_c_u13__out);
    // Generated from the component: Modbus3.Termination1
    // Generated from the component: Modbus3.Bus Split1
    _modbus3_bus_split1__out = _modbus3_modbus_device3__config3__comp_holding_out__out[0];
    _modbus3_bus_split1__out1 = _modbus3_modbus_device3__config3__comp_holding_out__out[1];
    _modbus3_bus_split1__out2 = _modbus3_modbus_device3__config3__comp_holding_out__out[2];
    _modbus3_bus_split1__out3 = _modbus3_modbus_device3__config3__comp_holding_out__out[3];
    // Generated from the component: Modbus1.Product5
    _modbus1_product5__out = (_flow_flag1__out * _modbus1_constant5__out);
    // Generated from the component: Modbus2.Product5
    _modbus2_product5__out = (_flow_flag2__out * _modbus2_constant5__out);
    // Generated from the component: Modbus3.Product5
    _modbus3_product5__out = (_flow_flag3__out * _modbus3_constant5__out);
    // Generated from the component: Modbus1.Product1
    _modbus1_product1__out = (_freq_error1__out * _modbus1_constant1__out);
    // Generated from the component: Modbus2.Product1
    _modbus2_product1__out = (_freq_error2__out * _modbus2_constant1__out);
    // Generated from the component: Modbus3.Product1
    _modbus3_product1__out = (_freq_error3__out * _modbus3_constant1__out);
    // Generated from the component: Modbus1.Product6
    _modbus1_product6__out = (_q_level1__out * _modbus1_constant6__out);
    // Generated from the component: Modbus2.Product6
    _modbus2_product6__out = (_q_level2__out * _modbus2_constant6__out);
    // Generated from the component: Modbus3.Product6
    _modbus3_product6__out = (_q_level3__out * _modbus3_constant6__out);
    // Generated from the component: Modbus1.Product3
    _modbus1_product3__out = (_volt_error1__out * _modbus1_constant3__out);
    // Generated from the component: Modbus2.Product3
    _modbus2_product3__out = (_volt_error2__out * _modbus2_constant3__out);
    // Generated from the component: Modbus3.Product3
    _modbus3_product3__out = (_volt_error3__out * _modbus3_constant3__out);
    // Generated from the component: Modbus1.C function2
    _modbus1_c_function2__in = _modbus1_bus_split1__out1;
    _modbus1_c_function2__sign = _modbus1_bus_split1__out;
    _modbus1_c_function2__out = _modbus1_c_function2__in * ((-2 * _modbus1_c_function2__sign) + 1);
    // Generated from the component: Modbus1.C function4
    _modbus1_c_function4__in = _modbus1_bus_split1__out3;
    _modbus1_c_function4__sign = _modbus1_bus_split1__out2;
    _modbus1_c_function4__out = _modbus1_c_function4__in * ((-2 * _modbus1_c_function4__sign) + 1);
    // Generated from the component: Modbus1.Probe2
    HIL_OutAO(0x4001, (float)_modbus1_bus_split1__out1);
    // Generated from the component: Modbus1.Probe3
    HIL_OutAO(0x4002, (float)_modbus1_bus_split1__out3);
    // Generated from the component: Modbus2.C function2
    _modbus2_c_function2__in = _modbus2_bus_split1__out1;
    _modbus2_c_function2__sign = _modbus2_bus_split1__out;
    _modbus2_c_function2__out = _modbus2_c_function2__in * ((-2 * _modbus2_c_function2__sign) + 1);
    // Generated from the component: Modbus2.C function4
    _modbus2_c_function4__in = _modbus2_bus_split1__out3;
    _modbus2_c_function4__sign = _modbus2_bus_split1__out2;
    _modbus2_c_function4__out = _modbus2_c_function4__in * ((-2 * _modbus2_c_function4__sign) + 1);
    // Generated from the component: Modbus3.C function2
    _modbus3_c_function2__in = _modbus3_bus_split1__out1;
    _modbus3_c_function2__sign = _modbus3_bus_split1__out;
    _modbus3_c_function2__out = _modbus3_c_function2__in * ((-2 * _modbus3_c_function2__sign) + 1);
    // Generated from the component: Modbus3.C function4
    _modbus3_c_function4__in = _modbus3_bus_split1__out3;
    _modbus3_c_function4__sign = _modbus3_bus_split1__out2;
    _modbus3_c_function4__out = _modbus3_c_function4__in * ((-2 * _modbus3_c_function4__sign) + 1);
    // Generated from the component: Modbus1.C function5
    _modbus1_c_function5__in = _modbus1_product5__out;
    if (_modbus1_c_function5__in >= 0) {
        _modbus1_c_function5__out = _modbus1_c_function5__in;
        _modbus1_c_function5__sign = 0;
    }
    else {
        _modbus1_c_function5__out = _modbus1_c_function5__in * -1;
        _modbus1_c_function5__sign = 1;
    }
    // Generated from the component: Modbus1.flow
    HIL_OutAO(0x4005, (float)_modbus1_product5__out);
    // Generated from the component: Modbus2.C function5
    _modbus2_c_function5__in = _modbus2_product5__out;
    if (_modbus2_c_function5__in >= 0) {
        _modbus2_c_function5__out = _modbus2_c_function5__in;
        _modbus2_c_function5__sign = 0;
    }
    else {
        _modbus2_c_function5__out = _modbus2_c_function5__in * -1;
        _modbus2_c_function5__sign = 1;
    }
    // Generated from the component: Modbus2.flow
    HIL_OutAO(0x400b, (float)_modbus2_product5__out);
    // Generated from the component: Modbus3.C function5
    _modbus3_c_function5__in = _modbus3_product5__out;
    if (_modbus3_c_function5__in >= 0) {
        _modbus3_c_function5__out = _modbus3_c_function5__in;
        _modbus3_c_function5__sign = 0;
    }
    else {
        _modbus3_c_function5__out = _modbus3_c_function5__in * -1;
        _modbus3_c_function5__sign = 1;
    }
    // Generated from the component: Modbus3.flow
    HIL_OutAO(0x4011, (float)_modbus3_product5__out);
    // Generated from the component: Modbus1.C function1
    _modbus1_c_function1__in = _modbus1_product1__out;
    if (_modbus1_c_function1__in >= 0) {
        _modbus1_c_function1__out = _modbus1_c_function1__in;
        _modbus1_c_function1__sign = 0;
    }
    else {
        _modbus1_c_function1__out = _modbus1_c_function1__in * -1;
        _modbus1_c_function1__sign = 1;
    }
    // Generated from the component: Modbus1.Probe1
    HIL_OutAO(0x4000, (float)_modbus1_product1__out);
    // Generated from the component: Modbus2.C function1
    _modbus2_c_function1__in = _modbus2_product1__out;
    if (_modbus2_c_function1__in >= 0) {
        _modbus2_c_function1__out = _modbus2_c_function1__in;
        _modbus2_c_function1__sign = 0;
    }
    else {
        _modbus2_c_function1__out = _modbus2_c_function1__in * -1;
        _modbus2_c_function1__sign = 1;
    }
    // Generated from the component: Modbus2.Probe1
    HIL_OutAO(0x4006, (float)_modbus2_product1__out);
    // Generated from the component: Modbus3.C function1
    _modbus3_c_function1__in = _modbus3_product1__out;
    if (_modbus3_c_function1__in >= 0) {
        _modbus3_c_function1__out = _modbus3_c_function1__in;
        _modbus3_c_function1__sign = 0;
    }
    else {
        _modbus3_c_function1__out = _modbus3_c_function1__in * -1;
        _modbus3_c_function1__sign = 1;
    }
    // Generated from the component: Modbus3.Probe1
    HIL_OutAO(0x400c, (float)_modbus3_product1__out);
    // Generated from the component: Modbus1.C function6
    _modbus1_c_function6__in = _modbus1_product6__out;
    if (_modbus1_c_function6__in >= 0) {
        _modbus1_c_function6__out = _modbus1_c_function6__in;
        _modbus1_c_function6__sign = 0;
    }
    else {
        _modbus1_c_function6__out = _modbus1_c_function6__in * -1;
        _modbus1_c_function6__sign = 1;
    }
    // Generated from the component: Modbus1.Q_level
    HIL_OutAO(0x4003, (float)_modbus1_product6__out);
    // Generated from the component: Modbus2.C function6
    _modbus2_c_function6__in = _modbus2_product6__out;
    if (_modbus2_c_function6__in >= 0) {
        _modbus2_c_function6__out = _modbus2_c_function6__in;
        _modbus2_c_function6__sign = 0;
    }
    else {
        _modbus2_c_function6__out = _modbus2_c_function6__in * -1;
        _modbus2_c_function6__sign = 1;
    }
    // Generated from the component: Modbus2.Q_level
    HIL_OutAO(0x4009, (float)_modbus2_product6__out);
    // Generated from the component: Modbus3.C function6
    _modbus3_c_function6__in = _modbus3_product6__out;
    if (_modbus3_c_function6__in >= 0) {
        _modbus3_c_function6__out = _modbus3_c_function6__in;
        _modbus3_c_function6__sign = 0;
    }
    else {
        _modbus3_c_function6__out = _modbus3_c_function6__in * -1;
        _modbus3_c_function6__sign = 1;
    }
    // Generated from the component: Modbus3.Q_level
    HIL_OutAO(0x400f, (float)_modbus3_product6__out);
    // Generated from the component: Modbus1.C function3
    _modbus1_c_function3__in = _modbus1_product3__out;
    if (_modbus1_c_function3__in >= 0) {
        _modbus1_c_function3__out = _modbus1_c_function3__in;
        _modbus1_c_function3__sign = 0;
    }
    else {
        _modbus1_c_function3__out = _modbus1_c_function3__in * -1;
        _modbus1_c_function3__sign = 1;
    }
    // Generated from the component: Modbus1.Voltage_diff
    HIL_OutAO(0x4004, (float)_modbus1_product3__out);
    // Generated from the component: Modbus2.C function3
    _modbus2_c_function3__in = _modbus2_product3__out;
    if (_modbus2_c_function3__in >= 0) {
        _modbus2_c_function3__out = _modbus2_c_function3__in;
        _modbus2_c_function3__sign = 0;
    }
    else {
        _modbus2_c_function3__out = _modbus2_c_function3__in * -1;
        _modbus2_c_function3__sign = 1;
    }
    // Generated from the component: Modbus2.Voltage_diff
    HIL_OutAO(0x400a, (float)_modbus2_product3__out);
    // Generated from the component: Modbus3.C function3
    _modbus3_c_function3__in = _modbus3_product3__out;
    if (_modbus3_c_function3__in >= 0) {
        _modbus3_c_function3__out = _modbus3_c_function3__in;
        _modbus3_c_function3__sign = 0;
    }
    else {
        _modbus3_c_function3__out = _modbus3_c_function3__in * -1;
        _modbus3_c_function3__sign = 1;
    }
    // Generated from the component: Modbus3.Voltage_diff
    HIL_OutAO(0x4010, (float)_modbus3_product3__out);
    // Generated from the component: Modbus1.Product2
    _modbus1_product2__out = (_modbus1_c_function2__out * _modbus1_constant2__out);
    // Generated from the component: Modbus1.Product4
    _modbus1_product4__out = (_modbus1_c_function4__out * _modbus1_constant4__out);
    // Generated from the component: Modbus2.Probe2
    HIL_OutAO(0x4007, (float)_modbus2_c_function2__out);
    // Generated from the component: Modbus2.Product2
    _modbus2_product2__out = (_modbus2_c_function2__out * _modbus2_constant2__out);
    // Generated from the component: Modbus2.Probe3
    HIL_OutAO(0x4008, (float)_modbus2_c_function4__out);
    // Generated from the component: Modbus2.Product4
    _modbus2_product4__out = (_modbus2_c_function4__out * _modbus2_constant4__out);
    // Generated from the component: Modbus3.Probe2
    HIL_OutAO(0x400d, (float)_modbus3_c_function2__out);
    // Generated from the component: Modbus3.Product2
    _modbus3_product2__out = (_modbus3_c_function2__out * _modbus3_constant2__out);
    // Generated from the component: Modbus3.Probe3
    HIL_OutAO(0x400e, (float)_modbus3_c_function4__out);
    // Generated from the component: Modbus3.Product4
    _modbus3_product4__out = (_modbus3_c_function4__out * _modbus3_constant4__out);
    // Generated from the component: Modbus1.Bus Join2
    _modbus1_bus_join2__out[0] = _modbus1_c_function1__out;
    _modbus1_bus_join2__out[1] = _modbus1_c_function1__sign;
    _modbus1_bus_join2__out[2] = _modbus1_c_function3__out;
    _modbus1_bus_join2__out[3] = _modbus1_c_function3__sign;
    _modbus1_bus_join2__out[4] = _modbus1_c_function5__out;
    _modbus1_bus_join2__out[5] = _modbus1_c_function5__sign;
    _modbus1_bus_join2__out[6] = _modbus1_c_function6__out;
    _modbus1_bus_join2__out[7] = _modbus1_c_function6__sign;
    // Generated from the component: Modbus2.Bus Join2
    _modbus2_bus_join2__out[0] = _modbus2_c_function1__out;
    _modbus2_bus_join2__out[1] = _modbus2_c_function1__sign;
    _modbus2_bus_join2__out[2] = _modbus2_c_function3__out;
    _modbus2_bus_join2__out[3] = _modbus2_c_function3__sign;
    _modbus2_bus_join2__out[4] = _modbus2_c_function5__out;
    _modbus2_bus_join2__out[5] = _modbus2_c_function5__sign;
    _modbus2_bus_join2__out[6] = _modbus2_c_function6__out;
    _modbus2_bus_join2__out[7] = _modbus2_c_function6__sign;
    // Generated from the component: Modbus3.Bus Join2
    _modbus3_bus_join2__out[0] = _modbus3_c_function1__out;
    _modbus3_bus_join2__out[1] = _modbus3_c_function1__sign;
    _modbus3_bus_join2__out[2] = _modbus3_c_function3__out;
    _modbus3_bus_join2__out[3] = _modbus3_c_function3__sign;
    _modbus3_bus_join2__out[4] = _modbus3_c_function5__out;
    _modbus3_bus_join2__out[5] = _modbus3_c_function5__sign;
    _modbus3_bus_join2__out[6] = _modbus3_c_function6__out;
    _modbus3_bus_join2__out[7] = _modbus3_c_function6__sign;
    // Generated from the component: u_P1
    HIL_OutAO(0x4012, (float)_modbus1_product2__out);
    // Generated from the component: u_Q1
    HIL_OutAO(0x4015, (float)_modbus1_product4__out);
    // Generated from the component: u_P2
    HIL_OutAO(0x4013, (float)_modbus2_product2__out);
    // Generated from the component: u_Q2
    HIL_OutAO(0x4016, (float)_modbus2_product4__out);
    // Generated from the component: u_P3
    HIL_OutAO(0x4014, (float)_modbus3_product2__out);
    // Generated from the component: u_Q3
    HIL_OutAO(0x4017, (float)_modbus3_product4__out);
    // Generated from the component: Modbus1.ModBus Device1 (config1).comp_reg_in
    XIo_OutInt32(0x55000178, _modbus1_bus_join2__out[0]);
    XIo_OutInt32(0x5500017c, _modbus1_bus_join2__out[1]);
    XIo_OutInt32(0x55000180, _modbus1_bus_join2__out[2]);
    XIo_OutInt32(0x55000184, _modbus1_bus_join2__out[3]);
    XIo_OutInt32(0x55000188, _modbus1_bus_join2__out[4]);
    XIo_OutInt32(0x5500018c, _modbus1_bus_join2__out[5]);
    XIo_OutInt32(0x55000190, _modbus1_bus_join2__out[6]);
    XIo_OutInt32(0x55000194, _modbus1_bus_join2__out[7]);
    // Generated from the component: Modbus2.ModBus Device2 (config2).comp_reg_in
    XIo_OutInt32(0x550001a4, _modbus2_bus_join2__out[0]);
    XIo_OutInt32(0x550001a8, _modbus2_bus_join2__out[1]);
    XIo_OutInt32(0x550001ac, _modbus2_bus_join2__out[2]);
    XIo_OutInt32(0x550001b0, _modbus2_bus_join2__out[3]);
    XIo_OutInt32(0x550001b4, _modbus2_bus_join2__out[4]);
    XIo_OutInt32(0x550001b8, _modbus2_bus_join2__out[5]);
    XIo_OutInt32(0x550001bc, _modbus2_bus_join2__out[6]);
    XIo_OutInt32(0x550001c0, _modbus2_bus_join2__out[7]);
    // Generated from the component: Modbus3.ModBus Device3 (config3).comp_reg_in
    XIo_OutInt32(0x550001d0, _modbus3_bus_join2__out[0]);
    XIo_OutInt32(0x550001d4, _modbus3_bus_join2__out[1]);
    XIo_OutInt32(0x550001d8, _modbus3_bus_join2__out[2]);
    XIo_OutInt32(0x550001dc, _modbus3_bus_join2__out[3]);
    XIo_OutInt32(0x550001e0, _modbus3_bus_join2__out[4]);
    XIo_OutInt32(0x550001e4, _modbus3_bus_join2__out[5]);
    XIo_OutInt32(0x550001e8, _modbus3_bus_join2__out[6]);
    XIo_OutInt32(0x550001ec, _modbus3_bus_join2__out[7]);
    //@cmp.out.block.end
    //////////////////////////////////////////////////////////////////////////
    // Update block
    //////////////////////////////////////////////////////////////////////////
    //@cmp.update.block.start
    // Generated from the component: Modbus1.ModBus Device1 (config1).comp_coil_out
    // Generated from the component: Modbus1.ModBus Device1 (config1).comp_holding_out
    // Generated from the component: Modbus2.ModBus Device2 (config2).comp_coil_out
    // Generated from the component: Modbus2.ModBus Device2 (config2).comp_holding_out
    // Generated from the component: Modbus3.ModBus Device3 (config3).comp_coil_out
    // Generated from the component: Modbus3.ModBus Device3 (config3).comp_holding_out
    // Generated from the component: Modbus1.ModBus Device1 (config1).comp_coil_in
    // Generated from the component: Modbus1.ModBus Device1 (config1).comp_discrete_in
    // Generated from the component: Modbus1.ModBus Device1 (config1).comp_holding_in
    // Generated from the component: Modbus2.ModBus Device2 (config2).comp_coil_in
    // Generated from the component: Modbus2.ModBus Device2 (config2).comp_discrete_in
    // Generated from the component: Modbus2.ModBus Device2 (config2).comp_holding_in
    // Generated from the component: Modbus3.ModBus Device3 (config3).comp_coil_in
    // Generated from the component: Modbus3.ModBus Device3 (config3).comp_discrete_in
    // Generated from the component: Modbus3.ModBus Device3 (config3).comp_holding_in
    // Generated from the component: Modbus1.C function2
    // Generated from the component: Modbus1.C function4
    // Generated from the component: Modbus2.C function2
    // Generated from the component: Modbus2.C function4
    // Generated from the component: Modbus3.C function2
    // Generated from the component: Modbus3.C function4
    // Generated from the component: Modbus1.C function5
    // Generated from the component: Modbus2.C function5
    // Generated from the component: Modbus3.C function5
    // Generated from the component: Modbus1.C function1
    // Generated from the component: Modbus2.C function1
    // Generated from the component: Modbus3.C function1
    // Generated from the component: Modbus1.C function6
    // Generated from the component: Modbus2.C function6
    // Generated from the component: Modbus3.C function6
    // Generated from the component: Modbus1.C function3
    // Generated from the component: Modbus2.C function3
    // Generated from the component: Modbus3.C function3
    // Generated from the component: Modbus1.ModBus Device1 (config1).comp_reg_in
    // Generated from the component: Modbus2.ModBus Device2 (config2).comp_reg_in
    // Generated from the component: Modbus3.ModBus Device3 (config3).comp_reg_in
    //@cmp.update.block.end
}
// ----------------------------------------------------------------------------------------  //-----------------------------------------------------------------------------------------