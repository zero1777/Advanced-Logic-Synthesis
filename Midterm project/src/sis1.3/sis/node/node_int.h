/*
 * Revision Control Information
 *
 * $Source: /vol/opua/opua2/sis/sis-1.1/common/src/sis/node/RCS/node_int.h,v $
 * $Author: sis $
 * $Revision: 1.2 $
 * $Date: 1992/05/06 18:57:41 $
 *
 */
/*
 *  daemon's are in a linked list by daemon type
 */

typedef struct daemon_struct daemon_t;
struct daemon_struct {
    void (*func)();
    daemon_t *next;
};

#define node_has_function(f)		(f->F != 0)

extern void network_rehash_names();
extern void node_complement();
extern void node_invalid();
extern void make_common_base_by_name();
extern void make_common_base();
extern void node_discard_all_daemons();
extern void set_espresso_flags();
extern node_t *node_sort_for_printing();
extern int fancy_lex_order();
extern void node_remove_dup_fanin();
