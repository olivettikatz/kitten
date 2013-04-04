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

FILE *tse_stream_file_get(tse_stream *s)
{
	if (s == NULL)
		return NULL;
	return s->desc;
}

FILE *tse_stream_file_set(tse_stream *s, FILE *f)
{
	if (s == NULL)
		return NULL;
	return (s->desc = f);
}

size_t tse_stream_bufsize_get(tse_stream *s)
{
	if (s == NULL)
		return 0;
	return s->bufsize;
}

char *tse_stream_buffer_pull(tse_stream *s)
{
	if (s == NULL)
		return NULL;
	return tse_memstream_pull(s->desc, &s->buf, &s->bufsize);
}

int tse_stream_to_region(tse_stream *s, tse_region *r)
{
	char *buf = tse_stream_buffer_pull(s);
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
			if (c >= r->w-3)
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

		if (l >= r->h-2)
		{
			for (int j = 1; j < r->h-2; j++)
			{
				memcpy(tmp[j-1], tmp[j], sizeof(char)*(r->w-3));
			}

			l--;
			memset(tmp[l], ' ', r->w-3);
		}
	}

	for (int i = 0; i < r->h-2; i++)
	{
		tse_moveabs(r->x+1, r->y+1+i);
		tse_puts(tmp[i]);
	}

	return 0;
}

