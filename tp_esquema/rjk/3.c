#ifndef REGISTRY_H
#define REGISTRY_H

#include "node.h"
#include <stdbool.h>

#define NODES_REGISTRY_FILENAME "nodes_registry.json"

// Attempts to register the current node in the registry file.
// This will involve reading, adding/updating, and writing back.
bool register_node_in_registry(NodeState* node, const char* ip, int port);

// Reads all known nodes from the registry file into the node's known_nodes list.
void load_known_nodes_from_registry(NodeState* node);

#endif // REGISTRY_H