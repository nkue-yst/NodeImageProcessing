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

    this->node_list_.push_back(new ImageNode(id));
    this->id_list_.push_back(id);
}

int32_t NodeEditor::findAvailableID()
{
    for (int32_t i = 0; i < this->max_node_num_; ++i)
    {
        if (!(std::find(this->id_list_.begin(), this->id_list_.end(), i) != this->id_list_.end()))
        {
            return i;
        }
    }

    // Return error id (-1)
    return -1;
}
