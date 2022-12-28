#include "NodeEditor.hpp"

#include <algorithm>

#include "imnodes.h"

#include "BinarizationNode.hpp"
#include "ImageSourceNode.hpp"

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
        ImNodes::Link(link.id, link.start_attr, link.end_attr);
    }

    ImNodes::EndNodeEditor();

    // Connect new link
    {
        static int32_t link_index = 0;
        Link new_link;
        if (ImNodes::IsLinkCreated(&new_link.start_attr, &new_link.end_attr))
        {
            {
                auto old_link = std::find_if(
                    this->link_list_.begin(),
                    this->link_list_.end(),
                    [new_link](const Link& link) -> bool
                {
                    return link.end_attr == new_link.end_attr;
                });

                // Disconnect old connection
                if (old_link != this->link_list_.end())
                {
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
                            return new_link.start_attr == pin.id_;
                        }) != node->output_pin_list_.end();
                    });

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
                            return new_link.end_attr == pin.id_;
                        })!= node->input_pin_list_.end();
                    });

                    std::cout << (*start_node)->title_ << ", " << (*end_node)->title_ << std::endl;

                    (*end_node)->inputDisconnect(old_link->end_attr);
                    this->link_list_.erase(old_link);
                }

                // Connect new connection
                new_link.id = link_index++;
                this->link_list_.push_back(new_link);
            }

            // Call connected event
            {
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
                        return new_link.start_attr == pin.id_;
                    }) != node->output_pin_list_.end();
                });

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
                        return new_link.end_attr == pin.id_;
                    })!= node->input_pin_list_.end();
                });

                (*end_node)->inputConnect(new_link.end_attr, (*start_node));
                (*start_node)->outputConnect(new_link.start_attr, (*end_node));
            }
        }
    }

    // Disconnect destroyed link
    {
        int32_t link_id;
        if (ImNodes::IsLinkDestroyed(&link_id))
        {
            auto iter = std::find_if(
                this->link_list_.begin(),
                this->link_list_.end(),
                [link_id](const Link& link) -> bool
            {
                return link.id == link_id;
            });

            auto start_node = std::find_if(
                this->node_list_.begin(),
                this->node_list_.end(),
                [link_id, iter](NodeBase* node) -> bool
            {
                return std::find_if(
                    node->output_pin_list_.begin(),
                    node->output_pin_list_.end(),
                    [link_id, iter](Pin pin) -> bool
                {
                    return (*iter).start_attr == pin.id_;
                }) != node->output_pin_list_.end();
            });

            auto end_node = std::find_if(
                this->node_list_.begin(),
                this->node_list_.end(),
                [link_id, iter](NodeBase* node) -> bool
            {
                return std::find_if(
                    node->input_pin_list_.begin(),
                    node->input_pin_list_.end(),
                    [link_id, iter](Pin pin) -> bool
                {
                    return (*iter).end_attr == pin.id_;
                }) != node->input_pin_list_.end();
            });

            (*start_node)->outputDisconnect((*iter).start_attr);
            (*end_node)->inputDisconnect((*iter).end_attr);
            this->link_list_.erase(iter);
        }
    }
}

void NodeEditor::newImageNode(NodeType type, const char* file_path)
{
    // Generate new ImageNode
    ImageNode* new_node = ImageNode::create(type);
    
    // Node setting
    new_node->id_ = this->findAvailableID();
    this->assignAvailablePins(new_node->input_pin_list_);
    this->assignAvailablePins(new_node->output_pin_list_);

    if (type == NT_ImageSource)
        ((ImageSourceNode*)new_node)->loadSource(file_path);

    // Add to list for management nodes
    this->node_list_.push_back(new_node);
    this->id_list_.push_back(new_node->id_);
}

int32_t NodeEditor::findAvailableID()
{
    for (int32_t i = 0; i < INT32_MAX; ++i)
    {
        if (!(std::find(this->id_list_.begin(), this->id_list_.end(), i) != this->id_list_.end()))
        {
            return i;
        }
    }

    // Return error id (-1)
    return -1;
}

void NodeEditor::assignAvailablePins(std::vector<Pin>& pin_list)
{
    for (Pin& pin : pin_list)
    {
        pin.id_ = this->pin_id_++;
    }
}
