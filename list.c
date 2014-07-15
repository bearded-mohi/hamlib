#include "list.h"
#include <assert.h>
#include <stdlib.h>


struct list {
	struct list *next;
	void *pval;
};


void
list_insert(list **self, void *pval, int pos) {
	list *newbie = calloc(1, sizeof(list));
	newbie->pval = pval;
	newbie->next = NULL;
	list *ctx = *self;
	if(*self) {			
		if(pos) {
			pos--;
			while(pos--) ctx = ctx->next;
			newbie->next = ctx->next;
			ctx->next = newbie;
		} else {
			newbie->next = *self;
			*self = newbie;
		}
	} else {
		*self = newbie;
	}
}

void
list_reverse(list **self) {
	assert(*self);

	list *cur, *frwd, *loop;

	cur = *self;
	frwd = (*self)->next;
	(*self)->next = NULL;

	while(frwd) {
		loop = frwd->next;;
		frwd->next = cur;
		cur = frwd;
		frwd = loop;
	}
	*self = cur;
}

void
list_foreach(list *self, void (*action)(void *)) {
	assert(self);

	do {
		action(self->pval);
	} while((self = self->next));
}

void
list_delete(list **self, void (*dctor)(void *)) {
	assert(*self);

	// TODO: implement
}