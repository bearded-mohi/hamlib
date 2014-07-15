#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED


typedef struct list list;


void list_insert(list **self, void *pval, int pos);

void list_reverse(list **self);

void list_foreach(list *self, void (*action)(void *));

void list_delete(list **self, void (*dctor)(void *));

#endif