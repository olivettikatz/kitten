#ifndef __SOUNDING_CRYPT_H
#define __SOUNDING_CRYPT_H

#include <stdio.h>

extern char *sounding_crypt_dir;

unsigned int sounding_timestamp();

char *sounding_generate_path(char *prefix, char *ext);
int sounding_file_write(char *path, void *data, size_t size);
int sounding_file_read(char *path, void **data, size_t *size);

int sounding_sign(void **data, size_t *size);
int sounding_unsign(void **data, size_t *size);
int sounding_encrypt(char *reciepient, void **data, size_t *size);
int sounding_decrypt(void **data, size_t *size);
int sounding_export_key(char *user, void **data, size_t *size);
int sounding_import_key(void **data, size_t *size);

#endif
