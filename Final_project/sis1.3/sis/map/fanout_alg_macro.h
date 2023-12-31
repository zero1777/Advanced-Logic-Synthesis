/*
 * Revision Control Information
 *
 * $Source: /vol/opua/opua2/sis/sis-1.1/common/src/sis/map/RCS/fanout_alg_macro.h,v $
 * $Author: sis $
 * $Revision: 1.2 $
 * $Date: 1992/05/06 18:55:51 $
 *
 */
/* file @(#)fanout_alg_macro.h	1.3 */
/* last modified on 6/28/91 at 17:21:06 */

	/* Functions to initialize fanout optimization algorithms. */

extern void noalg_init();
extern void lt_trees_init();
extern void two_level_init();
extern void fanout_dump_init();
extern void mixed_lt_trees_init();
extern void bottom_up_init();
extern void balanced_init();
extern void top_down_init();

	/* Functions to update fanout optimization properties. */

extern void fanout_opt_update_size_threshold();
extern void fanout_opt_update_peephole_flag();
extern void lt_trees_set_max_n_gaps();
extern void fanout_dump_set_dump_threshold();
extern void mixed_lt_trees_set_max_n_gaps();
extern void mixed_lt_trees_set_max_x_index();
extern void mixed_lt_trees_set_max_y_index();
extern void top_down_set_mode();
extern void top_down_set_debug();
