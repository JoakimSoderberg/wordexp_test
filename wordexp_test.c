
#include <stdio.h>
#include <stdlib.h>
#include <wordexp.h>
#include <assert.h>
#include <string.h>

static void xfree(void *p)
{
	void **pp;
	assert(p);

	pp = (void **)p;

	if (*pp)
	{
		free(*pp);
		*pp = NULL;
	}
}

char **test_wordexp(const char *cmdline, int *argc)
{
	char **argv = NULL;
	int i;
	int ret;
	wordexp_t p;
	memset(&p, 0, sizeof(p));

	// Note! This expands shell variables.
	if ((ret = wordexp(cmdline, &p, 0)))
	{
		fprintf(stderr, "wordexp error %d: '%s'\n", ret, cmdline);
		return NULL;
	}

	*argc = p.we_wordc;

	if (!(argv = calloc(*argc, sizeof(char *))))
	{
		fprintf(stderr, "Out of memory!\n");
		goto fail;
	}

	for (i = 0; i < p.we_wordc; i++)
	{
		if (!(argv[i] = strdup(p.we_wordv[i])))
		{
			fprintf(stderr, "Out of memory!\n");
			goto fail;
		}
	}

	wordfree(&p);

	return argv;

fail:
	wordfree(&p);

	if (argv)
	{
		for (i = 0; i < *argc; i++)
		{
			xfree(&argv[i]);
		}

		free(argv);
	}

	return NULL;
}

void wordexp_free(char ***argv, int argc)
{
	size_t i;
	assert(argv);

	if (*argv)
	{
		for (i = 0; i < (size_t)argc; i++)
		{
			xfree(&((*argv)[i]));
		}

		xfree(argv);
	}
}

int main(int argc, char **argv)
{
	const char *cmd = "abc def \"ghi jkl\"";
	int count = 0;
	char **args = NULL;
	int i;

	args = test_wordexp(cmd, &count);

	for (i = 0; i < count; i++)
	{
		printf("%s\n", args[i]);
	}

	wordexp_free(&args, count);

	return 0;
}
