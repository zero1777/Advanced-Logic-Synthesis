/*
 * Revision Control Information
 *
 * $Source: /vol/opua/opua2/sis/sis-1.1/common/src/sis/io/RCS/write_eqn.c,v $
 * $Author: sis $
 * $Revision: 1.2 $
 * $Date: 1992/05/06 18:54:43 $
 *
 */
#include "sis.h"
#include "io_int.h"

void
write_eqn(fp, network, short_name)
FILE *fp;
network_t *network;
int short_name;
{
    lsGen gen;
    node_t *node;

    io_fput_params("\n", 78);

    io_fputs_break(fp, "INORDER =");
    foreach_primary_input(network, gen, node) {
	io_fputc_break(fp, ' ');
	io_write_name(fp, node, short_name);
    }
    io_fputs_break(fp, ";\n");

    io_fputs_break(fp, "OUTORDER =");
    foreach_primary_output(network, gen, node) {
	io_fputc_break(fp, ' ');
	io_write_name(fp, node, short_name);
    }
    io_fputs_break(fp, ";\n");

    foreach_node(network, gen, node) {
	write_sop(fp, node, short_name, 0);
    }
}


void					/* cstevens@ic */
write_sop(fp, node, short_name, slif_flag)
FILE *fp;
node_t *node;
int short_name;
int slif_flag;
{
    register int i, j;
    int first_cube, first_lit;
    node_cube_t cube;
    node_literal_t literal;
    node_t *fanin;

    if (io_node_should_be_printed(node) == 0) {
        return;
    }

    io_write_name(fp, node, short_name);
    io_fputs_break(fp, " = ");

    switch(node_function(node)) {
    case NODE_0:
	io_fputs_break(fp, "0;\n");
	return;
    case NODE_1:
	io_fputs_break(fp, "1;\n");
	return;
    case NODE_PO:
	io_write_name(fp, node_get_fanin(node, 0), short_name);
	io_fputs_break(fp, ";\n");
	return;
    }

    first_cube = 1;
    for(i = node_num_cube(node)-1; i >= 0; i--) {
	if (! first_cube) {
	    io_fputs_break(fp, " + ");
	}
	cube = node_get_cube(node, i);

	first_lit = 1;
	foreach_fanin(node, j, fanin) {
	    literal = node_get_literal(cube, j);
	    switch(literal) {
	    case ZERO:
	    case ONE:
		if (!slif_flag) {
		    if (! first_lit) {
			io_fputc_break(fp, '*');
		    }
		    if (literal == ZERO) {
			io_fputc_break(fp, '!');
		    }
		} else {
		    if (! first_lit) {
			io_fputc_break(fp, ' ');
		    }
		}
		io_write_name(fp, fanin, short_name);
		if (slif_flag) {
		    if (literal == ZERO) {
			io_fputc_break(fp, '\'');
		    }
		}
		first_lit = 0;
		break;
	    }
	}
	first_cube = 0;
    }
    io_fputs_break(fp, ";\n");
}
