#include "encoding.h"

char *read_ascii(char *path)
{
	FILE *f = fopen(path, "r");
	if (f == NULL)
	{
		interr("cannot open file for reading");
		intext_str("path", path);
		intkill();
		return NULL;
	}
	
	fseek(f, 0, SEEK_END);
	size_t l = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (l == 0)
	{
		fclose(f);
		interr("no point compiling an empty file");
		intext_str("path", path);
		intkill();
		return NULL;
	}

	char *buf = (char *)malloc(l);
	if (buf == NULL)
	{
		fclose(f);
		interr("cannot allocate memory for file");
		intext_str("path", path);
		intext_int("size", l);
		intkill();
		return NULL;
	}

	memset(buf, 0, l);
	if (fread(buf, 1, l, f) < l)
	{
		fclose(f);
		interr("estimated file size is greater than actual file size (piping problems?)");
		intext_str("path", path);
		intkill();
		return NULL;
	}

	fclose(f);
	return buf;
}

int check_utf8(char *buf)
{
	int col = 0;
	int line = 0;

	for (int i = 0; buf[i] != 0; i++)
	{
		if (buf[i] > 0x7f)
		{
			interr("illegal non-ascii character used (limit: 0x7f)");
			intext_int("line", line);
			intext_int("column", col);
			intext_hex("hexcode", buf[i]);
			intkill();
			return 1;
		}

		if (buf[i] == '\n')
		{
			col = 0;
			line++;
		}
		else
		{
			col++;
		}
	}
}

char *read_utf8(char *path)
{
	FILE *f = fopen(path, "r");
	if (f == NULL)
	{
		interr("cannot open file for reading");
		intext_str("path", path);
		intkill();
		return NULL;
	}
	
	fseek(f, 0, SEEK_END);
	size_t l = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (l == 0)
	{
		fclose(f);
		interr("no point compiling an empty file");
		intext_str("path", path);
		intkill();
		return NULL;
	}

	char *buf = (char *)malloc(l);
	if (buf == NULL)
	{
		fclose(f);
		interr("cannot allocate memory for file");
		intext_str("path", path);
		intext_int("size", l);
		intkill();
		return NULL;
	}

	memset(buf, 0, l);
	if (fread(buf, 1, l, f) < l)
	{
		fclose(f);
		interr("estimated file size is greater than actual file size (piping problems?)");
		intext_str("path", path);
		intkill();
		return NULL;
	}

	fclose(f);
	buf[l] = 0;

	if (check_utf8(buf))
		return NULL;

	return buf;
}

char *read_utf16(char *path)
{
	FILE *f = fopen(path, "r");
	if (f == NULL)
	{
		interr("cannot open file for reading");
		intext_str("path", path);
		intkill();
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	size_t l = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (l == 0)
	{
		fclose(f);
		interr("no point compiling an empty file");
		intext_str("path", path);
		intkill();
		return NULL;
	}

	if (l%2 != 0)
	{
		fclose(f);
		interr("utf-16 file has unaligned size");
		intext_str("path", path);
		intext_int("size", l);
		intkill();
		return NULL;
	}

	short *buf = (short *)malloc(l);
	if (buf == NULL)
	{
		fclose(f);
		interr("cannot allocate memory for file");
		intext_str("path", path);
		intext_int("size", l);
		intkill();
		return NULL;
	}

	memset(buf, 0, l);
	if (fread(buf, 2, l*0.5, f) < l)
	{
		fclose(f);
		interr("estimated file size is greater than actual file size (piping problems?)");
		intext_str("path", path);
		intkill();
		return NULL;
	}

	char *nbuf = (char *)malloc(l*0.5);
	memset(nbuf, 0, l*0.5);

	for (int i = 0; i < l*0.5; i++)
		nbuf[l] = (char)buf[l];

	if (check_utf8(nbuf))
		return NULL;

	fclose(f);
	return nbuf;
}

char *read_utf32(char *path)
{
	FILE *f = fopen(path, "r");
	if (f == NULL)
	{
		interr("cannot open file for reading");
		intext_str("path", path);
		intkill();
		return NULL;
	}

	fseek(f, 0, SEEK_END);
	size_t l = ftell(f);
	fseek(f, 0, SEEK_SET);

	if (l == 0)
	{
		fclose(f);
		interr("no point compiling an empty file");
		intext_str("path", path);
		intkill();
		return NULL;
	}

	if (l%4 != 0)
	{
		fclose(f);
		interr("utf-32 file has unaligned size");
		intext_str("path", path);
		intext_int("size", l);
		intkill();
		return NULL;
	}

	int *buf = (int *)malloc(l);
	if (buf == NULL)
	{
		fclose(f);
		interr("cannot allocate memory for file");
		intext_str("path", path);
		intext_int("size", l);
		intkill();
		return NULL;
	}

	memset(buf, 0, l);
	if (fread(buf, 4, l*0.25, f) < l)
	{
		fclose(f);
		interr("estimated file size is greater than actual file size (piping problems?)");
		intext_str("path", path);
		intkill();
		return NULL;
	}

	char *nbuf = (char *)malloc(l*0.25);
	memset(nbuf, 0, l*0.25);

	for (int i = 0; i < l*0.25; i++)
		nbuf[l] = (char)buf[l];

	if (check_utf8(nbuf))
		return NULL;

	fclose(f);
	return nbuf;
}

char *read_encoded(char *path, int enc)
{
	if (enc == ASCII)
		return read_ascii(path);
	else if (enc == UTF8)
		return read_utf8(path);
	else if (enc == UTF16)
		return read_utf16(path);
	else if (enc == UTF32)
		return read_utf32(path);
	else
		return NULL;
}

