#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "node.h" // Include NodeState

// Scans the shared folder and populates the node's shared_files list
void scan_shared_folder(NodeState* node);

#endif // FILE_MANAGER_H