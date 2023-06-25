/*
 * Revision Control Information
 *
 * $Source: /vol/opua/opua2/sis/sis-1.1/common/src/sis/test/RCS/init_test.c,v $
 * $Author: sis $
 * $Revision: 1.2 $
 * $Date: 1992/05/06 19:03:20 $
 *
 */
#include "sis.h"
#include "test_int.h"


/*
 *  called when the program starts up
 */
init_test()
{
    com_add_command("_test", com_test, /* changes_network */ 1);
}
