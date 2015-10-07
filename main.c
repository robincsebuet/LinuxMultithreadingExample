/**
 * Redistribution of this file is permitted under the GNU General
 * Public License v2.
 *
 * Copyright 2012 by Gabriel Parmer.
 * Author: Gabriel Parmer, gparmer@gwu.edu, 2012
 */
/* 
 * This is a HTTP server.  It accepts connections on port 8080, and
 * serves a local static document.
 *
 * The clients you can use are 
 * - httperf (e.g., httperf --port=8080),
 * - wget (e.g. wget localhost:8080 /), 
 * - or even your browser.  
 *
 * To measure the efficiency and concurrency of your server, use
 * httperf and explore its options using the manual pages (man
 * httperf) to see the maximum number of connections per second you
 * can maintain over, for example, a 10 second period.
 *
 * Example usage:
 * # make test1
 * # make test2
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/sysinfo.h>
#include "util.h" 		/* client_process */
#include "server.h"		/* server_accept and server_create */
#include "myQueue.h"

#define MAX_DATA_SZ 1024
#define MAX_CONCURRENCY 4
#define MAX_CONCURRENT_REQUEST_NUMBER 1000000
static int TOTAL_NUMBER_OF_CORES_IN_OS = 0;

/* 
 * This is the function for handling a _single_ request.  Understand
 * what each of the steps in this function do, so that you can handle
 * _multiple_ requests.  Use this function as an _example_ of the
 * basic functionality.  As you increase the server in functionality,
 * you will want to probably keep all of the functions called in this
 * function, but define different code to use them.
 */
void server_single_request(int accept_fd) {
	int fd;

	/* 
	 * The server thread will always want to be doing the accept.
	 * That main thread will want to hand off the new fd to the
	 * new threads/processes/thread pool.
	 */
	int i = 0;
	for (i = 0; i < 1000; i++) {
		fd = server_accept(accept_fd);
		client_process(fd);
	}
	/* 
	 * A loop around these two lines will result in multiple
	 * documents being served.
	 */

	return;
}

/* 
 * This implementation uses a single master thread which then spawns a
 * new thread to handle each incoming requst.  Each of these worker
 * threads should process a single request and then terminate.
 */
void server_simple_thread(int accept_fd) {

	int counter = 0;
	while (1) {
		counter++;
		int fd = server_accept(accept_fd);
		pthread_t serving_pthread;
		//int serving_thread_id = pthread_create(& serving_pthread, NULL, server_simple_thread_client_process, (void*)(&fd));

		pthread_create(&serving_pthread, NULL,
				server_simple_thread_client_process, (void*) (&fd));
		printf("Server request %d ", counter);
	}
}

/* 
 * The following implementation uses a thread pool.  This collection
 * of threads is of maximum size MAX_CONCURRENCY, and is created by
 * pthread_create.  These threads retrieve data from a shared
 * data-structure with the main thread.  The synchronization around
 * this shared data-structure is done using mutexes + condition
 * variables (for a bounded structure).
 */

void server_thread_pool_bounded(int accept_fd) {

	init_request_queue(&acccepted_request_queue, MAX_CONCURRENT_REQUEST_NUMBER);
	int i = 0;
	pthread_t pool_bounded_server_threads[MAX_CONCURRENCY];
	for (; i < MAX_CONCURRENCY; i++) {
		pthread_create(&pool_bounded_server_threads[i], NULL,
				pool_bounded_server_process, (void *)(&acccepted_request_queue));
		printf("thread created %d \n",i);
	}
	printf("thread created all \n");
	while (1) {
		int fd = server_accept(accept_fd);
		put_request_in_queue(&acccepted_request_queue,fd);
		printf("A new request recvd %d\n",fd);
	}
//	for (i=0; i < MAX_CONCURRENCY; i++) {
//		pthread_join(pool_bounded_server_threads[i], NULL);
//	}
	return;
}

typedef enum {
	SERVER_TYPE_ONE = 0,
	SERVER_TYPE_SIMPLE_THREAD =1,
	SERVER_TYPE_THREAD_POOL_BOUND =2,
} server_type_t;

int main(int argc, char *argv[]) {
	server_type_t server_type;
	short int port;
	int accept_fd;

	TOTAL_NUMBER_OF_CORES_IN_OS = get_nprocs();
	printf("Number of cores in the OS is %d", TOTAL_NUMBER_OF_CORES_IN_OS);

	if (argc != 3) {
		printf("Proper usage of http server is:\n%s <port> <#>\n"
				"port is the port to serve on, # is either\n"
				"0: server only a single request\n"
				"1: use a master thread that spawns new threads for "
				"each request\n"
				"2: use a thread pool and a _bounded_ buffer with "
				"mutexes + condition variables\n", argv[0]);
		return -1;
	}

	port = atoi(argv[1]);
	accept_fd = server_create(port);
	if (accept_fd < 0)
		return -1;

	server_type = atoi(argv[2]);

	switch (server_type) {
	case SERVER_TYPE_ONE:
		printf("djfsjdhfsjfh1");
		fflush(stdout);
		server_single_request(accept_fd);
		break;

	case SERVER_TYPE_THREAD_POOL_BOUND:
		server_thread_pool_bounded(accept_fd);
		break;
	case SERVER_TYPE_SIMPLE_THREAD:
		server_simple_thread(accept_fd);
		break;
	}
	close(accept_fd);

	return 0;
}
