#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "list.h"

/* Published to Copilot monitor via extern "fr_list_ok". */
bool fr_list_ok = true;

/* Latch set if the Copilot trigger fires. */
bool fr_list_violation = false;

/* From the Copilot-generated monitor. */
void step( void );

static const List_t * g_last_list = NULL;

/* Optional: model a single-event upset (SEU) as a one-bit flip.
 * This is enabled only when compiling with -DSEU_XLISTEND_ITEMVALUE.
 */
#ifdef SEU_XLISTEND_ITEMVALUE
    static bool g_seu_done = false;

    #ifdef CBMC
        _Bool __CPROVER_nondet_bool( void );
        unsigned __CPROVER_nondet_uint( void );
        void __CPROVER_assume( _Bool );
        static bool fr_nondet_bool( void ) { return (bool) __CPROVER_nondet_bool(); }
        static unsigned fr_nondet_uint( void ) { return __CPROVER_nondet_uint(); }
    #else
        /* For native runs, default to no injection. */
        static bool fr_nondet_bool( void ) { return false; }
        static unsigned fr_nondet_uint( void ) { return 0U; }
    #endif

    static void fr_seu_flip_one_bit_xlistend_itemvalue( const List_t * pxListConst )
    {
        if( pxListConst == NULL )
        {
            return;
        }

        List_t * const pxList = ( List_t * ) pxListConst;
        const unsigned width = ( unsigned ) ( sizeof( pxList->xListEnd.xItemValue ) * 8U );
        unsigned bit = fr_nondet_uint();

        #ifdef CBMC
            __CPROVER_assume( bit < width );
        #else
            bit = bit % width;
        #endif

        pxList->xListEnd.xItemValue ^= ( ( TickType_t ) 1U << bit );
    }
#endif

static bool fr_list_check_basic( const List_t * pxList )
{
    if( pxList == NULL )
    {
        return false;
    }

    /* xListEnd is a MiniListItem_t when configUSE_MINI_LIST_ITEM==1.
     * It still contains xItemValue/pxNext/pxPrevious.
     */
    const MiniListItem_t * const pxEndMini = &( pxList->xListEnd );
    ListItem_t * const pxEnd = ( ListItem_t * ) pxEndMini;

    if( pxEndMini->xItemValue != portMAX_DELAY )
    {
        return false;
    }

    if( pxEndMini->pxNext == NULL || pxEndMini->pxPrevious == NULL )
    {
        return false;
    }

    /* Local circularity around the marker. */
    if( pxEndMini->pxNext->pxPrevious != pxEnd )
    {
        return false;
    }

    if( pxEndMini->pxPrevious->pxNext != pxEnd )
    {
        return false;
    }

    if( pxList->uxNumberOfItems == ( UBaseType_t ) 0 )
    {
        if( pxEndMini->pxNext != pxEnd || pxEndMini->pxPrevious != pxEnd )
        {
            return false;
        }
    }
    else
    {
        /* Non-empty list: head/tail are not the marker itself. */
        if( pxEndMini->pxNext == pxEnd || pxEndMini->pxPrevious == pxEnd )
        {
            return false;
        }
    }

    return true;
}

void frtrace_list_enter( const List_t * pxList )
{
    g_last_list = pxList;
}

void frtrace_list_enter_remove( const ListItem_t * pxItem )
{
    /* Before removal, the item should still point at its container list. */
    if( pxItem != NULL )
    {
        g_last_list = pxItem->pxContainer;
    }
    else
    {
        g_last_list = NULL;
    }
}

void frtrace_list_exit( void )
{
#ifdef SEU_XLISTEND_ITEMVALUE
    /* Inject at most once, at a nondeterministic time. */
    if( !g_seu_done && fr_nondet_bool() )
    {
        fr_seu_flip_one_bit_xlistend_itemvalue( g_last_list );
        g_seu_done = true;
    }
#endif

    fr_list_ok = fr_list_check_basic( g_last_list );

    /* Advance Copilot monitor after each list operation. */
    step();
}

/* Copilot trigger handler.
 * The generated monitor calls this when (not fr_list_ok) becomes true.
 */
void list_integrity( void )
{
    fr_list_violation = true;
}
