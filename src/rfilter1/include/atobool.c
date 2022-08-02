 /* file atobool.c */

#if !defined(COMPILE_ENVIRONMENT)
#include "stdcenvf.h" /* std compile environment for functions */
#endif

 boolean atobool(Char *s)
         
 /*********************************************/
 /*  Returns T if string s is "T" or "TRUE"   */
 /* in upper or lower case, F otherwise.      */
 /*********************************************/
 {boolean ignore_case = T, ans;
   if ((string_equal(s,(Char *)"T",ignore_case)) ||
       (string_equal(s,(Char *)"YES",ignore_case)) ||
       (string_equal(s,(Char *)"TRUE",ignore_case))) ans = T;
  else ans = F;
  return ans;
 }
