/*
 * client for the Echo service
 *
 * usage: ./sinkclient
 *
 * reads each line from standard input, sends it to Echo service on localhost,
 * receives the status
 */

#include "srpc.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define HOST "localhost"
#define PORT 20000
#define SERVICE "Echo"
#define USAGE "./sinkclient [-h host] [-p port] [-s service]"

int main(int argc, char *argv[]) {
	RpcConnection rpc;
	char buf[250];
	Q_Decl(query,256);
	char resp[251];
	int n;
	unsigned len;
	char *host;
	char *service;
	unsigned short port;
	struct timeval start, stop;
	unsigned long count = 0;
	unsigned long msec;
	double mspercall;
	int i, j;

	host = HOST;
	service = SERVICE;
	port = PORT;
	for (i = 1; i < argc; ) {
		if ((j = i + 1) == argc) {
			fprintf(stderr, "usage: %s\n", USAGE);
			exit(1);
		}
		if (strcmp(argv[i], "-h") == 0)
			host = argv[j];
		else if (strcmp(argv[i], "-p") == 0)
			port = atoi(argv[j]);
		else if (strcmp(argv[i], "-s") == 0)
			service = argv[j];
		else {
			fprintf(stderr, "Unknown flag: %s %s\n", argv[i], argv[j]);
		}
		i = j + 1;
	}
	assert(rpc_init(0));
	if (!(rpc = rpc_connect(host, port, service, random() % 32768))) {
		fprintf(stderr, "Failure to connect to %s at %s:%05u\n",
				service, host, port);
		exit(-1);
	}
	gettimeofday(&start, NULL);
	while (fgets(buf, sizeof(buf), stdin) != NULL) {
		count++;
		sprintf(query, "SINK:%s", buf);
		n = strlen(query) + 1;
		if (! rpc_call(rpc, Q_Arg(query), n, resp, sizeof(resp), &len)) {
			fprintf(stderr, "rpc_call() failed\n");
			break;
		}
		if (resp[0] != '1') {
			fprintf(stderr, "Echo server returned ERR\n");
			break;
		}
	}
	gettimeofday(&stop, NULL);
	if (stop.tv_usec < start.tv_usec) {
		stop.tv_usec += 1000000;
		stop.tv_sec--;
	}
	msec = 1000 * (stop.tv_sec - start.tv_sec) +
		(stop.tv_usec - start.tv_usec) / 1000;
	mspercall = (double)msec / (double)count;
	fprintf(stderr, "%ld lines Sink'd in %ld.%03ld seconds, %.3fms/call\n",
		count, msec/1000, msec%1000, mspercall);
	rpc_disconnect(rpc);
	return 0;
}
