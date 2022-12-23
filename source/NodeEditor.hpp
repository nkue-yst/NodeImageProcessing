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

    // Search for available pin ID
    std::vector<int32_t> findAvailablePins(uint32_t pin_num);

    // Node list
    std::vector<class NodeBase*> node_list_;

    // Using node id list
    std::vector<int32_t> id_list_;

    // Using pin id list
    std::vector<int32_t> pin_list_;
};
