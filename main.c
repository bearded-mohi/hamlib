#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "hamlib.h"
#include "lexer.h"


int main(int p, char **args) {
	lex("%head#some.b-head title=\"lala\"\n\t.b-menu");
}
