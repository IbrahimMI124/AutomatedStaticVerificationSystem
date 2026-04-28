#ifndef HOST_SINGLE_KERNEL_H
#define HOST_SINGLE_KERNEL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int StatusType;
typedef uint32_t TickType;
typedef int CounterType;

enum { E_OK = 0 };

#define DeclareTask(name) void name(void)
#define DeclareCounter(name) extern CounterType name

#define TASK(name) void name(void)
#define TerminateTask() return

StatusType SignalCounter(int counter);
StatusType ActivateTask(int task_id);
void ShutdownOS(StatusType ercd);

#ifdef __cplusplus
}
#endif

#endif
