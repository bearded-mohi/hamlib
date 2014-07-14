#include "lexer.h"
#include <stdio.h>
#include <ctype.h>


#define SEEK_TO_EOW while(isalpha(*++in)); in--;

#define SEEK_TO_EO_CSS_NAME \
do{	in++; }while('-' == *in || '_' == *in || isalpha(*in)); in--;

#define SEEK_TO_CLOSE_BR while(*in && '"' != *++in);

#define TOKEN_START tkn_start = in - in_first;

#define TOKEN(t) printf(#t": %d %d\n", tkn_start, (int)(in - in_first));


void lex(char *in) {
	char *in_first = in;
	int   tkn_start;

	do {
		if('%' == *in) {
			TOKEN_START;			
			SEEK_TO_EOW;
			TOKEN(TAG);
		} else if('.' == *in) {
			TOKEN_START;
			SEEK_TO_EO_CSS_NAME;
			TOKEN(CLASS);
		} else if('#' == *in) {
			TOKEN_START;
			SEEK_TO_EO_CSS_NAME;
			TOKEN(ID);
		} else if('"' == *in) {
			TOKEN_START;
			SEEK_TO_CLOSE_BR;
			TOKEN(CDATA);
		} else if('=' == *in) {
			TOKEN_START;
			TOKEN(EQL);
		} else if('\t' == *in) {
			TOKEN_START;
			TOKEN(INDENT);
		} else if(' ' == *in) {
			continue;		
		} else if(isalpha(*in)){
			TOKEN_START;
			SEEK_TO_EOW;
			TOKEN(ATTR);
		} else if('\0' == *in){
			break;
		} else {
			TOKEN_START;
			TOKEN(ERROR);
		}
	} while(*in++);
}