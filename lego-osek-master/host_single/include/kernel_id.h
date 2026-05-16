#ifndef HOST_SINGLE_KERNEL_ID_H
#define HOST_SINGLE_KERNEL_ID_H

/* Minimal IDs for the host simulation.
 * These correspond to names used by the selected app.
 */

extern int SysTimerCnt;

#ifdef RMS_APP
enum {
    Task1_ID = 0,
    Task2_ID = 1,
    TaskLCD_ID = 2,
};
#else
enum {
    TaskInitialize_ID = 0,
    TaskControl_ID = 1,
    TaskSonar_ID = 2,
    TaskLCD_ID = 3,
};
#endif

#endif
