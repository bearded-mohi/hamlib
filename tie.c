#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

struct tie {
	char c;
	int val;              /* zero is not valid value */
	struct tie *lchild;
	struct tie *rsibling;
};

/*
 * Add new key-value pair. Zero is not valid value
 */
static void tie_add(struct tie *root, char *key, int val)
{
	assert(root);
	assert(key);
	assert(val);

	struct tie *p = root;
	struct tie *t = NULL;
	do {
		if (!p->lchild) {
			p->lchild = calloc(1, sizeof(struct tie));
			p->lchild->c = *key;

			p = p->lchild;
			continue;
		}

		/* find the child who match current char */
		for(t = p->lchild; t; t = t->rsibling) {
			if (t->c == *key) {
				break;
			}
		}

		/* now t is pointer to this child or NULL
		 * if there is not such child */
		if (!t) {
			t = calloc(1, sizeof *t);
			t->c = *key;
			/* insert in second position */
			t->rsibling = p->lchild->rsibling;
			p->lchild->rsibling = t;
		}
		p = t;
	} while (*++key);
	p->val = val;
}

/*
 * Get value by key. Return zero if nothing found
 */
static int tie_get(struct tie *root, char *key)
{
	struct tie *p = root;
	struct tie *t = NULL;
	do {
		for (t = p->lchild; t; t = t->rsibling) {
			if (t->c == *key) break;
		}
		if (t) {
			p = t;
		} else {
			return 0;
		}
	} while (*++key);
	return p->val;
}

int main(int argc, char **argv)
{
	struct tie *root = calloc(1, sizeof *root);

	tie_add(root, "a",       10);
	tie_add(root, "abbr",    20);
	tie_add(root, "div",     30);
	tie_add(root, "address", 40);

	assert(10 == tie_get(root, "a"));
	assert(30 == tie_get(root, "div"));
	assert(20 == tie_get(root, "abbr"));
	assert(40 == tie_get(root, "address"));
	assert(0 == tie_get(root, "abb"));
}