#include "generator.h"
#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static list *_elements;

static void
print_indent(HtmlElement *el) {
	for(int i = 0; i < el->indent; i++) {
		printf("\t");
	}	
}

static void
print_id(HtmlElement *el) {
	if (el->id) {
		printf(" id=\"%s\"", el->id);
	}
}

static void
print_single_class(void *class_name) {
	printf("%s ", (char *)class_name);
}

static void
print_classes(HtmlElement *el) {
	if (el->classes) {
		list_reverse(&el->classes);
		printf(" class=\"");
		list_foreach(el->classes, print_single_class);
		printf("\"");
	}
}

static void
print_single_attribute(void *attr) {
	HtmlAttribute *attribute = attr;
	printf(" %s=%s", attribute->name, attribute->val);
}

static void
print_attributes(HtmlElement *el) {
	if(el->attributes) {
		list_reverse(&el->attributes);
		list_foreach(el->attributes, print_single_attribute);
	}
}

static void
print_text(HtmlElement *el) {
	printf("\n");
	if (el->text) {
		print_indent(el);
		printf("\t%s\n", el->text);
	}
}

static void
print_element(void *el) {
	HtmlElement *html_el = el;

	print_indent(html_el);

	if (0 == strcmp("_text", html_el->tag)) {
		printf("%s\n", html_el->text);
	} else if (html_el->_is_closed) {
		printf("</%s>\n", html_el->tag);
	} else {
		printf("<%s", html_el->tag);
		print_id(html_el);
		print_classes(html_el);		
		print_attributes(html_el);
		printf(">");
		print_text(html_el);
	}
}

static bool
is_pair(HtmlElement *el) {
	/* TODO: use enum for element_type */
	if (0 == strcmp("_text", el->tag)) {
		return false;
	}
	return true;
}

void
generator_add_element(HtmlElement *el) {
	/* open tag */
	list_insert(&_elements, el, el->indent);
	if (is_pair(el)) {
		/* close tag */
		HtmlElement *tclose = calloc(1, sizeof(HtmlElement));
		tclose->tag = el->tag;
		tclose->indent = el->indent;
		tclose->_is_closed = true;
		list_insert(&_elements, tclose, tclose->indent);
	}
}

void
generator_get_result() {
	list_reverse(&_elements);
	list_foreach(_elements, print_element);
	list_delete(&_elements, (void (*)(void *)) HtmlElement_delete);
}