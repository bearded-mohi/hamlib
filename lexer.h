#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

enum ttype {
	TOKEN_ERROR,
	TOKEN_EOF,
	TOKEN_TAG,
	TOKEN_ID,
	TOKEN_CLASS,
	TOKEN_ATTR,
	TOKEN_EQL,
	TOKEN_CDATA,
	TOKEN_LBRACKET,
	TOKEN_RBRACKET,
	TOKEN_INDENT	
};

extern char *ttype2str[];

void lexer_init(char *in);

void lexer_getposition(int *line, int *column);

enum ttype lexer_gettoken(char **pval);

#endif