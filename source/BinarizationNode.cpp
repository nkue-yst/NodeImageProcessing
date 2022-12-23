#include "BinarizationNode.hpp"

#include "imgui.h"
#include "imnodes.h"

BinarizationNode::BinarizationNode(int32_t id, std::vector<int32_t> pins)
    : NodeBase(id, pins)
{
}

void BinarizationNode::draw()
{
    // Set style
    ImNodesStyle& style = ImNodes::GetStyle();
    style.Colors[ImNodesCol_TitleBar] = IM_COL32(128 * 0.8, 128 * 0.8, 255 * 0.8, 200);
    style.Colors[ImNodesCol_TitleBarHovered] = IM_COL32(128, 128, 255, 200);
    style.Colors[ImNodesCol_TitleBarSelected] = IM_COL32(128, 128, 255, 200);
    
    ImNodes::BeginNode(this->getID());
    
    // Title bar
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted("Binarization");
    ImNodes::EndNodeTitleBar();

    // Image data
    ImGui::Image((void*)(intptr_t)this->image_data_, ImVec2(100.f, 100.f));
    
    // Output pins
    ImNodes::BeginInputAttribute(this->pin_list_.at(0));
    ImGui::Text("Input");
    ImNodes::EndOutputAttribute();

    ImNodes::BeginOutputAttribute(this->pin_list_.at(1));
    ImGui::Indent(75.f);
    ImGui::Text("RGB");
    ImNodes::EndOutputAttribute();

    ImNodes::EndNode();
}

void BinarizationNode::connect()
{
    this->binarization();
}

void BinarizationNode::binarization()
{

}
