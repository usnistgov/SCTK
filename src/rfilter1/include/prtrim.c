/* file prtrim.c */

#if !defined(COMPILE_ENVIRONMENT)
#include "stdcenvf.h" /* std compile environment for functions */
#endif

 /***********************************************/
 /*  prtrim(s)                                  */
 /*  Returns a pointer to the last              */
 /*  non-whitespace character in the string s,  */
 /***********************************************/
  char *prtrim(char *s)
  {char *p1;
   p1 = s + strlen(s) - 1;
   while ((p1 >= s) && (isspace(*p1))) p1--;
   return p1;
  }
