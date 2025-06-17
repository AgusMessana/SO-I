// node.h
#ifndef NODE_H
#define NODE_H

#include <pthread.h>
#include <stdbool.h>
#include <sys/socket.h> // For sockaddr_in

#define MAX_PATH 256
#define MAX_FILENAME 64
#define MAX_NODES 100 // Maximum known nodes

// Structure for a shared file
typedef struct {
    char name[MAX_FILENAME];
    // Add other file metadata if needed in future stages (e.g., size, hash)
} SharedFile;

// Structure for a known P2P node
typedef struct {
    char id[32]; // Unique node ID
    char ip[16]; // IP address string (e.g., "192.168.1.100")
    int port;    // Port number
} KnownNode;

// Main Node State structure
typedef struct {
    char node_id[32];
    char shared_folder_path[MAX_PATH];
    char downloads_folder_path[MAX_PATH];

    // Shared Files List
    SharedFile *shared_files;
    int num_shared_files;
    int shared_files_capacity;
    pthread_mutex_t shared_files_mutex; // Mutex for shared_files access

    // Known Nodes List
    KnownNode *known_nodes;
    int num_known_nodes;
    int known_nodes_capacity;
    pthread_mutex_t known_nodes_mutex; // Mutex for known_nodes access

    // Networking related
    int listen_sockfd; // Socket for incoming connections
    int epoll_fd;      // epoll instance file descriptor
    pthread_t server_thread; // Thread for the TCP server
    pthread_t cli_thread;    // Thread for the CLI

    bool running; // Flag to control node lifecycle

    // Add other fields as needed
} NodeState;

// Function prototypes
NodeState* node_init(const char* id, const char* shared_path, const char* downloads_path);
void node_destroy(NodeState* node);
void node_add_shared_file(NodeState* node, const char* filename);
void node_add_known_node(NodeState* node, const char* id, const char* ip, int port);

#endif // NODE_H