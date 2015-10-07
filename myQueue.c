#include <stdio.h>
#include "myQueue.h"
#include <pthread.h>

int init_request_queue(request_queue * req_queue,
		int max_number_of_concurennt_request) {
	req_queue->head = 0;
	req_queue->tail = -1;
	req_queue->max_queue_capacity = max_number_of_concurennt_request;
	req_queue->request_list = (int *) malloc(
			sizeof(int) * max_number_of_concurennt_request);

	if (req_queue->request_list == NULL) {
		printf(
				"Error in allocating memory for intializing request list. Exiting \n");
		exit(1);
	}

	if (pthread_mutex_init(&req_queue->queue_locker, NULL) != 0) {
		printf("\n mutex init failed\n");
		exit(1);
	}
	req_queue->queue_cond_var = (pthread_cond_t )PTHREAD_COND_INITIALIZER;

	printf("Success in initializing queue \n");
	return 0;
}

void put_request_in_queue(request_queue * req_queue, int request_id) {
	pthread_mutex_lock(&req_queue->queue_locker);

	if (req_queue->tail >= req_queue->max_queue_capacity) {
		//wait
		if (req_queue->tail == req_queue->head) {
			printf("Amito hege dichi");
			req_queue->tail = -1;
			req_queue->head = 0;
		} else {
			pthread_cond_wait(&req_queue->full_queue_cond,
					&req_queue->queue_locker);
			printf(
					"Waiting for queue element to be consumed . now tail is %d \n",
					req_queue->tail);
		}

	}
	req_queue->request_list[++req_queue->tail] = request_id;
	pthread_cond_signal(&req_queue->queue_cond_var);
	printf("Put give a signal\n");
	pthread_mutex_unlock(&req_queue->queue_locker);
	printf("New element putted in queue . tail is %d\n", req_queue->tail);

}
int get_request_from_queue(request_queue * req_queue) {
	pthread_mutex_lock(&req_queue->queue_locker);
	if (req_queue->tail <= req_queue->head) {
		//wait
		pthread_cond_wait(&req_queue->queue_cond_var, &req_queue->queue_locker);
		printf(
				"Waiting for queue element to be produced . now head is %d and tails is %d\n",
				req_queue->head, req_queue->tail);
	}
	int request_id = req_queue->request_list[req_queue->head++];

	pthread_cond_signal(&req_queue->full_queue_cond);
	printf("get give a signal\n");
	pthread_mutex_unlock(&req_queue->queue_locker);

	return request_id;
}
