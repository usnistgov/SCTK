/* file mupper.c */

#if !defined(COMPILE_ENVIRONMENT)
#include "stdcenvf.h" /* std compile environment for functions */
#endif

 /*********************************************/
 /*  make_upper(s)                            */
 /*  makes *s be all upper case               */
 /*  returns pointer to *s                    */
 /*********************************************/
  char *make_upper(char *s)
  {char *pi;
   pi = s;
   while (*pi != '\0')
      {*pi = (char)toupper((int)*pi);
       pi += 1;
      }
   return s;
  }
