/*
 * myQueue.cpp
 *
 *  Created on: Sep 19, 2015
 *      Author: debobroto
 */


#ifndef MY_QUEUE_FOR_WEB_SERVER

#define MY_QUEUE_FOR_WEB_SERVER
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <pthread.h>


typedef struct {
	int head, tail;
	int * request_list;
	int max_queue_capacity;
	pthread_mutex_t queue_locker ;
	pthread_cond_t queue_cond_var;
	pthread_cond_t full_queue_cond;
}request_queue ;

int init_request_queue(request_queue * req_queue, int max_number_of_concurennt_request);
void put_request_in_queue(request_queue * req_queue,int request_id);
int get_request_from_queue(request_queue * re_queue);

request_queue acccepted_request_queue;
#endif

