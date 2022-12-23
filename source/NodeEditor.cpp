#include "NodeEditor.hpp"

#include <algorithm>

#include "imnodes.h"

#include "BinarizationNode.hpp"
#include "ImageNode.hpp"
#include "NodeBase.hpp"

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
            auto iter = std::find_if(
                this->link_list_.begin(),
                this->link_list_.end(),
                [new_link](const Link& link) -> bool
            {
                return link.end_attr == new_link.end_attr;
            });

            // Disconnect old connection
            if (iter != this->link_list_.end())
            {
                this->link_list_.erase(iter);
            }

            // Connect new connection
            new_link.id = link_index++;
            this->link_list_.push_back(new_link);
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

            this->link_list_.erase(iter);
        }
    }
}

void NodeEditor::newImageNode()
{
    int32_t id = this->findAvailableID();
    std::vector<int32_t> pin_list = this->findAvailablePins(4);

    this->node_list_.push_back(new ImageNode(id, pin_list));
    this->id_list_.push_back(id);
}

void NodeEditor::newBinarizationNode()
{
    int32_t id = this->findAvailableID();
    std::vector<int32_t> pin_list = this->findAvailablePins(2);

    this->node_list_.push_back(new BinarizationNode(id, pin_list));
    this->id_list_.push_back(id);
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

std::vector<int32_t> NodeEditor::findAvailablePins(uint32_t pin_num)
{
    std::vector<int32_t> pin_list;

    while (pin_num > 0)
    {
        for (int32_t i = 0; i < INT32_MAX; ++i)
        {
            if (!(std::find(this->pin_list_.begin(), this->pin_list_.end(), i) != this->pin_list_.end()))
            {
                pin_list.push_back(i);
                this->pin_list_.push_back(i);
                break;
            }
        }

        --pin_num;
    }

    return pin_list;
}
