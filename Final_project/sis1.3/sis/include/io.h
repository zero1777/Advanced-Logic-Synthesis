/*
 * Revision Control Information
 *
 * $Source: /vol/opua/opua2/sis/sis-1.1/common/src/sis/io/RCS/io.h,v $
 * $Author: sis $
 * $Revision: 1.2 $
 * $Date: 1992/05/06 18:54:43 $
 *
 */
#ifndef IO_H
#define IO_H

EXTERN void write_blif ARGS((FILE *, network_t *, int, int));
EXTERN void write_pla ARGS((FILE *, network_t *));
EXTERN void write_eqn ARGS((FILE *, network_t *, int));

#ifdef SIS
EXTERN int write_kiss ARGS((FILE *, graph_t *));
EXTERN int read_kiss ARGS((FILE *, graph_t **));
EXTERN network_t *read_slif ARGS((FILE *));
#endif /* SIS */

EXTERN int read_blif ARGS((FILE *, network_t **));
EXTERN network_t *sis_read_pla ARGS((FILE *, int));
EXTERN network_t *read_eqn ARGS((FILE *));
EXTERN network_t *read_eqn_string ARGS((char *));

EXTERN void read_register_filename ARGS((char *));

	/* Has variable # arguments -- problems with prototype	*/
EXTERN void read_error ();

#endif
