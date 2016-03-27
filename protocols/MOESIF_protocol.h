#ifndef _MOESIF_CACHE_H
#define _MOESIF_CACHE_H

#include "../sim/types.h"
#include "../sim/enums.h"
#include "../sim/module.h"
#include "../sim/mreq.h"
#include "protocol.h"

/**
* author: Sahil Gupta
* This file contains the methods for the MOESIF protocol which follows the snooping protocol.
*/

/** Cache states.  */
typedef enum {
    MOESIF_CACHE_I = 1,
    MOESIF_CACHE_S,
    MOESIF_CACHE_E,
    MOESIF_CACHE_O,
    MOESIF_CACHE_M,
    MOESIF_CACHE_F,
    
    //All the intermediate states added here
    MOESIF_CACHE_IS_Intermediate,
    MOESIF_CACHE_IM_Intermediate,
    MOESIF_CACHE_SM_Intermediate,
    MOESIF_CACHE_OM_Intermediate,
    MOESIF_CACHE_FM_Intermediate
} MOESIF_cache_state_t;

class MOESIF_protocol : public Protocol {
public:
    MOESIF_protocol (Hash_table *my_table, Hash_entry *my_entry);
    ~MOESIF_protocol ();

    MOESIF_cache_state_t state;
    
    void process_cache_request (Mreq *request);
    void process_snoop_request (Mreq *request);
    void dump (void);

    inline void do_cache_F (Mreq *request);
    inline void do_cache_I (Mreq *request);
    inline void do_cache_S (Mreq *request);
    inline void do_cache_E (Mreq *request);
    inline void do_cache_O (Mreq *request);
    inline void do_cache_M (Mreq *request);

    inline void do_snoop_F (Mreq *request);
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
    inline void do_snoop_FM_Intermediate (Mreq *request);
};

#endif // _MOESIF_CACHE_H
