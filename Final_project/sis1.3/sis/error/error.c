/*
 * Revision Control Information
 *
 * $Source: /vol/opua/opua2/sis/sis-1.1/common/src/sis/error/RCS/error.c,v $
 * $Author: sis $
 * $Revision: 1.2 $
 * $Date: 1992/05/06 18:52:17 $
 *
 */
#include "sis.h"

static char *error_str = 0;
static int error_str_len, error_str_maxlen;


void
error_init()
{
    if (error_str != 0) {
	FREE(error_str);
    }
    error_str_len = 0;
    error_str_maxlen = 100;
    error_str = ALLOC(char, error_str_maxlen);
    *error_str = '\0';
}


void
error_append(s)
char *s;
{
    int slen;

    slen = strlen(s);
    if (error_str_len + slen + 1 > error_str_maxlen) {
	error_str_maxlen = (error_str_len + slen) * 2;	/* cstevens@ic */
	error_str = REALLOC(char, error_str, error_str_maxlen);
    }
    (void) strcpy(error_str + error_str_len, s);
    error_str_len += slen;
}


char *
error_string()
{
    return error_str;
}


void
error_cleanup()
{
    FREE(error_str);
    error_str_len = 0;
    error_str_maxlen = 0;
    error_str = 0;
}
