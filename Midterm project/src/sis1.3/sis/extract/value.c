/*
 * Revision Control Information
 *
 * $Source: /vol/opua/opua2/sis/sis-1.2/common/src/sis/extract/RCS/value.c,v $
 * $Author: sis $
 * $Revision: 1.3 $
 * $Date: 1993/07/19 22:21:17 $
 *
 */
#include "sis.h"
#include "extract_int.h"


value_cell_t *
value_cell_alloc()
{
    value_cell_t *value_cell;

    value_cell = ALLOC(value_cell_t, 1);
    value_cell->cube_number = 0;
    value_cell->sis_index = -1;
    value_cell->value = 1;
    value_cell->ref_count = 1;
    return value_cell;
}


void
value_cell_free(value_cell)
value_cell_t *value_cell;
{
    if (--value_cell->ref_count == 0) {
	FREE(value_cell);
    }
}


void
free_value_cells(A)
sm_matrix *A;
{
    register sm_row *prow;
    register sm_element *p;

    sm_foreach_row(A, prow) {
	sm_foreach_row_element(prow, p) {
	    value_cell_free((value_cell_t *) p->user_word);
	}
    }
}
