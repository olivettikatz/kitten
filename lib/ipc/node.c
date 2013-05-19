#include "node.h"

char *kic_index = "/dev/shm/sounding.idx";

kic_node kic_node_create(char *n, kic_callback_recieve h)
{
	kic_node tmp;
	struct ifaddrs *ifaddrs = NULL;
	getifaddrs(&ifaddrs);

	tmp.ip = NULL;
	for (struct ifaddrs *i = ifaddrs; i != NULL; i = i->ifa_next)
	{
		if (i->ifa_addr->sa_family == AF_INET)
		{
			void *addr = &((struct sockaddr_in *)i->ifa_addr)->sin_addr;
			tmp.ip = (char *)malloc(INET_ADDRSTRLEN);
			inet_ntop(AF_INET, addr, tmp.ip, INET_ADDRSTRLEN);
		}
		else if (i->ifa_addr->sa_family == AF_INET6)
		{
			void *addr = &((struct sockaddr_in6 *)i->ifa_addr)->sin6_addr;
			tmp.ip = (char *)malloc(INET6_ADDRSTRLEN);
			inet_ntop(AF_INET6, addr, tmp.ip, INET6_ADDRSTRLEN);
		}
	}
	if (tmp.ip == NULL)
		return tmp;
	if (ifaddrs != NULL)
		freeifaddrs(ifaddrs);

	tmp.name = n;
	tmp.server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (tmp.server_sockfd < 0)
	{
		return tmp;
	}

	bzero((char *)&tmp.server_addr_server, sizeof(tmp.server_addr_server));
	tmp.server_port = 0;
	tmp.server_addr_server.sin_family = AF_INET;
	tmp.server_addr_server.sin_addr.s_addr = INADDR_ANY;
	tmp.server_addr_server.sin_port = htons(0);
	if (bind(tmp.server_sockfd, (struct sockaddr *)&tmp.server_addr_server, sizeof(tmp.server_addr_server)) < 0)
	{
		return tmp;
	}

	struct sockaddr_in socktmp;
	socklen_t socktmpsize = sizeof(sockaddr_in);
	if (getsockname(tmp.server_sockfd, (struct sockaddr *)&socktmp, &socktmpsize) < 0)
	{
		return tmp;
	}
	tmp.server_port = ntohs(socktmp.sin_port);

	tmp.handler = h;

	FILE *f = fopen(kic_index, "a");
	if (f == NULL)
	{
		return tmp;
	}

	fprintf(f, "%s:%i %s\n", tmp.ip, tmp.server_port, tmp.name);
	fclose(f);

	tmp.server_handler_argument = NULL;
	tmp.server_buffer_size = KIC_SERVER_BUFFER_SIZE_DEFAULT;

	return tmp;
}

void kic_node_destroy(kic_node *n)
{
	close(n->server_sockfd);
	free(n->ip);
	n->ip = NULL;
	n->name = NULL;

	FILE *f = fopen(kic_index, "r");
	if (f != NULL)
	{
		fseek(f, SEEK_END, 0);
		size_t s = ftell(f);
		fseek(f, SEEK_SET, 0);
		char *buf = (char *)malloc(sizeof(s));
		fread(buf, 1, s, f);

		char *buf2 = (char *)malloc(sizeof(s));
		buf2[0] = 0;

		char *savei;
		for (char *i = strtok_r(buf, "\n", &savei); i != NULL; i = strtok_r(NULL, "\n", &savei))
		{
			if (strlen(i) <= 1)
				continue;
			char *ip = i;
			char *port = strchr(ip, ':');
			*port = 0;
			port++;
			char *name = strchr(port, ' ');
			*name = 0;
			name++;

			if (strcmp(ip, n->ip) != 0 || atoi(port) != n->server_port || strcmp(name, n->name) != 0)
				sprintf(buf2, "%s:%s %s\n", ip, port, name);
		}
		fclose(f);
		free(buf);

		f = fopen(kic_index, "w");
		fwrite(buf2, 1, s, f);
		fclose(f);
	}
}

char *kic_node_ip_get(kic_node *n)
{
	if (n == NULL)
		return NULL;
	return n->ip;
}

char *kic_node_name_get(kic_node *n)
{
	if (n == NULL)
		return NULL;
	return n->name;
}

int kic_node_port_get(kic_node *n)
{
	if (n == NULL)
		return -1;
	return n->server_port;
}

int kic_node_main(kic_node *n)
{
	while(1)
	{
		listen(n->server_sockfd, 1);
		socklen_t clilen = sizeof(n->server_addr_client);
		n->server_sockfdtmp = accept(n->server_sockfd, (struct sockaddr *)&n->server_addr_client, &clilen);
		if (n->server_sockfdtmp < 0)
			continue;
		void *buf = malloc(n->server_buffer_size);
		size_t size = read(n->server_sockfdtmp, buf, n->server_buffer_size);
		if (size < 0)
			continue;
		server_handler(buf, size, n, n->server_handler_argument);
		close(n->server_sockfdtmp);
	}
}

kic_node *kic_list()
{
	FILE *f = fopen(kic_index, "r");
	if (f != NULL)
	{
		fseek(f, SEEK_END, 0);
		size_t s = ftell(f);
		fseek(f, SEEK_SET, 0);
		char *buf = (char *)malloc(sizeof(s));
		fread(buf, 1, s, f);

		int n = 0;
		for (int i = 0; i < s; i++)
			if (buf[i] == '\n')
				n++;

		kic_node *rtn = (kic_node *)malloc(sizeof(kic_node)*n);
		int nrtn = 0;

		char *savei;
		for (char *i = strtok_r(buf, "\n", &savei); i != NULL; i = strtok_r(NULL, "\n", &savei))
		{
			if (strlen(i) <= 1)
				continue;
			char *ip = i;
			char *port = strchr(ip, ':');
			*port = 0;
			port++;
			char *name = strchr(port, ' ');
			*name = 0;
			name++;

			rtn[nrtn].ip = ip;
			rtn[nrtn].name = name;
			rtn[nrtn].server_port = atoi(port);
			rtn[nrtn].server_sockfd = -1;
			rtn[nrtn].server_sockfdtmp = -1;
			rtn[nrtn].server_handler = NULL;
			nrtn++;
		}
		fclose(f);

		return rtn;
	}
	else
	{
		return NULL;
	}
}

int kic_node_respond(kic_node *n, void *data, size_t size)
{
	if (write(n->sockfdtmp, data, size) < 0)
		return 1;
	return 0;
}

int kic_node_send(kic_node *from, kic_node *to, void *data, size_t *size)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		return 1;

	struct hostent *serv = gethostbyname(to->ip);
	if (serv == NULL)
		return 2;

	struct sockaddr_in serv_addr;
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)&serv->h_addr_list[0], (char *)&serv_addr.sin_addr, serv->h_length);
	serv_addr.sin_port = htons(to->server_port);
	
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		return 3;

	if (write(sockfd, data, *size) < 0)
		return 4;

	bzero((char *)data, *size);
	*size = read(sockfd, data, *size);
	if (*size < 0)
		return 5;

	close(sockfd);
	return 0;
}

