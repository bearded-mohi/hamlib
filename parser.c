#include "lexer.h"
#include "parser.h"
#include <stdio.h>

document = node*
node = element | (element indent node)
element = tag? id? class* attrs cdata?
attrs = attr* | (attr eql cdata)

static ttype cur_token;

static void one_or_zero(ttype tkn) {
	if(cur_token == tkn) {
		printf("one\n");
	}
}

static void zero_or_more(ttype tkn) {

}

void document() {

}

void node() {

}

void element() {
	one_or_zero(TOKEN_TAG);
	one_or_zero(TOKEN_ID);
	zero_or_more(TOKEN_CLASS);
	attrs();
	one_or_zero(TOKEN_CDATA);
}

void attrs() {

}

void parse() {
	document();
}