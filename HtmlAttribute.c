#include "HtmlAttribute.h"
#include <stdlib.h>

HtmlAttribute *
HtmlAttribute_new() {
	return calloc(1, sizeof(HtmlAttribute));
}

void
HtmlAttribute_delete(HtmlAttribute *self) {
	if (self->name) free(self->name);
	if (self->val) free(self->val);
}