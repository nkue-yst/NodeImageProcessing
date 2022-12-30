#include "BinarizationNode.hpp"

#include <opencv2/opencv.hpp>

BinarizationNode::BinarizationNode()
{
    // Node setting
    this->setPinNum(1, 1);
    this->title_ = "Binarization";
    this->node_color_ = NodeColor::ImageProcessingNode;

    // Pin name setting
    this->input_pin_list_.at(0).name_ = "InputImage";
    this->output_pin_list_.at(0).name_ = "Result";
}

void BinarizationNode::updateData()
{
    // Read image data from previous node
    if (this->input_pin_list_.at(0).connected_node_)
    {
        // Set drawing size
        this->width_ = static_cast<ImageNode*>(this->input_pin_list_.at(0).connected_node_)->width_;
        this->height_ = static_cast<ImageNode*>(this->input_pin_list_.at(0).connected_node_)->height_;

        if (!static_cast<ImageNode*>(this->input_pin_list_.at(0).connected_node_)->image_data_cv_.empty())
        {
            // Update cv frame data
            this->image_data_cv_.release();
            this->image_data_cv_ = static_cast<ImageNode*>(this->input_pin_list_.at(0).connected_node_)->image_data_cv_;
            
            this->binarization();

            // Update gl texture frame data
            glDeleteTextures(1, &this->image_data_gl_);
            this->image_data_gl_ = this->convertCVmatToGLtexture(&this->image_data_cv_);
        }
    }
    else
    {
        cv::Mat mat = cv::Mat::zeros(100, 100, CV_8UC3);
        this->image_data_cv_ = mat;
        glDeleteTextures(1, &this->image_data_gl_);
    }
}

void BinarizationNode::binarization()
{
    // Grayscaling
    cv::cvtColor(this->image_data_cv_, this->image_data_cv_, cv::COLOR_BGR2GRAY);

    // Binarization
    cv::threshold(this->image_data_cv_, this->image_data_cv_, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}
