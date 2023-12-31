/*
 * Revision Control Information
 *
 * $Source: /vol/opua/opua2/sis/sis-1.1/common/src/sis/node/RCS/nodeindex.h,v $
 * $Author: sis $
 * $Revision: 1.2 $
 * $Date: 1992/05/06 18:57:41 $
 *
 */
#ifndef NODEINDEX_H
#define NODEINDEX_H


/*
 *  a quick way to associate cubes (i.e., sm_row *) to small integers
 *  and back.  Used to build the kernel_cube matrix
 */

typedef struct nodeindex_struct nodeindex_t;
struct nodeindex_struct {
    st_table *node_to_int;
    array_t *int_to_node;
};


EXTERN struct nodeindex_struct *nodeindex_alloc ARGS((void));
EXTERN void nodeindex_free ARGS((struct nodeindex_struct *));
EXTERN int nodeindex_insert ARGS((struct nodeindex_struct *, node_t *));
EXTERN int nodeindex_indexof ARGS((struct nodeindex_struct *, node_t *));
EXTERN node_t *nodeindex_nodeof ARGS((struct nodeindex_struct *, int));

#endif
