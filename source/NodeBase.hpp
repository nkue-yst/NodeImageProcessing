#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "imnodes.h"

#include "NodeStyle.hpp"

struct Pin
{
public:
    int32_t id_;
    const char* name_;

    class NodeBase* connected_node;
};

// Kind of node
enum NodeType_
{
    // ↓ ImageNode ↓
    __ImageNode,
    NT_ImageSource,
    NT_Binarization,
    NT_GrayScaling,

    // ↓ MovieNode ↓
    __MovieNode,
    // NT_MovieSourceNode,

    // ↓ AudioNode ↓
    __AudioNode,
    // NT_AudioSourceNode,

    NT_UNDEFINED,
} typedef NodeType;

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

    // Disconnected event
    virtual void disconnect(NodeBase* node) {}

    int32_t getID() const { return this->id_; }

    void setPinNum(uint32_t input, uint32_t output)
    {
        this->input_pin_ = input;
        this->input_pin_list_.resize(input);

        this->output_pin_ = output;
        this->output_pin_list_.resize(output);
    }

    // Node color
    Color node_color_;

    // Node title
    std::string title_;

    // Node ID
    int32_t id_;

    // Number of input pin
    uint32_t input_pin_;

    // Number of output pin
    uint32_t output_pin_;

    // Input pin list
    std::vector<Pin> input_pin_list_;

    // Output pin list
    std::vector<Pin> output_pin_list_;
};
