#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

char *lang[] = {
	"_unknown_",
	"a",
	"abbr",
	"address",
	"head",
	"header"
};

enum lang_token {
	UNKNOWN,
	A,
	ABBR,
	ADDRESS,
	HEAD,
	HEADER
};

struct tree {
	enum lang_token token;
	struct tree    *children[22];
	int             selfp;
};

static struct tree _mem[1000];
static int         _memtop;

static struct tree *aralloc()
{
	_mem[_memtop].selfp = _memtop++;
	return &_mem[_memtop];
}


static struct tree *troot, *tcur;

static void get_group(int h,
                      int inner_group_start,
                      int outer_group_end,
                      int *inner_group_end,
                      char *ch)
{
	*ch = lang[inner_group_start][h];
	for (int i = inner_group_start; i <= outer_group_end; i++) {
		if (lang[i][h] != *ch) {
			*inner_group_end = i - 1;
			return;
		}
	}
	*inner_group_end = outer_group_end;
}

static void preftree(struct tree *root, int group_start, int group_end, int h)
{
	int  gstart = group_start;
	int  gend = group_end;
	char gchar;
	do {
		get_group(h, gstart, group_end, &gend, &gchar);
		if (gchar) {
			struct tree *child = aralloc();
			root->children[gchar - 97] = child;
			preftree(child, gstart, gend, h + 1);
		} else {
			root->token = gstart;
		}
		gstart = gend + 1;
	} while(gend != group_end);
}


static void init()
{
	troot = aralloc();
	preftree(troot, 1, 5, 0);
}

static void incsearch(char ch)
{
	if (tcur->children[ch - 97]) {
		tcur = tcur->children[ch - 97];
	} else {
		/* TODO: dont touch tree! */
		tcur->token = UNKNOWN;
	}
}

static void test(char *str, enum lang_token expected)
{
	char *orig = str;
	tcur = troot;
	do {
		incsearch(*str);
	} while (*++str);
	if (expected == tcur->token) {
		printf("ACCEPTED: %s\n", orig);
	} else {
		printf("FAILED: %s\n", orig);
	}
}

static void printtree(struct tree *root)
{
	printf("\t{ tkn = %s");
}

static void printdefs(struct tree *root)
{
	int   len = sizeof(lang) / sizeof(lang[0]);
	char *s;
	printf("enum token {\n");
	for (int i = 1; i < len; i++) {
		printf("\tTKN_");
		s = lang[i];
		do { putchar(toupper(*s)); } while (*++s);
		printf((i != len - 1) ? ",\n" : "\n");
	}
	printf("};\n");
	printf("struct tree { enum token tkn; int lchild; int rsibling; };\n");
	printf("struct tree _mem[] = {\n");
	printtree(root);
	printf("};");
}

int main(int argc, char **argv)
{
	init();
	test("a", A);
	test("abbr", ABBR);
	test("address", ADDRESS);
	test("head", HEAD);
	test("header", HEADER);
	printdefs(troot);
}