#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "imnodes.h"

#include "Clock.hpp"
#include "NodeStyle.hpp"

struct Pin
{
public:
    int32_t id_;
    const char* name_;

    std::vector<class NodeBase*> connected_node_list_;
};

// Kind of node
typedef enum
{
    // ↓ ImageNode ↓
    __ImageNode,
    NT_ImageSource,
    NT_Binarization,
    NT_GrayScaling,
    NT_EdgeDetection,

    // ↓ VideoNode ↓
    __VideoNode,
    NT_VideoSource,

    // ↓ AudioNode ↓
    __AudioNode,
    // NT_AudioSource,

    // ↓ Others ↓
    NT_InformationNode,

    NT_UNDEFINED,
} NodeType;

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
    virtual void drawOptions() {}

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

    // Draw title bar
    void drawTitleBar()
    {
        ImNodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(this->title_.c_str());
        ImNodes::EndNodeTitleBar();
    }

    // Connected event
    void inputConnect(int32_t pin_id, NodeBase* node)
    {
        auto pin = std::find_if(
            this->input_pin_list_.begin(),
            this->input_pin_list_.end(),
            [pin_id](Pin& pin) -> bool
        {
            return pin.id_ == pin_id;
        });

        (*pin).connected_node_list_.push_back(node);

        this->update();
    }

    void outputConnect(int32_t pin_id, NodeBase* node)
    {
        auto pin = std::find_if(
            this->output_pin_list_.begin(),
            this->output_pin_list_.end(),
            [pin_id](Pin& pin) -> bool
        {
            return pin.id_ == pin_id;
        });

        (*pin).connected_node_list_.push_back(node);
    }

    // Disconnected event
    void inputDisconnect(int32_t pin_id, NodeBase* node)
    {
        auto pin = std::find_if(
            this->input_pin_list_.begin(),
            this->input_pin_list_.end(),
            [pin_id](Pin& pin) -> bool
        {
            return pin.id_ == pin_id;
        });

        auto disconnect_iter = std::find_if(
            (*pin).connected_node_list_.begin(),
            (*pin).connected_node_list_.end(),
            [node](NodeBase* iter)
        {
            return node == iter;
        });

        (*pin).connected_node_list_.erase(disconnect_iter);

        this->update();
    }

    void outputDisconnect(int32_t pin_id, NodeBase* node)
    {
        auto pin = std::find_if(
            this->output_pin_list_.begin(),
            this->output_pin_list_.end(),
            [pin_id](Pin& pin) -> bool
        {
            return pin.id_ == pin_id;
        });

        auto disconnect_iter = std::find_if(
            (*pin).connected_node_list_.begin(),
            (*pin).connected_node_list_.end(),
            [node](NodeBase* iter)
        {
            return node == iter;
        });

        (*pin).connected_node_list_.erase(disconnect_iter);
    }

    // Update all child node
    void update()
    {
        this->updateData();

        for (Pin& pin : this->output_pin_list_)
        {
            for (auto& node : pin.connected_node_list_)
            {
                node->update();
            }
        }
    }

    // Update image, audio or video data
    virtual void updateData() {}

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
