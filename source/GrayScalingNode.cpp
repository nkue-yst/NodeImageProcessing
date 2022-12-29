#include "GrayScalingNode.hpp"

#include <opencv2/opencv.hpp>

#include "imgui.h"
#include "imnodes.h"

GrayScalingNode::GrayScalingNode()
{
    // Node setting
    this->setPinNum(1, 1);
    this->title_ = "GrayScaling";
    this->node_color_ = NodeColor::ImageProcessingNode;

    // Pin name setting
    this->input_pin_list_.at(0).name_ = "InputImage";
    this->output_pin_list_.at(0).name_ = "Result";
}

void GrayScalingNode::updateData()
{
    // Read image data from previous node
    if (this->input_pin_list_.at(0).connected_node_)
    {
        if (!static_cast<ImageNode*>(this->input_pin_list_.at(0).connected_node_)->image_data_cv_.empty())
        {
            // Update cv frame data
            this->image_data_cv_ = static_cast<ImageNode*>(this->input_pin_list_.at(0).connected_node_)->image_data_cv_;

            this->grayscaling();

            // Update gl frame data
            glDeleteTextures(1, &this->image_data_gl_);
            this->image_data_gl_ = this->convertCVmatToGLtexture(&this->image_data_cv_);
        }
    }
    else
    {
        this->image_data_cv_.release();
        cv::Mat mat = cv::Mat::zeros(100, 100, CV_8UC3);
        this->image_data_cv_ = mat;

        glDeleteTextures(1, &this->image_data_gl_);
    }
}

void GrayScalingNode::grayscaling()
{
    // Grayscaling
    cv::cvtColor(this->image_data_cv_, this->image_data_cv_, cv::COLOR_BGR2GRAY);
}
