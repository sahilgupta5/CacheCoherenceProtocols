#ifndef _MESI_CACHE_H
#define _MESI_CACHE_H

#include "../sim/types.h"
#include "../sim/enums.h"
#include "../sim/module.h"
#include "../sim/mreq.h"
#include "protocol.h"

/**
* author: Sahil Gupta
* This file contains the methods for the MESI protocol which follows the snooping protocol.
*/

/** Cache states.  */
typedef enum {
    MESI_CACHE_I = 1,
    MESI_CACHE_S,
    MESI_CACHE_E,
    MESI_CACHE_M,
    
    //All the intermediate states added here
    MESI_CACHE_IS_Intermediate,
    MESI_CACHE_IM_Intermediate,
    MESI_CACHE_SM_Intermediate
} MESI_cache_state_t;

class MESI_protocol : public Protocol {
public:
    MESI_protocol (Hash_table *my_table, Hash_entry *my_entry);
    ~MESI_protocol ();

    MESI_cache_state_t state;
    
    void process_cache_request (Mreq *request);
    void process_snoop_request (Mreq *request);
    void dump (void);

    /* Functions that specify the actions to take on requests from the processor
     * when the cache is in various states
     */
    inline void do_cache_I (Mreq *request);
    inline void do_cache_S (Mreq *request);
    inline void do_cache_E (Mreq *request);
    inline void do_cache_M (Mreq *request);

    /* Functions that specify the actions to take on snooped requests
     * when the cache is in various states
     */
    inline void do_snoop_I (Mreq *request);
    inline void do_snoop_S (Mreq *request);
    inline void do_snoop_E (Mreq *request);
    inline void do_snoop_M (Mreq *request);
    
    //snooping for all the intermediate states.
    inline void do_snoop_IM_Intermediate (Mreq *request);
    inline void do_snoop_SM_Intermediate (Mreq *request);
    inline void do_snoop_IS_Intermediate (Mreq *request);
};

#endif // _MESI_CACHE_H
