/* file gdatadef.h         */
/* global data definitions */
#ifndef GDATADEF_HEADER
#define GDATADEF_HEADER

 int db_level = 0;
 int detail_level = 0;
 char db[272] = "*DB: ", *pdb = &db[0];
 boolean memory_trace = F;
 char null_char_string[1] = "", *EMPTY_STRING = &null_char_string[0];
 char *NIL_STRING = "(nil)";
 int nstr_empty[1] = {0}, *EMPTY_INT_STRING = &nstr_empty[0];

#endif
