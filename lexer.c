#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"

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

#define SEEK_TO_EOW while(isalnum(*++pos));

#define SEEK_TO_EO_CSS_NAME \
do{ pos++; }while('-' == *pos || '_' == *pos || isalnum(*pos));

#define SEEK_BEFORE_CLOSE_QT while('"' != *pos) pos++;

#define TOKEN_START tkn_start = pos - in;

#define TOKEN_VALUE \
tkn_len = (pos - in) - tkn_start;\
*pval = malloc(sizeof(char) * (tkn_len + 1));\
memcpy(*pval, in + tkn_start, tkn_len);\
(*pval)[tkn_len] = '\0';\

#define TOKEN(T) return TOKEN_##T;


/* source input */
static char *in;

/* pointer to current position in source */
static char *pos;

/* list of all html5 tags */
#if 0
static char *tags[] = {
	"a",
	"abbr",
	"address",
	"area",
	"article",
	"aside",
	"audio",
	"b",
	"base",
	"bdi",
	"bdo",
	"blockquote",
	"body",
	"br",
	"button",
	"canvas",
	"caption",
	"cite",
	"code",
	"col",
	"colgroup",
	"data",
	"datalist",
	"dd",
	"del",
	"details",
	"dfn",
	"dialog",
	"div",
	"dl",
	"dt",
	"em",
	"embed",
	"fieldset",
	"figcaption",
	"figure",
	"footer",
	"form",
	"h1",
	"h2",
	"h3",
	"h4",
	"h5",
	"h6",
	"head",
	"header",
	"hgroup",
	"hr",
	"html",
	"i",
	"iframe",
	"img",
	"input",
	"ins",
	"kbd",
	"keygen",
	"label",
	"legend",
	"li",
	"link",
	"main",
	"map",
	"mark",
	"menu",
	"menuitem",
	"meta",
	"meter",
	"nav",
	"noscript",
	"object",
	"ol",
	"optgroup",
	"option",
	"output",
	"p",
	"param",
	"pre",
	"progress",
	"q",
	"rb",
	"rp",
	"rt",
	"rtc",
	"ruby",
	"s",
	"samp",
	"script",
	"section",
	"select",
	"small",
	"source",
	"span",
	"strong",
	"style",
	"sub",
	"summary",
	"sup",
	"table",
	"tbody",
	"td",
	"template",
	"textarea",
	"tfoot",
	"th",
	"thead",
	"time",
	"title",
	"tr",
	"track",
	"u",
	"ul",
	"var",
	"video",
	"wbr"
};
#endif

void lexer_init(char *input)
{
	assert(input);

	in = input;
	pos = input;
}

enum ttype lexer_gettoken(char **pval)
{
	assert(NULL == *pval);

	int tkn_start = 0;
	int tkn_len = 0;

	SEEK_TO_NONBLANK;

	if ('%' == *pos) {
		/* skip percent symbol */
		pos++;

		TOKEN_START;
		SEEK_TO_EOW;
		TOKEN_VALUE;
		TOKEN(TAG);
	} else if ('.' == *pos) {
		/* skip dot symbol */
		pos++;

		TOKEN_START;
		SEEK_TO_EO_CSS_NAME;
		TOKEN_VALUE;
		TOKEN(CLASS);
	} else if ('#' == *pos) {
		/* skip sharp symbol */
		pos++;

		TOKEN_START;
		SEEK_TO_EO_CSS_NAME;
		TOKEN_VALUE;
		TOKEN(ID);
	} else if ('"' == *pos) {
		/* skip open qoute */
		pos++;

		TOKEN_START;
		SEEK_BEFORE_CLOSE_QT;
		TOKEN_VALUE;

		/* skip close quote */
		pos++;
		TOKEN(CDATA);
	} else if ('=' == *pos) {
		TOKEN_START;
		pos++;
		TOKEN(EQL);
	} else if ('\t' == *pos) {
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
	} else if (isalpha(*pos)){
		TOKEN_START;
		SEEK_TO_EOW;
		TOKEN_VALUE;
		TOKEN(ATTR);
	} else if ('\0' == *pos){
		TOKEN(EOF);
	}
	TOKEN_START;
	pos++;
	TOKEN(ERROR);
}