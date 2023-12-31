/*
 * Revision Control Information
 *
 * $Source: /vol/opua/opua2/sis/sis-1.2/common/src/sis/io/RCS/read_eqn.y,v $
 * $Author: sis $
 * $Revision: 1.4 $
 * $Date: 1993/06/02 23:48:07 $
 *
 */
%{
#include "sis.h"
#include "io_int.h"
#include <setjmp.h>

#ifdef YYBYACC
extern char *yytext;
#else
#if defined(hpux) || defined(__osf__)
extern unsigned char yytext[];
#else
extern char yytext[];
#endif
#endif

static network_t *global_network;
static lsList po_list;

static int yyerror();
#ifndef FLEX_SCANNER
#undef yywrap
static int input();
static int unput();
static int yywrap();
#endif


static void
do_assign(name, expr)
char *name;
node_t *expr;
{
    char errmsg[1024];
    node_t *node;

    node = read_find_or_create_node(global_network, name);
    if (node_function(node) != NODE_UNDEFINED) {
	(void) sprintf(errmsg, "Attempt to redefine '%s'\n", name);
	yyerror(errmsg);   /* never returns */
    }
    FREE(name);
    node_replace(node, expr);
}


static node_t *
do_sexpr_list(list, func)
array_t *list;
node_t *(*func)();
{
    int i;
    node_t *node, *node1, *node2;

    node1 = array_fetch(node_t *, list, 0);
    node = node_dup(node1);
    node_free(node1);
    for(i = 1; i < array_n(list); i++) {
	node1 = node;
	node2 = array_fetch(node_t *, list, i);
	node = (*func)(node1, node2);
	node_free(node1);
	node_free(node2);
    }
    array_free(list);
    return node;
}

%}

%union {
    char *strval;
    node_t *node;
    array_t *array;
}

%type <strval> identifier
%type <node> expr sexpr
%type <array> sexpr_list
%left OPR_OR
%left OPR_AND CONST_ZERO CONST_ONE IDENTIFIER LPAREN
%left OPR_XOR OPR_XNOR
%left OPR_NOT OPR_NOT_POST
%token NAME INORDER OUTORDER PASS ASSIGN SEMI LPAREN RPAREN END
%start program

%%
program :	/* EMPTY */
	|	prog
	|	prog END SEMI
	|	sexpr
		    { do_assign(util_strsav("SILLY"), $1); }
	;

prog	:	stat SEMI
	|       prog stat SEMI
	;

stat    :	/* empty */
	|	NAME ASSIGN identifier
		    {
			network_set_name(global_network, $3);
			FREE($3);
		    }

	|	INORDER ASSIGN input_list

	|	OUTORDER ASSIGN output_list

     	|	identifier ASSIGN expr
		    { do_assign($1, $3); }

     	|	LPAREN ASSIGN identifier sexpr RPAREN
		    { do_assign($3, $4); }
	;


expr	:	expr OPR_OR expr
		    { $$ = node_or($1, $3); node_free($1); node_free($3);}

	|	expr OPR_AND expr
		    { $$ = node_and($1, $3); node_free($1); node_free($3);}

	|	expr expr %prec OPR_AND
		    { $$ = node_and($1, $2); node_free($1); node_free($2);}

	|	expr OPR_XOR expr
		    { $$ = node_xor($1, $3); node_free($1); node_free($3);}

	|	expr OPR_XNOR expr
		    { $$ = node_xnor($1, $3); node_free($1); node_free($3);}

	|	expr OPR_NOT_POST
		    { $$ = node_not($1); node_free($1); }

	|	OPR_NOT expr
		    { $$ = node_not($2); node_free($2); }

	|	CONST_ZERO
		    { $$ = node_constant(0); }

	|	CONST_ONE
		    { $$ = node_constant(1); }

	|	LPAREN expr RPAREN
		    { $$ = $2; }

	|	identifier
		    {
			node_t *node;
			node = read_find_or_create_node(global_network, $1);
			$$ = node_literal(node, 1);
			FREE($1);
		    }
	;


sexpr	:	LPAREN OPR_AND sexpr_list RPAREN
		    { $$ = do_sexpr_list($3, node_and); }

	|	LPAREN OPR_OR sexpr_list RPAREN
		    { $$ = do_sexpr_list($3, node_or); }

	|	LPAREN OPR_NOT sexpr RPAREN
		    { $$ = node_not($3); node_free($3); }

	|	CONST_ZERO
		    { $$ = node_constant(0); }

	|	CONST_ONE
		    { $$ = node_constant(1); }

	|	LPAREN CONST_ZERO RPAREN
		    { $$ = node_constant(0); }

	|	LPAREN CONST_ONE RPAREN
		    { $$ = node_constant(1); }

	|	identifier
		    {
			node_t *node;
			node = read_find_or_create_node(global_network, $1);
			$$ = node_literal(node, 1);
			FREE($1);
		    }
	;


sexpr_list:	sexpr
		    {
			$$ = array_alloc(node_t *, 10);
			array_insert_last(node_t *, $$, $1);
		    }
	|	sexpr_list sexpr
		    {
			array_insert_last(node_t *, $1, $2);
		    }

identifier:	IDENTIFIER
		    { $$ = util_strsav(yytext); }
	;


input_list:	/* empty */
	|	input_list identifier
		    {
			node_t *node;
			char errmsg[1024];
			node = read_find_or_create_node(global_network, $2);
			if (node_function(node) != NODE_UNDEFINED) {
			    (void) sprintf(errmsg, 
				"Attempt to redefine '%s'\n", $2);
			    yyerror(errmsg);   /* never returns */
			}
			network_change_node_type(global_network, 
							node, PRIMARY_INPUT);
			FREE($2);
		    }
	;


output_list:	/* empty */
	|	output_list identifier
		    {
			node_t *node;
			node = read_find_or_create_node(global_network, $2);
			LS_ASSERT(lsNewEnd(po_list, (lsGeneric) node, LS_NH));
			FREE($2);
		    }
	;

%%

#include "eqnlex.c"

static char *string;
static int file_mode;
static int nbuffer;
static char buffer[20];
static int last_was_newline;
static jmp_buf jmpbuf;

#ifndef FLEX_SCANNER
static int
input()
{
    register int c;
    register char *prompt;

    if (nbuffer > 0) {
	c = buffer[--nbuffer];
    } else if (file_mode) {
	if (last_was_newline && yyin == stdin) {
	    prompt = com_get_flag("prompt2");
	    if (prompt != NULL) fputs(prompt,stdout);
	}
	c = getc(yyin);
    } else {
	c = *string;
	if (c != 0) string++;
    }

    last_was_newline = (c == '\n');
    if (last_was_newline) read_lineno++;
    if (c == EOF) c = 0;
    return c;
}

static int
unput(c)
int c;
{
    if (nbuffer < 20) buffer[nbuffer++] = c;
    if (c == '\n') read_lineno--;
}
#else

char *eqn_inp_ptr;
char* eqn_inp_lim;

int eqn_input(char* buf, int max_size)
{
    char c;
    int i, n;
	 
    if (! file_mode) {
	if (max_size > (eqn_inp_lim - eqn_inp_ptr))
	    n = eqn_inp_lim - eqn_inp_ptr;
	else
	    n= max_size;
	if (n > 0) {
	    for (i = 0; i < n; i++)
		if (eqn_inp_ptr[i] == '\n') {
		    read_lineno++;
		    i++;
		    break;
		}
	    memcpy (buf, eqn_inp_ptr, i);
	    eqn_inp_ptr += i;
	}
    } else {
	c = getc(yyin);
	if (c == '\n')
	    read_lineno++;
	buf[0] = (c == EOF) ? (n = 0, YY_NULL) : (n = 1, c);
    }
    return n;
}

#endif

static int
equation_setup_string(s)
char *s;
{
    read_register_filename(NIL(char));
    file_mode = 0;
    string = s;
#ifdef FLEX_SCANNER
    yy_switch_to_buffer(
                 yy_create_buffer( yyin, YY_BUF_SIZE ) );
    yyrestart(yyin); 
    eqn_inp_lim = string + strlen(string);
    eqn_inp_ptr = string;
#endif
    nbuffer = 0;
}


static int
equation_setup_file(fp)
FILE *fp;
{
    yyin = fp;
#ifdef FLEX_SCANNER
    yy_switch_to_buffer(
                 yy_create_buffer( yyin, YY_BUF_SIZE ) );
    yyrestart(yyin); 
    BEGIN(INITIAL);
#endif
    file_mode = 1;
    nbuffer = 0;
    last_was_newline = 1;
}


static int
yyerror(errmsg)
char *errmsg;
{
    read_error(errmsg);
    longjmp(jmpbuf, 1);
}

#undef yywrap
static int
yywrap()
{
    return 1;
}


network_t *
read_eqn(fp)
FILE *fp;
{
    error_init();

    if (setjmp(jmpbuf)) {
	/* syntax error -- return from yyerror() */
	LS_ASSERT(lsDestroy(po_list, (void (*)()) 0));
	network_free(global_network);
	return 0;

    } else {
	global_network = network_alloc();
	read_filename_to_netname(global_network, read_filename);
	po_list = lsCreate();
	equation_setup_file(fp);
	(void) yyparse();

	if (!read_check_io_list(global_network, po_list)) {
	    network_free(global_network);
	    return 0;
	}
	read_hack_outputs(global_network, po_list);
	LS_ASSERT(lsDestroy(po_list, (void (*)()) 0));

	if (! network_is_acyclic(global_network)) {
	    network_free(global_network);
	    return 0;
	}

	return global_network;
    }
}

network_t *
read_eqn_string(s)
char *s;
{
    error_init();

    if (setjmp(jmpbuf)) {
	/* syntax error -- return from yyerror() */
	LS_ASSERT(lsDestroy(po_list, (void (*)()) 0));
	network_free(global_network);
	return 0;

    } else {
	global_network = network_alloc();
	po_list = lsCreate();
	equation_setup_string(s);
	(void) yyparse();

	if (!read_check_io_list(global_network, po_list)) {
	    network_free(global_network);
	    return 0;
	}
	read_hack_outputs(global_network, po_list);
	LS_ASSERT(lsDestroy(po_list, (void (*)()) 0));

	if (! network_is_acyclic(global_network)) {
	    network_free(global_network);
	    return 0;
	}

	return global_network;
    }
}
