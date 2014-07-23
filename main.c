#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "parser.h"
#include "using.h"

static void read_from_stream(FILE *stm)
{
	assert(stm);

	#define BUF_SIZE 1024

	char   buf[BUF_SIZE];
	char  *input      = malloc(sizeof(char) * BUF_SIZE);
	size_t input_size = 1;

	input[0] = buf[0] = '\0';
	while (fgets(buf, BUF_SIZE, stm)) {
		input_size += strlen(buf);
		input = realloc(input, input_size);
		strcat(input, buf);
	}
	
	lexer_init(input);
	parse();
	free(input);
}

int main(int argc, char **argv)
{
	if (1 == argc) {
		read_from_stream(stdin);
	} else {
		printf("%s\n", USING);
		exit(EXIT_FAILURE);
	}
}
