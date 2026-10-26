/*
 * Copyright 2026 Everypin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "threads.h"
#include "bsp/drivers/wdg/hw_wdg.h"
#include "threads_def.h"

#include "button/button_task.h"
#include "indication/indication_task.h"
#include "monitor/monitor_task.h"
#include "regulator/regulator_task.h"
#include "sys_status/sys_status_task.h"
#include "usbctrl/tud_task.h"
#include "usbpd/usbpd_task.h"

#include <cmsis_os2.h>
#include <FreeRTOS.h>
#include <task.h>

// TODO: split to different files
void thread_usbpd_init(void)
{
    const osThreadAttr_t usbpd_thread_attr = {
        .name       = OS_USBPD_NAME,
        .stack_size = OS_USBPD_STACK_SIZE,
        .priority   = OS_USBPD_PRIORITY
    };

    osThreadNew(usbpd_init_task, NULL, &usbpd_thread_attr);
}

void thread_usbtmc_init(void)
{
    const osThreadAttr_t tud_thread_attr = {
        .name       = OS_TUD_NAME,
        .stack_size = OS_TUD_STACK_SIZE,
        .priority   = OS_TUD_PRIORITY
    };

    osThreadNew(tud_app_task, NULL, &tud_thread_attr);

    const osThreadAttr_t tmc_thread_attr = {
        .name       = OS_TMC_NAME,
        .stack_size = OS_TMC_STACK_SIZE,
        .priority   = OS_TMC_PRIORITY
    };

    osThreadNew(scpi_parser_app_task, NULL, &tmc_thread_attr);
}

void thread_indication_init(void)
{
    const osThreadAttr_t ind_thread_attr = {
        .name       = OS_IND_NAME,
        .stack_size = OS_IND_STACK_SIZE,
        .priority   = OS_IND_PRIORITY
    };

    osThreadNew(led_app_task, NULL, &ind_thread_attr);
}

void thread_regulator_init(void)
{
    const osThreadAttr_t regulator_thread_attr = {
        .name       = OS_REGULATOR_NAME,
        .stack_size = OS_REGULATOR_STACK_SIZE,
        .priority   = OS_REGULATOR_PRIORITY
    };

    osThreadNew(regulator_app_task, NULL, &regulator_thread_attr);
}

void thread_button_init(void)
{
    const osThreadAttr_t button_thread_attr = {
        .name       = OS_BUTTON_NAME,
        .stack_size = OS_BUTTON_STACK_SIZE,
        .priority   = OS_BUTTON_PRIORITY
    };

    osThreadNew(button_app_task, NULL, &button_thread_attr);
}

void thread_monitor_init(void)
{
    const osThreadAttr_t monitor_thread_attr = {
        .name       = OS_MONITOR_NAME,
        .stack_size = OS_MONITOR_STACK_SIZE,
        .priority   = OS_MONITOR_PRIORITY
    };

    osThreadNew(monitor_app_task, NULL, &monitor_thread_attr);
}

void thread_sys_status_init(void)
{
    const osThreadAttr_t sys_status_thread_attr = {
        .name       = OS_SYS_STATUS_NAME,
        .stack_size = OS_SYS_STATUS_STACK_SIZE,
        .priority   = OS_SYS_STATUS_PRIORITY
    };

    sys_status_queue_init();

    osThreadNew(sys_status_app_task, NULL, &sys_status_thread_attr);
}

//--------------------------------------------------------------------
// FreeRTOS hooks
//--------------------------------------------------------------------
void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char* pcTaskName)
{
    (void)pxTask;
    (void)pcTaskName;

    taskDISABLE_INTERRUPTS();
}

void vApplicationTickHook(void)
{
    // HAL_IncTick();
    // USBPD_DPM_TimerCounter();
}

/* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide
 * an implementation of vApplicationGetIdleTaskMemory() to provide the memory
 * that is used by the Idle task. */
void vApplicationGetIdleTaskMemory(
    StaticTask_t** ppxIdleTaskTCBBuffer,
    StackType_t**  ppxIdleTaskStackBuffer,
    uint32_t*      pulIdleTaskStackSize
)
{
    /* If the buffers to be provided to the Idle task are declared inside this
     * function then they must be declared static - otherwise they will be
     * allocated on the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t  uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
      state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
      Note that, as the array is necessarily of type StackType_t,
      configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so
 * the application must provide an implementation of
 * vApplicationGetTimerTaskMemory() to provide the memory that is used by the
 * Timer service task. */
void vApplicationGetTimerTaskMemory(
    StaticTask_t** ppxTimerTaskTCBBuffer,
    StackType_t**  ppxTimerTaskStackBuffer,
    uint32_t*      pulTimerTaskStackSize
)
{
    /* If the buffers to be provided to the Timer task are declared inside this
     * function then they must be declared static - otherwise they will be
     * allocated on the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t  uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
      task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
      Note that, as the array is necessarily of type StackType_t,
      configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

void vApplicationIdleHook(void)
{
    hw_wdg_feed();
}
