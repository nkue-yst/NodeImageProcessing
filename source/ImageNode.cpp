#include "ImageNode.hpp"

#include "imgui.h"
#include "imnodes.h"

#include "BinarizationNode.hpp"
#include "ImageSourceNode.hpp"

ImageNode* ImageNode::create(NodeType type)
{
    switch (type)
    {
    case NT_ImageSource:
        return new ImageSourceNode();

    case NT_Binarization:
        return new BinarizationNode();
    
    default:
        return nullptr;
    }
}

void ImageNode::draw()
{
    // Set style
    ImNodesStyle& style = ImNodes::GetStyle();
    style.Colors[ImNodesCol_TitleBar] = IM_COL32(255 * 0.8, 128 * 0.8, 128 * 0.8, 200);
    style.Colors[ImNodesCol_TitleBarHovered] = IM_COL32(255, 128, 128, 200);
    style.Colors[ImNodesCol_TitleBarSelected] = IM_COL32(255, 128, 128, 200);

    ImNodes::BeginNode(this->getID());
    
    // Title bar
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted("Image");
    ImNodes::EndNodeTitleBar();

    // Image data
    ImGui::Image((void*)(intptr_t)this->image_data_, ImVec2(100.f, 100.f));
    
    // Input pins
    uint32_t i = 0;
    for (; i < this->input_pin_; ++i)
    {
        ImNodes::BeginInputAttribute(this->pin_list_.at(i));
        ImNodes::EndInputAttribute();
    }

    // Output pins
    for (; i < this->input_pin_ + this->output_pin_; ++i)
    {
        ImNodes::BeginOutputAttribute(this->pin_list_.at(i));
        ImNodes::EndInputAttribute();
    }

    ImNodes::EndNode();
}
