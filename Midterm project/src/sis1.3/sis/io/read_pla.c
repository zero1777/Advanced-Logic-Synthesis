/*
 * Revision Control Information
 *
 * $Source: /vol/opua/opua2/sis/sis-1.1/common/src/sis/io/RCS/read_pla.c,v $
 * $Author: sis $
 * $Revision: 1.2 $
 * $Date: 1992/05/06 18:54:43 $
 *
 */
#include "sis.h"
#include "io_int.h"

network_t *
sis_read_pla(fp, single_output)
FILE *fp;
int single_output;
{
    pPLA PLA;
    network_t *network;
    network_t *dc_network;

    if ((PLA = espresso_read_pla(fp)) == 0) {
    return 0;
    }
    if (single_output) {
	if ((network = pla_to_network_single(PLA)) != 0) {
	    read_filename_to_netname(network, read_filename);
	    dc_network = pla_to_dcnetwork_single(PLA);
	    if (dc_network != NIL (network_t)){
	    read_filename_to_netname(dc_network, read_filename);
	    network_sweep(dc_network);
	    network_collapse(dc_network);
	    }
	    network->dc_network = dc_network;
	}
    } else {
	if ((network = pla_to_network(PLA)) != 0) {
	    read_filename_to_netname(network, read_filename);
	    dc_network = pla_to_dcnetwork_single(PLA);
	    if (dc_network != NIL (network_t)){
	    read_filename_to_netname(dc_network, read_filename);
	    }
	    network->dc_network = dc_network;
	}
    }
    discard_pla(PLA);

    return network;
}
