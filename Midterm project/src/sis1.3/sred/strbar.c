/*
 * Revision Control Information
 *
 * $Source$
 * $Author$
 * $Revision$
 * $Date$
 *
 */
#include "reductio.h"

strbar(s)
char s[];

{
/* returns 1 if s is a string of all '-' (bars , meaning don't care) */
int i,allbar;

i = 0;
if (osymb) {
    allbar = !strcmp(s, ASTER);
}
else {
  allbar = 1;
  while (s[i] != '\0' && allbar == 1)
  {
    if (s[i] != '-') allbar = 0;
    i++;
  }
}  
return(allbar);

}
