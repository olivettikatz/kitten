#ifndef __SOUNDING_NODE_H
#define __SOUNDING_NODE_H

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

#define SOUNDING_SERVER_BUFFER_SIZE_DEFAULT 1024

extern char *sounding_index;

typedef int (*sounding_callback_recieve)(void *data, size_t size, sounding_node *node, void *other);

typedef struct
{
	char *ip;
	char *name;

	int server_sockfd;
	int server_sockfdtmp;
	int server_port;
	struct sockaddr_in server_addr_server, server_addr_client;
	sounding_callback_recieve server_handler;
	void *server_handler_argument;
	size_t server_buffer_size;
} sounding_node;

sounding_node sounding_node_create(char *n, sounding_callback_recieve h);
void sounding_node_destroy(sounding_node *n);
void sounding_node_buffer_size_set(sounding_node *n, size_t s);
void sounding_node_handler_argument_set(sounding_node *n, void *a);
char *sounding_node_ip_get(sounding_node *n);
char *sounding_node_name_get(sounding_node *n);
int sounding_node_port_get(sounding_node *n);
int sounding_node_main(sounding_node *n);
sounding_node *sounding_list();

int sounding_node_respond(sounding_node *n, void *data, size_t size);
int sounding_node_send(sounding_node *from, sounding_node *to, void *data, size_t *size);

#endif
