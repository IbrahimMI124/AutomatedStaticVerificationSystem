#include "kernel.h"
#include "kernel_id.h"

#include <stdio.h>
#include <stdlib.h>

extern void ecrobot_device_initialize(void);
extern void ecrobot_device_terminate(void);
extern void user_1ms_isr_type2(void);

#ifdef RMS_APP
extern void Task1(void);
extern void Task2(void);
extern void TaskLCD(void);
#else
extern void TaskInitialize(void);
extern void TaskControl(void);
extern void TaskSonar(void);
extern void TaskLCD(void);
#endif

static TickType g_now_ms = 0;

int SysTimerCnt = 0;

struct task_state
{
    int pending;
};

#ifdef RMS_APP
enum { TASK_COUNT = 3 };
#else
enum { TASK_COUNT = 4 };
#endif

static struct task_state g_tasks[TASK_COUNT];

static TickType g_next_control = 0;
static TickType g_next_sonar = 0;
static TickType g_next_lcd = 0;

static void run_one_task(int task_id)
{
    g_tasks[task_id].pending = 0;
    switch(task_id)
    {
#ifdef RMS_APP
        case Task1_ID: Task1(); break;
        case Task2_ID: Task2(); break;
        case TaskLCD_ID: TaskLCD(); break;
#else
        case TaskInitialize_ID: TaskInitialize(); break;
        case TaskControl_ID: TaskControl(); break;
        case TaskSonar_ID: TaskSonar(); break;
        case TaskLCD_ID: TaskLCD(); break;
#endif
        default: break;
    }
}

static void run_ready_tasks(void)
{
#ifdef RMS_APP
    /* Priority order from rms.oil: Task1(3) > Task2(2) > TaskLCD(1) */
    for(;;)
    {
        if(g_tasks[Task1_ID].pending) { run_one_task(Task1_ID); continue; }
        if(g_tasks[Task2_ID].pending) { run_one_task(Task2_ID); continue; }
        if(g_tasks[TaskLCD_ID].pending) { run_one_task(TaskLCD_ID); continue; }
        break;
    }
#else
    /* Priority order from nxtgt.oil: Initialize(4) > Control(3) > Sonar(2) > LCD(1) */
    for(;;)
    {
        if(g_tasks[TaskInitialize_ID].pending) { run_one_task(TaskInitialize_ID); continue; }
        if(g_tasks[TaskControl_ID].pending) { run_one_task(TaskControl_ID); continue; }
        if(g_tasks[TaskSonar_ID].pending) { run_one_task(TaskSonar_ID); continue; }
        if(g_tasks[TaskLCD_ID].pending) { run_one_task(TaskLCD_ID); continue; }
        break;
    }
#endif
}

StatusType ActivateTask(int task_id)
{
    if(task_id < 0 || task_id >= TASK_COUNT) return 1;
    g_tasks[task_id].pending = 1;
    return E_OK;
}

StatusType SignalCounter(int counter)
{
    (void)counter;
    g_now_ms++;

#ifdef RMS_APP
    if(g_now_ms >= g_next_control)
    {
        g_next_control = g_now_ms + 1;
        (void)ActivateTask(Task1_ID);
    }

    if(g_now_ms >= g_next_sonar)
    {
        g_next_sonar = g_now_ms + 500;
        (void)ActivateTask(Task2_ID);
    }

    if(g_now_ms >= g_next_lcd)
    {
        g_next_lcd = g_now_ms + 500;
        (void)ActivateTask(TaskLCD_ID);
    }
#else
    if(g_now_ms >= g_next_control)
    {
        g_next_control = g_now_ms + 10;
        (void)ActivateTask(TaskControl_ID);
    }

    if(g_now_ms >= g_next_sonar)
    {
        g_next_sonar = g_now_ms + 50;
        (void)ActivateTask(TaskSonar_ID);
    }

    if(g_now_ms >= g_next_lcd)
    {
        g_next_lcd = g_now_ms + 500;
        (void)ActivateTask(TaskLCD_ID);
    }
#endif

    return E_OK;
}

void ShutdownOS(StatusType ercd)
{
    fprintf(stderr, "ShutdownOS(%d)\n", ercd);
    abort();
}

TickType osek_now_ms(void)
{
    return g_now_ms;
}

int osek_sim_run(TickType duration_ms)
{
    ecrobot_device_initialize();

#ifndef RMS_APP
    /* TaskInitialize AUTOSTART=TRUE in nxtgt.oil */
    (void)ActivateTask(TaskInitialize_ID);
    run_ready_tasks();
#endif

    /* Alarm autostart initial ALARMTIME=1 */
    g_next_control = 1;
    g_next_sonar = 1;
    g_next_lcd = 1;

    for(TickType i = 0; i < duration_ms; i++)
    {
        user_1ms_isr_type2();
        run_ready_tasks();
    }

    ecrobot_device_terminate();
    return 0;
}
