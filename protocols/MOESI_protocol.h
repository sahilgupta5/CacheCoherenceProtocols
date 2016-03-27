#ifndef _MOESI_CACHE_H
#define _MOESI_CACHE_H

#include "../sim/types.h"
#include "../sim/enums.h"
#include "../sim/module.h"
#include "../sim/mreq.h"
#include "protocol.h"

/**
* author: Sahil Gupta
* This file contains the methods for the MOESI protocol which follows the snooping protocol.
*/

/** Cache states.  */
typedef enum {
    MOESI_CACHE_I = 1,
    MOESI_CACHE_S,
    MOESI_CACHE_E,
    MOESI_CACHE_O,
    MOESI_CACHE_M,
    
    //All the intermediate states added here
    MOESI_CACHE_IS_Intermediate,
    MOESI_CACHE_IM_Intermediate,
    MOESI_CACHE_SM_Intermediate,
    MOESI_CACHE_OM_Intermediate,
} MOESI_cache_state_t;

class MOESI_protocol : public Protocol {
public:
    MOESI_protocol (Hash_table *my_table, Hash_entry *my_entry);
    ~MOESI_protocol ();

    MOESI_cache_state_t state;
    
    void process_cache_request (Mreq *request);
    void process_snoop_request (Mreq *request);
    void dump (void);

    inline void do_cache_I (Mreq *request);
    inline void do_cache_S (Mreq *request);
    inline void do_cache_E (Mreq *request);
    inline void do_cache_O (Mreq *request);
    inline void do_cache_M (Mreq *request);

    inline void do_snoop_I (Mreq *request);
    inline void do_snoop_S (Mreq *request);
    inline void do_snoop_E (Mreq *request);
    inline void do_snoop_O (Mreq *request);
    inline void do_snoop_M (Mreq *request);
    
    //snooping for all the intermediate states.
    inline void do_snoop_IS_Intermediate (Mreq *request);
    inline void do_snoop_IM_Intermediate (Mreq *request);
    inline void do_snoop_SM_Intermediate (Mreq *request);
    inline void do_snoop_OM_Intermediate (Mreq *request);
};

#endif // _MOESI_CACHE_H
