#include "stream.h"

FILE *open_memstream(char **ptr, size_t *sizeloc);

tse_stream tse_stream_new()
{
	tse_stream tmp;
	tmp.desc = tse_memstream_new(&tmp.buf, &tmp.bufsize);
	return tmp;
}

tse_stream tse_stream_new_from(FILE *f)
{
	tse_stream tmp;
	tmp.desc = f;
	tmp.buf = NULL;
	tmp.bufsize = 0;
	return tmp;
}

int tse_stream_del(tse_stream *s)
{
	if (s == NULL)
		return 1;

	if (s->buf != NULL)
		tse_memstream_del(s->desc);

	return 0;
}

int tse_stream_clear(tse_stream *s)
{
	if (s == NULL)
		return 1;

	tse_memstream_del(s->desc);
	s->desc = tse_memstream_new(&s->buf, &s->bufsize);
	return 0;
}

char *tse_stream_read(tse_stream *s)
{
	if (s == NULL)
		return NULL;

	fflush(s->desc);
	return s->buf;
}

int tse_stream_to_region(tse_stream *s, tse_region *r)
{
	char *buf = tse_stream_read(s);
	if (buf == NULL)
		return 1;

	int c = 0;
	int l = 0;
	char **tmp = (char **)malloc(sizeof(char *)*(r->h-2));
	for (int i = 0; i < r->h-2; i++)
	{
		tmp[i] = (char *)malloc(sizeof(char)*(r->w-3));
		memset(tmp[i], ' ', r->w-3);
	}

	for (int i = 0; buf[i] != 0; i++)
	{
		if (buf[i] == '\n')
		{
			c = 0;
			l++;
		}
		else
		{
			if (c >= r->w-1-r->border*2)
			{
				c = 0;
				l++;
				tmp[l][c++] = buf[i];
			}
			else
			{
				tmp[l][c++] = buf[i];
			}
		}

		if (l >= r->h-r->border*2)
		{
			for (int j = 1; j < r->h-r->border*2; j++)
			{
				memcpy(tmp[j-1], tmp[j], sizeof(char)*(r->w-1-r->border*2));
			}

			l--;
			memset(tmp[l], ' ', r->w-1-r->border*2);
		}
	}

	for (int i = 0; i < r->h-r->border*2; i++)
	{
		tse_moveabs(r->x+r->border, r->y+r->border+i);
		tse_puts(tmp[i]);
	}

	return 0;
}

