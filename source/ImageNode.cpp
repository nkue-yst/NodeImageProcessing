#include "ImageNode.hpp"

#include "imgui.h"
#include "imnodes.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImageNode::ImageNode(int32_t id)
    : NodeBase(id)
{
    // Load image file
    int32_t width = 0;
    int32_t height = 0;
    uint8_t* image_data = stbi_load("sample/github.png", &width, &height, NULL, 4);

    // Create OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload pixels into texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    this->image_data_ = image_texture;
}

void ImageNode::draw()
{
    ImNodes::BeginNode(this->getID());
    ImGui::Image((void*)(intptr_t)this->image_data_, ImVec2(100.f, 100.f));
    ImNodes::EndNode();
}
