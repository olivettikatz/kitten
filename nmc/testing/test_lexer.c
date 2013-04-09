#include "../data/encoding.h"
#include "../parsing/lex.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("error: usage: %s <INPUT FILE>\n", argv[0]);
		return 1;
	}

	char *buf = read_encoded(argv[1], UTF8);
	if (buf == NULL)
		return 2;

	printf("\"%s\"\n", buf);

	token *toks = lex(buf);
	if (toks == NULL)
		return 3;

	for (int i = 0; toks[i].str != NULL; i++)
		printf("'%s' ", strndup(toks[i].str, toks[i].strsize));
	printf("\n");

	return 0;
}
