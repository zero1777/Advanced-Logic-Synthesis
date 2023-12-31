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

myqspivot(array,init,length)
int *array[2];
int init;
int length;

/* returns 0 if array[init],...array[length] have identical keys,
otherwise the index of the larger of the leftmost two different keys */

{
int k; /* runs left to right looking for a different key */
int firstkey; /* value of first key found */

firstkey = array[1][init-1];
for (k=init+1; k<length; k++) /* scan for different key */
{
  if (array[1][k-1] > firstkey) return(k); /* select larger key */
   else if (array[1][k-1]  < firstkey) return(init);
} 
return(0);

}
