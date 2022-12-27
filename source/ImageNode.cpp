#include "ImageNode.hpp"

#include "imgui.h"
#include "imnodes.h"

#include "BinarizationNode.hpp"
#include "GrayScalingNode.hpp"
#include "ImageSourceNode.hpp"

ImageNode* ImageNode::create(NodeType type)
{
    switch (type)
    {
    case NT_ImageSource:
        return new ImageSourceNode();

    case NT_Binarization:
        return new BinarizationNode();
    
    case NT_GrayScaling:
        return new GrayScalingNode();

    default:
        return nullptr;
    }
}

void ImageNode::draw()
{
    // Set style
    this->applyNodeColor();

    ImNodes::BeginNode(this->getID());
    
    // Title bar
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(this->title_.c_str());
    ImNodes::EndNodeTitleBar();

    // Image data
    ImGui::Image((void*)(intptr_t)this->image_data_gl_, ImVec2(100.f, 100.f));
    
    // Input pins
    uint32_t i = 0;
    for (; i < this->input_pin_; ++i)
    {
        ImNodes::BeginInputAttribute(this->input_pin_list_.at(i).id_);
        ImGui::Text(this->input_pin_list_.at(i).name_, NULL);
        ImNodes::EndInputAttribute();
    }

    // Output pins
    for (; i < this->input_pin_ + this->output_pin_; ++i)
    {
        ImNodes::BeginOutputAttribute(this->output_pin_list_.at(i).id_);
        const float text_width = ImGui::CalcTextSize(this->output_pin_list_.at(i).name_).x;
        ImGui::Indent(100.f - text_width);
        ImGui::Text(this->output_pin_list_.at(i).name_, NULL);
        ImNodes::EndInputAttribute();
    }

    ImNodes::EndNode();
}

GLuint ImageNode::convertCVmatToGLtexture(cv::Mat* mat)
{
    GLuint texture_id;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    cv::cvtColor((*mat), (*mat), cv::COLOR_RGB2BGR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (*mat).cols, (*mat).rows, 0, GL_RGB, GL_UNSIGNED_BYTE, (*mat).ptr());

    return texture_id;
}
