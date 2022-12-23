#include "ImageNode.hpp"

#include "imnodes.h"

ImageNode::ImageNode(int32_t id)
    : NodeBase(id)
{
}

void ImageNode::draw()
{
    ImNodes::BeginNode(this->getID());
    ImGui::Dummy(ImVec2(50.f, 50.f));
    ImNodes::EndNode();
}
