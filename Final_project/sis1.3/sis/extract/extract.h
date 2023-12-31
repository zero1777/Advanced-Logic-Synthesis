/*
 * Revision Control Information
 *
 * $Source: /vol/opua/opua2/sis/sis-1.2/common/src/sis/extract/RCS/extract.h,v $
 * $Author: sis $
 * $Revision: 1.3 $
 * $Date: 1993/07/19 21:33:38 $
 *
 */
extern void ex_kernel_gen();
extern void ex_subkernel_gen();
extern node_t *ex_find_divisor();
extern node_t *ex_find_divisor_quick();

/* Some definitions for making the structures in fast_extract exportable */

#define SHORT short int
#define UNSIGNED unsigned char

/*  Define the data structure a double cube diviosr */
typedef struct double_cube_divisor_struct ddivisor_t;
struct double_cube_divisor_struct {
    sm_row *cube1;          /* the first cube of the double-cube divisor */
    sm_row *cube2;          /* the second cube of the double-cube divisor */
    lsHandle handle;        /* lsHandle to double-cube divisor set */
    lsHandle sthandle;      /* lsHandle to corresponoding searching table */
    lsList div_index;       /* indicate where the divisor comes from */
    SHORT weight;           /* the weight of double-cube divisor */
    UNSIGNED dtype;         /* the type of double-cube divisor */
    UNSIGNED weight_status; /* indicate the weight can be changed or not */
    UNSIGNED status;        /* indicate whether the divisor is changed or not */
    UNSIGNED level;         /* indicates the level of 2-cube kernels */
};


#define fx_get_div_handle(p)    ((p)->handle)
