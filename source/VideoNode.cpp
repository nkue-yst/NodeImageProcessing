#include "VideoNode.hpp"

#include "NodeBase.hpp"
#include "VideoSourceNode.hpp"

VideoNode* VideoNode::create(NodeType type)
{
    switch (type)
    {
    case NT_VideoSource:
        return new VideoSourceNode();

    default:
        return nullptr;
    }
}

void VideoNode::draw()
{
    // Set style
    this->applyNodeColor();

    ImNodes::BeginNode(this->getID());

    // Title bar
    this->drawTitleBar();

    // Frame data
    ImGui::Image((void*)(uintptr_t)this->frame_data_gl_, ImVec2(100.f, 100.f));

    // Option parts
    this->drawOptions();
    ImGui::Dummy(ImVec2(0.f, 10.f));

    // Input pins
    for (Pin& pin : this->input_pin_list_)
    {
        ImNodes::BeginInputAttribute(pin.id_);
        ImGui::Text(pin.name_, NULL);
        ImNodes::EndInputAttribute();
    }

    // Output pins
    for (Pin& pin : this->output_pin_list_)
    {
        ImNodes::BeginOutputAttribute(pin.id_);
        const float text_width = ImGui::CalcTextSize(pin.name_).x;
        ImGui::Indent(100.f - text_width);
        ImGui::Text(pin.name_, NULL);
        ImNodes::EndOutputAttribute();
    }

    ImNodes::EndNode();
}

GLuint VideoNode::convertCVmatToGLtexture(cv::Mat* mat)
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
