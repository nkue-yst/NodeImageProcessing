#include "ImageSourceNode.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ImGuiFileDialog.h"

#include "NodeEditor.hpp"

ImageSourceNode::ImageSourceNode()
{
    // Node setting
    this->setPinNum(0, 1);
    this->title_ = "ImageSource";
    this->node_color_ = NodeColor::ImageSourceNode;

    // Pin name setting
    this->pin_names_.push_back("RGB");
}

void ImageSourceNode::draw()
{
    // Set style
    this->applyNodeColor();

    ImNodes::BeginNode(this->getID());
    
    // Title bar
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(this->title_.c_str());
    ImNodes::EndNodeTitleBar();

    // Button to open select file dialog
    if (ImGui::Button(" Open "))
    {
        const char* filters = "Image files (*.png *.jpg *.jpeg){.png,.jpg,.jpeg}";
        ImGuiFileDialog::Instance()->OpenDialog("SelectImageDlgKey", "Select Image File", filters, ".");

        NodeEditor::get().tmp_node_ = this;
    }

    // Image data
    ImGui::Image((void*)(intptr_t)this->image_data_gl_, ImVec2(100.f, 100.f));
    
    // Input pins
    uint32_t i = 0;
    for (; i < this->input_pin_; ++i)
    {
        ImNodes::BeginInputAttribute(this->pin_attr_list_.at(i));
        ImGui::Text(this->pin_names_.at(i), NULL);
        ImNodes::EndInputAttribute();
    }

    // Output pins
    for (; i < this->input_pin_ + this->output_pin_; ++i)
    {
        ImNodes::BeginOutputAttribute(this->pin_attr_list_.at(i));
        const float text_width = ImGui::CalcTextSize(this->pin_names_.at(i)).x;
        ImGui::Indent(100.f - text_width);
        ImGui::Text(this->pin_names_.at(i), NULL);
        ImNodes::EndInputAttribute();
    }

    ImNodes::EndNode();
}

void ImageSourceNode::loadSource(const char* file_path)
{
    // When file_path is null, generate empty image
    if (!file_path)
    {
        this->image_data_cv_ = cv::Mat::zeros(100, 100, CV_8UC3);
        return;
    }

    // Load image file
    int32_t width = 0;
    int32_t height = 0;
    uint8_t* image_data = stbi_load(file_path, &width, &height, NULL, 4);

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

    this->image_data_gl_ = image_texture;

    // Load image file for processing
    this->image_data_cv_ = cv::imread(file_path);
}
