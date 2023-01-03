#include "InformationNode.hpp"

InformationNode::InformationNode()
{
    // Node setting
    this->setPinNum(1, 0);
    this->title_ = "Information";
    this->node_color_ = NodeColor::InformationNode;

    // Pin name setting
    this->input_pin_list_.at(0).name_ = "Input";
}

void InformationNode::draw()
{
    // Set style
    this->applyNodeColor();

    ImNodes::BeginNode(this->getID());

    // Title bar
    this->drawTitleBar();

    // Draw information of connected node
    std::string title = "null";
    if (this->input_pin_list_.at(0).connected_node_list_.size() > 0)
    {
        title = this->input_pin_list_.at(0).connected_node_list_.at(0)->title_;
    }
    ImGui::Text("Title: %s", title.c_str());

    // Input pins
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();
    for (Pin& pin : this->input_pin_list_)
    {
        ImNodes::BeginInputAttribute(pin.id_);
        ImGui::Text(pin.name_, NULL);
        ImNodes::EndInputAttribute();
    }

    ImNodes::EndNode();
}
