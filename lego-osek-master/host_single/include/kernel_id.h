#ifndef HOST_SINGLE_KERNEL_ID_H
#define HOST_SINGLE_KERNEL_ID_H

/* Minimal IDs for the host simulation.
 * These correspond to names used by nxtgt.c / nxtgt.oil.
 */

extern int SysTimerCnt;

enum {
    TaskInitialize_ID = 0,
    TaskControl_ID = 1,
    TaskSonar_ID = 2,
    TaskLCD_ID = 3,
};

#endif
