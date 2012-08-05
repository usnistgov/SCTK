#define MAIN
#include "sctk.h"

static int v = 1;

int test_separate_chars(TEXT *from, TEXT *exp, int flag){
  TEXT *buff;
  int size = 10;
  
  if (v > 1)
   printf("Testing TEXT_separate_chars(/%s/, //, %d) =? void, should be /%s/\n",
             from, flag, exp); 

  alloc_singZ(buff,size, TEXT, NULL_TEXT);

  TEXT_separate_chars(from, &buff, &size, flag);
  if (TEXT_strcmp(exp, buff) != 0){
    if (v)
      printf("Error: TEXT_separate_chars(/%s/, //, %d) =? void, arg1 is /%s/ but should be /%s/\n",
             from, flag, buff, exp);    
    return 1;
  }
  return(0);
}

int test_void_Fof_TPtr_TPtr(TEXT *exp_arg1, TEXT *function, TEXT *arg1, TEXT *arg2){
  TEXT cp_arg1[100];
  cp_arg1[0] = NULL_TEXT;
  TEXT_strcpy(cp_arg1, arg1);
  if (v > 1)
    printf("Testing %s(/%s/, /%s/) =? void, Arg1 should equal /%s/\n", function, arg1, arg2, exp_arg1);
  if (TEXT_strcmp(function, "TEXT_delete_chars") == 0){
    TEXT_delete_chars(arg1, arg2);
  } else {
    printf("Error: unknown function %s\n",function);
    return(1);
  }
  if (TEXT_strcmp(exp_arg1, arg1) != 0){
    if (v)
      printf("Error: %s(/%s/, /%s/) =? void, arg1 is /%s/ but should be /%s/\n", function, cp_arg1, arg2, arg1, exp_arg1);
    
    return 1;
  }
  return(0);
}

int test_void_Fof_TPtr(TEXT *exp_arg1, TEXT *function, TEXT *arg1){
  TEXT cp_arg1[100];
  cp_arg1[0] = NULL_TEXT;
  TEXT_strcpy(cp_arg1, arg1);
  if (v > 1)
    printf("Testing %s(/%s/) =? void, Arg1 should equal /%s/\n", function, arg1, exp_arg1);
  if (TEXT_strcmp(function, "TEXT_str_to_low") == 0){
    TEXT_str_to_low(cp_arg1);
  } else if (TEXT_strcmp(function, "TEXT_str_to_upp") == 0){
    TEXT_str_to_upp(cp_arg1);
  } else {
    printf("Error: unknown function %s\n",function);
    return(1);
  }
  if (TEXT_strcmp(exp_arg1, cp_arg1) != 0){
    if (v)
      printf("Error: %s(/%s/) =? void, arg1 is /%s/ but should be /%s/\n", function, arg1, cp_arg1, exp_arg1);
    
    return 1;
  }
  return(0);
}

int test_sizet_Fof_TPtr_TPtr(size_t expected, TEXT *function, TEXT *arg1, TEXT *arg2){
  size_t rtn = 0;
  if (v > 1)
    printf("Testing %s(/%s/, /%s/) =? /%lu/\n", function, arg1, arg2, (unsigned long)expected);
  if (TEXT_strcmp(function, "TEXT_strspn") == 0){
    rtn =  TEXT_strspn(arg1, arg2);
  } else if (TEXT_strcmp(function, "TEXT_strcspn") == 0){
    rtn =  TEXT_strcspn(arg1, arg2);
  } else {
    printf("Error: unknown function %s\n",function);
    return(1);
  }
  if (rtn != expected){
    if (v)
      printf("Error: %s(/%s/, /%s/) = /%lu/ but should be /%lu/\n", function, arg1, arg2, (unsigned long)rtn, (unsigned long)expected);
    
    return 1;
  }
  return(0);
}

int test_TPtr_Fof_TPtr(TEXT *expected, TEXT *function, TEXT *arg1){
  TEXT *rtn = (TEXT *)NULL;
  if (v > 1)
    printf("Testing %s(/%s/) =? /%s/\n", function, arg1, expected);
  if (TEXT_strcmp(function, "TEXT_strdup") == 0){
    rtn = TEXT_strdup(arg1);
  } else if (TEXT_strcmp(function, "TEXT_skip_wspace") == 0){
    rtn = TEXT_skip_wspace(arg1);
  } else {
    printf("Error: unknown function %s\n",function);
    return(1);
  }
  if (TEXT_strcmp(rtn, expected) != 0){
    if (v)
      printf("Error: %s(/%s/) = /%s/ but should be /%s/\n", function, arg1, rtn, expected);
    
    return 1;
  }
  return(0);
}

int test_TPtr_Fof_TPtr_TPtr(TEXT *expected, TEXT *function, TEXT *arg1, TEXT *arg2){
  TEXT *rtn = (TEXT *)NULL;
  if (v > 1)
    printf("Testing %s(/%s/, /%s/) =? /%s/\n", function, arg1, arg2, expected);
  if (TEXT_strcmp(function, "TEXT_add") == 0){
    rtn = TEXT_add(arg1, arg2);
  } else if (TEXT_strcmp(function, "TEXT_strcat") == 0){
    rtn = TEXT_strcat(arg1, arg2);
  } else if (TEXT_strcmp(function, "TEXT_strcpy") == 0){
    rtn = TEXT_strcpy(arg1, arg2);
  } else if (TEXT_strcmp(function, "TEXT_strqtok") == 0){
    rtn = TEXT_strqtok(arg1, arg2);
  } else if (TEXT_strcmp(function, "TEXT_strtok") == 0){
    rtn = TEXT_strtok(arg1, arg2);
  } else if (TEXT_strcmp(function, "TEXT_strtok") == 0){
    rtn = TEXT_strtok(arg1, arg2);
  } else if (TEXT_strcmp(function, "TEXT_strstr") == 0){
    rtn = TEXT_strstr(arg1, arg2);
  } else if (TEXT_strcmp(function, "tokenize_TEXT_first_alt") == 0){
    rtn = tokenize_TEXT_first_alt(arg1, arg2);
  } else {
    printf("Error: unknown function %s\n",function);
    return(1);
  }
  if (TEXT_strcmp(rtn, expected) != 0){
    if (v)
      printf("Error: %s(/%s/, /%s/) = /%s/ but should be /%s/\n", function, arg1, arg2, rtn, expected);
    
    return 1;
  }
  return(0);
}


int test_TPtr_Fof_TPtr_int(TEXT *expected, TEXT *function, TEXT *arg1, int arg2){
  TEXT *rtn = (TEXT *)NULL;
  if (v > 1)
    printf("Testing %s(/%s/, /%d/) =? /%s/\n", function, arg1, arg2, expected);
  if (TEXT_strcmp(function, "TEXT_strBdup") == 0){
    rtn = TEXT_strBdup(arg1, arg2);
  } else if (TEXT_strcmp(function, "TEXT_strBdup_noEscape") == 0){
    rtn = TEXT_strBdup_noEscape(arg1, arg2);
  } else {
    printf("Error: unknown function %s\n",function);
    return(1);
  }
  if (TEXT_strcmp(rtn, expected) != 0){
    if (v)
      printf("Error: %s(/%s/, /%d/) = /%s/ but should be /%s/\n", function, arg1, arg2, rtn, expected);
    
    return 1;
  }
  return(0);
}

int test_TPtr_Fof_TPtr_TPtr_T(TEXT *expected, TEXT *function, TEXT *arg1, TEXT *arg2, TEXT chr){
  TEXT *rtn = (TEXT *)NULL;
  if (v > 1)
    printf("Testing %s(/%s/, /%s/, /%c/) =? /%s/\n", function, arg1, arg2, chr, expected);
  if (TEXT_strcmp(function, "TEXT_strcpy_escaped") == 0){
    rtn = TEXT_strcpy_escaped(arg1, arg2, chr);
  } else {
    printf("Error: unknown function %s\n",function);
    return(1);
  }
  if (TEXT_strcmp(rtn, expected) != 0){
    if (v)
      printf("Error: %s(/%s/, /%s/, /%c/) = /%s/ but should be /%s/\n", function, arg1, arg2, chr, rtn, expected);
    return 1;
  }
  return(0);
}

int test_TPtr_Fof_TPtr_TPtr_int(TEXT *expected, TEXT *function, TEXT *arg1, TEXT *arg2, int arg3){
  TEXT *rtn = (TEXT *)NULL;
  if (v > 1)
    printf("Testing %s(/%s/, /%s/, /%d/) =? /%s/\n", function, arg1, arg2, arg3, expected);
  if (TEXT_strcmp(function, "TEXT_strBcpy") == 0){
    rtn = TEXT_strBcpy(arg1, arg2, arg3);
  } else {
    printf("Error: unknown function %s\n",function);
    return(1);
  }
  if (TEXT_strcmp(rtn, expected) != 0){
    if (v)
      printf("Error: %s(/%s/, /%s/, /%d/) = /%s/ but should be /%s/\n", function, arg1, arg2, arg3, rtn, expected);
    return 1;
  }
  return(0);
}

int test_int_Fof_TPtr(int expected, TEXT *function, TEXT *arg1){
  int rtn = 0;
  if (v > 1)
    printf("Testing %s(/%s/) =? /%d/\n", function, arg1, expected);
  if (TEXT_strcmp(function, "TEXT_nbytes_of_char") == 0){
    rtn =  TEXT_nbytes_of_char(arg1);
  } else if (TEXT_strcmp(function, "TEXT_strlen") == 0){
    rtn =  TEXT_strlen(arg1);
  } else if (TEXT_strcmp(function, "TEXT_chrlen") == 0){
    rtn =  TEXT_chrlen(arg1);
  } else if (TEXT_strcmp(function, "TEXT_is_empty") == 0){
    rtn =  TEXT_is_empty(arg1);
  } else {
    printf("Error: unknown function %s\n",function);
    return(1);
  }
  if (rtn != expected){
    if (v)
      printf("Error: %s(/%s/) = /%d/ but should be /%d/\n", function, arg1, rtn, expected);
    
    return 1;
  }
  return(0);
}


int test_int_Fof_TPtr_TPtr(int expected, TEXT *function, TEXT *arg1, TEXT *arg2){
  int rtn;
  if (v > 1)
    printf("Testing %s(/%s/, /%s/) =? /%d/\n", function, arg1, arg2, expected);
  if (TEXT_strcmp(function, "TEXT_strcasecmp") == 0){
    rtn = TEXT_strcasecmp(arg1, arg2);
  } else if (TEXT_strcmp(function, "TEXT_strcmp") == 0){
    rtn = TEXT_strcmp(arg1, arg2);
  } else {
    printf("Error: unknown function %s\n",function);
    return(1);
  }
  if (rtn != expected){
    if (v)
      printf("Error: %s(/%s/, /%s/) = /%d/ but should be /%d/\n", function, arg1, arg2, rtn, expected);
    
    return 1;
  }
  return(0);
}

int test_int_Fof_TPtr_TPtr_int(int expected, TEXT *function, TEXT *arg1, TEXT *arg2, int arg3){
  int rtn;
  if (v > 1)
    printf("Testing %s(/%s/, /%s/, %d) =? /%d/\n", function, arg1, arg2, arg3, expected);
  if (TEXT_strcmp(function, "TEXT_strCcasecmp") == 0){
    rtn = TEXT_strCcasecmp(arg1, arg2, arg3);
  } else if (TEXT_strcmp(function, "TEXT_strCcmp") == 0){
    rtn = TEXT_strCcmp(arg1, arg2, arg3);
  } else {
    printf("Error: unknown function %s\n",function);
    return(1);
  }
  if (rtn != expected){
    if (v)
      printf("Error: %s(/%s/, /%s/, %d) = /%d/ but should be /%d/\n", function, arg1, arg2, arg3, rtn, expected);
    
    return 1;
  }
  return(0);
}

int test_int_Fof_TPtrPtr_TPtr_int(int expected, TEXT *function, TEXT **arg1, TEXT *arg2, int arg3, TEXT *exp_arg2){
  int rtn;
  TEXT *arg1_init = *arg1;
  if (v > 1)
    printf("Testing %s(/%s/, //, %d) =? /%d/, arg2 expected to be /%s/\n", function, *arg1, arg3, expected, exp_arg2);
  if (TEXT_strcmp(function, "find_next_TEXT_token") == 0){
    rtn = find_next_TEXT_token(arg1, arg2, arg3);
  } else if (TEXT_strcmp(function, "find_next_TEXT_alternation") == 0){
    rtn = find_next_TEXT_alternation(arg1, arg2, arg3);
  } else {
    printf("Error: unknown function %s\n",function);
    return(1);
  }
  if (rtn != expected){
    if (v)
      printf("Error: %s(/%s/, //, %d) = /%d/ but should be /%d/\n", function, arg1_init, arg3, rtn, expected);
    return(1);
  }
  if (TEXT_strcmp(arg2, exp_arg2) != 0){
    if (v)
      printf("Error: %s(/%s/, //, %d), Arg2 = /%s/ but should be /%s/\n", function, arg1_init, arg3, arg2, exp_arg2);
    return 1;
  }
  return(0);
}


int test_int_Fof_TEXT(int expected, TEXT *function, TEXT arg1){
  int rtn = 0;
  if (v > 1)
    printf("Testing %s(/%c/) =? /%d/\n", function, arg1, expected);
  if (TEXT_strcmp(function, "end_of_TEXT") == 0){
    rtn =  end_of_TEXT(arg1);
  } else {
    printf("Error: unknown function %s\n",function);
    return(1);
  }
  if (rtn != expected){
    if (v)
      printf("Error: %s(/%c/) = /%d/ but should be /%d/\n", function, arg1, rtn, expected);
    
    return 1;
  }
  return(0);
}


int test_int_Fof_TEXTLIST_TPtr(int expected, TEXT *function, TEXT_LIST *arg1, TEXT *arg2){
  int rtn;
  if (v > 1)
    printf("Testing %s(\"%s\", /%s/) =? /%d/\n", function, arg1->file, arg2, expected);
  if (TEXT_strcmp(function, "in_TEXT_LIST") == 0){
    rtn = in_TEXT_LIST(arg1, arg2);
  } else {
    printf("Error: unknown function %s\n",function);
    return(1);
  }
  if (rtn != expected){
    if (v)
      printf("Error: %s(\"%s\", /%s/) = /%d/ but should be /%d/\n", function, arg1->file, arg2, rtn, expected);
    
    return 1;
  }
  return(0);
}


/*************************************************/
/*  Dumps an arc of a network.                   */
/*************************************************/
void print_arc_text(ARC *arc, void *p)          
{
    if (arc != NULL){
	TEXT_strcat((TEXT *)p, ((WORD*)(arc->data))->value);
	TEXT_strcat((TEXT *)p, (TEXT *)" ");
    }
    return;
} /* end of function "print_arc" */

void getText(NETWORK *net, TEXT *report){
  *report = NULL_TEXT;
  Network_traverse(net,NULL,0,print_arc_text,(void *)(report),NT_For);
  report[TEXT_strlen(report) - 1] = NULL_TEXT;
}

int testNetExpandChar(TEXT *str, TEXT *sepStr, int char_align, int optDel){
    NETWORK *net;
    int error = 0;
    TEXT report[200];

//    db = 20;
    printf("Making network for /%s/\n",str);
    if ((net = Network_create_from_TEXT(str, "TestNet",
					   print_WORD,
					   equal_WORD2,
					   release_WORD, null_alt_WORD,
					   opt_del_WORD,
					   copy_WORD, make_empty_WORD,
					   use_count_WORD))
	== NULL_NETWORK){
        fprintf(stderr,"Network_create_from_TEXT failed\n");
	return(1);
    }

    if (optDel){
        Network_traverse(net,NULL,0,decode_opt_del,0,NT_For);
    }
//    Network_traverse(netCopy,NULL,0,decode_fragment,0,NT_For);
    
//    Network_traverse(net,NULL,0,print_arc,0,NT_For);
    getText(net, (TEXT*)&report);
    printf("   Initial Text Its: %s\n",report);
    if (TEXT_strcmp(report, str) != 0){
      printf("Error: testNetExpandChar cali=%d failed initial network creation %s != /%s/\n",char_align, str,report);
      return(1);
    }

    Network_traverse(net,NULL,0,expand_words_to_chars,&char_align,0);
    getText(net, (TEXT*)&report);
    printf("   Expanded Text Its: %s\n",report);
    if (TEXT_strcmp(report, sepStr) != 0){
      printf("Error: testNetExpandChar cali=%d failed separation of %s.  expected /%s/ got /%s/\n",char_align, str,sepStr,report);
      return(1);
    }
    
    return(0);
}

int unitTestTEXT(){
  TEXT buff[100]; 

  int err = 0, tst = 0;
  int iter, i;
  TEXT *X, *Y, *A, *B, *lexicon;
  int charSizeX = 0;
  int charSizeY = 0;

  // Extra characters
  TEXT *b = (TEXT *)" ";
  TEXT *e = (TEXT *)"";
  TEXT *q = (TEXT *)"\"";
  TEXT *esc = (TEXT *)"\\";
  TEXT *hyphen = (TEXT *)"-";
  TEXT *oBe = (TEXT *)"(";
  TEXT *oEn = (TEXT *)")";
  TEXT *aBe = (TEXT *)"{";
  TEXT *aAl = (TEXT *)"/";
  TEXT *aEn = (TEXT *)"}";
  TEXT *lar = (TEXT *)"{/}";
  TEXT *semiColon = (TEXT *)";";
  TEXT *t;

  /// *********** X is three characters
  /// *********** Y is two characters
  
  // ASCII text /foo/ and /ba/
  TEXT ASCII_X[20]; TEXT ASCII_X_LC[20]; TEXT ASCII_Y[20]; TEXT ASCII_A[20]; TEXT ASCII_B[20]; 
  ASCII_X[0] = (TEXT)'T'; ASCII_X[1] = (TEXT)'O'; ASCII_X[2] = (TEXT)'O'; ASCII_X[3] = NULL_TEXT;   
  ASCII_Y[0] = (TEXT)'i'; ASCII_Y[1] = (TEXT)'s'; ASCII_Y[2] = NULL_TEXT;   
  ASCII_A[0] = (TEXT)'c'; ASCII_A[1] = (TEXT)'d'; ASCII_A[2] = NULL_TEXT;   
  ASCII_B[0] = (TEXT)'C'; ASCII_B[1] = (TEXT)'D'; ASCII_B[2] = NULL_TEXT;   
  
  // EXTENDED ASCII  8859-1 /cup/ and /pa/
  TEXT EXTASCII_X[20], EXTASCII_Y[20], EXTASCII_A[20], EXTASCII_B[20]; 
  EXTASCII_X[0] = (TEXT)(0xc7); EXTASCII_X[1] = (TEXT)(0xdc); EXTASCII_X[2] = (TEXT)(0xfe); EXTASCII_X[3] = NULL_TEXT;
  EXTASCII_Y[0] = (TEXT)(0xfe); EXTASCII_Y[1] = (TEXT)(0xE1); EXTASCII_Y[2] = NULL_TEXT;
  EXTASCII_A[0] = (TEXT)(0xe7); EXTASCII_A[1] = (TEXT)(0xe9); EXTASCII_A[2] = NULL_TEXT;   
  EXTASCII_B[0] = (TEXT)(0xc7); EXTASCII_B[1] = (TEXT)(0xc9); EXTASCII_B[2] = NULL_TEXT;   

  // Two byte GB words //, 
  TEXT GB_X[20], GB_Y[20];  
  GB_X[0] = (TEXT)(0xb6); GB_X[1] = (TEXT)(0xd4);
        GB_X[2] = (TEXT)(0xb6); GB_X[3] = (TEXT)(0xbc); 
        GB_X[4] = (TEXT)(0xb5); GB_X[5] = (TEXT)(0xc4); 
        GB_X[6] = NULL_TEXT;
  GB_Y[0] = (TEXT)(0xb7); GB_Y[1] = (TEXT)(0xbd);
        GB_Y[2] = (TEXT)(0xc3); GB_Y[3] = (TEXT)(0xe6);
        GB_Y[4] = NULL_TEXT;
  
  // Two byte Unicode code words, /alpha chi rho/ /Mu phi/
  TEXT UTF8_2B_X[20], UTF8_2B_Y[20];  
  UTF8_2B_X[0] = (TEXT)(0xce); UTF8_2B_X[1] = (TEXT)(0xb1);
        UTF8_2B_X[2] = (TEXT)(0xcf); UTF8_2B_X[3] = (TEXT)(0x87); 
        UTF8_2B_X[4] = (TEXT)(0xcf); UTF8_2B_X[5] = (TEXT)(0x81); 
        UTF8_2B_X[6] = NULL_TEXT;
  UTF8_2B_Y[0] = (TEXT)(0xce); UTF8_2B_Y[1] = (TEXT)(0xbc);
        UTF8_2B_Y[2] = (TEXT)(0xcf); UTF8_2B_Y[3] = (TEXT)(0x86);
        UTF8_2B_Y[4] = NULL_TEXT;
  
  // Three byte Letters /DAJ/ /EL/
  TEXT UTF8_3B_X[20], UTF8_3B_Y[20];  
  UTF8_3B_X[0] = (TEXT)(0xe1); UTF8_3B_X[1] = (TEXT)(0x8e); UTF8_3B_X[2] = (TEXT)(0xa0); 
        UTF8_3B_X[3] = (TEXT)(0xe1); UTF8_3B_X[4] = (TEXT)(0x8e); UTF8_3B_X[5] = (TEXT)(0xaa); 
        UTF8_3B_X[6] = (TEXT)(0xe1); UTF8_3B_X[7] = (TEXT)(0x8e); UTF8_3B_X[8] = (TEXT)(0xab); 
        UTF8_3B_X[9] = NULL_TEXT;
  UTF8_3B_Y[0] = (TEXT)(0xe1); UTF8_3B_Y[1] = (TEXT)(0x8f); UTF8_3B_Y[2] = (TEXT)(0x8b); 
        UTF8_3B_Y[3] = (TEXT)(0xe1); UTF8_3B_Y[4] = (TEXT)(0x8f); UTF8_3B_Y[5] = (TEXT)(0x9e); 
        UTF8_3B_Y[6] = NULL_TEXT;
  
  // Four byte Letters  /)gamma(/ /~=/ page with code points U+10080 to U+1047F
  TEXT UTF8_4B_X[20], UTF8_4B_Y[20]; 
  UTF8_4B_X[0] = (TEXT)(0xf0); UTF8_4B_X[1] = (TEXT)(0x90); UTF8_4B_X[2] = (TEXT)(0x85); UTF8_4B_X[3] = (TEXT)(0x80); 
        UTF8_4B_X[4] = (TEXT)(0xf0); UTF8_4B_X[5] = (TEXT)(0x90); UTF8_4B_X[6] = (TEXT)(0x85); UTF8_4B_X[7] = (TEXT)(0x83); 
        UTF8_4B_X[8] = (TEXT)(0xf0); UTF8_4B_X[9] = (TEXT)(0x90); UTF8_4B_X[10] = (TEXT)(0x85); UTF8_4B_X[11] = (TEXT)(0x81); 
        UTF8_4B_X[12] = NULL_TEXT;
  UTF8_4B_Y[0] = (TEXT)(0xf0); UTF8_4B_Y[1] = (TEXT)(0x90); UTF8_4B_Y[2] = (TEXT)(0x85); UTF8_4B_Y[3] = (TEXT)(0xbc); 
        UTF8_4B_Y[4] = (TEXT)(0xf0); UTF8_4B_Y[5] = (TEXT)(0x90); UTF8_4B_Y[6] = (TEXT)(0x86); UTF8_4B_Y[7] = (TEXT)(0x90); 
        UTF8_4B_Y[8] = NULL_TEXT;

  TEXT bXD[200],    *bX = (TEXT *)&bXD;
  TEXT XbD[200],    *Xb = (TEXT *)&XbD;
  TEXT XsXD[200],    *XsX = (TEXT *)&XsXD;
  TEXT XesXsYsYD[200],    *XesXsYsY = (TEXT *)&XesXsYsYD;
  TEXT XbAD[200],    *XbA = (TEXT *)&XbAD;
  TEXT bbXD[200],   *bbX = (TEXT *)&bbXD;
  TEXT XbbD[200],   *Xbb = (TEXT *)&XbbD;
  TEXT YbD[200],     *Yb = (TEXT *)&YbD;
  TEXT XYD[200],     *XY = (TEXT *)&XYD;
  TEXT XbAbYD[200],   *XbAbY = (TEXT *)&XbAbYD;
  TEXT XhD[200],   *Xh = (TEXT *)&XhD;
  TEXT oXoD[200],   *oXo = (TEXT *)&oXoD;
  TEXT oXhoD[200],   *oXho = (TEXT *)&oXhoD;
  TEXT oXhoboAoboYoD[200],   *oXhoboAoboYo = (TEXT *)&oXhoboAoboYoD;
  TEXT XbBbYD[200],   *XbBbY = (TEXT *)&XbBbYD;
  TEXT XbYD[200],   *XbY = (TEXT *)&XbYD;
  TEXT XhYD[200],   *XhY = (TEXT *)&XhYD;
  TEXT XbYbD[200], *XbYb = (TEXT *)&XbYbD;
  TEXT qXD[200],    *qX = (TEXT *)&qXD;
  TEXT YqD[200],    *Yq = (TEXT *)&YqD;
  TEXT eqXbYeqD[40], *eqXbYeq = (TEXT *)&eqXbYeqD;
  TEXT qXbYqD[40], *qXbYq = (TEXT *)&qXbYqD;
  TEXT lngD[2000],   *lng = (TEXT *)&lngD;  
  TEXT X1bX2bX3D[2000], *X1bX2bX3 = (TEXT *)X1bX2bX3D;
  TEXT oX1oboX2oboX3oD[2000], *oX1oboX2oboX3o = (TEXT *)oX1oboX2oboX3oD;
  TEXT obX1bX2bX3boD[2000], *obX1bX2bX3bo = (TEXT *)obX1bX2bX3boD;
  TEXT Y1bY2D[200],   *Y1bY2 = (TEXT *)Y1bY2D;
  TEXT A1bA2D[200],   *A1bA2 = (TEXT *)A1bA2D;

  TEXT lXD[200],   *lX = (TEXT *)lXD;
  TEXT lXrD[200],   *lXr = (TEXT *)lXrD;
  TEXT lYrD[200],   *lYr = (TEXT *)lYrD;
  TEXT blXrbD[200],   *blXrb = (TEXT *)blXrbD;
  TEXT lbXbrD[200],   *lbXbr = (TEXT *)lbXbrD;
  TEXT lbXbrbD[200],   *lbXbrb = (TEXT *)lbXbrbD;
  TEXT lXrlYrD[200],   *lXrlYr = (TEXT *)lXrlYrD;
  TEXT blXrblYrbD[200],   *blXrblYrb = (TEXT *)blXrblYrbD;
  TEXT llXrslYrrD[200],   *llXrslYrr = (TEXT *)llXrslYrrD;
  TEXT lXrslYrD[200],   *lXrslYr = (TEXT *)lXrslYrD;  
  TEXT blbXbrbslYrD[200],   *blbXbrbslYr = (TEXT *)blbXbrbslYrD; 
  TEXT llXrslYrrbXD[200],   *llXrslYrrbX = (TEXT *)llXrslYrrbXD;  *llXrslYrrbX = NULL_TEXT;
  
  TEXT X1bX2bX3bhbY1bY2D[200],   *X1bX2bX3bhbY1bY2 = (TEXT *)&X1bX2bX3bhbY1bY2D;  
  TEXT X1bX2bX3bY1bY2D[200],   *X1bX2bX3bY1bY2 = (TEXT *)&X1bX2bX3bY1bY2D;  
  TEXT X1bX2bX3bA1A2bY1bY2D[200],   *X1bX2bX3bA1A2bY1bY2 = (TEXT *)&X1bX2bX3bA1A2bY1bY2D;  
  TEXT X1bX2bX3bA1bA2bY1bY2D[200],   *X1bX2bX3bA1bA2bY1bY2 = (TEXT *)&X1bX2bX3bA1bA2bY1bY2D;  

  for (iter=0; iter<6; iter++){
    if (iter == 0){
      printf("Testing ASCII data\n");
      // ASCII Data
      X = (TEXT *)&ASCII_X;
      Y = (TEXT *)&ASCII_Y;
      charSizeX = 1;
      charSizeY = 1;
      if (TEXT_set_encoding("ASCII") != 1){
        tst++; err++;
        printf("Error: Unable to set ASCII encoding\n");
      }
      lexicon = (TEXT *)"testdata/tests.lex";
    } else if (iter == 1){
      printf("Testing Extended ASCII data\n");
      // extended ASCII Data
      // ASCII Data
      X = (TEXT *)&EXTASCII_X;
      Y = (TEXT *)&EXTASCII_Y;
      charSizeX = 1;
      charSizeY = 1;
      if (TEXT_set_encoding("EXT_ASCII") != 1){
        tst++; err++;
        printf("Error: Unable to set EXTASCII encoding\n");
      }
    } else if (iter == 2){
      printf("Testing 2-byte UTF-8 data\n");
      // 2Byte UTF-8 Data
      X = (TEXT *)&UTF8_2B_X;
      Y = (TEXT *)&UTF8_2B_Y;
      charSizeX = 2;
      charSizeY = 2;
      if (TEXT_set_encoding("UTF-8") != 1){
        tst++; err++;
        printf("Error: Unable to set UTF-8 encoding\n");
      }
    } else if (iter == 3){
      printf("Testing 3-byte UTF-8 data\n");
      X = (TEXT *)&UTF8_3B_X;
      Y = (TEXT *)&UTF8_3B_Y;
      charSizeX = 3;
      charSizeY = 3;
      if (TEXT_set_encoding("UTF-8") != 1){
        tst++; err++;
        printf("Error: Unable to set UTF-8 encoding\n");
      }
    } else if (iter == 4){
      printf("Testing 4-byte UTF-8 data\n");
      X = (TEXT *)&UTF8_4B_X;
      Y = (TEXT *)&UTF8_4B_Y;
      charSizeX = 4;
      charSizeY = 4;
      if (TEXT_set_encoding("UTF-8") != 1){
        tst++; err++;
        printf("Error: Unable to set UTF-8 encoding\n");
      }
    } else if (iter == 5){
      printf("Testing 2-byte GB data\n");
      X = (TEXT *)&GB_X;
      Y = (TEXT *)&GB_Y;
      charSizeX = 2;
      charSizeY = 2;
      if (TEXT_set_encoding("GB") != 1){
        tst++; err++;
        printf("Error: Unable to set GB encoding\n");
      }
      lexicon = (TEXT *)"testdata/mand.lex";
    } else {
      fprintf(stderr,"Internal Error: abort\n");
      exit(1);
    }
    A = (TEXT *)&ASCII_A;
    B = (TEXT *)&ASCII_B;

    bXD[0] = NULL_TEXT;  TEXT_strcat(bX, b);  TEXT_strcat(bX, X);
    XbD[0] = NULL_TEXT;  TEXT_strcat(Xb, X);  TEXT_strcat(Xb, b);
    XsXD[0] = NULL_TEXT;  TEXT_strcat(XsX, X); TEXT_strcat(XsX, semiColon);  TEXT_strcat(XsX, X);
    XesXsYsYD[0] = NULL_TEXT;  TEXT_strcat(XesXsYsY, X); TEXT_strcat(XesXsYsY, esc);  TEXT_strcat(XesXsYsY, semiColon); 
        TEXT_strcat(XesXsYsY, X); TEXT_strcat(XesXsYsY, semiColon);TEXT_strcat(XesXsYsY, Y);TEXT_strcat(XesXsYsY, semiColon);
        TEXT_strcat(XesXsYsY, Y);

    XbAD[0] = NULL_TEXT;  TEXT_strcat(XbA, X);  TEXT_strcat(XbA, b); TEXT_strcat(XbA, A);
    bbXD[0] = NULL_TEXT; TEXT_strcat(bbX, b); TEXT_strcat(bbX, b); TEXT_strcat(bbX, X);
    XbbD[0] = NULL_TEXT; TEXT_strcat(Xbb, Xb); TEXT_strcat(Xbb, b);
    YbD[0] = NULL_TEXT;  TEXT_strcat(Yb, Y);  TEXT_strcat(Yb, b); 
    XYD[0] = NULL_TEXT; TEXT_strcat(XY, X);    TEXT_strcat(XY, Y); 
    XbAbYD[0] = NULL_TEXT; TEXT_strcat(XbAbY, Xb);    TEXT_strcat(XbAbY, A); TEXT_strcat(XbAbY, b); TEXT_strcat(XbAbY, Y); 
    t = Xh; *t = NULL_TEXT; TEXT_strcat(t, X);  TEXT_strcat(t, hyphen);
    t = oXo; *t = NULL_TEXT; TEXT_strcat(t, oBe); TEXT_strcat(t, X);  TEXT_strcat(t, oEn);
    t = oXho; *t = NULL_TEXT; TEXT_strcat(t, oBe); TEXT_strcat(t, X);  TEXT_strcat(t, hyphen);  TEXT_strcat(t, oEn);
    t = oXhoboAoboYo; *t = NULL_TEXT; 
        TEXT_strcat(t, oBe); TEXT_strcat(t, X);  TEXT_strcat(t, hyphen); TEXT_strcat(t, oEn);    TEXT_strcat(t, b); 
        TEXT_strcat(t, oBe); TEXT_strcat(t, A);  TEXT_strcat(t, oEn);    TEXT_strcat(t, b); 
        TEXT_strcat(t, oBe); TEXT_strcat(t, Y);  TEXT_strcat(t, oEn);
    XbBbYD[0] = NULL_TEXT; TEXT_strcat(XbBbY, Xb);    TEXT_strcat(XbBbY, B); TEXT_strcat(XbBbY, b); TEXT_strcat(XbBbY, Y); 
    XbYD[0] = NULL_TEXT; TEXT_strcat(XbY, Xb);  TEXT_strcat(XbY, Y); 
    XhYD[0] = NULL_TEXT; TEXT_strcat(XhY, X); TEXT_strcat(XhY, hyphen);  TEXT_strcat(XhY, Y); 
    XbYbD[0] = NULL_TEXT; TEXT_strcat(XbYb, Xb); TEXT_strcat(XbYb, Yb); 
    qXD[0] = NULL_TEXT; TEXT_strcat(qX, q); TEXT_strcat(qX, X); 
    YqD[0] = NULL_TEXT; TEXT_strcat(Yq, Y); TEXT_strcat(Yq, q); 
    qXbYq[0] = NULL_TEXT; TEXT_strcat(qXbYq, q); TEXT_strcat(qXbYq, XbY); TEXT_strcat(qXbYq, q);  
    t = eqXbYeq; *t = NULL_TEXT; 
        TEXT_strcat(t, esc); TEXT_strcat(t, q);  TEXT_strcat(t, XbY); TEXT_strcat(t, esc);
        TEXT_strcat(t, q);     
    lng[0] = NULL_TEXT; TEXT_strcat(lng, XbYb); TEXT_strcat(lng, q); TEXT_strcat(lng, XbY); TEXT_strcat(lng, q); TEXT_strcat(lng, b); TEXT_strcat(lng, XbYb); 
    
    // Build the space separated data
    X1bX2bX3bY1bY2[0] = NULL_TEXT;  
    X1bX2bX3bhbY1bY2[0] = NULL_TEXT;  
    X1bX2bX3bA1A2bY1bY2[0] = NULL_TEXT;  
    X1bX2bX3bA1bA2bY1bY2[0] = NULL_TEXT;  
    X1bX2bX3[0] = NULL_TEXT;  
    oX1oboX2oboX3o[0] = NULL_TEXT;
    obX1bX2bX3bo[0] = NULL_TEXT;
    Y1bY2[0] = NULL_TEXT;  
    A1bA2[0] = NULL_TEXT;  
    // Start with the X value
    for (i=0; i<3; i++){
        buff[0] = NULL_TEXT;
        TEXT_strCcpy(buff, X+(i*charSizeX), 1);
        //
        TEXT_strcat(X1bX2bX3, buff);
        if (i != 3-1) {
           TEXT_strcat(X1bX2bX3, b);
        }
        //
        TEXT_strcat(oX1oboX2oboX3o, oBe);
        TEXT_strcat(oX1oboX2oboX3o, buff);
        TEXT_strcat(oX1oboX2oboX3o, oEn);
        if (i != 3-1) {
           TEXT_strcat(oX1oboX2oboX3o, b);
        }
    }
    TEXT_strcat(obX1bX2bX3bo, oBe); TEXT_strcat(obX1bX2bX3bo, b); TEXT_strcat(obX1bX2bX3bo, X1bX2bX3); TEXT_strcat(obX1bX2bX3bo, b);  
        TEXT_strcat(obX1bX2bX3bo, oEn);
    // Start with the Y value
    for (i=0; i<2; i++){
        buff[0] = NULL_TEXT;
        TEXT_strCcpy(buff, Y+(i*charSizeY), 1);
        TEXT_strcat(Y1bY2, buff);
        if (i != 2-1) {
           TEXT_strcat(Y1bY2, b);
        }
    }
    // Start with the ASCII value
    for (i=0; i<2; i++){
        buff[0] = NULL_TEXT;
        TEXT_strCcpy(buff, A+(i*1), 1);
        TEXT_strcat(A1bA2, buff);
        if (i != 2-1) {
           TEXT_strcat(A1bA2, b);
        }
    }
    X1bX2bX3bY1bY2[0] = NULL_TEXT;  
    X1bX2bX3bhbY1bY2[0] = NULL_TEXT;  
    X1bX2bX3bA1A2bY1bY2[0] = NULL_TEXT;  
    X1bX2bX3bA1bA2bY1bY2[0] = NULL_TEXT;  

    printf("  X = /%s/\n",X);
    printf("  Y = /%s/\n",Y);
    printf("  A = /%s/\n",A);
    printf("  XbAbY = /%s/\n",XbAbY);
    printf("  oXhoboAoboYo = /%s/\n",oXhoboAoboYo);
    printf("  eqXbYeq = /%s/\n", eqXbYeq);
    printf("  X1bX2bX3 = /%s/\n",X1bX2bX3);
    printf("  Y1bY2 = /%s/\n",Y1bY2);
    printf("  A1bA2 = /%s/\n",A1bA2);
    printf("  A1bA2 = /%s/\n",A1bA2);
    //      
    TEXT_strcat(X1bX2bX3bY1bY2, X1bX2bX3);
    TEXT_strcat(X1bX2bX3bY1bY2, b);
    TEXT_strcat(X1bX2bX3bY1bY2, Y1bY2);
    //      
    TEXT_strcat(X1bX2bX3bhbY1bY2, X1bX2bX3);
    TEXT_strcat(X1bX2bX3bhbY1bY2, b);
    TEXT_strcat(X1bX2bX3bhbY1bY2, hyphen);
    TEXT_strcat(X1bX2bX3bhbY1bY2, b);
    TEXT_strcat(X1bX2bX3bhbY1bY2, Y1bY2);
    //        
    TEXT_strcat(X1bX2bX3bA1A2bY1bY2, X1bX2bX3);
    TEXT_strcat(X1bX2bX3bA1A2bY1bY2, b);
    TEXT_strcat(X1bX2bX3bA1A2bY1bY2, A);
    TEXT_strcat(X1bX2bX3bA1A2bY1bY2, b);
    TEXT_strcat(X1bX2bX3bA1A2bY1bY2, Y1bY2);
    //
    TEXT_strcat(X1bX2bX3bA1bA2bY1bY2, X1bX2bX3);
    TEXT_strcat(X1bX2bX3bA1bA2bY1bY2, b); 
    TEXT_strcat(X1bX2bX3bA1bA2bY1bY2, A1bA2); 
    TEXT_strcat(X1bX2bX3bA1bA2bY1bY2, b); 
    TEXT_strcat(X1bX2bX3bA1bA2bY1bY2, Y1bY2); 

    *lX = NULL_TEXT; TEXT_strcat(lX, aBe); TEXT_strcat(lX, X);
    *lXr = NULL_TEXT; TEXT_strcat(lXr, aBe); TEXT_strcat(lXr, X); TEXT_strcat(lXr, aEn);   //  /{X}/
    *lYr = NULL_TEXT; TEXT_strcat(lYr, aBe); TEXT_strcat(lYr, Y); TEXT_strcat(lYr, aEn);   
    *blXrb = NULL_TEXT; TEXT_strcat(blXrb, b); TEXT_strcat(blXrb, lXr); TEXT_strcat(blXrb, b);     
    *lbXbr = NULL_TEXT; TEXT_strcat(lbXbr, aBe); TEXT_strcat(lbXbr, b); TEXT_strcat(lbXbr, X); TEXT_strcat(lbXbr, b); TEXT_strcat(lbXbr, aEn);    
    *lbXbrb = NULL_TEXT; TEXT_strcat(lbXbrb, lbXbr); TEXT_strcat(lbXbrb, b);
    *lXrlYr = NULL_TEXT; TEXT_strcat(lXrlYr, lXr); TEXT_strcat(lXrlYr, lYr);  
    *blXrblYrb = NULL_TEXT; TEXT_strcat(blXrblYrb, b); TEXT_strcat(blXrblYrb, lXr); TEXT_strcat(blXrblYrb, b); TEXT_strcat(blXrblYrb, lYr); TEXT_strcat(blXrblYrb, b);
    *llXrslYrr = NULL_TEXT; TEXT_strcat(llXrslYrr, aBe); TEXT_strcat(llXrslYrr, lXr);TEXT_strcat(llXrslYrr, aAl); TEXT_strcat(llXrslYrr, lYr); TEXT_strcat(llXrslYrr, aEn);
    *llXrslYrrbX = NULL_TEXT; TEXT_strcat(llXrslYrrbX, llXrslYrr);  TEXT_strcat(llXrslYrrbX, bX); 
    *lXrslYr = NULL_TEXT; TEXT_strcat(lXrslYr, lXr);  TEXT_strcat(lXrslYr, aAl); TEXT_strcat(lXrslYr, lYr); 
    *blbXbrbslYr = NULL_TEXT; TEXT_strcat(blbXbrbslYr, b); TEXT_strcat(blbXbrbslYr, lbXbr); TEXT_strcat(blbXbrbslYr, b);
            TEXT_strcat(blbXbrbslYr, aAl); TEXT_strcat(blbXbrbslYr, lYr); 

    if (iter == 5 || iter == 0){
        TEXT_LIST *tl;
        tl = load_TEXT_LIST(lexicon, 0);
        tst++; err+= test_int_Fof_TEXTLIST_TPtr(-1, "in_TEXT_LIST", tl, X) ;   
        tst++; err+= test_int_Fof_TEXTLIST_TPtr(5, "in_TEXT_LIST", tl, Y) ;   
    }
    
    tst++; err+= test_TPtr_Fof_TPtr_int(X, "TEXT_strBdup", X, 3*charSizeX);    
    tst++; err+= test_TPtr_Fof_TPtr_int(X, "TEXT_strBdup", XbAbY, 3*charSizeX);    
    tst++; err+= test_TPtr_Fof_TPtr_int(XbAbY, "TEXT_strBdup", XbAbY, 3*charSizeX + 1 + 2 + 1 + 2*charSizeY);    
    tst++; err+= test_TPtr_Fof_TPtr_int(XbA, "TEXT_strBdup", XbAbY, 3*charSizeX + 1 + 2);    

    tst++; err+= test_TPtr_Fof_TPtr_int(X, "TEXT_strBdup_noEscape", X, 3*charSizeX);    
    tst++; err+= test_TPtr_Fof_TPtr_int(X, "TEXT_strBdup_noEscape", XbAbY, 3*charSizeX);    
    tst++; err+= test_TPtr_Fof_TPtr_int(XbAbY, "TEXT_strBdup_noEscape", XbAbY, 3*charSizeX + 1 + 2 + 1 + 2*charSizeY);    
    tst++; err+= test_TPtr_Fof_TPtr_int(XbA, "TEXT_strBdup_noEscape", XbAbY, 3*charSizeX + 1 + 2);    

    tst++; err+= test_TPtr_Fof_TPtr_int(qXbYq, "TEXT_strBdup_noEscape", eqXbYeq, 1 + 1 + 3*charSizeX + 1 + 2*charSizeY + 1 + 1);    
    tst++; err+= test_TPtr_Fof_TPtr_int(XsX, "TEXT_strBdup_noEscape", XesXsYsY, 3*charSizeX + 1 + 1 + 3*charSizeX);    

    buff[0] = NULL_TEXT;
    tst++; err+= test_TPtr_Fof_TPtr_TPtr_int(X, "TEXT_strBcpy", buff, X, 3*charSizeX);    
    buff[0] = NULL_TEXT;
    tst++; err+= test_TPtr_Fof_TPtr_TPtr_int(X, "TEXT_strBcpy", buff, XbAbY, 3*charSizeX);    
    buff[0] = NULL_TEXT;
    tst++; err+= test_TPtr_Fof_TPtr_TPtr_int(XbAbY, "TEXT_strBcpy", buff, XbAbY, 3*charSizeX + 1 + 2 + 1 + 2*charSizeY);    
    buff[0] = NULL_TEXT;
    tst++; err+= test_TPtr_Fof_TPtr_TPtr_int(XbA, "TEXT_strBcpy", buff, XbAbY, 3*charSizeX + 1 + 2);    
 
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(X, "TEXT_strstr", X, X);    
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(XbAbY, "TEXT_strstr", XbAbY, X);    
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(Y, "TEXT_strstr", XbAbY, Y);    

    if (TEXT_get_encoding() == ASCII){
        tst++; err+= test_void_Fof_TPtr((TEXT *)&ASCII_A, "TEXT_str_to_low", (TEXT *)&ASCII_B);    
        tst++; err+= test_void_Fof_TPtr((TEXT *)&ASCII_B, "TEXT_str_to_upp", (TEXT *)&ASCII_A);    
    } else if (TEXT_get_encoding() == EXTASCII){
        tst++; err+= test_void_Fof_TPtr((TEXT *)&EXTASCII_A, "TEXT_str_to_low", (TEXT *)&EXTASCII_B);    
        tst++; err+= test_void_Fof_TPtr((TEXT *)&EXTASCII_B, "TEXT_str_to_upp", (TEXT *)&EXTASCII_A);    
    } else {
        tst++; err+= test_void_Fof_TPtr(XbAbY, "TEXT_str_to_low", XbBbY);    
        tst++; err+= test_void_Fof_TPtr(XbBbY, "TEXT_str_to_upp", XbAbY);    
    }
    
    if (TEXT_get_encoding() == ASCII){
       tst++; err+= test_int_Fof_TPtr_TPtr(0, "TEXT_strcasecmp", (TEXT *)&ASCII_A, (TEXT *)&ASCII_A);    
       tst++; err+= test_int_Fof_TPtr_TPtr(0, "TEXT_strcasecmp", (TEXT *)&ASCII_A, (TEXT *)&ASCII_B);    
       tst++; err+= test_int_Fof_TPtr_TPtr(0, "TEXT_strcasecmp", (TEXT *)&ASCII_B, (TEXT *)&ASCII_A);    
       tst++; err+= test_int_Fof_TPtr_TPtr(0, "TEXT_strcasecmp", (TEXT *)&ASCII_B, (TEXT *)&ASCII_B);    
       tst++; err+= test_int_Fof_TPtr_TPtr(1, "TEXT_strcasecmp", (TEXT *)&ASCII_X, (TEXT *)&ASCII_Y);    
    } else {
        tst++; err+= test_int_Fof_TPtr_TPtr(0, "TEXT_strcasecmp", X, X);    
        tst++; err+= test_int_Fof_TPtr_TPtr(-1, "TEXT_strcasecmp", X, Y);    
        tst++; err+= test_int_Fof_TPtr_TPtr(1, "TEXT_strcasecmp", Y, X);    
        tst++; err+= test_int_Fof_TPtr_TPtr(1, "TEXT_strcasecmp", Y, A);    
        tst++; err+= test_int_Fof_TPtr_TPtr(1, "TEXT_strcasecmp", Y, (TEXT *)0);    
        tst++; err+= test_int_Fof_TPtr_TPtr(-1, "TEXT_strcasecmp", (TEXT *)0, Y);    
        tst++; err+= test_int_Fof_TPtr_TPtr(0, "TEXT_strcasecmp", (TEXT *)0, (TEXT *)0);    

    }
    tst++; err+= test_int_Fof_TPtr_TPtr(0, "TEXT_strcmp", X, X);    
    tst++; err+= test_int_Fof_TPtr_TPtr(-1, "TEXT_strcmp", X, Y);    
    tst++; err+= test_int_Fof_TPtr_TPtr(1, "TEXT_strcmp", Y, X);    
    tst++; err+= test_int_Fof_TPtr_TPtr(1, "TEXT_strcmp", Y, A);    
    tst++; err+= test_int_Fof_TPtr_TPtr(1, "TEXT_strcmp", Y, (TEXT *)0);    
    tst++; err+= test_int_Fof_TPtr_TPtr(-1, "TEXT_strcmp", (TEXT *)0, Y);    
    tst++; err+= test_int_Fof_TPtr_TPtr(0, "TEXT_strcmp", (TEXT *)0, (TEXT *)0);    
    
    tst++; err+= test_int_Fof_TPtr_TPtr(0, "TEXT_strcasecmp", XbAbY, XbBbY);    
    tst++; err+= test_int_Fof_TPtr_TPtr_int(0, "TEXT_strCcasecmp", XbAbY, XbBbY, 3);    
    tst++; err+= test_int_Fof_TPtr_TPtr_int(0, "TEXT_strCcasecmp", XbAbY, XbBbY, 6);    

    tst++; err+= test_int_Fof_TPtr_TPtr_int(0, "TEXT_strCcmp", XbAbY, XbBbY, 4);    
    tst++; err+= test_int_Fof_TPtr_TPtr_int(1, "TEXT_strCcmp", XbAbY, XbBbY, 6);    
    

    // Test int ___(TEXT)
    tst++; err+=  test_int_Fof_TEXT(0, "end_of_TEXT", *X);
    buff[0] = NULL_TEXT;
    tst++; err+=  test_int_Fof_TEXT(1, "end_of_TEXT", *buff);

    // Test int ___(TEXT *)
    tst++; err+=  test_int_Fof_TPtr(charSizeX, "TEXT_nbytes_of_char", X);
    tst++; err+=  test_int_Fof_TPtr(1, "TEXT_nbytes_of_char", bX);
    tst++; err+=  test_int_Fof_TPtr(charSizeX, "TEXT_nbytes_of_char", Xb);
    tst++; err+=  test_int_Fof_TPtr(1, "TEXT_nbytes_of_char", b);
    tst++; err+=  test_int_Fof_TPtr(1, "TEXT_nbytes_of_char", e);
    tst++; err+=  test_int_Fof_TPtr(charSizeY, "TEXT_nbytes_of_char", Y);
    tst++; err+=  test_int_Fof_TPtr(charSizeX * 3, "TEXT_strlen", X);
    tst++; err+=  test_int_Fof_TPtr(charSizeX * 3 + 1, "TEXT_strlen", Xb);
    tst++; err+=  test_int_Fof_TPtr(charSizeX * 3 + 1 + charSizeY * 2, "TEXT_strlen", XbY);
    tst++; err+=  test_int_Fof_TPtr(charSizeX * 3 + 1 + charSizeY * 2 + 1, "TEXT_strlen", XbYb);

    tst++; err+=  test_int_Fof_TPtr(3, "TEXT_chrlen", X);
    tst++; err+=  test_int_Fof_TPtr(3 + 1, "TEXT_chrlen", Xb);
    tst++; err+=  test_int_Fof_TPtr(3 + 1 + 2, "TEXT_chrlen", XbY);
    tst++; err+=  test_int_Fof_TPtr(3 + 1 + 2 + 1, "TEXT_chrlen", XbYb);

    tst++; err+=  test_int_Fof_TPtr(1, "TEXT_is_empty", b);
    tst++; err+=  test_int_Fof_TPtr(0, "TEXT_is_empty", bbX);
    tst++; err+=  test_int_Fof_TPtr(0, "TEXT_is_empty", Yb);
    
    // Test TEXT* ___(TEXT *)
    tst++; err+= test_TPtr_Fof_TPtr(X, "TEXT_strdup", X);
    tst++; err+= test_TPtr_Fof_TPtr(XbA, "TEXT_strdup", XbA);
    tst++; err+= test_TPtr_Fof_TPtr(X, "TEXT_skip_wspace", X);
    tst++; err+= test_TPtr_Fof_TPtr(X, "TEXT_skip_wspace", bX);
    tst++; err+= test_TPtr_Fof_TPtr(X, "TEXT_skip_wspace", bbX);
    
    // Test TEXT* ___(TEXT *, TEXT *)
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(XY, "TEXT_add", X, Y);
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(XbY, "TEXT_add", Xb, Y);
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(XbYb, "TEXT_add", Xb, Yb);
    
    buff[0] = NULL_TEXT;
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(Xb, "TEXT_strcat", (TEXT *)&buff, Xb);
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(XbYb, "TEXT_strcat", (TEXT *)&buff, Yb);

    buff[0] = NULL_TEXT;
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(Xb, "TEXT_strcat", (TEXT *)&buff, Xb);
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(XbA, "TEXT_strcat", (TEXT *)&buff, A);     
    TEXT_strcat((TEXT *)buff, b);
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(XbAbY, "TEXT_strcat", (TEXT *)&buff, Y);

    buff[0] = NULL_TEXT;
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(A, "TEXT_strcat", (TEXT *)&buff, A);     

    buff[0] = NULL_TEXT;
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(Xb, "TEXT_strcpy", (TEXT *)&buff, Xb);
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(Xb, "TEXT_strcpy", (TEXT *)&buff, Xb);
    buff[0] = NULL_TEXT;

    // Test size_y __(TEXT *, TEXT *)
    tst++; err+= test_sizet_Fof_TPtr_TPtr(0, "TEXT_strspn", Xb, b); 
    tst++; err+= test_sizet_Fof_TPtr_TPtr(1, "TEXT_strspn", bX, b);
    tst++; err+= test_sizet_Fof_TPtr_TPtr(2, "TEXT_strspn", llXrslYrrbX, lar); 

    // strcspn
    tst++; err+= test_sizet_Fof_TPtr_TPtr(charSizeX * 3, "TEXT_strcspn", Xb, b); 
    tst++; err+= test_sizet_Fof_TPtr_TPtr(0, "TEXT_strcspn", bX, b); 
    tst++; err+= test_sizet_Fof_TPtr_TPtr(0, "TEXT_strcspn", llXrslYrrbX, lar); 

    // strqtok
    TEXT_strcpy((TEXT *)&buff, lng);
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(X, "TEXT_strqtok", (TEXT *)&buff, " ");
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(Y, "TEXT_strqtok", (TEXT *)0, " ");
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(qXbYq, "TEXT_strqtok", (TEXT *)0, " ");
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(X, "TEXT_strqtok", (TEXT *)0, " ");
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(Y, "TEXT_strqtok", (TEXT *)0, " ");    

    // strtok
    TEXT_strcpy((TEXT *)&buff, lng);
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(X, "TEXT_strtok", (TEXT *)&buff, " ");
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(Y, "TEXT_strtok", (TEXT *)0, " ");
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(qX, "TEXT_strtok", (TEXT *)0, " ");
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(Yq, "TEXT_strtok", (TEXT *)0, " ");
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(X, "TEXT_strtok", (TEXT *)0, " ");    
    tst++; err+= test_TPtr_Fof_TPtr_TPtr(Y, "TEXT_strtok", (TEXT *)0, " ");    
    
    // TEXT_delete_chars
    TEXT_strcpy((TEXT *)&buff, bX);
    tst++; err+= test_void_Fof_TPtr_TPtr(X, "TEXT_delete_chars", (TEXT *)&buff, " ");    
    TEXT_strcpy((TEXT *)&buff, Xb);
    tst++; err+= test_void_Fof_TPtr_TPtr(X, "TEXT_delete_chars", (TEXT *)&buff, " ");    
    TEXT_strcpy((TEXT *)&buff, qXbYq);
    tst++; err+= test_void_Fof_TPtr_TPtr(XY, "TEXT_delete_chars", (TEXT *)&buff, " \"");    
    
    // TEXT_strcpy_escaped
    buff[0] = NULL_TEXT;
    tst++; err+= test_TPtr_Fof_TPtr_TPtr_T(XbYb, "TEXT_strcpy_escaped", (TEXT *)&buff, XbYb, '"');    
    tst++; err+= test_TPtr_Fof_TPtr_TPtr_T(eqXbYeq, "TEXT_strcpy_escaped", (TEXT *)&buff, qXbYq, '"');    

    // TEXT_separate_chars  
    tst++; err+= test_separate_chars(XhY, X1bX2bX3bhbY1bY2, 0);    
    tst++; err+= test_separate_chars(XY, X1bX2bX3bY1bY2, CALI_DELHYPHEN);    
    tst++; err+= test_separate_chars(oXo, obX1bX2bX3bo, 0);    
    tst++; err+= test_separate_chars(oXo, obX1bX2bX3bo, CALI_DELHYPHEN);    
    if (TEXT_get_encoding() == ASCII || TEXT_get_encoding() == EXTASCII){
       tst++; err+= test_separate_chars(XbAbY, XbAbY, CALI_NOASCII);    
       tst++; err+= test_separate_chars(XbAbY, X1bX2bX3bA1bA2bY1bY2, CALI_DELHYPHEN);    
       tst++; err+= test_separate_chars(XbAbY, X1bX2bX3bA1bA2bY1bY2, 0);    
    } else {       
       tst++; err+= test_separate_chars(XbAbY, X1bX2bX3bA1A2bY1bY2, CALI_NOASCII);    
       tst++; err+= test_separate_chars(XbAbY, X1bX2bX3bA1bA2bY1bY2, CALI_DELHYPHEN);    
       tst++; err+= test_separate_chars(XbAbY, X1bX2bX3bA1bA2bY1bY2, 0);    
       tst++; err+= test_separate_chars(oXo, obX1bX2bX3bo, CALI_NOASCII);    
    }
    if (TEXT_get_encoding() == ASCII){
      tst++; err+= test_separate_chars(XY, XY, CALI_NOASCII);    
      tst++; err+= test_separate_chars(XhY, XhY, CALI_NOASCII);    
      tst++; err+= test_separate_chars(XY, XY, CALI_DELHYPHEN + CALI_NOASCII);           
      tst++; err+= test_separate_chars(XhY, XY, CALI_DELHYPHEN + CALI_NOASCII);          
    }
         // Alternation Parsing
     {  
         TEXT tbuf[200], ttbuf[200], *tptr;
         tptr = XbAbY;
         *ttbuf = NULL_TEXT;
         //db = 20;
         tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_token", &tptr, (TEXT *)&tbuf, 200, X);    
         tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_token", &tptr, (TEXT *)&tbuf, 200, A);    
         tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_token", &tptr, (TEXT *)&tbuf, 200, Y);    
         tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(0, "find_next_TEXT_token", &tptr, (TEXT *)&tbuf, 200, ttbuf);    

         // Test looking for bounding braces        
         tptr = lX;    tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(0, "find_next_TEXT_token", &tptr, tbuf, 200, lX);    
         tptr = lXr;   tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_token", &tptr, tbuf, 200, lXr);    
         tptr = blXrb; tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_token", &tptr, tbuf, 200, lXr);    
         tptr = lbXbr; tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_token", &tptr, tbuf, 200, lbXbr);
         
         tptr = lXrlYr; tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_token", &tptr, tbuf, 200, lXr);    
                        tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_token", &tptr, tbuf, 200, lYr);    
                        tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(0, "find_next_TEXT_token", &tptr, tbuf, 200, e);    
         tptr = blXrblYrb; tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_token", &tptr, tbuf, 200, lXr);    
                           tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_token", &tptr, tbuf, 200, lYr);    
         tptr = llXrslYrr; tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_token", &tptr, tbuf, 200, llXrslYrr);    
         tptr = llXrslYrrbX; tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_token", &tptr, tbuf, 200, llXrslYrr);    
//db = 23;
         // test looking for alternates
         tptr = X; tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_alternation", &tptr, tbuf, 200, X);    
         tptr = XbY; tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_alternation", &tptr, tbuf, 200, XbY);    
         tptr = llXrslYrrbX; tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_alternation", &tptr, tbuf, 200, llXrslYrrbX);    
         tptr = llXrslYrr; tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_alternation", &tptr, tbuf, 200, llXrslYrr);    
         tptr = lXrslYr; tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_alternation", &tptr, tbuf, 200, lXr);    
                         tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_alternation", &tptr, tbuf, 200, lYr);    
                         tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(0, "find_next_TEXT_alternation", &tptr, tbuf, 200, e);    
         tptr = blbXbrbslYr; tst++; err+= test_int_Fof_TPtrPtr_TPtr_int(1, "find_next_TEXT_alternation", &tptr, tbuf, 200, lbXbrb);    

         tst++; err+= test_TPtr_Fof_TPtr_TPtr(Xb, "tokenize_TEXT_first_alt", Xb, (TEXT *)" \t");
         tst++; err+= test_TPtr_Fof_TPtr_TPtr(Xb, "tokenize_TEXT_first_alt", blbXbrbslYr, (TEXT *)" \t");

     }
     // NetADT Testing
     { 
//       tst++; err += testNetExpandChar(X, X1bX2bX3, CALI_ON, 0);
//       tst++; err += testNetExpandChar(X, X1bX2bX3, CALI_ON + CALI_DELHYPHEN, 0);
//       tst++; err += testNetExpandChar(Xh, X1bX2bX3, CALI_ON + CALI_DELHYPHEN, 0);
//       tst++; err += testNetExpandChar(oXho, obX1bX2bX3bo, CALI_ON + CALI_DELHYPHEN, 0);
//       tst++; err += testNetExpandChar(oXho, oX1oboX2oboX3o, CALI_ON + CALI_DELHYPHEN, 1);
       if (TEXT_get_encoding() == ASCII || TEXT_get_encoding() == EXTASCII){
//          tst++; err += testNetExpandChar(X, X, CALI_ON + CALI_NOASCII, 0);
//          tst++; err += testNetExpandChar(X, X, CALI_ON + CALI_DELHYPHEN + CALI_NOASCII, 0);
//          tst++; err += testNetExpandChar(Xh, X, CALI_ON + CALI_DELHYPHEN + CALI_NOASCII, 0);
//          tst++; err += testNetExpandChar(oXo, oXo, CALI_ON + CALI_NOASCII,0 );
        } else {
//          tst++; err += testNetExpandChar(X, X1bX2bX3, CALI_ON + CALI_NOASCII, 0);
//          tst++; err += testNetExpandChar(X, X1bX2bX3, CALI_ON + CALI_DELHYPHEN + CALI_NOASCII, 0);
//          tst++; err += testNetExpandChar(Xh, X1bX2bX3, CALI_ON + CALI_DELHYPHEN + CALI_NOASCII, 0);
//          tst++; err += testNetExpandChar(oXo, obX1bX2bX3bo, CALI_ON + CALI_NOASCII, 0);
//          tst++; err += testNetExpandChar(oXo, oX1oboX2oboX3o, CALI_ON + CALI_NOASCII, 1);
        }
     }
     printf("\n");
  }

  
  
  printf("Total Tests: %d\nErrors: %d\n", tst, err);

  return(err);
}


int main(int argc, char **argv){
  int error;
  
  error = unitTestTEXT();
//  error += unitTestNetADT((TEXT *)"ab (cd) fr- (ag-)");
  exit(error);
}
 


