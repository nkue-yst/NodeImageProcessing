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
private:
    NodeEditor() = default;
    ~NodeEditor() = default;

public:
    NodeEditor(const NodeEditor&) = delete;
    NodeEditor& operator=(const NodeEditor&) = delete;
    NodeEditor(NodeEditor&&) = delete;
    NodeEditor& operator=(NodeEditor&&) = delete;

    // Getter for singleton instance
    static inline NodeEditor& get()
    {
        static NodeEditor instance_;
        return instance_;
    }

    // Initialize node editor
    void init();

    // Draw node editor and nodes
    void draw();

    // Create new ImageNode and add to node list
    void newImageNode(NodeType type, const char* file_path = nullptr);

private:
    // Search for available node ID
    int32_t findAvailableID();

    // Search for available pin ID
    void assignAvailablePins(std::vector<Pin>& pin_list);

public:
    NodeBase* tmp_node_;

    // Node list
    std::vector<class NodeBase*> node_list_;

private:
    // Using node id list
    std::vector<int32_t> id_list_;

    // Using pin id list
    int32_t pin_id_;

    // Connected node pairs
    std::vector<Link> link_list_;
};
