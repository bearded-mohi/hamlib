#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hamlib.h"

struct tag {
	struct tag *next;
	char *str;
};

static struct tag *_list        = NULL;
static int         _prev_indent = 0;
static int         _indent      = 0;

void list_insert(struct tag **list, char *str, int pos) {
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

void list_reverse(struct tag **list) {
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

void list_print(struct tag *list) {
	do {
		printf("%s\n", list->str);
	} while((list = list->next));
}

void list_free(struct tag **list) {

}

void tag_put(char *str) {
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

void tab_put() {
	_indent++;
}

void aparse(char *haml) {
	tag_put("html");
	tab_put();tag_put("head");	
	tab_put();tab_put();tag_put("meta");
	tab_put();tab_put();tag_put("title");
	tab_put();tab_put();tag_put("script");
	tab_put();tag_put("body");	
	tab_put();tab_put();tag_put("h1");
	tab_put();tab_put();tag_put("p");
	tab_put();tab_put();tab_put();tag_put("img");
	tab_put();tab_put();tab_put();tag_put("span");
	tab_put();tab_put();tag_put("p");

	list_reverse(&_list);
	list_print(_list);
	list_free(&_list);
}






