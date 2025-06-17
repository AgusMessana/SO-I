#include "node.h"
#include "file_manager.h"
#include "networking.h"
#include "cli.h"
#include "registry.h" // Include if you want to use it from main for init

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For sleep, gethostname

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <node_id>\n", argv[0]);
        return 1;
    }

    char node_id[32];
    strncpy(node_id, argv[1], sizeof(node_id) - 1);
    node_id[sizeof(node_id) - 1] = '\0';

    // Define paths relative to the executable for simplicity
    const char* shared_folder = "./shared_files";
    const char* downloads_folder = "./downloads";

    // Create folders if they don't exist
    // You'll need sys/stat.h and mkdir for this.
    // mkdir(shared_folder, 0755);
    // mkdir(downloads_folder, 0755);

    NodeState* node = node_init(node_id, shared_folder, downloads_folder);
    if (!node) {
        fprintf(stderr, "Failed to initialize node.\n");
        return 1;
    }

    // 1. Scan shared folder
    scan_shared_folder(node);
    printf("Node '%s' initialized. Shared files scanned.\n", node->node_id);

    // 2. Get local IP and register (simplified for now)
    // You'll need to get the actual local IP address programmatically.
    // For testing, you might hardcode it or use "127.0.0.1"
    char local_ip[16] = "127.0.0.1"; // Placeholder - get actual IP
    // Example: gethostname and then gethostbyname or similar for real IP
    // Or for LAN, query network interfaces.

    // This part needs a robust way to get the local IP visible on the network.
    // For a LAN P2P, you might need to iterate through network interfaces.
    // Simple approach for testing: if only one interface and known IP range,
    // or just use INADDR_ANY and assume the OS handles routing.
    // For now, let's assume `local_ip` is correctly determined.

    // This is a very basic registration. True consensus comes later.
    if (!register_node_in_registry(node, local_ip, LISTEN_PORT)) {
        fprintf(stderr, "Warning: Could not register node in %s. Check permissions/existence.\n", NODES_REGISTRY_FILENAME);
    } else {
        printf("Node registered in %s\n", NODES_REGISTRY_FILENAME);
    }

    // 3. Load other known nodes from registry (for potential future outgoing connections)
    load_known_nodes_from_registry(node);
    printf("Known nodes loaded from registry.\n");
    // For debugging, print them
    pthread_mutex_lock(&node->known_nodes_mutex);
    printf("Currently known nodes:\n");
    for(int i = 0; i < node->num_known_nodes; ++i) {
        printf("- ID: %s, IP: %s, Port: %d\n", node->known_nodes[i].id, node->known_nodes[i].ip, node->known_nodes[i].port);
    }
    pthread_mutex_unlock(&node->known_nodes_mutex);


    // 4. Start the TCP server thread
    if (pthread_create(&node->server_thread, NULL, tcp_server_thread, (void*)node) != 0) {
        fprintf(stderr, "Failed to create server thread.\n");
        node_destroy(node);
        return 1;
    }
    printf("TCP Server started on port %d.\n", LISTEN_PORT);

    // 5. Start the CLI thread
    if (pthread_create(&node->cli_thread, NULL, cli_thread, (void*)node) != 0) {
        fprintf(stderr, "Failed to create CLI thread.\n");
        node->running = false; // Ensure server thread can exit if it started
        pthread_join(node->server_thread, NULL); // Wait for server to clean up
        node_destroy(node);
        return 1;
    }
    printf("CLI started. Type 'id_nodo', 'listar_mis_archivos', or 'salir'.\n");

    // Main thread waits for CLI and server threads to finish
    pthread_join(node->cli_thread, NULL);
    pthread_join(node->server_thread, NULL);

    printf("Node shutting down.\n");
    node_destroy(node);
    return 0;
}