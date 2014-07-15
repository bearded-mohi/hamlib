#ifndef HTML_ELEMENT_H_INCLUDED
#define HTML_ELEMENT_H_INCLUDED


#include "HtmlAttribute.h"
#include <stdbool.h>

typedef struct list list;

typedef struct {
	int   indent;
	char *tag;
	char *id;	
	char *text;
	list *attributes;
	list *classes;
	bool  _is_closed;
} 
HtmlElement;


HtmlElement *HtmlElement_new();

void HtmlElement_add_class(HtmlElement *self, char *class_name);

void HtmlElement_add_attribute(HtmlElement *self, HtmlAttribute *attribute);

void HtmlElement_delete(HtmlElement *self);


#endif