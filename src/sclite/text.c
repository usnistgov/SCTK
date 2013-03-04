#include "sctk.h"

//s The encoding is now a typed field
static enum TEXT_ENCODINGS STATIC_ENCODING = ASCII;
static enum TEXT_COMPARENORM STATIC_NORMALIZATION = CASE;
static enum TEXT_LANG_PROFILE STATIC_LPROF = LPROF_GENERIC;

// This contains the known upper and lower conversions.  It is structured as {Upper, Lower}
static int numKCC_babel_turkish =  7;
static int KCC_babel_turkish[][2] = { // Babel Turkish
                                         { 0xC7,  0xE7}, { 0xD6,  0xF6}, {  0xDC,  0xFC}, { 0x130,  0x69}, 
                                         { 0x49, 0x131}, {0x11E, 0x11F},  {0x15E, 0x15F}
                                        };

// These static variables are used as temporary memory for case conversion
static TEXT *STATIC_CASECONVTEXT;
static int STATIC_CASECONVTEXT_SIZE = -1;

#define is_EXTASC(_p)  (((*(_p)) & 0x80) != 0)
#define is_2byte(_p)  (TEXT_nbytes_of_char(_p) == 2)
#define is_ASCII(_p)  (STATIC_ENCODING == ASCII || STATIC_ENCODING == EXTASCII || \
                       (STATIC_ENCODING != ASCII && STATIC_ENCODING != EXTASCII && \
                       (((*(_p)) & 0x80) == 0)))
                        
/// XXX This isn't done until these are all deleted :)
//
//
#define VTisspace(_p) (is_2byte(&(_p)) ? 0 : (is_EXTASC(&(_p)) ? 0 : isspace(_p)))
//#define VTisupper(_p) (is_2byte(&(_p)) ? 0 : \
//		       (is_EXTASC(&(_p)) ? (((unsigned char)(_p) >= 192) && \
//					    ((unsigned char)(_p) <= 223)) \
//			: isupper(_p)))
//#define VTtoupper(_p) ((is_EXTASC(_p)) ? (char)(*((unsigned char *)_p) - 32) : toupper(*(_p)))
//
//#define VTislower(_p) (is_2byte(&(_p)) ? 0 : \
//		       (is_EXTASC(&(_p)) ? ((unsigned char)(_p) >= 224) \
//			: islower(_p)))
//#define VTtolower(_p) ((is_EXTASC(_p)) ? (char)(*((unsigned char *)_p) + 32) : tolower(*(_p)))

// tested
TEXT *TEXT_skip_wspace(TEXT *ptr) {
    while (VTisspace(*ptr)) ptr++;
    return(ptr);
}

// tested 
int end_of_TEXT(TEXT text){
    return(text == '\0');
}

// new 
int TEXT_set_encoding(char *encoding){
    if ((TEXT_strcasecmp((TEXT *)encoding,(TEXT *)"EUC") == 0) ||
	(TEXT_strcasecmp((TEXT *)encoding,(TEXT *)"GB") == 0)){
        STATIC_ENCODING = GB;                                                                                                    
	return(1);
    } else if (TEXT_strcasecmp((TEXT *)encoding,(TEXT *)"EXT_ASCII") == 0) {
        STATIC_ENCODING = EXTASCII;                                                                                                    
	return(1);
    } else if (TEXT_strcasecmp((TEXT *)encoding,(TEXT *)"ASCII") == 0) {
        STATIC_ENCODING = ASCII;                                                                                                    
	return(1);
    } else if (TEXT_strcasecmp((TEXT *)encoding,(TEXT *)"UTF-8") == 0) {
        STATIC_ENCODING = UTF8;                                                                                                    
	return(1);
    }
    return(0);
}

enum TEXT_ENCODINGS TEXT_get_encoding(){
    return(STATIC_ENCODING);
}

// new 
int TEXT_set_lang_prof(char *lprof){
    if (TEXT_strcasecmp((TEXT *)lprof,(TEXT *)"generic") == 0){
        STATIC_LPROF = LPROF_GENERIC;                                                                                                    
	return(1);
    } else if (TEXT_strcasecmp((TEXT *)lprof,(TEXT *)"babel_turkish") == 0) {
        STATIC_LPROF = LPROF_BABEL_TURKISH;                                                                                                    
	return(1);
    }
    return(0);
}

enum TEXT_LANG_PROFILE TEXT_lang_profile(){
    return(STATIC_LPROF);
}

int TEXT_set_compare_normalization(char *normalization){                                                                         
    if (TEXT_strcasecmp((TEXT *)normalization,(TEXT *)"CASE") == 0){                                                             
        STATIC_NORMALIZATION = CASE;                                                                                             
        return(1);                                                                                                               
    } else if (TEXT_strcasecmp((TEXT *)normalization,(TEXT *)"NONE") == 0){                                                           
        STATIC_NORMALIZATION = NONE;                                                                                             
        return(1);                                                                                                               
    }                                                                                                                            
    return(0);                                                                                                                   
}

// tested
// Return the number of bytes consumed be the first char at the pointer
int TEXT_nbytes_of_char(TEXT *p){
    if (STATIC_ENCODING == ASCII || STATIC_ENCODING == EXTASCII){
        return 1;
    } else {
        if (((*p) & 0x80) == 0){
            return 1;
        } else if (STATIC_ENCODING == GB) {
            return 2;
        } else if (STATIC_ENCODING == UTF8){
            if (((*p) & 0x80) == 0){
                return 1;
            } else if (((*p) & 0xE0) == 0xC0){
                return 2;
            } else if (((*p) & 0xf0) == 0xE0){
                return 3;
            } else if (((*p) & 0xF8) == 0xF0){
                return 4;
            } else if (((*p) & 0xFC) == 0xF8){
                return 5;
            } else if (((*p) & 0xFE) == 0xFC){
                return 6;
            } else {
                fprintf(stderr, "Error: UTF-8 parsing of character size failed. first char %x %s\n",(TEXT)*p, p);
                exit(1);
            }
        }
    }
}
   
// tested                                                                                     
void TEXT_delete_chars(TEXT *arr, TEXT *set){ 
    TEXT *last = arr;   
    int i;
    while (*arr != NULL_TEXT){
        int sz = TEXT_nbytes_of_char(arr);
    	if (sz > 1){
	    for (i=0; i<sz; i++)
	       *(last++) = *(arr++);
	} else {
	    if (TEXT_strchr(set,*arr) == NULL)
		*(last++) = *(arr++);
	    else
		arr++;
	}
    }
    *last = (TEXT)'\0';
}

//// tested
//void TEXT_separate_chars(TEXT *from, TEXT **to, int *to_size, int flag){
//    int cs, os, i;
//    TEXT *tp = *to;
//    int not_ASCII = (flag & CALI_NOASCII) != 0;
//    int del_HYPHEN = (flag & CALI_DELHYPHEN) != 0;
//    int charSize;
//    *tp = '\0';
//    while (*from != NULL_TEXT){
//        cs = TEXT_nbytes_of_char(from);
//        if (cs == 1 && *from == ' ') {
//           from++;
//	   if (tp-1 >= *to && (*(tp-1) != ' ')) *(tp++) = ' ';
//	      continue;
//        }
//	
//	if ((tp-*to) + cs > *to_size-3){
//	    os = tp-*to;
//	    expand_singarr((*to),os,(*to_size),2,TEXT);
//	    tp = *to + os;
//	}
//	if ((tp-*to) != 0 && (*(tp-1) != ' ')) 
//	    if ((cs == 1 && !not_ASCII) || (cs > 1)) {
//		*(tp++) = ' ';
//	    } 
//        for (i=0; i<cs; i++)
//          *(tp++) = *(from++);
//    }
//    *tp = '\0';
//    if (del_HYPHEN)
//	TEXT_delete_chars(*to,(TEXT *)"-");
//}

// tested
void TEXT_separate_chars(TEXT *from, TEXT **to, int *to_size, int flag){
    int lastcs, cs, os, i, lastIsAscii, isAscii, isHyphen, isSpace, lastIsSpace;
    TEXT *tp = *to, *next;
    int not_ASCII = (flag & CALI_NOASCII) != 0;
    int del_HYPHEN = (flag & CALI_DELHYPHEN) != 0;
    int charSize;
    *tp = '\0';
    
    // Handle the empty string
    if (*from == NULL_TEXT)
        return;

    lastIsAscii = 0;
    lastIsSpace = 1;
    do {
        cs = TEXT_nbytes_of_char(from);
        if ((tp - *to) + cs + 2 > *to_size){
            int os = *to_size;
            expand_singarr((*to),os,(*to_size),2,TEXT);
            tp = *to + TEXT_strlen(*to);
        }
        isAscii = is_ASCII(from);
        isHyphen = (cs == 1 && *from == '-');
        isSpace = (cs == 1 && *from == ' ');
//        printf("  from: %s, to: %s, notAscii: %d, del_HYPHEN: %d, cs: %d isAscii: %d, isHyphen: %d, isSpace: %d, lastIsSpace: %d\n",
//                from,*to,cs,not_ASCII, del_HYPHEN,isAscii,isHyphen,isSpace,lastIsSpace);
        if (tp == *to){
          // nothing is is to so far
          if (del_HYPHEN && isHyphen){
              ; // do not copy
          } else {
              // ok to copy but copy everything
              TEXT_strCcpy(tp, from, 1);
              tp += cs;
              lastIsAscii = isAscii;
              lastIsSpace = isSpace;
//              printf("   First copy %s\n",*to);
          }
        } else {
          if (del_HYPHEN && isHyphen){
            ; // do not copy
          } else {
            if (lastIsSpace){
              ; // do not copy, do not change last is space
            } else {
              if (isSpace){
                ; // no not add a space
              } else if (not_ASCII && isAscii && lastIsAscii) {
                ; // do not add a space
              } else {
                TEXT_strCcpy(tp, (TEXT *)" ", 1);
                tp += 1;
//                printf("    Add space\n");
                }
              }
  
              TEXT_strCcpy(tp, from, 1);
              tp += cs;
              lastIsAscii = isAscii;
              lastIsSpace = isSpace;
            }
        }
//        printf("    end from: %s, to: /%s/\n",from,*to);
        from += cs;
    } while (*from != NULL_TEXT);
}

// tested
TEXT *TEXT_strcpy(TEXT *p1, TEXT *p2){
    return((TEXT *)strcpy((char *)p1, (char *)p2));
}

// tested 
TEXT *TEXT_strcpy_escaped(TEXT *p1, TEXT *p2, TEXT chr){
  TEXT *orig = p1;
  while (*p2 != '\0'){
    if (*p2 == chr) {
      *p1 = '\\'; p1 ++;
    }
    *p1 = *p2; p1++; p2++;
  }
  *p1 = '\0';
  return orig;
}

// tested
TEXT *TEXT_strstr(TEXT *src, TEXT *sub){
    int len_src, len_sub;
    int i;
    TEXT *t_src = src;

    if (src == (TEXT*)0 || sub == (TEXT*)0)
        return((TEXT*)0);

    len_src = TEXT_chrlen(src);
    len_sub = TEXT_chrlen(sub);
//    printf("  %d %d\n",len_src, len_sub);
    for (i=0; i<=len_src-len_sub; i++){
//        printf("%d %s =? %s\n",i,t_src, sub);
        if (TEXT_strCcasecmp(t_src,sub,len_sub) == 0){
	    return((TEXT *)t_src);
	}
	t_src += TEXT_nbytes_of_char(t_src);
    }

    return((TEXT *)0);
}

// tested
int TEXT_strlen(TEXT *text){
    register int i = 0;
    while (*text != '\0'){
        // This code should not assume a valid multi-byte character sequence 
        i++;
        text++;
    }
    return(i);
}

// tested
int TEXT_chrlen(TEXT *text){
    register int i = 0;
    while (*text != '\0'){
        text += TEXT_nbytes_of_char(text);
        i++;
    }
    return(i);
}

// Tested
TEXT *TEXT_add(TEXT *p1, TEXT *p2){
    char *sp;

    alloc_singarr(sp,TEXT_strlen(p1) + TEXT_strlen(p2) + 1,char);
    strcpy(sp,(char *)p1);
    strcat(sp,(char *)p2);
    return((TEXT *)sp);
}

//  tested
TEXT *TEXT_strBdup(TEXT *p, int n){
    char *cp;

    alloc_singarr(cp,n + 1,char);
    strncpy(cp,(char *)p,n);
    *(cp+n) = '\0';
    return((TEXT *)cp);
}

// n is the max number of bytes of the input to copy to the output.  This is typically calculated by pointer math
//  tested
TEXT *TEXT_strBdup_noEscape(TEXT *p, int n){
    char *cp, *begin;
    int i;
    alloc_singarr(cp,n + 1,char);
    begin = cp;
//    printf("  TEXT_strBdup_noEscape: %s n=%d\n",p,n);
    for (i=0; i<n; i++){
      if (*p != '\\'){
	*cp++ = *p++;
      } else {
	p++;
// JF changed this.  N now refers the the number of bytes in the ref....
//	i--;  // jumping over an escape
      }
    }
    *cp = '\0';
    return((TEXT *)begin);
}

// tested
TEXT *TEXT_strdup(TEXT *p){
    char *cp;
    int len;
    len = ((TEXT_strlen(p) == 0) ? 1 : TEXT_strlen(p)) + 1;

    alloc_singarr(cp,len,char);
    strcpy(cp,(char *)p);
    return((TEXT *)cp);
}

// Tested
TEXT *TEXT_strcat(TEXT *p, TEXT *p1){
    return((TEXT *)strcat((char *)p,(char *)p1));
}

/*
 * Repaired by Jon Fiscus,  May 1, 1997.  Had to fix the case ("f","for")
 * which resulted in the "prime-the-pump" loop
 */
// Tested
int TEXT_strcmp_master(TEXT *p1, TEXT *p2, int n, int doCase){
    TEXT *_p1 = p1, *_p2 = p2;
    TEXT *static_p1, *static_p2;
    int static_p1_len = -1, static_p2_len = -1;
    int c1, c2, i, iteration = 0;

    if (_p1 == (TEXT *)0 && _p2 == (TEXT *)0)  return(0);
    if (_p2 == (TEXT *)0)  return(1);
    if (_p1 == (TEXT *)0)  return(-1);

    // Data exists. convert case 
    if (doCase){
      if (static_p1_len == -1){
        static_p1_len = static_p2_len = 100;
        alloc_singarr(static_p1, static_p1_len, TEXT);
        alloc_singarr(static_p2, static_p2_len, TEXT);
      }
      TEXT_str_case_change_with_mem_expand_from_array2(&static_p1, &static_p1_len, p1, 1);
      TEXT_str_case_change_with_mem_expand_from_array2(&static_p2, &static_p2_len, p2, 1);
      _p1 = static_p1;
      _p2 = static_p2;
    }

    do {
        if (end_of_TEXT(*_p1) && end_of_TEXT(*_p2))   return(0);
        if (end_of_TEXT(*_p2))                        return(1);
        if (end_of_TEXT(*_p1))                        return(-1);
        c1 = TEXT_nbytes_of_char(_p1);
        c2 = TEXT_nbytes_of_char(_p2);
        if (c1 != c2) return (c1 < c2 ? -1 : 1);  
        // Lengths match
        if (c1 > 1){
            for (i=0; i<c1; i++){
               if (*((unsigned char *)_p1) != *((unsigned char *)_p2))
                  return ((*((unsigned char *)_p1) < *((unsigned char *)_p2)) ? -1 : 1);
               _p1++;
               _p2++;
            }
        } else {
            if (*_p1 != *_p2)
                return (*_p1 < *_p2 ? -1 : 1);
            _p1++;
            _p2++;
        }
        iteration ++;
    } while ((n == -1) || (iteration < n));
    return(0);
}

// Tested
//int TEXT_strcmp_master(TEXT *p1, TEXT *p2, int n, int doCase){
//    TEXT *_p1 = p1, *_p2 = p2;
//    int c1, c2, i, iteration = 0;
//
//    if (_p1 == (TEXT *)0 && _p2 == (TEXT *)0)  return(0);
//    if (_p2 == (TEXT *)0)  return(1);
//    if (_p1 == (TEXT *)0)  return(-1);
//    do {
//        if (end_of_TEXT(*_p1) && end_of_TEXT(*_p2))   return(0);
//        if (end_of_TEXT(*_p2))                        return(1);
//        if (end_of_TEXT(*_p1))                        return(-1);
//        c1 = TEXT_nbytes_of_char(_p1);
//        c2 = TEXT_nbytes_of_char(_p2);
//        if (c1 != c2) return (c1 < c2 ? -1 : 1);  
//        // Lengths match
//        if (c1 > 1 || doCase == 0){
//            // NO CASE CONVERSON FOR NOW !!!
//            for (i=0; i<c1; i++){
//               if (*((unsigned char *)_p1) != *((unsigned char *)_p2))
//                  return ((*((unsigned char *)_p1) < *((unsigned char *)_p2)) ? -1 : 1);
//               _p1++;
//               _p2++;
//            }
//        } else {
//            TEXT char1 = *_p1; 
//            TEXT char2 = *_p2;
//            if (STATIC_ENCODING == EXTASCII){
//                // Handle both ASCII and EXTASCII
//                if (is_EXTASC(&char1)) { if (char1 >= 192 && char1 <= 223) { char1 -= 32; } }
//                else { char1 = tolower(char1); }
//
//                if (is_EXTASC(&char1)) { if (char2 >= 192 && char2 <= 223) { char2 -= 32; } }
//                else { char2 = tolower(char2); }                
//            } else {
//                // Handle JUST ASCII
//                char1 = toupper(char1);
//                char2 = toupper(char2);
//            }
//            if (char1 != char2)
//                return (char1 < char2 ? -1 : 1);
//            _p1++;
//            _p2++;
//        }
//        iteration ++;
//    } while ((n == -1) || (iteration < n));
//    return(0);
//}

// tested
int TEXT_strCcasecmp(TEXT *p1, TEXT *p2, int n){
    return(TEXT_strcmp_master(p1, p2, n, 1));
}

// tested
int TEXT_strcasecmp(TEXT *p1, TEXT *p2){
    return(TEXT_strcmp_master(p1, p2, -1, 1));
}

//  tested
int TEXT_strCcmp(TEXT *p, TEXT *p1, int n){
    return(TEXT_strcmp_master(p, p1, n, 0));
}

int TEXT_strBcmp(TEXT *p, TEXT *p1, int n){
    return(strncmp(p, p1, n));
}

//  tested
int TEXT_strcmp(TEXT *p, TEXT *p1){
    return(TEXT_strcmp_master(p, p1, -1, 0));
}

// NO Test needed
int qsort_TEXT_strcmp(const void *p, const void *p1){
    return(TEXT_strcmp(*((TEXT **)p),*((TEXT **)p1)));
}

// NO Test needed
int bsearch_TEXT_strcmp(const void *p, const void *p1){
    return(TEXT_strcmp((TEXT *)p,*((TEXT **)p1)));
}

// tested -- copies the number fo BYTES
TEXT *TEXT_strBcpy(TEXT *p, TEXT *p1, int n){
   int i;
   TEXT *p_t = p, *p1_t = p1;
   for (i=0; i<n && *p1_t != NULL_TEXT; i++)
      *p_t++ = *p1_t++;
   *p_t = NULL_TEXT;
   return(p);     
}

//  tested  -- copies the number of chars
TEXT *TEXT_strCcpy(TEXT *p, TEXT *p1, int c){
   int i, y, nchar;
   TEXT *p_t = p, *p1_t = p1;
   for (i=0; i<c; i++){
      nchar = TEXT_nbytes_of_char(p1_t);
      for(y=0; y<nchar; y++)
         *p_t++ = *p1_t++;
   }
   *p_t = NULL_TEXT;
   return(p);     
}

// no test needed
float TEXT_atof(TEXT *p){
    return(atof((char *)p));
}

/// no test needed.  
TEXT *TEXT_strchr(TEXT *p, TEXT t){
    return((TEXT *)strchr((char *)p,(char)t));
}

//Tested
TEXT *TEXT_strtok(TEXT *p, TEXT *t){
    static TEXT *basep = (TEXT *)NULL, *ext;

    /* printf("strtok  p=%d  *p=%s  Text = %s\n",p,(p==NULL)?"":p,t); */

    if (p == (TEXT *)NULL){
	if (basep == (TEXT *)NULL)
	    return((TEXT *)0);
	p = basep;
    } else
	basep = p;
    if (*basep == '\0')
	return((TEXT *)0);

    ext = basep;
    /* skip white space */

    while (*ext != '\0' && strchr((char *)t,(char)*ext) != NULL){
	int sz = TEXT_nbytes_of_char(ext);
        ext+= sz;
        basep+= sz;
    }
    if (*(p = ext) == '\0') return((TEXT *)0);

    /* skip the token */
    while (*ext != '\0'){
	/* printf("      check %x\n",*ext); */
	int sz = TEXT_nbytes_of_char(ext);
	if (sz == 1) {
	    if (strchr((char *)t,(char)*ext) != NULL){
		*ext = '\0';
		basep = ext+1;
		/* printf("   Rtn '%s'\n",p);  */
		return(p);
	    }
	    ext++;
	} else
	    ext+=sz;
    }

    basep = ext + ((*ext == '\0') ? 0 : 1);
/*     printf("   Rtn '%s'\n",p);  */
    return(p);
}

// no test needed
TEXT *TEXT_strrchr(TEXT *p, TEXT t){
    return((TEXT *)strrchr((char *)p,(char)t));
}


// 
long int TEXT_getUTFCodePoint(TEXT *buf){
    if (buf == (TEXT *)0) return (0);

    if (STATIC_ENCODING != UTF8){
        fprintf(stderr,"Error: Attempt to get a UTF8 codepoint when the encoding is not UTF8\n");
        exit(1);
    }
    
    int n = TEXT_nbytes_of_char(buf);
    long long int codePoint = 0;
    if (n == 1) {
        codePoint = *buf;
    } else if (n == 2){
        codePoint = ((*((TEXT *)buf) & 0x1F) << 6) + ((*((TEXT *)(buf+1)) & 0x3F));
    } else if (n == 3){
        codePoint = ((*((TEXT *)buf) & 0xF) << 12) + ((*((TEXT *)(buf+1)) & 0x3F) << 6) + ((*((TEXT *)(buf+2)) & 0x3F));
    } else if (n == 4){
        codePoint = ((*((TEXT *)buf) & 0x7) << 18) + ((*((TEXT *)(buf+1)) & 0x3F) << 12) + ((*((TEXT *)(buf+2)) & 0x3F) << 6)  + ((*((TEXT *)(buf+3)) & 0x3F));
    } else if (n == 5){
        codePoint = ((*((TEXT *)buf) & 0x3) << 24) + ((*((TEXT *)(buf+1)) & 0x3F) << 18) + ((*((TEXT *)(buf+2)) & 0x3F) << 12) + ((*((TEXT *)(buf+3)) & 0x3F) << 6)  + ((*((TEXT *)(buf+4)) & 0x3F));
    } else if (n == 6){
        fprintf(stderr,"Error: Attempt to codepoint for 6-byte UTF8 codepoint is not supported\n");
        exit(1);
// This code works BUT, it exceeds the size of an int, SO, this is not portable
//    } else if (n == 6){
//        codePoint = ((*((TEXT *)buf) & 0x1) << 36) + ((*((TEXT *)(buf+1)) & 0x3F) << 24) + ((*((TEXT *)(buf+2)) & 0x3F) << 18) + ((*((TEXT *)(buf+3)) & 0x3F) << 12) + ((*((TEXT *)(buf+4)) & 0x3F) << 6) + ((*((TEXT *)(buf+5)) & 0x3F));
    } else {
        fprintf(stderr,"Error: Attempt to get UTF8 codepoint for larger than 5-byte codepoints\n");
        exit(1);
    }
    
    return codePoint;
}


// 
TEXT* TEXT_UTFCodePointToTEXT(long int c){
    static TEXT val[10];
    TEXT *b;
    int max = 10;
    int i;
    
    if (STATIC_ENCODING != UTF8){
        fprintf(stderr,"Error: Attempt convert a UTF8 codepoint to TEXT when the encoding is not UTF8\n");
        exit(1);
    }
    for (i=0, b=(TEXT *)&val; i<max; i++) {
        *b++ = (TEXT)'\0' ;
    }
    b = (TEXT *)&val;
     
    if      (c<    0x80) *b++=c;
    else if (c<   0x800) *b++=192+c/64, *b++=128+c%64;
    else if (c< 0x10000) *b++=224+c/4096, *b++=128+c/64%64, *b++=128+c%64;
    else if (c< 0x20000) *b++=240+c/262144, *b++=128+c/4096%64, *b++=128+c/64%64, *b++=128+c%64;
    else {
        fprintf(stderr,"Error: Attempt convert a UTF8 codepoint to TEXT resulting in more that 4-byte UTF8\n");
        exit(1);
    }
      
    return (TEXT *)&val;
}

int getKnownUFTCaseCP(int inCP, int toLow){
   int outCP = -1, i;

   if (STATIC_LPROF == LPROF_BABEL_TURKISH){
     for (i=0; i<numKCC_babel_turkish && outCP == -1; i++){     
       if (KCC_babel_turkish[i][(toLow ? 0 : 1)] == inCP){
         return KCC_babel_turkish[i][(!toLow ? 0 : 1)];
       }
     }
   }
   return -1;
}

// tested
TEXT *TEXT_str_to_master(TEXT *bufTEXT, int toLow){
    int c1, c2;
    TEXT *outP, *buf, holdSpace[10], *hptr;
                                
    buf = bufTEXT;
    hptr = (TEXT *)holdSpace;
    
    // Allocated data if needed
    if (STATIC_CASECONVTEXT_SIZE == -1){
        STATIC_CASECONVTEXT_SIZE = TEXT_chrlen(buf) + 1;
        if (STATIC_CASECONVTEXT_SIZE < 5) STATIC_CASECONVTEXT_SIZE = 5;  // Set a minimum
        alloc_singZ(STATIC_CASECONVTEXT, STATIC_CASECONVTEXT_SIZE, TEXT, (TEXT)'\0');
    }
    outP = STATIC_CASECONVTEXT;
    *outP = (TEXT )0;
    
    if (bufTEXT == (TEXT *)0)  return (bufTEXT);
    if (*bufTEXT == NULL_TEXT)  return (STATIC_CASECONVTEXT);

//    printf("str to mast /%s/ - tmpSize=%d\n",bufTEXT,STATIC_CASECONVTEXT_SIZE);

    do {
        c1 = TEXT_nbytes_of_char(buf);
        
        if (STATIC_ENCODING == GB){
          TEXT_strCcpy(hptr, buf, 1);
          if (c1 == 1) *hptr = (toLow ? tolower(*hptr) :  toupper(*hptr));  // ONLY if 1 char
        } else if (STATIC_ENCODING == UTF8){
          int inCP, outChangeCP;
          inCP = TEXT_getUTFCodePoint(buf);
          outChangeCP = getKnownUFTCaseCP(inCP, toLow);
          if (outChangeCP > 0){
            TEXT_strCcpy(hptr, TEXT_UTFCodePointToTEXT(outChangeCP), 1);         
          } else {
            TEXT_strCcpy(hptr, buf, 1);         
            if (c1 == 1) *hptr = (toLow ? tolower(*hptr) :  toupper(*hptr));  // ONLY if 1 char
          }         
        } else if (STATIC_ENCODING == ASCII){
          TEXT_strCcpy(hptr, buf, 1);
          *hptr = (toLow ? tolower(*hptr) :  toupper(*hptr));  // ONLY if 1 char
        } else if (STATIC_ENCODING == EXTASCII){
          TEXT_strCcpy(hptr, buf, 1);
          if (is_EXTASC(hptr)) { 
            if (toLow) {
              if (*hptr >= 192 && *hptr <= 223) { *hptr += 32; } 
            } else {
              if (*hptr >= 224) *hptr -= 32;
            }
          } else {
            *hptr = (toLow ? tolower(*hptr) :  toupper(*hptr));  // ONLY if 1 char
          }
        }

        // hptr is the converted char
        c2 = TEXT_nbytes_of_char(hptr);
        if (TEXT_strlen(STATIC_CASECONVTEXT) + c2 + 1> STATIC_CASECONVTEXT_SIZE){
          expand_singarr(STATIC_CASECONVTEXT, STATIC_CASECONVTEXT_SIZE, STATIC_CASECONVTEXT_SIZE, 1.5, TEXT);
          //printf("Expanded Now %d with %s\n",STATIC_CASECONVTEXT_SIZE, STATIC_CASECONVTEXT);
          outP = (TEXT *)STATIC_CASECONVTEXT + TEXT_strlen(STATIC_CASECONVTEXT);
        }
        TEXT_strCcpy(outP, hptr, 1);
                     
        buf += c1;
        outP += c2;
        //printf("  loop %s\n",STATIC_CASECONVTEXT);
    } while (! end_of_TEXT(*buf));

    return (STATIC_CASECONVTEXT);
}

void TEXT_str_case_change_with_mem_expand(TEXT **buf, int *len, int toLow){
    TEXT *lc = TEXT_str_to_master(*buf, toLow);
    int lc_len = TEXT_strlen(lc);
    if (lc_len+1 > *len){
	expand_singarr_to_size((*buf),(*len),(*len),lc_len + 1,TEXT)
    }
    TEXT_strcpy(*buf, lc);
}

void TEXT_str_case_change_with_mem_expand_from_array2(TEXT **buf, int *len, TEXT *arr2, int toLow){
    TEXT *lc = TEXT_str_to_master(arr2, toLow);
    int lc_len = TEXT_strlen(lc);
    if (lc_len+1 > *len){
	expand_singarr_to_size((*buf),(*len),(*len),lc_len + 1,TEXT)
    }
    TEXT_strcpy(*buf, lc);
}

// tested
//TEXT *TEXT_str_to_upp(TEXT *buf){
//    return TEXT_str_to_master(buf, 0);
//}
//
//TEXT *TEXT_str_to_low(TEXT *buf){
//    return TEXT_str_to_master(buf, 1);
//}


/*********************************************************************/
/* A safe version of fgets(), it will check to make sure that if len */
/* characters where read, the last character before the NULL is a    */
/* '\n'.                                                             */
/*********************************************************************/
// no test needed
TEXT *TEXT_fgets(TEXT *arr, int len, FILE *fp){
    unsigned char *tc, ans;

    if (fgets((char *)arr,len,fp) == NULL)
	return(NULL);

    tc = arr;
    while (*tc != '\0') tc++;
    if ((tc - arr) == len-1)
	if (*(tc-1) != '\n'){
	    fprintf(stderr,"Warning: TEXT_fgets could not read");
	    fprintf(stderr," and entire line\nDo you want");
	    fprintf(stderr," to (d) dump core or (c) continue? [d]  ");
	    ans = getchar();
	    if ((ans == 'c') || (ans == 'C'))
		;
	    else
		abort();
        }
    return(arr);
}

// no test needed
TEXT *TEXT_ensure_fgets(TEXT **arr, int *len, FILE *fp){
    TEXT *tc, *xp;

    if (fgets((char *)*arr,*len,fp) == NULL)
	return(NULL);
    tc = *arr;
    tc = TEXT_strlen(*arr) + *arr;
    while (*(tc-1) != '\n'){
	if ((tc - *arr) < *len-1)
	    return(*arr);
	/* Alloc some data, and re-read */
	alloc_singarr(xp,*len * 2,TEXT);

	strcpy((char *)xp,(char *)*arr);
	free_singarr(*arr,TEXT);
	*arr = xp;
	if (fgets((char *)(*arr + *len - 1),*len+1,fp) == NULL)
	    return(*arr);
	*len *= 2;
	tc = TEXT_strlen(*arr) + *arr;
    }
    return(*arr);
}

void TEXT_free(TEXT *p){
    free((char *)p);
}

// tested
int find_next_TEXT_token(TEXT **ctext, TEXT *token, int len){
    char *proc="find_next_TEXT_token", *pt=(char *)token;
    int c=0, alt_cnt=0, nchar, i;

    if (db >= 10) fprintf(stdout,"Entering: %s\n",proc);
    if (db >= 11) fprintf(stdout,"    function args: ctext='%s' len=%d\n",
			*ctext,len);
    *token = NULL_TEXT;
    
    /* Skip leading white space */
    while (VTisspace(**ctext)){
	(*ctext) += TEXT_nbytes_of_char(*ctext);
    }

    /* if we're at the end, there isn't a token */
    if (end_of_TEXT(**ctext))
	return(0);

    if (**ctext == ALT_BEGIN) {
	/* Nab the alternation */
	do {
	    nchar = TEXT_nbytes_of_char(*ctext);
            if (db >= 20)
		printf("ALT Char %c nchar=%d %s\n",**ctext,nchar,*ctext);
	    if (**ctext == ALT_BEGIN)
		alt_cnt ++;
	    if (**ctext == ALT_END)
		alt_cnt --;
            for (i=0; i<nchar; i++){
   	        if (++c > len) {
		    fprintf(stderr,"proc: %s increase token size > %d\n",proc,len);
		    return(0);
 	        }
		*(token++) = *(*ctext)++; 
            }
	} while (!end_of_TEXT(**ctext) && 
		 (alt_cnt > 0));
        *(token) = NULL_TEXT;
	if (db >= 20) {*token = '\0'; printf("       Token now %s\n",pt);}
        if (alt_cnt > 0) return(0);
    } else {
	/* Nab the word */
	do {
            nchar = TEXT_nbytes_of_char(*ctext);
            if (db>=20)
		printf("Char /%s/ %x nchar=%d\n",*ctext,**ctext,nchar);
            for (i=0; i<nchar; i++){
   	        if (++c > len) {
		    fprintf(stderr,"proc: %s increase token size > %d\n",proc,len);
		    return(0);
 	        }
		*(token++) = *(*ctext)++; 
            }
	    if (db >= 20) {*token = '\0'; printf("       Token now /%s/\n",pt);}
	} while (!end_of_TEXT(**ctext) && ! VTisspace(**ctext));
    }
    *token = NULL_TEXT;
    return(1);
}

/// tested
int find_next_TEXT_alternation(TEXT **ctext, TEXT *token, int len){
    char *proc="find_next_TEXT_alternation";
    int c=0, nchar, i;
    TEXT *t_token = token;
    int alt_cnt=0;

    if (db > 10) printf("Entering: %s\n",proc);
    if (db > 11) printf("    function args: ctext='%s' len=%d\n",
			*ctext,len);
    
    *token = NULL_TEXT;
    
    /* Skip leading white space */
    while (VTisspace(**ctext) ||  **ctext == '/'){
	(*ctext) += TEXT_nbytes_of_char(*ctext);
    }

    /* if we're at the end, there isn't a token */
    if (end_of_TEXT(**ctext))
	return(0);

    do {
        nchar = TEXT_nbytes_of_char(*ctext);
        if (nchar > 1){
            for (i=0; i<nchar; i++){
   	        if (++c > len) {
		    fprintf(stderr,"proc: %s increase token size > %d\n",proc,len);
		    return(0);
 	        }
		*(token++) = *(*ctext)++; 
 	    }   
        } else {
            if (**ctext == ALT_BEGIN) alt_cnt++;
	    if (**ctext == ALT_END) alt_cnt--;
	    if (++c > len) {
		fprintf(stderr,"proc: %s increase token size > %d\n",
			proc,len);
		return(0);
	    }
	    *(token++) = *(*ctext)++; 
	}
        if (db > 20){
            *token = NULL_TEXT;
            printf("  Now %s\n",t_token);
        }	
    } while ((**ctext != '/' || (**ctext == '/' && alt_cnt > 0)) &&
	     (**ctext != ALT_END || (**ctext == ALT_END && alt_cnt > 0)) && 
	     !end_of_TEXT(**ctext));
    *token = NULL_TEXT;

    return(1);
}

/***************************************************************/
/*  Return 1 if the string is empty, i.e. containing all       */
/*  spaces, or tabs.                                           */
/***************************************************************/
// tested
int TEXT_is_empty(TEXT *str)
{
    if (str == NULL) return(0);
    while (*str != NULL_TEXT){
        int sz = TEXT_nbytes_of_char(str);
        if (sz > 1)
           return(0);
        if (! isspace((int)*str))
           return(0);
        str++;
    }
    if (*str == '\0')
        return(1);
    return(0);
}

/*******************************************************************/
/*   check the character pointer to see if it points to the        */
/*   comment character                                             */
/*******************************************************************/
// does not need tested
int TEXT_is_comment(TEXT *str)
{
   if ((*str == COMMENT_CHAR) && (*(str+1) != COMMENT_CHAR)){
       fprintf(stderr,"Warning: The comment designation is now ");
       fprintf(stderr,"%c%c, the line below\n",COMMENT_CHAR,COMMENT_CHAR);
       fprintf(stderr,"         has only one comment character, this may");
       fprintf(stderr," be an error\n         %s\n",str);
   }
   
   if ((*str == COMMENT_CHAR) && (*(str+1) == COMMENT_CHAR))
       return(1);
   else
       return(0);
}

/*******************************************************************/
/*   check the character pointer to see if it points to the        */
/*   comment_info character                                        */
/*******************************************************************/
// Does not need tested
int TEXT_is_comment_info(TEXT *str)
{
   if ((*str == COMMENT_INFO_CHAR) && (*(str+1) != COMMENT_INFO_CHAR)){
      fprintf(stderr,"Warning: The comment designation is now ");
      fprintf(stderr,"%c%c, the line below\n",
	      COMMENT_INFO_CHAR,COMMENT_INFO_CHAR);
      fprintf(stderr,"         has only one comment info character, this may");
      fprintf(stderr," be an error\n         %s\n",str);
  }
   if ((*str == COMMENT_INFO_CHAR) && (*(str+1) == COMMENT_INFO_CHAR))
       return(1);
   else
       return(0);
}


/*******************************************************************/
/*   Use strtok to tokenize a text stream,  If an alternate trans  */
/*   is found, only return the first alternate                     */
/*******************************************************************/
// tested
TEXT *tokenize_TEXT_first_alt(TEXT *p, TEXT *set){
    TEXT *ctxt;
    static int firstalt=1;
    static int in_alt=0, alt_cnt=0;
    
    if (p != NULL) in_alt = alt_cnt = 0;
    if (db >= 20) printf("  text is %s, set is %s\n",p, set);
    ctxt = TEXT_strtok(p,set);
        
    while (ctxt != NULL){
	if (*ctxt == '{'){
	    if (firstalt)
		fprintf(stderr,"Warning: Alternates in reference texts"
			" removed in favor of the first alternate.\n");
	    firstalt=0;
	    in_alt = 1;
	    alt_cnt = 0;
	} else {
	    if (*ctxt == '}')
		in_alt = 0;
	    else {
		if (*ctxt == '/')
		    alt_cnt++;
		
		if (!in_alt || (alt_cnt == 0))
		    /* Return the alternate IF IT IS NOT a NULL '@' */
		    if (TEXT_strcmp(ctxt,(TEXT *)"@") != 0){
                        if (db >= 20) printf("   ctxt %s\n",ctxt);
			return(ctxt);
                    }
	    }
	}
	ctxt = TEXT_strtok(NULL,set);
    }
    if (db >= 20) printf("   ctxt %s\n",ctxt);
    return(ctxt);
}

// Tested
size_t TEXT_strspn(TEXT *str, TEXT *set)
{
    TEXT *p = str;

    if (p == (TEXT *)0) {
	fprintf(stderr,"Error: TEXT_strspn string arg was a NULL pointer\n");
	exit(1);
    }
    if (set == (TEXT *)0) {
	fprintf(stderr,"Error: TEXT_strspn set arg was a NULL pointer\n");
	exit(1);
    }

    while (*p != '\0')
	if (TEXT_nbytes_of_char(p) > 1) 
	    return(p - str);
	else {
	    if (TEXT_strchr(set,*p) == NULL)
		return(p - str);
	    p++;
	}
    return(p - str);
}

// Tested - But set MUST be ASCII
// Number of BYTES of the initial part of str1 not containing any of the
// characters that are part of str2
size_t TEXT_strcspn(TEXT *str, TEXT *set)
{
    TEXT *p = str;
    size_t n;

    if (p == (TEXT *)0) {
	fprintf(stderr,"Error: TEXT_strcspn string arg was a NULL pointer\n");
	exit(1);
    }
    if (set == (TEXT *)0) {
	fprintf(stderr,"Error: TEXT_strcspn set arg was a NULL pointer\n");
	exit(1);
    }

    while (*p != '\0'){
	if ((n = TEXT_nbytes_of_char(p)) > 1) {
	    p += n;
	} else {
	    if (TEXT_strchr(set,*p) != NULL)
		return(p - str);
	    p++;
	}
    }
    return(p - str);
}

//Tested
/* Perform a strtok function, except ignore any characters inside of */
/* double quote '"' marks.                                           */
TEXT *TEXT_strqtok(TEXT *buf, TEXT *set)
{
    static int data_len=100;
    static TEXT *data=(TEXT *)0, *token=(TEXT *)0;
    static TEXT *tptr, *ptr, *ptr2, *pt;
    int terminate;

    if (data == (TEXT *)0 && buf == (TEXT *)0)
      return((TEXT *)0);

    /* initialize some memory */
    if (data == (TEXT *)0){
	alloc_singZ(data,data_len,TEXT,'\0');
	alloc_singZ(token,data_len,TEXT,'\0');
    }

    if (buf != (TEXT *)0){
	/* Do we need more memory ? */
	if (TEXT_strlen(buf) > data_len-1){
	  free_singarr(data,TEXT);
	  free_singarr(token,TEXT);
	  data_len = TEXT_strlen(buf) + 20;
	  alloc_singZ(data,data_len,TEXT,'\0');
	  alloc_singZ(token,data_len,TEXT,'\0');
	}
	TEXT_strcpy(data,buf);
	ptr=data;
    }

    tptr = token;
    /* skip the initial white space */
    ptr += TEXT_strspn(ptr,set);

    if (*ptr == '\0') {
      /* Clean up the memory */
      free_singarr(data,TEXT);
      free_singarr(token,TEXT);
      data_len = 100;
      
      return((TEXT *)0);
    }

    ptr2 = ptr;
    terminate = 0;
    /* locate the first occurance of the separator character */
    while (*ptr2 != '\0' && !terminate) {
        int sz = TEXT_nbytes_of_char(ptr2);
	if (sz > 1)
	    ptr2 += sz;
	else if (*ptr2 == '"'){
	    if ((pt = TEXT_strchr(ptr2 + 1,'"')) != NULL)
		ptr2 = pt + 1;
	    else
		ptr2 += TEXT_strlen(ptr2);
	}
	else if (TEXT_strchr(set,*ptr2) != NULL) 
	    terminate = 1;
	else
	    ptr2++;
    }

    for (; ptr<ptr2; ptr++, tptr++)
	*tptr = *ptr;
    *tptr = '\0';
    if (*ptr != '\0')
	ptr++;

    return(token);
}


/***********************************************************************/
/*   The TEXT_LIST utilities                                           */

TEXT_LIST *init_TEXT_LIST(void){
    TEXT_LIST *tl;

    alloc_singarr(tl,1,TEXT_LIST);
    tl->file = (char *)0;
    tl->num = 0;
    tl->max = 100;
    alloc_singarr(tl->elem,tl->max,TEXT *);
    return(tl);
}

int add_TEXT_LIST(TEXT_LIST *tl, TEXT *str){
    if (tl == (TEXT_LIST *)0 || str == (TEXT *)0) 
	return(0);
    if (tl->num >= tl->max)
	expand_singarr(tl->elem,tl->num,tl->max,2,TEXT *);
    tl->elem[tl->num++] = TEXT_strdup(str);
    if (tl->num > 1 && TEXT_strcmp(str,tl->elem[tl->num-2]) < 0)
      fprintf(scfp,"Error: Adding lexical item %d out of order '%s' !> '%s'\n",
	      tl->num-1,str,tl->elem[tl->num-2]);
    
    return(1);
}

// no test needed
void free_TEXT_LIST(TEXT_LIST **tl)
{
    int e;
    TEXT_LIST *ptl = *tl;

    *tl = (TEXT_LIST *)0;
    for (e=0; e<ptl->num; e++)
	free_singarr(ptl->elem[e],TEXT);
    free_singarr(ptl->elem,TEXT *);
    if (ptl->file != (char *)0) free_singarr(ptl->file,char);
    free_singarr(ptl,TEXT_LIST);   
}

// tested
TEXT_LIST *load_TEXT_LIST(char *file, int col)
{
    TEXT *buf, *beg, *end;
    int buf_len = 100, i;
    TEXT_LIST *tl;
    FILE *fp;

    if (file == (char *)0 || *file == '\0') {
	fprintf(scfp,"\nError: load_TEXT_LIST bad Arguments\n");
	return (TEXT_LIST *)0;
    }
    if ((fp = fopen(file,"r")) == NULL) {
	fprintf(scfp,"\nError: load_TEXT_LIST open of '%s' failed\n",file);
	return (TEXT_LIST *)0;
    }
    
    alloc_singZ(buf,buf_len,TEXT,'\0');
    tl = init_TEXT_LIST();
    tl->file = (char *)TEXT_strdup((TEXT *)file);
    

    while (TEXT_ensure_fgets(&buf,&buf_len,fp) != NULL){
	TEXT_xnewline(buf);
	if (col == -1){
	    if (! add_TEXT_LIST(tl,buf)){
		fprintf(scfp,"\nError: Unable to add word from lexicon\n");
		free_TEXT_LIST(&tl);
		return (TEXT_LIST *)0;
	    }
	} else {
	    /* only story column 'col' */
	    beg = buf;
	    /* skip the initial whitespace */
	    beg += TEXT_strspn(beg,(TEXT *)" \t\n");
	    for (i=0; i<col; i++){
		beg += TEXT_strcspn(beg,(TEXT *)" \t\n");
		beg += TEXT_strspn(beg,(TEXT *)" \t\n");
	    }
	    end = beg + TEXT_strcspn(beg,(TEXT *)" \t\n") - 1;
	    *(end+1) = '\0';
	    if (! add_TEXT_LIST(tl,beg)){
		fprintf(scfp,"\nError: Unable to add word from lexicon\n");
		free_TEXT_LIST(&tl);
		return (TEXT_LIST *)0;
	    }
	}
    }
    free_singarr(buf,TEXT);
    fclose(fp);
    return tl;
}

void dump_TEXT_LIST(TEXT_LIST *tl, FILE *fp)
{
    int e;

    fprintf(fp,"Dump of TEXT LIST file: '%s'\n",tl->file);
    for (e=0; e<tl->num; e++)
	fprintf(fp,"   %4d: %s\n",e,tl->elem[e]);
}

int in_TEXT_LIST(TEXT_LIST *tl, TEXT *str)
{
    TEXT **ind;

    if ((ind = (TEXT **)bsearch(str,tl->elem,tl->num,sizeof(TEXT *),
				bsearch_TEXT_strcmp)) != NULL)
	return(ind - tl->elem);
    return(-1);
/*
    for (e=0; e<tl->num; e++)
	if (TEXT_strcmp(tl->elem[e],str) == 0)
	    return(e);
*/
}

int WORD_in_TEXT_LIST(void *data, void *elem)
{
    if (in_TEXT_LIST((TEXT_LIST *)data,
		     (! ((WORD *)elem)->opt_del) ? 
		     ((WORD *)elem)->value : ((WORD *)elem)->intern_value) >= 0)
	return 1;
    return 0;
}

int TEXT_nth_field(TEXT **to_addr, int *to_len, TEXT *from, int field){
    TEXT *p_from = from, *p_to, *to = *to_addr;
    int i=0;
    if (from == (TEXT *)0 || to == (TEXT *)0)
	return(0);

    p_from = TEXT_skip_wspace(p_from);
    while (i < field && *p_from != NULL_TEXT){
	p_from += TEXT_strcspn(p_from,(TEXT *)" \t\n");
	p_from = TEXT_skip_wspace(p_from);
    }
    p_to = p_from + TEXT_strcspn(p_from,(TEXT *)" \t\n");
    TEXT_strBcpy(to,p_from,p_to-p_from);
    return(1);
}

/* Return true if the text value begins or ends with a hyphen */
int TEXT_is_wfrag(TEXT *text){
  if (text == (TEXT *)0) return(0);

  if (*(text) == '-')    return(1);
  if (*(text + TEXT_strlen(text) - 1) == '-') return(1);
  return (0);
}

