char command[256];
while (node->running) {
    printf("> ");
    if (fgets(command, sizeof(command), stdin) == NULL) {
        // Handle EOF (Ctrl+D) or error
        break;
    }
    command[strcspn(command, "\n")] = 0; // Remove newline

    if (strcmp(command, "id_nodo") == 0) {
        printf("Node ID: %s\n", node->node_id);
    } else if (strcmp(command, "listar_mis_archivos") == 0) {
        pthread_mutex_lock(&node->shared_files_mutex); // Acquire mutex
        printf("Shared files:\n");
        for (int i = 0; i < node->num_shared_files; i++) {
            printf("- %s\n", node->shared_files[i].name);
        }
        pthread_mutex_unlock(&node->shared_files_mutex); // Release mutex
    } else if (strcmp(command, "salir") == 0) {
        printf("Shutting down node...\n");
        node->running = false; // Set flag to stop other threads
        // Potentially signal server thread to wake up (e.g., by writing to a pipe)
        // and allow it to gracefully exit.
        break;
    } else {
        printf("Unknown command: %s\n", command);
    }
}