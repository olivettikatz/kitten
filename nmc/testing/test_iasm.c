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

	token *toks = lex(buf);
	if (toks == NULL)
		return 3;

	return 0;
}
