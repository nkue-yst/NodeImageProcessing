#pragma once

#include <cstdint>
#include <vector>

#include "NodeBase.hpp"

struct Link
{
    int32_t id;
    int32_t start_attr, end_attr;
};

class NodeEditor
{
public:
    // Initialize node editor
    void init();

    // Draw node editor and nodes
    void draw();

    // Create new ImageNode and add to node list
    void newImageNode(NodeType type);

private:
    // Search for available node ID
    int32_t findAvailableID();

    // Search for available pin ID
    std::vector<int32_t> assignAvailablePins(uint32_t pin_num);

    // Node list
    std::vector<class NodeBase*> node_list_;

    // Using node id list
    std::vector<int32_t> id_list_;

    // Using pin id list
    std::vector<int32_t> pin_list_;

    // Connected node pairs
    std::vector<Link> link_list_;
};
