#include "sctk.h"

static int STATIC_ext_ASCII = 1;


#define is_2byte(_p)  (((*(_p)) & 0x80) != 0 && (!STATIC_ext_ASCII))
#define is_EXTASC(_p)  (((*(_p)) & 0x80) != 0)

#define VTisspace(_p) (is_2byte(&(_p)) ? 0 : (is_EXTASC(&(_p)) ? 0 : isspace(_p)))
#define VTisupper(_p) (is_2byte(&(_p)) ? 0 : \
		       (is_EXTASC(&(_p)) ? (((unsigned char)(_p) >= 192) && \
					    ((unsigned char)(_p) <= 223)) \
			: isupper(_p)))
#define VTtoupper(_p) ((is_EXTASC(_p)) ? (char)(*((unsigned char *)_p) - 32) : toupper(*(_p)))

#define VTislower(_p) (is_2byte(&(_p)) ? 0 : \
		       (is_EXTASC(&(_p)) ? ((unsigned char)(_p) >= 224 && \
					    (unsigned char)(_p) <= 255) \
			: islower(_p)))
#define VTtolower(_p) ((is_EXTASC(_p)) ? (char)(*((unsigned char *)_p) + 32) : tolower(*(_p)))

TEXT *TEXT_skip_wspace(TEXT *ptr) {
    while (VTisspace(*ptr)) ptr++;
    return(ptr);
}

int end_of_TEXT(TEXT text){
    return(text == '\0');
}

int TEXT_set_coding(char *encoding){
    if ((TEXT_strcasecmp((TEXT *)encoding,(TEXT *)"EUC") == 0) ||
	(TEXT_strcasecmp((TEXT *)encoding,(TEXT *)"GB") == 0)){
	STATIC_ext_ASCII = 0;
	return(1);
    } else if (TEXT_strcasecmp((TEXT *)encoding,(TEXT *)"EXT_ASCII") == 0) {
	STATIC_ext_ASCII = 1;
	return(1);
    } 
    return(0);
}

void TEXT_delete_chars(TEXT *arr, TEXT *set){ 
    TEXT *last = arr;
    while (*arr != NULL_TEXT){
	if (is_2byte(arr)){
	    *(last++) = *(arr++);
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

void TEXT_separate_chars(TEXT *from, TEXT **to, int *to_size, int flag){
    int cs, os;
    TEXT *tp = *to;
    int not_ASCII = (flag & CALI_NOASCII) != 0;
    int del_HYPHEN = (flag & CALI_DELHYPHEN) != 0;

    *tp = '\0';
    while (*from != NULL_TEXT){
	if (is_2byte(from)){
	    cs = 2;
	} else {
	    if (isspace(*from)) {
		from++;
		if (tp-1 >= *to && (*(tp-1) != ' ')) *(tp++) = ' ';
		continue;
	    }
	    cs = 1;
	}
	if ((tp-*to) + cs > *to_size-3){
	    os = tp-*to;
	    expand_singarr((*to),os,(*to_size),2,TEXT);
	    tp = *to + os;
	}
	if ((tp-*to) != 0 && (*(tp-1) != ' ')) 
	    if ((cs == 1 && !not_ASCII) || (cs == 2)) {
		*(tp++) = ' ';
	    } 
	if (cs == 2) *(tp++) = *(from++);
	*(tp++) = *(from++);
    }
    *tp = '\0';
    if (del_HYPHEN)
	TEXT_delete_chars(*to,(TEXT *)"-");
}

TEXT *TEXT_strcpy(TEXT *p1, TEXT *p2){
    return((TEXT *)strcpy((char *)p1, (char *)p2));
}

TEXT *TEXT_strstr(TEXT *src, TEXT *sub){
    int len_src, len_sub;
    int i;

    if (src == (TEXT*)0 || sub == (TEXT*)0)
        return((TEXT*)0);

    len_src = TEXT_strlen(src);
    len_sub = TEXT_strlen(sub);
    for (i=0; i<len_src-len_sub; i++){
        if (TEXT_strncasecmp(src+i,sub,len_sub) == 0){
	    return((TEXT *)src+i);
	}
    }

    return((TEXT *)0);
}

int TEXT_strlen(TEXT *text){
    register int i = 0;
    while (*text != '\0'){
	if (is_2byte(text))
	    i++ , text++;
	i++ , text++;
    }
    return(i);
}

int TEXT_chrlen(TEXT *text){
    register int i = 0;
    while (*text != '\0'){
	if (is_2byte(text))
	    text++;
	i++ , text++;
    }
    return(i);
}

TEXT *TEXT_add(TEXT *p1, TEXT *p2){
    char *sp;

    alloc_singarr(sp,TEXT_strlen(p1) + TEXT_strlen(p2) + 1,char);
    strcpy(sp,(char *)p1);
    strcat(sp,(char *)p2);
    return((TEXT *)sp);
}

TEXT *TEXT_strndup(TEXT *p, int n){
    char *cp;

    alloc_singarr(cp,n + 1,char);
    strncpy(cp,(char *)p,n);
    *(cp+n) = '\0';
    return((TEXT *)cp);
}

TEXT *TEXT_strdup(TEXT *p){
    char *cp;
    int len;
    len = ((TEXT_strlen(p) == 0) ? 1 : TEXT_strlen(p)) + 1;

    alloc_singarr(cp,len,char);
    strcpy(cp,(char *)p);
    return((TEXT *)cp);
}

int TEXT_strcmp(TEXT *p, TEXT *p1){
    register unsigned char *u1=(unsigned char *)p, *u2=(unsigned char *)p1; 

    for (; *u1 == *u2; u1++, u2++)
	if (*u1 == '\0')
	    return(0);
    /* printf("p=%s   p1=%s   ret %x-%x = %d\n",p,p1,*u1,*u2,*u1-*u2); */
    return(*u1 - *u2);
}


TEXT *TEXT_strcat(TEXT *p, TEXT *p1){
    return((TEXT *)strcat((char *)p,(char *)p1));
}

/*
 * Repaired by Jon Fiscus,  May 1, 1997.  Had to fix the case ("f","for")
 * which resulted in the "prime-the-pump" loop
 */
int TEXT_strcasecmp(TEXT *p1, TEXT *p2){
    TEXT x1, x2='\0';
    x1 = (VTisupper(*p1)) ? VTtolower(p1) : *p1;
    x2 = (VTisupper(*p2)) ? VTtolower(p2) : *p2;
    while (x1 != '\0' && x2 != '\0'){
	if (x1 != x2)
	  return(x1 - x2);
	p1++;
	p2++;
	x1 = (VTisupper(*p1)) ? VTtolower(p1) : *p1;
	x2 = (VTisupper(*p2)) ? VTtolower(p2) : *p2;
    }
    return((x1 == '\0' && x2 == '\0') ? 0 : x1 - x2);
}

int TEXT_strncasecmp(TEXT *p1, TEXT *p2, int n){
    TEXT x1, x2='\0';

    while ((x1 = (VTisupper(*p1)) ? VTtolower(p1) : *p1) != '\0' &&
	   (x2 = (VTisupper(*p2)) ? VTtolower(p2) : *p2) != '\0' &&
	   n > 0){
	if (x1 != x2)
	    return(x1 - x2);
	p1++;
	p2++;
	n--;
    }
    return((x1 == '\0') || (n == 0) ? 0 : x1 - x2);
}

int TEXT_strncmp(TEXT *p, TEXT *p1, int n){
    return(strncmp((char *)p,(char *)p1, n));
}

int qsort_TEXT_strcmp(const void *p, const void *p1){
    return(TEXT_strcmp(*((TEXT **)p),*((TEXT **)p1)));
}

int bsearch_TEXT_strcmp(const void *p, const void *p1){
    return(TEXT_strcmp((TEXT *)p,*((TEXT **)p1)));
}

TEXT *TEXT_strncpy(TEXT *p, TEXT *p1, int n){
    return((TEXT *)strncpy((char *)p,(char *)p1,n));
}

float TEXT_atof(TEXT *p){
    return(atof((char *)p));
}

TEXT *TEXT_strchr(TEXT *p, TEXT t){
    return((TEXT *)strchr((char *)p,(char)t));
}

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
	if (is_2byte(ext)) {
	    ext++;
	    basep++;
	}
	ext++;
	basep++;
    }
    if (*(p = ext) == '\0') return((TEXT *)0);

    /* skip the token */
    while (*ext != '\0'){
	/* printf("      check %x\n",*ext); */
	if (!is_2byte(ext)) {
	    if (strchr((char *)t,(char)*ext) != NULL){
		*ext = '\0';
		basep = ext+1;
		/* printf("   Rtn '%s'\n",p);  */
		return(p);
	    }
	    ext++;
	} else
	    ext+=2;
    }

    basep = ext + ((*ext == '\0') ? 0 : 1);
/*     printf("   Rtn '%s'\n",p);  */
    return(p);
}

TEXT *TEXT_strrchr(TEXT *p, TEXT t){
    return((TEXT *)strrchr((char *)p,(char)t));
}

void TEXT_str_to_low(TEXT *buf){
    while (*buf != '\0'){
	if (is_2byte(buf))
	    buf+=2;
	else {
	    if (VTisupper(*buf)) 
		*buf = VTtolower(buf);
	    buf++;
	}
     }
}

void TEXT_str_to_upp(TEXT *buf){
    while (*buf != '\0'){
	if (is_2byte(buf))
	    buf+=2;
	else {
	    if (VTislower(*buf))
		*buf = VTtoupper(buf);
	    buf++;
	}
    }
}

/*********************************************************************/
/* A safe version of fgets(), it will check to make sure that if len */
/* characters where read, the last character before the NULL is a    */
/* '\n'.                                                             */
/*********************************************************************/
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

int find_next_TEXT_token(TEXT **ctext, TEXT *token, int len){
    char *proc="find_next_TEXT_token", *pt=(char *)token;
    int c=0, alt_cnt=0;

    if (db >= 10) fprintf(stdout,"Entering: %s\n",proc);
    if (db >= 11) fprintf(stdout,"    function args: ctext='%s' len=%d\n",
			*ctext,len);
    *token = NULL_TEXT;
    
    /* Skip leading white space */
    while (VTisspace(**ctext)){
	if (is_2byte(*ctext)) (*ctext)++;
	(*ctext)++;
    }

    /* if we're at the end, there isn't a token */
    if (end_of_TEXT(**ctext))
	return(0);

    if (**ctext == ALT_BEGIN) {
	/* Nab the alternation */
	do {
	    if (db >= 20)
		printf("ALT Char %c %d %s\n",**ctext,is_2byte(*ctext),*ctext);
	    if (**ctext == ALT_BEGIN)
		alt_cnt ++;
	    if (**ctext == ALT_END)
		alt_cnt --;
	    if (c + (is_2byte(*ctext) ? 2 : 1) > len) {
		fprintf(stderr,"proc: %s increase token size > %d\n",proc,len);
		return(0);
		}
	    if (is_2byte(*ctext))  *(token++) = **ctext;
	    *(token++) = *(*ctext + (is_2byte(*ctext) ? 1 : 0)); 
	} while (!end_of_TEXT(**ctext) && 
		 (**ctext != ALT_END || (**ctext == ALT_END && alt_cnt > 0)) &&
		 !end_of_TEXT(* ((*ctext)+= (is_2byte(*ctext) ? 2 : 1))));
	if (**ctext == ALT_END && alt_cnt == 0)
	    ++(*ctext);
	if (db >= 20) {*token = '\0'; printf("       Token now %s\n",pt);}
    } else {
	/* Nab the word */
	do {
	    if (db>=20)
		printf("Char %s %x %d\n",*ctext,**ctext,is_2byte(*ctext));
	    if (is_2byte(*ctext)) {
		if (++c > len) {
		    fprintf(stderr,"proc: %s increase token size > %d\n",
			    proc,len);
		    return(0);
		}
		*(token++) = *(*ctext)++; 
	    }
	    if (++c > len) {
		fprintf(stderr,"proc: %s increase token size > %d\n",proc,len);
		return(0);
	    }
	    *(token++) = *(*ctext)++;
	    if (db >= 20) {*token = '\0'; printf("       Token now %s\n",pt);}
	} while (!end_of_TEXT(**ctext) && ! VTisspace(**ctext));
    }
    *token = NULL_TEXT;
    return(1);
}


int find_next_TEXT_alternation(TEXT **ctext, TEXT *token, int len){
    char *proc="find_next_TEXT_alternation";
    int c=0;
    int alt_cnt=0;

    if (db > 10) printf("Entering: %s\n",proc);
    if (db > 11) printf("    function args: ctext='%s' len=%d\n",
			*ctext,len);
    
    *token = NULL_TEXT;
    
    /* Skip leading white space */
    while (VTisspace(**ctext) ||  **ctext == '/')
	if (is_2byte(*ctext)) 
	    (*ctext)+=2;
	else
	    (*ctext)++;

    /* if we're at the end, there isn't a token */
    if (end_of_TEXT(**ctext))
	return(0);

    do {
	if (**ctext == ALT_BEGIN) alt_cnt++;
	if (**ctext == ALT_END) alt_cnt--;
	if (is_2byte(*ctext)) {
	    if (++c > len) {
		fprintf(stderr,"proc: %s increase token size > %d\n",
			proc,len);
		return(0);
	    }
	    *(token++) = *(*ctext)++; 
	}
	if (++c > len) {
	    fprintf(stderr,"proc: %s increase output token size > %d\n",
		    proc,len);
	    return(0);
	}
	*(token++) = **ctext;
    } while ((*(++*ctext) != '/' || (**ctext == '/' && alt_cnt > 0)) &&
	     (**ctext != ALT_END || (**ctext == ALT_END && alt_cnt > 0)) && 
	     !end_of_TEXT(**ctext));
    *token = NULL_TEXT;

    return(1);
}

/***************************************************************/
/*  Return 1 if the string is empty, i.e. containing all       */
/*  spaces, or tabs.                                           */
/***************************************************************/
int TEXT_is_empty(TEXT *str)
{
    if (str == NULL) return(0);
    while (VTisspace(*str))
        str++;
    if (*str == '\0')
        return(1);
    return(0);
}

/*******************************************************************/
/*   check the character pointer to see if it points to the        */
/*   comment character                                             */
/*******************************************************************/
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
TEXT *tokenize_TEXT_first_alt(TEXT *p, TEXT *set){
    TEXT *ctxt;
    static int firstalt=1;
    static int in_alt=0, alt_cnt=0;
    
    if (p != NULL) in_alt = alt_cnt = 0;

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
		    if (TEXT_strcmp(ctxt,(TEXT *)"@") != 0)
			return(ctxt);
	    }
	}
	ctxt = TEXT_strtok(NULL,(TEXT *)" \t\n");
    }
    return(ctxt);
}

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
	if (is_2byte(p)) 
	    return(p - str);
	else {
	    if (TEXT_strchr(set,*p) == NULL)
		return(p - str);
	    p++;
	}
    return(p - str);
}


size_t TEXT_strcspn(TEXT *str, TEXT *set)
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
	if (is_2byte(p)) 
	    p+=2;
	else {
	    if (TEXT_strchr(set,*p) != NULL)
		return(p - str);
	    p++;
	}
    return(p - str);
}

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
	if (is_2byte(ptr2))
	    ptr2++;
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
    TEXT_strncpy(to,p_from,p_to-p_from);
    return(1);
}

/* Return true if the text value begins or ends with a hyphen */
int TEXT_is_wfrag(TEXT *text){
  if (text == (TEXT *)0) return(0);

  if (*(text) == '-')    return(1);
  if (*(text + TEXT_strlen(text) - 1) == '-') return(1);
  return (0);
}

