/**
 * Redistribution of this file is permitted under the GNU General
 * Public License v2.
 *
 * Copyright 2012 by Gabriel Parmer.
 * Author: Gabriel Parmer, gparmer@gwu.edu, 2012
 */

#ifndef UTIL_H
#define UTIL_H

void client_process(int fd);
void *server_simple_thread_client_process(void * fd_pointer);
void *pool_bounded_server_process();
#endif
