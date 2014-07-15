#ifndef HTML_ATTRIBUTE_H_INCLUDED
#define HTML_ATTRIBUTE_H_INCLUDED


typedef struct {
	char *name;
	char *val;
}
HtmlAttribute;


HtmlAttribute *HtmlAttribute_new();

void HtmlAttribute_delete(HtmlAttribute *self);


#endif