#include "generator.h"
#include "HtmlElement.h"
#include "lexer.h"
#include "parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>


static enum ttype cur_token;
static char *cur_token_val;

static HtmlElement   *el;
static HtmlAttribute *attr;


static bool maybe_one(enum ttype tkn, void (*callback)(char *)) {
	if (cur_token == tkn) {		
		/* now callback is responsible for memory managment of cur_token_val */
		callback(cur_token_val);
		cur_token_val = NULL;

		cur_token = lexer_gettoken(&cur_token_val);
		return true;
	}
	return false;
}

static void maybe_more(enum ttype tkn, void (*callback)(char *)) {
	while(maybe_one(tkn, callback));
}

static void match(enum ttype tkn, void (*callback)(char *)) {
	if (!maybe_one(tkn, callback)) {
		printf("Error: token %d expected\n", tkn);
	}
}

static void tag(char *val) {
	el->tag = val;
}

static void id(char *val) {
	el->id = val;
}

static void class_name(char *val) {
	HtmlElement_add_class(el, val);
}

static void attr_name(char *val) {
	attr = HtmlAttribute_new();
	attr->name = val;
}

static void attr_val(char *val) {
	attr->val = val;
	HtmlElement_add_attribute(el, attr);
}

static void eql(char *val) {

}

static void text(char *val) {
	el->text = val;
}

static void indent(char *val) {
	el->indent++;
}

void element() {
	el = HtmlElement_new();

	maybe_more(TOKEN_INDENT, indent);
	maybe_one(TOKEN_TAG, tag);
	maybe_one(TOKEN_ID, id);
	maybe_more(TOKEN_CLASS, class_name);	
	while (maybe_one(TOKEN_ATTR, attr_name)) {
		match(TOKEN_EQL, eql);
		match(TOKEN_CDATA, attr_val);
	}
	maybe_one(TOKEN_CDATA, text);

	/* responsible for el memory managment */
	generator_add_element(el);
}


void parse() {
	cur_token = lexer_gettoken(&cur_token_val);
	while(TOKEN_EOF != cur_token && TOKEN_ERROR != cur_token) {		
		element();
	}
	generator_get_result();
}