/*
 * Revision Control Information
 *
 * $Source: /vol/opua/opua2/sis/sis-1.1/common/src/sis/genlib/RCS/genlib_int.h,v $
 * $Author: sis $
 * $Revision: 1.2 $
 * $Date: 1992/05/06 18:53:42 $
 *
 */
/* file @(#)genlib_int.h	1.1                      */
/* last modified on 5/29/91 at 12:35:25   */
#include <stdio.h>
#include "sis.h"
#include "genlib.h"
#include "sptree.h"
#include "comb.h"


typedef struct function_struct function_t;
struct function_struct {
    char *name;
    tree_node_t *tree;
    function_t *next;		/* someday, multiple-output */
};


typedef struct pin_info_struct pin_info_t;
struct pin_info_struct {
    char *name;
    char *phase;
    double value[10];
    pin_info_t *next;
};

/* sequential support */
typedef struct latch_info_struct latch_info_t;
struct latch_info_struct {
    char *in;
    char *out;
    char *type;
};

typedef struct constraint_info_struct constraint_info_t;
struct constraint_info_struct {
  char *name;
  double setup;
  double hold;
  constraint_info_t *next;
};

extern FILE *gl_out_file;
extern int read_lineno;		/* hack ! */
