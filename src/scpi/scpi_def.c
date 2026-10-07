#include "scpi_def.h"
#include "control/control.h"
#include "scpi/ext_conn/digital_ctl.h"
#include "scpi/scpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char         scpi_input_buffer[SCPI_INPUT_BUFFER_LENGTH];
scpi_error_t scpi_error_queue_data[SCPI_ERROR_QUEUE_SIZE];
scpi_t       scpi_context;

extern uint8_t buffer_in[225];
extern size_t  buffer_in_len;

const scpi_command_t scpi_commands[] = {
    /* IEEE Mandated Commands (SCPI std V1999.0 4.1.1) */
    {
     .pattern  = "*CLS",
     .callback = SCPI_CoreCls,
     },
    {
     .pattern  = "*ESE",
     .callback = SCPI_CoreEse,
     },
    {
     .pattern  = "*ESE?",
     .callback = SCPI_CoreEseQ,
     },
    {
     .pattern  = "*ESR?",
     .callback = SCPI_CoreEsrQ,
     },
    {
     .pattern  = "*IDN?",
     .callback = SCPI_CoreIdnQ,
     },
    {
     .pattern  = "*OPC",
     .callback = SCPI_CoreOpc,
     },
    {
     .pattern  = "*OPC?",
     .callback = SCPI_CoreOpcQ,
     },
    {
     .pattern  = "*RST",
     .callback = SCPI_CoreRst,
     },
    {
     .pattern  = "*SRE",
     .callback = SCPI_CoreSre,
     },
    {
     .pattern  = "*SRE?",
     .callback = SCPI_CoreSreQ,
     },
    {
     .pattern  = "*STB?",
     .callback = SCPI_CoreStbQ,
     },
    {
     .pattern  = "*TST?",
     .callback = SCPI_CoreTstQ,
     },
    {
     .pattern  = "*WAI",
     .callback = SCPI_CoreWai,
     },
    {
     .pattern  = "*SAV",
     .callback = SCPI_SaveState,
     },
    {
     .pattern  = "*RCL",
     .callback = SCPI_RecallState,
     },

    /* Required SCPI commands (SCPI std V1999.0 4.2.1) */
    {
     .pattern  = "SYSTem:ERRor[:NEXT]?",
     .callback = SCPI_SystemErrorNextQ,
     },
    {
     .pattern  = "SYSTem:ERRor:COUNt?",
     .callback = SCPI_SystemErrorCountQ,
     },
    {
     .pattern  = "SYSTem:VERSion?",
     .callback = SCPI_SystemVersionQ,
     },
    /* Base functions */
    {
     .pattern  = "OUTPut[:STATe]",
     .callback = SCPI_Output,
     },
    {
     .pattern  = "OUTPut[:STATe]?",
     .callback = SCPI_OutputQ,
     },
    /* SYSTem Subsystem */
    {
     .pattern  = "SYSTem:CAPability?",
     .callback = SCPI_SystemCapabilityQ,
     },
    {
     .pattern  = "SYSTem:LOCal",
     .callback = SCPI_SystemLocal,
     },
    {
     .pattern  = "SYSTem:REMote",
     .callback = SCPI_SystemRemote,
     },
    {
     .pattern  = "SYSTem:RWLock",
     .callback = SCPI_SystemRWlock,
     },
    /* SOURce Subsystem */
    {
     .pattern  = "[SOURce]:CURRent[:LEVel][:IMMediate][:AMPLitude]",
     .callback = SCPI_Current,
     },
    {
     .pattern  = "[SOURce]:CURRent[:LEVel][:IMMediate][:AMPLitude]?",
     .callback = SCPI_CurrentQ,
     },
    {
     .pattern  = "[SOURce]:VOLTage[:LEVel][:IMMediate][:AMPLitude]",
     .callback = SCPI_Voltage,
     },
    {
     .pattern  = "[SOURce]:VOLTage[:LEVel][:IMMediate][:AMPLitude]?",
     .callback = SCPI_VoltageQ,
     },
    {
     .pattern  = "[SOURce]:CURRent:PROTection:STATe",
     .callback = SCPI_CurrentProtectionState,
     },
    {
     .pattern  = "[SOURce]:CURRent:PROTection:STATe?",
     .callback = SCPI_CurrentProtectionStateQ,
     },
    {
     .pattern  = "[SOURce]:CURRent:PROTection:LEVel",
     .callback = SCPI_CurrentProtectionLevel,
     },
    {
     .pattern  = "[SOURce]:CURRent:PROTection:LEVel?",
     .callback = SCPI_CurrentProtectionLevelQ,
     },
    {
     .pattern  = "[SOURce]:CURRent:PROTection:CLEar",
     .callback = SCPI_CurrentProtectionClear,
     },
    /* STATus subsystem */
    {
     .pattern  = "STATus:OPERation[:EVENt]?",
     .callback = SCPI_StatusOperationEventQ,
     },
    {
     .pattern  = "STATus:OPERation:CONDition?",
     .callback = SCPI_StatusOperationConditionQ,
     },
    {
     .pattern  = "STATus:OPERation:ENABle",
     .callback = SCPI_StatusOperationEnable,
     },
    {
     .pattern  = "STATus:OPERation:ENABle?",
     .callback = SCPI_StatusOperationEnableQ,
     },
    {
     .pattern  = "STATus:QUEStionable[:EVENt]?",
     .callback = SCPI_StatusQuestionableEventQ,
     },
    {
     .pattern  = "STATus:QUEStionable:CONDition?",
     .callback = SCPI_StatusQuestionableConditionQ,
     },
    {
     .pattern  = "STATus:QUEStionable:ENABle",
     .callback = SCPI_StatusQuestionableEnable,
     },
    {
     .pattern  = "STATus:QUEStionable:ENABle?",
     .callback = SCPI_StatusQuestionableEnableQ,
     },
    {
     .pattern  = "STATus:PRESet",
     .callback = SCPI_StatusPreset,
     },
    /* MEASure subsystem */
    {
     .pattern  = "MEASure[:SCALar]:VOLTage[:DC]?",
     .callback = SCPI_MeasureVoltageQ,
     },
    {
     .pattern  = "MEASure[:SCALar]:CURRent[:DC]?",
     .callback = SCPI_MeasureCurrentQ,
     },
    {
     .pattern  = "MEASure[:SCALar]:POWer[:DC]?",
     .callback = SCPI_MeasurePowerQ,
     },
    {
     .pattern  = "MEASure[:SCALar]:TEMPerature:SHUNt?",
     .callback = SCPI_MeasureTemperatureShuntQ,
     },
    {
     .pattern  = "MEASure[:SCALar]:TEMPerature:TERMinal?",
     .callback = SCPI_MeasureTemperatureTerminalQ,
     },
    /* CALibration subsystem */
    {.pattern  = "CALibration:SECUre:STATe",
     .callback = SCPI_CalibrationSecureState},
    {.pattern  = "CALibration:SECUre:STATe?",
     .callback = SCPI_CalibrationSecureStateQ},
    {.pattern  = "CALibration:VOLTage:SLOPe",
     .callback = SCPI_CalibrationVoltageSlope},
    {.pattern  = "CALibration:VOLTage:SLOPe?",
     .callback = SCPI_CalibrationVoltageSlopeQ},
    {.pattern  = "CALibration:VOLTage:OFFSet",
     .callback = SCPI_CalibrationVoltageOffset},
    {.pattern  = "CALibration:VOLTage:OFFSet?",
     .callback = SCPI_CalibrationVoltageOffsetQ},
    {.pattern  = "CALibration:CURRent:SLOPe",
     .callback = SCPI_CalibrationCurrentSlope},
    {.pattern  = "CALibration:CURRent:SLOPe?",
     .callback = SCPI_CalibrationCurrentSlopeQ},
    {.pattern  = "CALibration:CURRent:OFFSet",
     .callback = SCPI_CalibrationCurrentOffset},
    {.pattern  = "CALibration:CURRent:OFFSet?",
     .callback = SCPI_CalibrationCurrentOffsetQ},
    {.pattern = "CALibration:STORe", .callback = SCPI_CalibrationStore},

    /* DIGital subsystem */
    {
     .pattern  = "[SOURce]:DIGital:COUNt?",
     .callback = SCPI_DigitalCountQ,
     },
    {
     .pattern  = "[SOURce]:DIGital:PIN#:FUNCtion?",
     .callback = SCPI_DigitalPinFunctionQ,
     },
    {
     .pattern  = "[SOURce]:DIGital:PIN#:DIRection",
     .callback = SCPI_DigitalPinDirection,
     },
    {
     .pattern  = "[SOURce]:DIGital:PIN#:DIRection?",
     .callback = SCPI_DigitalPinDirectionQ,
     },
    {
     .pattern  = "[SOURce]:DIGital:PIN#:MODE",
     .callback = SCPI_DigitalPinMode,
     },
    {
     .pattern  = "[SOURce]:DIGital:PIN#:MODE?",
     .callback = SCPI_DigitalPinModeQ,
     },
    {
     .pattern  = "[SOURce]:DIGital:PIN#:PULL",
     .callback = SCPI_DigitalPinPull,
     },
    {
     .pattern  = "[SOURce]:DIGital:PIN#:PULL?",
     .callback = SCPI_DigitalPinPullQ,
     },
    {
     .pattern  = "[SOURce]:DIGital:PIN#[:LEVel]",
     .callback = SCPI_DigitalPinLevel,
     },
    {
     .pattern  = "[SOURce]:DIGital:PIN#[:LEVel]?",
     .callback = SCPI_DigitalPinLevelQ,
     },

    SCPI_CMD_LIST_END
};

scpi_interface_t scpi_interface = {
    .error   = SCPI_Error,
    .write   = SCPI_Write,
    .control = SCPI_Control,
    .flush   = SCPI_Flush,
    .reset   = SCPI_Reset,
};

static size_t output_buffer_write(const char* data, size_t len)
{
    memcpy(buffer_in + buffer_in_len, data, len);
    buffer_in_len += len;
    buffer_in[buffer_in_len] = '\0';
    return len;
}

size_t SCPI_Write(scpi_t* context, const char* data, size_t len)
{
    (void)context;
    output_buffer_write(data, len);
    return SCPI_RES_OK;
}

scpi_result_t SCPI_Flush(scpi_t* context)
{
    (void)context;
    return SCPI_RES_OK;
}

int SCPI_Error(scpi_t* context, int_fast16_t err)
{
    (void)context;
    (void)err;
    return SCPI_RES_OK;
}

scpi_result_t SCPI_Control(
    scpi_t* context, scpi_ctrl_name_t ctrl, scpi_reg_val_t val
)
{
    (void)context;
    (void)ctrl;
    (void)val;
    return SCPI_RES_OK;
}

scpi_result_t SCPI_Reset(scpi_t* context)
{
    (void)context;

    SCPI_DigitalReset(context);

    return SCPI_RES_OK;
}
