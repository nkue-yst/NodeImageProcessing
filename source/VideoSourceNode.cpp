#include "VideoSourceNode.hpp"

#include "ImGuiFileDialog.h"

#include "NodeEditor.hpp"

VideoSourceNode::VideoSourceNode()
{
    // Node setting
    this->setPinNum(0, 1);
    this->title_ = "VideoSource";
    this->node_color_ = NodeColor::VideoSourceNode;

    // Pin name setting
    this->output_pin_list_.at(0).name_ = "RGB";
}

void VideoSourceNode::draw()
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
        const char* filters = "Image files (*.mp4){.mp4}";
        ImGuiFileDialog::Instance()->OpenDialog("SelectVideoDlgKey", "Select video File", filters, ".");

        NodeEditor::get().tmp_node_ = this;
    }

    // Frame data
    if (this->video_.isOpened())
    {
        this->video_.read(this->image_data_cv_);
        if (this->image_data_cv_.empty())
        {
            this->video_.set(cv::CAP_PROP_POS_FRAMES, 0);
            this->video_.read(this->image_data_cv_);
        }
        glDeleteTextures(sizeof(this->image_data_gl_), &this->image_data_gl_);
        this->image_data_gl_ = this->convertCVmatToGLtexture(&this->image_data_cv_);
    }
    ImGui::Image((void*)(intptr_t)this->image_data_gl_, ImVec2(100.f, 100.f));

    // Output pins
    for (Pin& pin : this->output_pin_list_)
    {
        ImNodes::BeginOutputAttribute(pin.id_);
        const float text_width = ImGui::CalcTextSize(pin.name_).x;
        ImGui::Indent(100.f - text_width);
        ImGui::Text(pin.name_, NULL);
        ImNodes::EndInputAttribute();
    }

    ImNodes::EndNode();

    // Update child node
    this->update();
}

void VideoSourceNode::loadSource(const char* file_path)
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
    int32_t max_frame = 0;
    int32_t fps = 0;

    this->video_.open(file_path);
    if (!this->video_.isOpened())
    {
        std::cout << "Failed to load: \"" << file_path << "\"" << std::endl;
        return;
    }

    width     = this->video_.get(cv::CAP_PROP_FRAME_WIDTH);
    height    = this->video_.get(cv::CAP_PROP_FRAME_HEIGHT);
    max_frame = this->video_.get(cv::CAP_PROP_FRAME_COUNT);
    fps       = this->video_.get(cv::CAP_PROP_FPS);

    // Get first frame
    this->video_.set(cv::CAP_PROP_POS_FRAMES, 0);
    this->video_.read(this->image_data_cv_);
    this->image_data_gl_ = this->convertCVmatToGLtexture(&this->image_data_cv_);

    // Update child node
    this->update();
}
