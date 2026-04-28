#include <stdbool.h>
#include <stdint.h>

#include "FreeRTOS.h"
#include "list.h"

extern bool fr_list_violation;

#ifdef CBMC
unsigned int nondet_uint( void );
#else
static uint32_t g_seed = 0xC0FFEEu;
static unsigned int nondet_uint( void )
{
    /* Deterministic LCG for normal (non-CBMC) runs. */
    g_seed = ( 1103515245u * g_seed + 12345u );
    return ( unsigned int ) ( g_seed >> 1 );
}
#endif

int main( void )
{
    enum { N = 4, STEPS = 8 };

    List_t list;
    ListItem_t items[ N ];
    bool in_list[ N ] = { false, false, false, false };

    vListInitialise( &list );

    for( unsigned i = 0; i < N; i++ )
    {
        vListInitialiseItem( &items[ i ] );
        listSET_LIST_ITEM_VALUE( &items[ i ], ( TickType_t ) ( i + 1u ) );
        listSET_LIST_ITEM_OWNER( &items[ i ], &items[ i ] );
    }

    for( unsigned step_idx = 0; step_idx < STEPS; step_idx++ )
    {
        unsigned op = nondet_uint() % 3u;
        unsigned idx = nondet_uint() % N;

        if( op == 0u )
        {
            /* Sorted insert. */
            if( !in_list[ idx ] )
            {
                vListInsert( &list, &items[ idx ] );
                in_list[ idx ] = true;
            }
        }
        else if( op == 1u )
        {
            /* Insert relative to pxIndex (round-robin fairness insertion). */
            if( !in_list[ idx ] )
            {
                vListInsertEnd( &list, &items[ idx ] );
                in_list[ idx ] = true;
            }
        }
        else
        {
            /* Remove by item. */
            if( in_list[ idx ] )
            {
                ( void ) uxListRemove( &items[ idx ] );
                in_list[ idx ] = false;
            }
        }

#ifdef CBMC
        __CPROVER_assert( fr_list_violation == false, "list_integrity trigger must never fire" );
#endif
    }

    return 0;
}
