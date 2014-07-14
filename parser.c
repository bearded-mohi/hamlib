#include "hamlib.h"
#include "lexer.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// document = node*
// node = element | (element indent node)
// element = tag? id? class* attrs cdata?
// attrs = attr* | (attr eql cdata)

static enum ttype cur_token;
static char *cur_token_val;
// static char *element_str;

static bool one_or_zero(enum ttype tkn) {
	if (cur_token == tkn) {		
		printf("%d: %s\n", tkn, cur_token_val);

		free(cur_token_val);
		cur_token_val = NULL;
		cur_token = lexer_gettoken(&cur_token_val);

		return true;
	}
	return false;
}

static void zero_or_more(enum ttype tkn) {
	while (cur_token == tkn) {
		printf("%d: %s\n", tkn, cur_token_val);

		free(cur_token_val);
		cur_token_val = NULL;
		cur_token = lexer_gettoken(&cur_token_val);
	}
}

static void match(enum ttype tkn) {
	if (!one_or_zero(tkn)) {
		printf("Error: token %d expected\n", tkn);
	}
}

void element() {
	one_or_zero(TOKEN_TAG);
	one_or_zero(TOKEN_ID);
	zero_or_more(TOKEN_CLASS);
	while (one_or_zero(TOKEN_ATTR)) {
		match(TOKEN_EQL);
		match(TOKEN_CDATA);
	}
	one_or_zero(TOKEN_CDATA);
}


void parse() {
	cur_token = lexer_gettoken(&cur_token_val);
	while(TOKEN_EOF != cur_token && TOKEN_ERROR != cur_token) {
		zero_or_more(TOKEN_INDENT);
		element();
	}

	// while(TOKEN_EOF != lexer_gettoken(&cur_token_val)) {
	// 	free(cur_token_val);
	// 	cur_token_val = NULL;
	// }
}