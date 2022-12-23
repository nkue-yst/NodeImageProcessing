#pragma once

#include <cstdint>
#include <vector>

class NodeEditor
{
public:
    // Initialize node editor
    void init();

    // Draw node editor and nodes
    void draw();

    // Create new node and add to node list
    void newImageNode();

private:
    // Search for available node ID
    int32_t findAvailableID();

    // Node list
    std::vector<class NodeBase*> node_list_;

    // Using node id list
    std::vector<int32_t> id_list_;

    // Max num of node
    int32_t max_node_num_ = 100;
};
