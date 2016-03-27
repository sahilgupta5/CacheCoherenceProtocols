#include "MOESIF_protocol.h"
#include "../sim/mreq.h"
#include "../sim/sim.h"
#include "../sim/hash_table.h"

extern Simulator *Sim;

/**
* author: Sahil Gupta
* This file contains the methods for the MOESIF protocol which follows the snooping protocol.
*/

/*************************
 * Constructor/Destructor.
 *************************/
MOESIF_protocol::MOESIF_protocol (Hash_table *my_table, Hash_entry *my_entry)
    : Protocol (my_table, my_entry)
{
    // Initialize lines to not have the data yet!
    this->state = MOESIF_CACHE_I;
}

MOESIF_protocol::~MOESIF_protocol ()
{    
}

void MOESIF_protocol::dump (void)
{
    const char *block_states[12] = {"X", "I", "S", "E", "O", "M", "F", "IS", "IM", "SM", "OM", "FM"};
    fprintf (stderr, "MOESIF_protocol - state: %s\n", block_states[state]);
}

void MOESIF_protocol::process_cache_request (Mreq *request)
{
	switch (state) {
    case MOESIF_CACHE_F:  do_cache_F (request); break;
    case MOESIF_CACHE_I:  do_cache_I (request); break;
    case MOESIF_CACHE_S:  do_cache_S (request); break;
    case MOESIF_CACHE_E:  do_cache_E (request); break;
    case MOESIF_CACHE_O:  do_cache_O (request); break;
    case MOESIF_CACHE_M:  do_cache_M (request); break;
    case MOESIF_CACHE_IM_Intermediate: break;
    case MOESIF_CACHE_IS_Intermediate: break;
    case MOESIF_CACHE_SM_Intermediate: break;
    case MOESIF_CACHE_OM_Intermediate: break;
    case MOESIF_CACHE_FM_Intermediate: break;
    default:
        fatal_error ("Invalid Cache State for MOESIF Protocol\n");
    }
}

void MOESIF_protocol::process_snoop_request (Mreq *request)
{
	switch (state) {
    case MOESIF_CACHE_F:  
    	do_snoop_F (request); 
    	break;
    case MOESIF_CACHE_I:  
    	do_snoop_I (request); 
    	break;
    case MOESIF_CACHE_S:  
    	do_snoop_S (request);
    	break;
    case MOESIF_CACHE_E:  
    	do_snoop_E (request); 
    	break;
    case MOESIF_CACHE_O:  
    	do_snoop_O (request); 
    	break;
    case MOESIF_CACHE_M:  
    	do_snoop_M (request); 
    	break;
    case MOESIF_CACHE_IM_Intermediate:  
    	do_snoop_IM_Intermediate (request); 
    	break;
    case MOESIF_CACHE_IS_Intermediate:  
    	do_snoop_IS_Intermediate (request); 
    	break;
    case MOESIF_CACHE_SM_Intermediate:  
    	do_snoop_SM_Intermediate (request); 
    	break;
    case MOESIF_CACHE_OM_Intermediate:  
    	do_snoop_OM_Intermediate (request); 
    	break;
    case MOESIF_CACHE_FM_Intermediate:  
    	do_snoop_FM_Intermediate (request); 
    	break;
    default:
    	fatal_error ("Invalid Cache State for MOESIF Protocol\n");
    }
}

inline void MOESIF_protocol::do_cache_F (Mreq *request)
{
    switch (request->msg) {
    case LOAD:
    	//data is clean and can be sent to the processor
        send_DATA_to_proc(request->addr);
        break;
    case STORE:
    	//put the request on the bus for block with the intent to modify
    	//this will lead to a cache miss
        send_GETM(request->addr);
        state = MOESIF_CACHE_FM_Intermediate;
        Sim->cache_misses++;
        break;
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_I (Mreq *request)
{
    switch (request->msg) {
    case LOAD:
        //this will lead to a cache miss
    	//get the data first from the memory with the intent to share
        send_GETS(request->addr);
        state = MOESIF_CACHE_IS_Intermediate;
        Sim->cache_misses++;
        break;
    case STORE:
    	//this will lead to a cache miss
    	//get the data first from the memory with the intent to modify
        send_GETM(request->addr);
        state = MOESIF_CACHE_IM_Intermediate;
        Sim->cache_misses++;
        break;
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_S (Mreq *request)
{
    switch (request->msg) {
    case LOAD:
    	//data is clean and can be sent to the processor
        send_DATA_to_proc(request->addr);
        break;
    case STORE:
    	//put the request on the bus for block with the intent to modify
    	//this will lead to a cache miss
        send_GETM(request->addr);
        state = MOESIF_CACHE_IM_Intermediate;
        Sim->cache_misses++;
        break;
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_E (Mreq *request)
{
    switch (request->msg) {
    case LOAD:
        send_DATA_to_proc(request->addr);
        break;
    case STORE:
    	//now we can send the data to the processor
        send_DATA_to_proc(request->addr);
        state = MOESIF_CACHE_M;
        Sim->silent_upgrades++;
        break;
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_O (Mreq *request)
{
    switch (request->msg) {
    case LOAD:
    	//data is clean and can be sent to the processor
        send_DATA_to_proc(request->addr);
        break;
    case STORE:
    	//put the request on the bus for block with the intent to modify
    	//this will lead to a cache miss
        send_GETM(request->addr);
        state = MOESIF_CACHE_OM_Intermediate;
        Sim->cache_misses++;
        break;
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_cache_M (Mreq *request)
{
    switch (request->msg) {
    case LOAD:
    case STORE:
    	//now we can send the data to the processor
        send_DATA_to_proc(request->addr);
        break;
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_F (Mreq *request)
{
	switch (request->msg) {
		case GETS:
            set_shared_line();
            send_DATA_on_bus(request->addr, request->src_mid);
            break;
       	case GETM:
          	state = MOESIF_CACHE_I;
            set_shared_line();
            send_DATA_on_bus(request->addr, request->src_mid);
            break;
       	 case DATA:
         	default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
	}
}

inline void MOESIF_protocol::do_snoop_I (Mreq *request)
{
    /**
    * If we snoop a message from another cache and we are in I, then we don't
    * need to do anything!  We obviously cannot supply data since we don't have
    * it, and we don't need to downgrade our state since we are already in I.
    */
    switch (request->msg) {
    case GETS: break;
    case GETM: break;
    case DATA: break;
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_S (Mreq *request)
{
    switch (request->msg) {
    case GETS:
        set_shared_line();
        break;
    case GETM:
        state = MOESIF_CACHE_I;
        break;
    case DATA: break;
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_E (Mreq *request)
{
    switch (request->msg) {
    case GETS:
        set_shared_line();
        send_DATA_on_bus(request->addr,request->src_mid);
        state = MOESIF_CACHE_F;
        break;
    case GETM:
        set_shared_line();
        send_DATA_on_bus(request->addr,request->src_mid);
        state = MOESIF_CACHE_I;
        break;
    case DATA: break;
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_O (Mreq *request)
{
    switch (request->msg) {
    case GETS:
        set_shared_line();
        send_DATA_on_bus(request->addr,request->src_mid);
        break;
    case GETM:
        set_shared_line();
        send_DATA_on_bus(request->addr,request->src_mid);
        state = MOESIF_CACHE_I;
        break;
    case DATA: 
        break;
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_M (Mreq *request)
{
    switch (request->msg) {
    case GETS:
        set_shared_line();
        send_DATA_on_bus(request->addr,request->src_mid);
        state = MOESIF_CACHE_O;
        break;
    case GETM:
        set_shared_line();
        send_DATA_on_bus(request->addr,request->src_mid);
        state = MOESIF_CACHE_I;
        break;
    case DATA: break;
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}


inline void MOESIF_protocol::do_snoop_IS_Intermediate (Mreq *request)
{
	switch (request->msg) {
	case GETS:
	case GETM:
		break;
	case DATA:
		send_DATA_to_proc(request->addr);
		
		if (get_shared_line())
		{
			state = MOESIF_CACHE_S;
		}
		else{
			state = MOESIF_CACHE_E;
		}
		break;
	default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
	}
}

inline void MOESIF_protocol::do_snoop_IM_Intermediate (Mreq *request)
{
    switch (request->msg) {
    case GETS: break;
    case GETM: break;
    case DATA:
        send_DATA_to_proc(request->addr);
        state = MOESIF_CACHE_M;            
        break;
    default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_SM_Intermediate (Mreq *request)
{
	switch (request->msg) {
	case GETS:
		set_shared_line();
	case GETM:
		if (request->src_mid != my_table->moduleID){
			state = MOESIF_CACHE_IM_Intermediate;
		}
		break;
	case DATA:
		send_DATA_to_proc(request->addr);
		state = MOESIF_CACHE_M;
		
		break;
	default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
	}
}

inline void MOESIF_protocol::do_snoop_OM_Intermediate (Mreq *request)
{
	switch (request->msg) {
	case GETS:
		set_shared_line();
		send_DATA_on_bus(request->addr,request->src_mid);
		break;
	case GETM:
		send_DATA_on_bus(request->addr,request->src_mid);
		state = MOESIF_CACHE_IM_Intermediate;
		break;
	case DATA:
		state = MOESIF_CACHE_M;
		break;
	default:
        request->print_msg (my_table->moduleID, "ERROR");
        fatal_error ("Client: I state shouldn't see this message\n");
    }
}

inline void MOESIF_protocol::do_snoop_FM_Intermediate (Mreq *request)
{
	switch(request->msg) {
	case GETS:
		set_shared_line();
		send_DATA_on_bus(request->addr,request->src_mid);
		break;
	case GETM:
		send_DATA_on_bus(request->addr,request->src_mid);
		if (request->src_mid != my_table->moduleID){
			state = MOESIF_CACHE_IM_Intermediate;
		}
		break;
	case DATA:
	   	state = MOESIF_CACHE_M;
	   	send_DATA_to_proc(request->addr);
	   	break;
	default:
	   	request->print_msg (my_table->moduleID, "ERROR");
       		fatal_error ("Client: SM state shouldn't see this message\n");
    }
}
