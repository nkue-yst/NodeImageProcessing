#include "NodeEditor.hpp"

#include <algorithm>

#include "imnodes.h"

#include "ImageNode.hpp"
#include "NodeBase.hpp"

void NodeEditor::init()
{
    ImNodes::StyleColorsDark();
    ImNodes::SetNodeGridSpacePos(1, ImVec2(200.f, 200.f));
}

void NodeEditor::draw()
{
    ImNodes::BeginNodeEditor();

    // Draw nodes
    for (NodeBase* node : this->node_list_)
    {
        node->draw();
    }

    ImNodes::EndNodeEditor();
}

void NodeEditor::newImageNode()
{
    int32_t id = this->findAvailableID();
    std::vector<int32_t> pin_list = this->findAvailablePins(4);

    this->node_list_.push_back(new ImageNode(id, pin_list));
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
