struct epoll_event events[MAX_EPOLL_EVENTS];
while (node->running) { // Use the node->running flag for graceful shutdown
    int num_events = epoll_wait(node->epoll_fd, events, MAX_EPOLL_EVENTS, -1); // -1 for infinite timeout

    for (int i = 0; i < num_events; i++) {
        if (events[i].data.fd == node->listen_sockfd) {
            // New incoming connection
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int client_sockfd = accept(node->listen_sockfd, (struct sockaddr*)&client_addr, &client_len);
            if (client_sockfd > 0) {
                // Make client socket non-blocking if you intend to do non-blocking reads/writes
                // fcntl(client_sockfd, F_SETFL, O_NONBLOCK);

                // Add new client socket to epoll for EPOLLIN events
                struct epoll_event event;
                event.events = EPOLLIN | EPOLLET; // Edge-triggered for performance
                event.data.fd = client_sockfd;
                epoll_ctl(node->epoll_fd, EPOLL_CTL_ADD, client_sockfd, &event);
                printf("Accepted new connection from %s:%d (FD: %d)\n",
                       inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), client_sockfd);
            } else {
                perror("accept");
            }
        } else {
            // Data on an existing connection (or connection closed)
            int current_sockfd = events[i].data.fd;
            if (events[i].events & EPOLLIN) {
                // Handle incoming data:
                // Read from current_sockfd. For simplicity, you might read a fixed buffer.
                // In a real system, you'd have a protocol (e.g., length-prefixed messages).
                char buffer[1024];
                ssize_t bytes_read = read(current_sockfd, buffer, sizeof(buffer) - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0';
                    printf("Received from FD %d: %s\n", current_sockfd, buffer);
                    // This is where you'd parse the request and act upon it.
                    // e.g., if it's a "list files" request, read shared_files and send back.
                } else if (bytes_read == 0) {
                    // Connection closed by client
                    printf("Client on FD %d disconnected.\n", current_sockfd);
                    epoll_ctl(node->epoll_fd, EPOLL_CTL_DEL, current_sockfd, NULL);
                    close(current_sockfd);
                } else {
                    // Error
                    perror("read");
                    epoll_ctl(node->epoll_fd, EPOLL_CTL_DEL, current_sockfd, NULL);
                    close(current_sockfd);
                }
            }
            // Handle EPOLLOUT if you need to do non-blocking writes for large data transfers
            // Handle EPOLLERR, EPOLLHUP for error/hangup
        }
    }
}