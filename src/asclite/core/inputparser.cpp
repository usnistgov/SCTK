/*
 * ASCLITE
 * Author: Jerome Ajot, Jon Fiscus, Nicolas Radde, Chris Laprun
 *
 * This software was developed at the National Institute of Standards and Technology by 
 * employees of the Federal Government in the course of their official duties. Pursuant
 * to title 17 Section 105 of the United States Code this software is not subject to
 * copyright protection and is in the public domain. ASCLITE is an experimental system.
 * NIST assumes no responsibility whatsoever for its use by other parties, and makes no
 * guarantees, expressed or implied, about its quality, reliability, or any other
 * characteristic. We would appreciate acknowledgement if the software is used.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS."  With regard to this software, NIST MAKES NO EXPRESS
 * OR IMPLIED WARRANTY AS TO ANY MATTER WHATSOEVER, INCLUDING MERCHANTABILITY,
 * OR FITNESS FOR A PARTICULAR PURPOSE.
 */

/**
 * This class represent a abstract input parser.
 * the main method is loadFile()
 */

#include "inputparser.h" // class's header file

/**
 * Build a Token with a text input
 */
Token* InputParser::BuildToken(int start, int dur, string text, Segment* parent)
{
    Token* token = Token::CreateWithDuration(start, dur, parent);
    token->SetSourceText(text);
    return token;
}

int InputParser::ParseString(string chaine)
{
	if(strchr(chaine.c_str(),'.') == NULL)
	{
		return(atoi(chaine.c_str()) * 1000);
	}
	else
	{
		int bf, af;
		char c2[BUFFER_SIZE];
		sscanf(chaine.c_str(), "%d.%s", (int*) &bf, (char*) &c2);
		
		int len = strlen(c2);
		
		af = atoi(c2);
				
		if(len > 3)
		{
			af = (int)((double)(af)/pow(10, len-3));
		}
		else
		{
			af = (int)((double)(af)*pow(10, 3-len));
		}
		
		return( bf*1000+af);		
	}
}
