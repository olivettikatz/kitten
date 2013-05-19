#ifndef __KIC_NODE_H
#define __KIC_NODE_H

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <malloc.h>
#include <unistd.h>
#include <time.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>

#define KIC_SERVER_BUFFER_SIZE_DEFAULT 1024

extern char *kic_index;

typedef int (*kic_callback_recieve)(void *data, size_t size, kic_node *node, void *other);

typedef struct
{
	char *ip;
	char *name;

	int server_sockfd;
	int server_sockfdtmp;
	int server_port;
	struct sockaddr_in server_addr_server, server_addr_client;
	kic_callback_recieve server_handler;
	void *server_handler_argument;
	size_t server_buffer_size;
} kic_node;

kic_node kic_node_create(char *n, kic_callback_recieve h);
void kic_node_destroy(kic_node *n);
void kic_node_buffer_size_set(kic_node *n, size_t s);
void kic_node_handler_argument_set(kic_node *n, void *a);
char *kic_node_ip_get(kic_node *n);
char *kic_node_name_get(kic_node *n);
int kic_node_port_get(kic_node *n);
int kic_node_main(kic_node *n);
kic_node *kic_list();

int kic_node_respond(kic_node *n, void *data, size_t size);
int kic_node_send(kic_node *from, kic_node *to, void *data, size_t *size);

#endif
