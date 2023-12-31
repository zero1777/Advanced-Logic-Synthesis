/*
 * Revision Control Information
 *
 * $Source: /vol/opua/opua2/sis/sis-1.1/common/src/sis/minimize/RCS/min_int.h,v $
 * $Author: sis $
 * $Revision: 1.2 $
 * $Date: 1992/05/06 18:57:23 $
 *
 */
#include "ros.h"

/* Functions defined in  minimize.c */

/* minimize.c */ extern pcover nocomp();
/* minimize.c */ extern pcover snocomp();
/* minimize.c */ extern pset_family ncp_unate_compl();
/* minimize.c */ extern pset_family ncp_unate_complement();
/* minimize.c */ extern void ncp_compl_special_cases();
/* minimize.c */ extern pcover nc_super_gasp();
/* minimize.c */ extern pcover nc_all_primes();
/* minimize.c */ extern pcover nc_make_sparse();
/* minimize.c */ extern pcover nc_expand_gasp();
/* minimize.c */ extern void nc_expand1_gasp();
/* minimize.c */ extern pcover nc_last_gasp();
/* minimize.c */ extern pcover nc_reduce_gasp();
/* minimize.c */ extern pcover nc_reduce();
/* minimize.c */ extern pcover nc_random_order();
/* minimize.c */ extern pcover nc_permute();
/* minimize.c */ extern pcover nc_mini_sort();
/* minimize.c */ extern pcube nc_reduce_cube();
/* minimize.c */ extern pcube nc_sccc();
/* minimize.c */ extern bool nc_sccc_special_cases();
/* minimize.c */ extern pcover ncp_expand();
/* minimize.c */ extern void ncp_expand1();
/* minimize.c */ extern pcover nc_first_expand();
/* minimize.c */ extern void rem_unnec_r_cubes();
/* dcsimp.c */ extern pcover dcsimp();
