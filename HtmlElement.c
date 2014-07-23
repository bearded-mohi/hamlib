#include "HtmlElement.h"
#include "list.h"
#include <stdlib.h>
#include <string.h>


#define DEFAULT_TAG    "div"
#define DEFAULT_TAG_LEN 3


HtmlElement *HtmlElement_new()
{
	HtmlElement *self = calloc(1, sizeof(HtmlElement));
	self->tag = malloc(sizeof(char) * DEFAULT_TAG_LEN + 1);
	memcpy(self->tag, DEFAULT_TAG, DEFAULT_TAG_LEN);
	self->tag[DEFAULT_TAG_LEN] = '\0';
	return self;
}

void HtmlElement_add_class(HtmlElement *self, char *class_name)
{
	list_insert(&self->classes, class_name, 0);
}

void HtmlElement_add_attribute(HtmlElement *self, HtmlAttribute *attribute)
{
	list_insert(&self->attributes, attribute, 0);
}

void HtmlElement_delete(HtmlElement *self)
{
	if (self->tag)
		free(self->tag);
	if (self->id)
		free(self->id);
	if (self->text)
		free(self->text);
	if (self->classes)
		list_delete(&self->classes, (void (*)(void *)) NULL);
	if (self->attributes)
		list_delete(&self->attributes, (void (*)(void *)) HtmlAttribute_delete);

	free(self);
}