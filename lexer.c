#include "lexer.h"
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

 char *ttype2str[] = {
	"Error",
	"EOF",
	"Tag",
	"Id",
	"Class",
	"Attribute",
	"=",
	"CDATA",
	"(",
	")",
	"Indent"
};

#define SEEK_TO_NONBLANK while(' ' == *pos || '\n' == *pos) pos++;

#define SEEK_TO_EOW while(isalpha(*++pos));

#define SEEK_TO_EO_CSS_NAME \
do{	pos++; }while('-' == *pos || '_' == *pos || isalpha(*pos));

#define SEEK_TO_CLOSE_QT do{ pos++; }while(*pos && '"' != *pos); pos++;

#define TOKEN_START tkn_start = pos - in;

#define TOKEN(T) \
tkn_len = (pos - in) - tkn_start;\
*pval = malloc(sizeof(char) * (tkn_len + 1));\
memcpy(*pval, in + tkn_start, tkn_len);\
(*pval)[tkn_len] = '\0';\
return TOKEN_##T;


/* source input */
static char *in;

/* pointer to current position in source */
static char *pos;


void lexer_init(char *input) {
	assert(input);
	
	in = input;
	pos = input;
}

enum ttype 
lexer_gettoken(char **pval) {
	assert(NULL == *pval);

	int tkn_start = 0;
	int tkn_len = 0;

	SEEK_TO_NONBLANK;

	if('%' == *pos) {
		/* skip percent symbol */
		pos++;

		TOKEN_START;			
		SEEK_TO_EOW;
		TOKEN(TAG);
	} else if('.' == *pos) {
		/* skip dot symbol */
		pos++;
		
		TOKEN_START;
		SEEK_TO_EO_CSS_NAME;
		TOKEN(CLASS);
	} else if('#' == *pos) {
		/* skip sharp symbol */
		pos++;

		TOKEN_START;
		SEEK_TO_EO_CSS_NAME;
		TOKEN(ID);
	} else if('"' == *pos) {
		TOKEN_START;
		SEEK_TO_CLOSE_QT;
		TOKEN(CDATA);
	} else if('=' == *pos) {
		TOKEN_START;
		pos++;
		TOKEN(EQL);
	} else if('\t' == *pos) {
		TOKEN_START;		
		pos++;
		TOKEN(INDENT);
	} else if ('(' == *pos) {
		TOKEN_START;
		pos++;
		TOKEN(LBRACKET);
	} else if (')' == *pos) {
		TOKEN_START;
		pos++;
		TOKEN(RBRACKET)
	} else if(isalpha(*pos)){
		TOKEN_START;
		SEEK_TO_EOW;
		TOKEN(ATTR);
	} else if('\0' == *pos){
		TOKEN(EOF);
	} 
	TOKEN_START;
	pos++;
	TOKEN(ERROR);
}