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

boolcmp(s,t)
char s[],t[];

{
/* returns 0 if s and t are equal in the boolean sense  -
   returns 1 otherwise                                     */

int i,result;

i = 0;
result = 0;
while (s[i] != '\0' && result == 0)
{
  if ( (s[i] == '0' && t[i] == '1') || (s[i] == '1' && t[i] == '0' ) ) 
    result = 1;
  i++;
}
  
return(result);

}
