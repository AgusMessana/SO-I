#ifndef NETWORKING_H
#define NETWORKING_H

#include "node.h"
#include <sys/socket.h> // For socket structures
#include <netinet/in.h> // For sockaddr_in

#define LISTEN_PORT 12345
#define MAX_EPOLL_EVENTS 64 // Max events to process at once

// Function to start the TCP server in a new thread
void* tcp_server_thread(void* arg);

// Function to initiate an outgoing TCP connection
int connect_to_peer(const char* ip, int port);

// Helper for sending/receiving data (can be refined later)
int send_message(int sockfd, const char* message);
char* receive_message(int sockfd); // Caller must free the returned string

#endif // NETWORKING_H