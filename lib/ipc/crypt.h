#ifndef __KIC_CRYPT_H
#define __KIC_CRYPT_H

#include <stdio.h>

extern char *kic_crypt_dir;

unsigned int kic_timestamp();

char *kic_generate_path(char *prefix, char *ext);
int kic_file_write(char *path, void *data, size_t size);
int kic_file_read(char *path, void **data, size_t *size);

int kic_sign(void **data, size_t *size);
int kic_unsign(void **data, size_t *size);
int kic_encrypt(char *reciepient, void **data, size_t *size);
int kic_decrypt(void **data, size_t *size);
int kic_export_key(char *user, void **data, size_t *size);
int kic_import_key(void **data, size_t *size);

#endif
