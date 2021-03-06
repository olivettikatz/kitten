#include "crypt.h"

unsigned int kic_timestamp()
{
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return time.tv_sec*1000+time.tv_nsec/1000;
}

char *kic_generate_path(char *prefix, char *ext)
{
	char *path = (char *)malloc(128);
	sprintf(path, "%s/%s_%i.%s", kic_crypt_dir, prefix, getpid(), ext);
	return path;
}

int kic_file_write(char *path, void *data, size_t size)
{
	FILE *f = fopen(path, "w");
	if (f == NULL)
		return 1;

	if (fwrite(data, 1, size, f) < size)
		return 2;

	fclose(f);
	return 0;
}

int kic_file_read(char *path, void **data, size_t *size)
{
	FILE *f = fopen(path, "r");
	if (f == NULL)
		return 1;

	fseek(f, SEEK_END, 0);
	*size = ftell(f);
	fseek(f, SEEK_SET, 0);

	*data = malloc(*size);
	if (*data == NULL)
		return 2;

	if (fread(*data, 1, *size, f) < *size)
		return 3;

	fclose(f);
	return 0;
}

int kic_sign(void **data, size_t *size)
{
	char *path = kic_generate_path("singee", "txt");
	if (kic_file_write(path, *data, *size))
		return 1;

	char *outpath = kic_generate_path("signee", "bin");

	char *args[] = {
		"gpg",
		"--output",
		outpath,
		"--sign",
		path,
		NULL
	};

	if (execvp(args[0], args) != 0)
		return 2;

	free(*data);
	if (kic_file_read(outpath, data, size))
		return 3;

	return 0;
}

int kic_unsign(void **data, size_t *size)
{
	char *path = kic_generate_path("unsignee", "bin");
	if (kic_file_write(path, *data, *size))
		return 1;

	char *outpath = kic_generate_path("unsignee", "txt");

	char *args[] = {
		"gpg",
		"--output",
		outpath,
		"--decrypt",
		path,
		NULL
	};

	if (execvp(args[0], args) != 0)
		return 2;

	free(*data);
	if (kic_file_read(outpath, data, size))
		return 3;

	return 0;
}

int kic_encrypt(char *recipient, void **data, size_t *size)
{
	char *path = kic_generate_path("encryptee", "txt");
	if (kic_file_write(path, *data, *size))
		return 1;

	char *outpath = kic_generate_path("encryptee", "bin");

	char *args[] = {
		"gpg",
		"--output",
		outpath,
		"--encrypt",
		"--recipient",
		recipient,
		path,
		NULL
	};

	if (execvp(args[0], args) != 0)
		return 2;

	free(*data);
	if (kic_file_read(outpath, data, size))
		return 3;

	return 0;
}

int kic_decrypt(void **data, size_t *size)
{
	char *path = kic_generate_path("decryptee", "bin");
	if (kic_file_write(path, *data, *size))
		return 1;

	char *outpath = kic_generate_path("decryptee", "txt");

	char *args[] = {
		"gpg",
		"--output",
		outpath,
		"--decrypt",
		path,
		NULL
	};

	if (execvp(args[0], args) != 0)
		return 2;

	free(*data);
	if (kic_file_read(outpath, data, size))
		return 3;

	return 0;
}

int kic_export_key(char *user, void **data, size_t *size)
{
	char *outpath = kic_generate_path("exportee", "bin");
	char *args[] = {
		"gpg",
		"--output",
		outpath,
		"--armor",
		"--export",
		user,
		NULL
	};

	if (execvp(args[0], args) != 0)
		return 1;

	if (kic_file_read(outpath, data, size))
		return 2;

	return 0;
}

int kic_import_key(void **data, size_t *size)
{
	char *outpath = kic_generate_path("importee", "bin");

	if (kic_write_file(outpath, *data, *size))
		return 1;

	char *args[] = {
		"gpg",
		"--import",
		outpath,
		NULL
	};

	if (execvp(args[0], args) != 0)
		return 2;

	return 0;
}

