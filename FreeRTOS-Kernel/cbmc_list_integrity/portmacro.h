#ifndef PORTMACRO_H
#define PORTMACRO_H

/* Minimal, CBMC-friendly port layer just to compile list.c.
 * This is not a functional hardware port.
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Type definitions. */
#define portCHAR          char
#define portFLOAT         float
#define portDOUBLE        double
#define portLONG          long
#define portSHORT         short
#define portSTACK_TYPE    uint32_t
#define portBASE_TYPE     int32_t

typedef portSTACK_TYPE StackType_t;
typedef portBASE_TYPE  BaseType_t;
typedef uint32_t       UBaseType_t;

#if ( configTICK_TYPE_WIDTH_IN_BITS == TICK_TYPE_WIDTH_16_BITS )
    typedef uint16_t TickType_t;
#elif ( configTICK_TYPE_WIDTH_IN_BITS == TICK_TYPE_WIDTH_32_BITS )
    typedef uint32_t TickType_t;
#elif ( configTICK_TYPE_WIDTH_IN_BITS == TICK_TYPE_WIDTH_64_BITS )
    typedef uint64_t TickType_t;
#else
    #error "Unsupported tick width"
#endif

#define portMAX_DELAY    ( ( TickType_t ) ( ~( ( TickType_t ) 0U ) ) )

/* Hardware specifics (dummy). */
#define portSTACK_GROWTH      ( -1 )
#define portTICK_PERIOD_MS    ( ( TickType_t ) 1000 / configTICK_RATE_HZ )
#define portBYTE_ALIGNMENT    8

/* Critical section / yield stubs. */
#define portENTER_CRITICAL()                ( ( void ) 0 )
#define portEXIT_CRITICAL()                 ( ( void ) 0 )
#define portDISABLE_INTERRUPTS()            ( ( void ) 0 )
#define portENABLE_INTERRUPTS()             ( ( void ) 0 )
#define portYIELD()                         ( ( void ) 0 )
#define portYIELD_WITHIN_API()              ( ( void ) 0 )
#define portEND_SWITCHING_ISR( x )          ( ( void ) ( x ) )
#define portYIELD_FROM_ISR( x )             portEND_SWITCHING_ISR( x )

#define portSET_INTERRUPT_MASK_FROM_ISR()   ( 0U )
#define portCLEAR_INTERRUPT_MASK_FROM_ISR( x ) ( ( void ) ( x ) )

/* Task function macros. */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters )    void vFunction( void * pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters )          void vFunction( void * pvParameters )

/* MPU wrappers disabled. */
#define portUSING_MPU_WRAPPERS 0
#define portNUM_CONFIGURABLE_REGIONS 1

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */
