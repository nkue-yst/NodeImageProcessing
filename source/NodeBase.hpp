#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "imnodes.h"

#include "NodeStyle.hpp"

// Kind of node
enum NodeType_
{
    // ↓ ImageNode ↓
    __ImageNode,
    NT_ImageSource,
    NT_Binarization,

    // ↓ MovieNode ↓
    __MovieNode,
    // NT_MovieSourceNode,

    // ↓ AudioNode ↓
    __AudioNode,
    // NT_AudioSourceNode,

    NT_UNDEFINED,
} typedef NodeType;

// Define number of pin by node type
/*
static std::unordered_map<NodeType, std::pair<uint32_t, uint32_t>> pin_num =
{
    { NT_ImageSource,  std::pair(0, 4) },
    { NT_Binarization, std::pair(1, 1) },

    { NT_UNDEFINED,    std::pair(0, 0) },
};
*/

// Base class of connectable node
class NodeBase
{
public:
    NodeBase()
        : id_(-1)
        , input_pin_(0)
        , output_pin_(0)
    {
    }

    virtual void draw() = 0;

    // Set node color
    void applyNodeColor()
    {
        ImNodesStyle& style = ImNodes::GetStyle();
        style.Colors[ImNodesCol_TitleBar] = IM_COL32(
            this->node_color_.r * 0.8,
            this->node_color_.g * 0.8,
            this->node_color_.b * 0.8,
            200
        );
        style.Colors[ImNodesCol_TitleBarHovered] = IM_COL32(
            this->node_color_.r,
            this->node_color_.g,
            this->node_color_.b,
            200
        );
        style.Colors[ImNodesCol_TitleBarSelected] = IM_COL32(
            this->node_color_.r,
            this->node_color_.g,
            this->node_color_.b,
            200
        );
    }

    // Connected event
    virtual void connect(NodeBase* node) {}

    int32_t getID() const { return this->id_; }

    void setPinNum(uint32_t input, uint32_t output)
    {
        this->input_pin_ = input;
        this->output_pin_ = output;
    }

    // Node color
    Color node_color_;

    std::vector<int32_t> pin_attr_list_;

    // Node title
    std::string title_;

    // Node ID
    int32_t id_;

    // Number of input pin
    uint32_t input_pin_;

    // Number of output pin
    uint32_t output_pin_;

    // Pin names
    std::vector<const char*> pin_names_;
};
