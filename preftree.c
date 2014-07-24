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

void get_group(int str_pos, int inner_group_start, int outer_group_end, int *inner_group_end, char *ch)
{
	*ch = lang[inner_group_start][str_pos];
	
	for (int i = inner_group_start; i <= outer_group_end; i++) {
		if (lang[i][str_pos] != *ch) {
			*inner_group_end = i - 1;
			return;
		}
	}
	*inner_group_end = outer_group_end;
}

void build_tree(struct tree *root, int group_start, int group_end, int str_pos)
{
	int  gstart = group_start;
	int  gend = group_end;
	char gchar;
	do {
		get_group(str_pos, gstart, group_end, &gend, &gchar);
		if (gchar) {
			struct tree *child = aralloc();
			root->children[gchar - 97] = child;
			build_tree(child, gstart, gend, str_pos + 1);
		} else {
			root->token = gstart;			
		}
		gstart = gend + 1;
	} while(gend != group_end);
}


void init()
{
	troot = aralloc();
	build_tree(troot, 1, 5, 0);
}

void incsearch(char ch)
{	
	if (tcur->children[ch - 97]) {
		tcur = tcur->children[ch - 97];
	} else {
		tcur->token = UNKNOWN;
	}	
}

void test(char *str, enum lang_token expected) {
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

int main(int argc, char **argv)
{
	init();
	
	test("a", A);
	test("abbr", ABBR);
	test("address", ADDRESS);
	test("head", HEAD);
	test("header", HEADER);
}