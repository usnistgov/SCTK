#include "sctk.h"

#ifdef __STDC__
# include <stdarg.h>
#else
# include <varargs.h>
#endif

char static_message_buffer[200];

#ifdef __STDC__
char *rsprintf(char *format , ...)
#else
char *rsprintf(va_alist)
va_dcl
#endif
{
    va_list args;
#ifndef __STDC__    
    char *format;
#endif

#ifdef __STDC__    
    va_start(args,format);
#else
    va_start(args);
    format = va_arg(args,char *);
#endif
    /*    printf("rsprintf:  format: %s\n",format); */
    vsprintf(static_message_buffer,format,args);
    /*    printf("rsprintf:  message: %s\n",static_message_buffer);*/
    return(static_message_buffer);
}

