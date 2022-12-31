#pragma once

#include <cstdint>
#include <vector>

#include "NodeBase.hpp"

struct Link
{
    int32_t id;

    int32_t start_pin_id;    // Start pin id
    int32_t end_pin_id;      // End pin id
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

    // Create new VideoNode and add to node list
    void newVideoNode(NodeType type, const char* file_path = nullptr);

    // Destroy node
    void deleteNode(NodeBase* node)
    {
        this->disconnectLinks(node);
        
        auto node_iter = std::find_if(
            this->node_list_.begin(),
            this->node_list_.end(),
            [node](NodeBase* a_node)
            {
                return node->id_ == a_node->id_;
            }
        );
        this->node_list_.erase(node_iter);
    }

private:
    // Disconnect links by link id
    void disconnectLinks(int32_t* link_id);

    // Disconnect links by iterator
    void disconnectLinks(std::vector<Link>::iterator iter);

    // Disconnect links by node
    void disconnectLinks(NodeBase* node);

    // Search for available pin ID
    void assignAvailablePins(std::vector<Pin>& pin_list);

public:
    NodeBase* tmp_node_;

    // Node list
    std::vector<class NodeBase*> node_list_;

    // Connected node pairs
    std::vector<Link> link_list_;

private:
    // Using node id list
    int32_t node_id_;

    // Using pin id list
    int32_t pin_id_;
};
