#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>

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

    // Connected event
    virtual void connect() {}

    int32_t getID() const { return this->id_; }

    void setPinNum(uint32_t input, uint32_t output)
    {
        this->input_pin_ = input;
        this->output_pin_ = output;
    }

    std::vector<int32_t> pin_list_;

    // Node ID
    int32_t id_;

    // Number of input pin
    uint32_t input_pin_;

    // Number of output pin
    uint32_t output_pin_;
};
