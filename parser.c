#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "generator.h"
#include "HtmlElement.h"
#include "lexer.h"
#include "parser.h"

static enum ttype cur_token;
static char      *cur_token_val;
static int        cur_token_indent;

static HtmlElement   *el;
static HtmlAttribute *attr;

static bool maybe_one(enum ttype tkn, void (*callback)(char *))
{
	if (cur_token == tkn) {
		if (callback) {
			/* now callback is responsible for memory managment
			 * of cur_token_val
			 */
			callback(cur_token_val);
		} else {
			free(cur_token_val);
		}
		cur_token_val = NULL;
		cur_token = lexer_gettoken(&cur_token_val);
		return true;
	}
	return false;
}

static void maybe_more(enum ttype tkn, void (*callback)(char *))
{
	while(maybe_one(tkn, callback))
		;
}

static void match(enum ttype tkn, void (*callback)(char *))
{
	if (!maybe_one(tkn, callback))
		printf("Error: token %s expected\n", ttype2str[tkn]);
}

static void tag(char *val)
{
	el->tag = val;
}

static void id(char *val)
{
	el->id = val;
}

static void class_name(char *val)
{
	HtmlElement_add_class(el, val);
}

static void attr_name(char *val)
{
	attr = HtmlAttribute_new();
	attr->name = val;
}

static void attr_val(char *val)
{
	attr->val = val;
	HtmlElement_add_attribute(el, attr);
}

static void cdata(char *val)
{
	el->text = val;
}

static void cdata_only_element(char *val)
{
	tag("_text");
	cdata(val);
}

static void indent(char *val)
{
	cur_token_indent++;
}

static void element()
{
	maybe_one(TOKEN_TAG, tag);
	maybe_one(TOKEN_ID, id);
	maybe_more(TOKEN_CLASS, class_name);
	if (TOKEN_LBRACKET == cur_token) {
		match(TOKEN_LBRACKET, NULL);
		while (maybe_one(TOKEN_ATTR, attr_name)) {
			match(TOKEN_EQL, NULL);
			match(TOKEN_CDATA, attr_val);
		}
		match(TOKEN_RBRACKET, NULL);
	}
	maybe_one(TOKEN_CDATA, cdata);
}

static void text()
{
	match(TOKEN_CDATA, cdata_only_element);
}

static void node()
{
	el = HtmlElement_new();
	maybe_more(TOKEN_INDENT, indent);
	if (TOKEN_TAG   == cur_token  ||
	    TOKEN_ID    == cur_token  ||
	    TOKEN_CLASS == cur_token
	    ) {
		element();
	} else if (TOKEN_CDATA == cur_token) {
		text();
	}
	el->indent = cur_token_indent;
	cur_token_indent = 0;
	generator_add_element(el);
}

static void document()
{
	while (TOKEN_EOF != cur_token)
		node();
}

void parse()
{
	cur_token = lexer_gettoken(&cur_token_val);
	document();
	generator_get_result();
}