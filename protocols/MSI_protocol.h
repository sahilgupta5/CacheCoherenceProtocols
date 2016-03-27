#ifndef _MSI_CACHE_H
#define _MSI_CACHE_H

#include "../sim/types.h"
#include "../sim/enums.h"
#include "../sim/module.h"
#include "../sim/mreq.h"
#include "protocol.h"

/**
* author: Sahil Gupta
* This file contains the methods for the MSI protocol which follows the snooping protocol.
*/

/** Cache states.  */
typedef enum {
    MSI_CACHE_I = 1,
    MSI_CACHE_S,
    MSI_CACHE_M,
    //Intermediate states which have been added
    MSI_CACHE_IS_Intermediate,
    MSI_CACHE_SM_Intermediate,
    MSI_CACHE_IM_Intermediate
} MSI_cache_state_t;

class MSI_protocol : public Protocol {
public:
    MSI_protocol (Hash_table *my_table, Hash_entry *my_entry);
    ~MSI_protocol ();

    // Cache state for this line
    MSI_cache_state_t state;
    
    void process_cache_request (Mreq *request);
    void process_snoop_request (Mreq *request);
    void dump (void);

    /* Functions that specify the actions to take on requests from the processor
     * when the cache is in various states
     */
    inline void do_cache_I (Mreq *request);
    inline void do_cache_S (Mreq * request);
    inline void do_cache_M (Mreq *request);

    /* Functions that specify the actions to take on snooped requests
     * when the cache is in various states
     */
    inline void do_snoop_I (Mreq *request);
    inline void do_snoop_S (Mreq * request);
    inline void do_snoop_M (Mreq *request);
    
    //Intermediate states which have been added
    inline void do_snoop_IS_Intermediate (Mreq *request);
    inline void do_snoop_SM_Intermediate (Mreq *request);
    inline void do_snoop_IM_Intermediate (Mreq *request);
};

#endif // _MSI_CACHE_H
