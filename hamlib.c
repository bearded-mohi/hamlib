#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hamlib.h"

typedef enum {
	TOKEN_EMPTY,		
	TOKEN_SPACE,
	TOKEN_TAB,
	TOKEN_CDATA,
	TOKEN_TAG,
	TOKEN_CLASS,
	TOKEN_ATTR
} token;

struct tag {
	struct tag *next;
	char *str;
};

static struct tag *_list        = NULL;
static int         _prev_indent = 0;
static int         _indent      = 0;

void
list_insert(struct tag **list, char *str, int pos) {
	struct tag *newbie = calloc(1, sizeof(struct tag));
	newbie->str = str;
	newbie->next = NULL;
	struct tag *ctx = *list;
	if(*list) {			
		if(pos) {
			pos--;
			while(pos--) ctx = ctx->next;
			newbie->next = ctx->next;
			ctx->next = newbie;
		} else {
			newbie->next = *list;
			*list = newbie;
		}
	} else {
		*list = newbie;
	}
}

void
list_reverse(struct tag **list) {
	assert(*list);

	struct tag *cur, *frwd, *loop;

	cur = *list;
	frwd = (*list)->next;
	(*list)->next = NULL;

	while(frwd) {
		loop = frwd->next;;
		frwd->next = cur;
		cur = frwd;
		frwd = loop;
	}
	*list = cur;
}

void
list_print(struct tag *list) {
	assert(list);

	do {
		printf("%s\n", list->str);
	} while((list = list->next));
}

void
list_free(struct tag **list) {
	assert(*list);
}

void
tag_put(char *str) {
	assert(str);

	if((_indent > _prev_indent) && ((_indent - _prev_indent) > 1)) {
		printf("wrong indent\n");
	}

	int len = strlen(str);
	char *tclose = malloc((len + 4 + _indent) * sizeof(char));
	char *topen = malloc((len + 3 + _indent) * sizeof(char));
	
	memset(tclose, '\t', _indent);
	memset(topen, '\t', _indent);

	sprintf(tclose + _indent, "</%s>", str);
	sprintf(topen + _indent, "<%s>", str);
	
	tclose[len + 3 + _indent] = '\0';
	topen[len + 2 + _indent] = '\0';

	list_insert(&_list, topen, _indent);
	if((strcmp("meta", str) != 0) && (strcmp("img", str) != 0))
	list_insert(&_list, tclose, _indent);

	_prev_indent = _indent;
	_indent = 0;
}

void
tab_put() {
	_indent++;
}

static void
token_val(token tkn, char *haml, int tkn_start, int tkn_end) {
	assert(haml);
	assert(tkn_end >= tkn_start); 

	switch (tkn) {
		case TOKEN_TAB: {
			tab_put();
			break;
		}			
		case TOKEN_TAG: {
			size_t size = (tkn_end - tkn_start); // skip first symbol %
			char *val = malloc(size * sizeof(char));
			memcpy(val, &haml[tkn_start + 1], size - 1); // skip %
			val[size - 1] = '\0';
			tag_put(val);
			free(val);
			break;
		}
		default:
			break;
	}
}

char *
aparse(char *haml) {
	assert(haml);

	token tkn      = TOKEN_EMPTY;
	int  tkn_start = 0;

	for(int pos = 0; ; pos++) {
		if('\t' == haml[pos]) {
			token_val(tkn, haml, tkn_start, pos);
			tkn = TOKEN_TAB;
			tkn_start = pos;			
		} else if('%' == haml[pos]) {
			token_val(tkn, haml, tkn_start, pos);
			tkn = TOKEN_TAG;
			tkn_start = pos;
		} else if('.' == haml[pos]) {
			if(TOKEN_TAG == tkn) {
				token_val(tkn, haml, tkn_start, pos);
				tkn = TOKEN_CLASS;
				tkn_start = pos;
			}
		} else if((' ' == haml[pos]) || ('\n' == haml[pos])) {
			if(TOKEN_CDATA != tkn) {
				token_val(tkn, haml, tkn_start, pos);
				tkn = TOKEN_SPACE;
				tkn_start = pos;
			}
		} else if('\0' == haml[pos]) {
			token_val(tkn, haml, tkn_start, pos);
			break;
		} else {
			if((TOKEN_TAG != tkn) && (TOKEN_CLASS != tkn) && (TOKEN_CDATA != tkn)) {
				tkn = TOKEN_CDATA;
				tkn_start = pos;
			}
		}
	}

	list_reverse(&_list);
	list_print(_list);
	list_free(&_list);

	return NULL;
}