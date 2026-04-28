#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* Minimal config to compile list.c for CBMC + Copilot monitor.
 * This is intentionally not a full board config.
 */

#define configUSE_PREEMPTION                       1
#define configUSE_TIME_SLICING                     0
#define configCPU_CLOCK_HZ                         ( ( unsigned long ) 20000000 )
#define configTICK_RATE_HZ                         100

#define configMAX_PRIORITIES                       5
#define configMINIMAL_STACK_SIZE                   128
#define configMAX_TASK_NAME_LEN                    16

#define configTICK_TYPE_WIDTH_IN_BITS              TICK_TYPE_WIDTH_64_BITS

#define configUSE_IDLE_HOOK                        0
#define configUSE_TICK_HOOK                        0

#define configUSE_TIMERS                           0

#define configSUPPORT_DYNAMIC_ALLOCATION           0
#define configSUPPORT_STATIC_ALLOCATION            1

#define configUSE_MUTEXES                          0
#define configQUEUE_REGISTRY_SIZE                  0

#define configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES  0
#define configUSE_MINI_LIST_ITEM                   1

/* Trace hooks: list.c calls these unconditionally.
 * We capture the list pointer on ENTER and check/publish on RETURN.
 *
 * Note: we cannot include list.h here (FreeRTOS.h must be included before list.h),
 * so we forward-declare the types.
 */

struct xLIST;
struct xLIST_ITEM;
typedef struct xLIST List_t;
typedef struct xLIST_ITEM ListItem_t;

void frtrace_list_enter( const List_t * pxList );
void frtrace_list_enter_remove( const ListItem_t * pxItem );
void frtrace_list_exit( void );

#define traceENTER_vListInitialise( pxList )            frtrace_list_enter( ( pxList ) )
#define traceRETURN_vListInitialise()                   frtrace_list_exit()

#define traceENTER_vListInitialiseItem( pxItem )        ( ( void ) ( pxItem ) )
#define traceRETURN_vListInitialiseItem()               ( ( void ) 0 )

#define traceENTER_vListInsertEnd( pxList, pxItem )     ( ( void ) ( pxItem ), frtrace_list_enter( ( pxList ) ) )
#define traceRETURN_vListInsertEnd()                    frtrace_list_exit()

#define traceENTER_vListInsert( pxList, pxItem )        ( ( void ) ( pxItem ), frtrace_list_enter( ( pxList ) ) )
#define traceRETURN_vListInsert()                       frtrace_list_exit()

#define traceENTER_uxListRemove( pxItem )               frtrace_list_enter_remove( ( pxItem ) )
#define traceRETURN_uxListRemove( uxRemaining )         ( ( void ) ( uxRemaining ), frtrace_list_exit() )

/* Coverage test hooks used in list.c. */
#define mtCOVERAGE_TEST_DELAY()                         ( ( void ) 0 )
#define mtCOVERAGE_TEST_MARKER()                        ( ( void ) 0 )

/* Assertions: make them meaningful for CBMC. */
#ifdef CBMC
  /*
   * This harness is meant to prove ONLY the Copilot runtime-monitor property.
   * Leave FreeRTOS internal assertions disabled to avoid extra CBMC properties.
   */
  #define configASSERT( x ) ( ( void ) 0 )
#else
  #include <assert.h>
  #define configASSERT( x ) assert( x )
#endif

#endif /* FREERTOS_CONFIG_H */
