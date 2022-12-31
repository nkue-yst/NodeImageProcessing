#include "NodeEditor.hpp"

#include <algorithm>

#include "imnodes.h"

#include "ImageSourceNode.hpp"
#include "VideoSourceNode.hpp"

void NodeEditor::init()
{
    ImNodes::StyleColorsDark();
    ImNodes::SetNodeGridSpacePos(1, ImVec2(200.f, 200.f));
    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);
}

void NodeEditor::draw()
{
    ImNodes::BeginNodeEditor();

    // Draw nodes
    for (NodeBase* node : this->node_list_)
    {
        node->draw();
    }

    // Draw connect lines
    for (const Link& link : this->link_list_)
    {
        ImNodes::Link(link.id, link.start_pin_id, link.end_pin_id);
    }

    ImNodes::EndNodeEditor();

    // Connect new link
    {
        static int32_t link_index = 0;
        Link new_link;
        if (ImNodes::IsLinkCreated(&new_link.start_pin_id, &new_link.end_pin_id))
        {
            {
                // Find connected link
                auto old_link = std::find_if(
                    this->link_list_.begin(),
                    this->link_list_.end(),
                    [new_link](const Link& link) -> bool
                    {
                        return link.end_pin_id == new_link.end_pin_id;
                    }
                );

                // Disconnect connected link if exists
                if (old_link != this->link_list_.end())
                {
                    this->disconnectLinks(old_link);
                }

                // Connect new link
                new_link.id = link_index++;
                this->link_list_.push_back(new_link);
            }

            // Connected event
            {
                // Find start node
                auto start_node = std::find_if(
                    this->node_list_.begin(),
                    this->node_list_.end(),
                    [new_link](NodeBase* node) -> bool
                    {
                        return std::find_if(
                            node->output_pin_list_.begin(),
                            node->output_pin_list_.end(),
                            [new_link](Pin pin) -> bool
                            {
                                return new_link.start_pin_id == pin.id_;
                            }
                        ) != node->output_pin_list_.end();
                    }
                );

                // Find end node
                auto end_node = std::find_if(
                    this->node_list_.begin(),
                    this->node_list_.end(),
                    [new_link](NodeBase* node) -> bool
                    {
                        return std::find_if(
                            node->input_pin_list_.begin(),
                            node->input_pin_list_.end(),
                            [new_link](Pin pin) -> bool
                            {
                                return new_link.end_pin_id == pin.id_;
                            }
                        )!= node->input_pin_list_.end();
                    }
                );

                // Call connected event
                (*end_node)->inputConnect(new_link.end_pin_id, (*start_node));
                (*start_node)->outputConnect(new_link.start_pin_id, (*end_node));
            }
        }
    }

    // Disconnect destroyed link
    {
        int32_t link_id;
        if (ImNodes::IsLinkDestroyed(&link_id))
        {
            this->disconnectLinks(&link_id);
        }
    }
}

void NodeEditor::newImageNode(NodeType type, const char* file_path)
{
    // Generate new ImageNode
    ImageNode* new_node = ImageNode::create(type);
    
    // Node setting
    new_node->id_ = this->node_id_++;
    this->assignAvailablePins(new_node->input_pin_list_);
    this->assignAvailablePins(new_node->output_pin_list_);

    if (type == NT_ImageSource)
        ((ImageSourceNode*)new_node)->loadSource(file_path);

    // Add to list for management nodes
    this->node_list_.push_back(new_node);
}

void NodeEditor::newVideoNode(NodeType type, const char* file_path)
{
    // Generate new VideoNode
    VideoNode* new_node = VideoNode::create(type);

    // Node setting
    new_node->id_ = this->node_id_++;
    this->assignAvailablePins(new_node->input_pin_list_);
    this->assignAvailablePins(new_node->output_pin_list_);

    if (type == NT_VideoSource)
        ((VideoSourceNode*)new_node)->loadSource(file_path);

    // Add to list for management nodes
    this->node_list_.push_back(new_node);
}

void NodeEditor::disconnectLinks(int32_t* link_id)
{
    // Find link iterator
    auto iter = std::find_if(
        this->link_list_.begin(),
        this->link_list_.end(),
        [link_id](const Link& link) -> bool
        {
            return link.id == *link_id;
        }
    );

    // Find start node
    auto start_node = std::find_if(
        this->node_list_.begin(),
        this->node_list_.end(),
        [iter](NodeBase* node) -> bool
        {
            return std::find_if(
                node->output_pin_list_.begin(),
                node->output_pin_list_.end(),
                [iter](Pin pin) -> bool
                {
                    return (*iter).start_pin_id == pin.id_;
                }
            ) != node->output_pin_list_.end();
        }
    );

    // Find end node
    auto end_node = std::find_if(
        this->node_list_.begin(),
        this->node_list_.end(),
        [iter](NodeBase* node) -> bool
        {
            return std::find_if(
                node->input_pin_list_.begin(),
                node->input_pin_list_.end(),
                [iter](Pin pin) -> bool
                {
                    return (*iter).end_pin_id == pin.id_;
                }
            ) != node->input_pin_list_.end();
        }
    );

    // Call disconnected event
    (*start_node)->outputDisconnect((*iter).start_pin_id, *end_node);
    (*end_node)->inputDisconnect((*iter).end_pin_id, *start_node);
    this->link_list_.erase(iter);
}

void NodeEditor::disconnectLinks(std::vector<Link>::iterator iter)
{
    // Find start node
    auto start_node = std::find_if(
        this->node_list_.begin(),
        this->node_list_.end(),
        [iter](NodeBase* node) -> bool
        {
            return std::find_if(
                node->output_pin_list_.begin(),
                node->output_pin_list_.end(),
                [iter](Pin pin) -> bool
                {
                    return (*iter).start_pin_id == pin.id_;
                }
            ) != node->output_pin_list_.end();
        }
    );

    // Find end node
    auto end_node = std::find_if(
        this->node_list_.begin(),
        this->node_list_.end(),
        [iter](NodeBase* node) -> bool
        {
            return std::find_if(
                node->input_pin_list_.begin(),
                node->input_pin_list_.end(),
                [iter](Pin pin) -> bool
                {
                    return (*iter).end_pin_id == pin.id_;
                }
            ) != node->input_pin_list_.end();
        }
    );

    // Call disconnected event
    (*start_node)->outputDisconnect((*iter).start_pin_id, *end_node);
    (*end_node)->inputDisconnect((*iter).end_pin_id, *start_node);
    this->link_list_.erase(iter);
}

void NodeEditor::disconnectLinks(NodeBase* node)
{
    // Disconnect for all input pins
    for (Pin& pin : node->input_pin_list_)
    {
        // For all connected nodes
        for (NodeBase* connected_node : pin.connected_node_list_)
        {
            // Find target pin
            auto pin_iter = std::find_if(
                connected_node->output_pin_list_.begin(),
                connected_node->output_pin_list_.end(),
                [node, connected_node](Pin& pin)
                {
                    return (std::find_if(
                        pin.connected_node_list_.begin(),
                        pin.connected_node_list_.end(),
                        [node](NodeBase* node_iter)
                        {
                            return node_iter->id_ == node->id_;
                        }
                    ) != pin.connected_node_list_.end());
                }
            );

            connected_node->outputDisconnect((*pin_iter).id_, node);
            node->inputDisconnect(pin.id_, connected_node);

            // Erase deleted link
            for (std::vector<Link>::const_iterator link = this->link_list_.begin(), end = this->link_list_.end(); link != end; ++link)
            {
                if ((*pin_iter).id_ == (*link).start_pin_id && pin.id_ == (*link).end_pin_id)
                {
                    this->link_list_.erase(link);
                }
            }
        }
    }

    // Disconnect for all output pins
    for (Pin& pin : node->output_pin_list_)
    {
        // For all connected nodes
        for (NodeBase* connected_node : pin.connected_node_list_)
        {
            // Find target pin
            auto pin_iter = std::find_if(
                connected_node->input_pin_list_.begin(),
                connected_node->input_pin_list_.end(),
                [node, connected_node](Pin& pin)
                {
                    return (std::find_if(
                        pin.connected_node_list_.begin(),
                        pin.connected_node_list_.end(),
                        [node](NodeBase* node_iter)
                        {
                            return node_iter->id_ == node->id_;
                        }
                    ) != pin.connected_node_list_.end());
                }
            );

            connected_node->inputDisconnect((*pin_iter).id_, node);
            node->outputDisconnect(pin.id_, connected_node);

            // Erase deleted link
            for (std::vector<Link>::const_iterator link = this->link_list_.begin(), end = this->link_list_.end(); link != end; ++link)
            {
                if (pin.id_ == (*link).start_pin_id && (*pin_iter).id_ == (*link).end_pin_id)
                {
                    this->link_list_.erase(link);
                }
            }
        }
    }
}

void NodeEditor::assignAvailablePins(std::vector<Pin>& pin_list)
{
    for (Pin& pin : pin_list)
    {
        pin.id_ = this->pin_id_++;
    }
}
