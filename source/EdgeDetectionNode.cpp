#include "EdgeDetectionNode.hpp"

#include <opencv2/opencv.hpp>

EdgeDetectionNode::EdgeDetectionNode()
{
    // Node setting
    this->setPinNum(1, 1);
    this->title_ = "EdgeDetection";
    this->node_color_ = NodeColor::ImageProcessingNode;

    // Pin name setting
    this->input_pin_list_.at(0).name_ = "InputImage";
    this->output_pin_list_.at(0).name_ = "Result";

    // Initialize parameter
    this->threshold[0] = 100.f;
    this->threshold[1] = 200.f;
}

void EdgeDetectionNode::updateData()
{
    // Read image data from previous node
    if (this->input_pin_list_.at(0).connected_node_)
    {
        if (!static_cast<ImageNode*>(this->input_pin_list_.at(0).connected_node_)->image_data_cv_.empty())
        {
            this->original_image_ = static_cast<ImageNode*>(this->input_pin_list_.at(0).connected_node_)->image_data_cv_;

            this->edgeDetection();
        }
    }
    else
    {
        cv::Mat mat = cv::Mat::zeros(100, 100, CV_8UC3);
        this->image_data_cv_ = mat;
        glDeleteTextures(1, &this->image_data_gl_);
    }
}

void EdgeDetectionNode::drawOptions()
{
    // Draw parameter slider
    ImGui::Text("Threshold");

    ImGui::PushItemWidth(100.f);
    if (ImGui::SliderInt2("", this->threshold, 0, 255))
    {
        if (this->threshold[0] > this->threshold[1])
        {
            int32_t tmp = this->threshold[0];
            this->threshold[0] = this->threshold[1];
            this->threshold[1] = tmp;
        }

        this->edgeDetection();
        this->update();
    }
    ImGui::PopItemWidth();
}

void EdgeDetectionNode::edgeDetection()
{
    if (this->original_image_.empty())
        return;

    // Grayscaling
    if (this->original_image_.channels() != 1)
        cv::cvtColor(this->original_image_, this->image_data_cv_, cv::COLOR_BGR2GRAY);

    // Edge detection by canny
    cv::Canny(this->image_data_cv_, this->image_data_cv_, this->threshold[0], this->threshold[1]);
    glDeleteTextures(1, &this->image_data_gl_);
    this->image_data_gl_ = this->convertCVmatToGLtexture(&this->image_data_cv_);
}
