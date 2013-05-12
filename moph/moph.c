#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#include <tse/tse.h>

char *strtok_r(char *str, const char *delim, char **saveptr);
char *strdup(const char *s);

#define MAX_STATUS_SIZE 128
#define MOPH_SIZE 32
#define MAX_MOPHS 1024

typedef enum
{
	message,
	value,
	percentage
} status_type;

typedef struct
{
	char *moph;
	char *title;
	status_type type;
	char *msg;
	int value;
	double perc;
} status;

typedef struct
{
	char *name;
	status buf[MOPH_SIZE];
	int nbuf;
} moph;

moph db[MAX_MOPHS];
tse_stream mainstream;
int ndb = 0;
int sel = -1;

moph moph_new(char *n)
{
	moph rtn;
	rtn.name = n;
	rtn.nbuf = 0;
	return rtn;
}

int moph_status(moph *m, status *s)
{
	if (m == NULL || s == NULL)
	{
		printf("error: cannot set status of null moph\n");
		sleep(1);
		return 1;
	}

	if (m->nbuf < MOPH_SIZE)
		m->buf[m->nbuf++] = *s;
	return 0;
}

int db_find(char *n)
{
	for (int i = 0; i < ndb; i++)
	{
		if (strcmp(db[i].name, n) == 0)
		{
			return i;
		}
	}

	return -1;
}

int db_append(moph m)
{
	db[ndb++] = m;
	return ndb-1;
}

status parse_status(char *s)
{
	status rtn;
	char *save;
	rtn.moph = strtok_r(s, " ", &save);
	if (rtn.moph == NULL)
	{
		printf("error: illegal status\n");
		sleep(1);
		return rtn;
	}
	rtn.title = strtok_r(NULL, " ", &save);
	if (rtn.title == NULL)
	{
		printf("error: illegal status\n");
		sleep(1);
		rtn.moph = NULL;
		return rtn;
	}

	char *tmp = strtok_r(NULL, " ", &save);
	if (tmp == NULL)
	{
		printf("error: illegal status\n");
		sleep(1);
		rtn.moph = NULL;
		return rtn;
	}

	if (strlen(tmp) == 0)
	{
		rtn.type = message;
		rtn.msg = tmp;
	}
	else if (tmp[strlen(tmp)-1] == '%')
	{
		tmp[strlen(tmp)-1] == 0;
		rtn.type = percentage;
		rtn.perc = atof(tmp);
	}
	else if (isdigit(tmp[0]))
	{
		rtn.type = value;
		rtn.value = atoi(tmp);
	}
	else
	{
		rtn.type = message;
		rtn.msg = tmp;
	}

	return rtn;
}

int status_ok(status *s)
{
	if (s == NULL)
		return 0;
	if (s->moph == NULL)
		return 0;
	return 1;
}

int status_display(status *s, FILE *f)
{
	if (!status_ok(s))
	{
		printf("error: cannot display illegal status\n");
		sleep(1);
		return 1;
	}

	fprintf(f, "[%s] %s ", s->moph, s->title);
	if (s->type == message)
		fprintf(f, "- %s\n", s->msg);
	else if (s->type == value)
		fprintf(f, "%i\n", s->value);
	else if (s->type == percentage)
		fprintf(f, "%.2f%%\n", s->perc);

	return 0;
}

char *read_from_pipe(char *path)
{
	int fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		mkfifo(path, 0666);
		fd = open(path, O_RDONLY);
	}

	if (fd < 0)
	{
		printf("error: cannot open pipe: %s\n", path);
		sleep(1);
		return NULL;
	}

	char buf[MAX_STATUS_SIZE];
	size_t l = read(fd, buf, sizeof(buf));
	close(fd);
	buf[l] = 0;
	if (strchr(buf, '\n'))
		*(strrchr(buf, '\n')) = 0;
	return strdup(buf);
}

int render(int w, int h)
{
	tse_clear();
	tse_region region = tse_region_new(0, 0, w/2, h);
	tse_region_render(&region);
	tse_stream_to_region(&mainstream, &region);

	if (sel != -1)
	{
		tse_region sr = tse_region_new(10, 5, (w-10)/2, (h-5)/2);
		tse_stream ss = tse_stream_new();
		tse_stream_clear(&ss);
		tse_region_render(&sr);
		fprintf(ss.desc, "%i mophs...\n", ndb);
		for (int i = 0; i < ndb; i++)
		{
			fprintf(ss.desc, "%s\n", db[i].name);
		}
		tse_stream_to_region(&ss, &sr);
	}

	return 0;
}

int input(int k)
{
	if (k == 'q')
	{
		return 1;
	}
	else if (k == 's')
	{
		if (sel == -1)
			sel = 0;
		else
			sel = -1;
		return 0;
	}
	else
	{
		return 0;
	}
}

int update()
{
	char *tmp = read_from_pipe("tmp.pipe");
	status s = parse_status(tmp);
	status_display(&s, mainstream.desc);
	if (db_find(s.moph) == -1)
	{
		moph tmp = moph_new(s.moph);
		moph_status(&tmp, &s);
		db_append(tmp);
	}

	return 1000000;
}

int main()
{
	tse_raw_input_enable();

	mainstream = tse_stream_new();
	tse_stream_clear(&mainstream);

	return tse_async(&render, &update, &input);
}

