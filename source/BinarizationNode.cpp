#include "BinarizationNode.hpp"

#include <opencv2/opencv.hpp>

#include "imgui.h"
#include "imnodes.h"

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

void BinarizationNode::connect(NodeBase* node)
{
    // Read image data from previous node
    this->image_data_cv_ = ((ImageNode*)node)->image_data_cv_;
    this->binarization();
    this->image_data_gl_ = this->convertCVmatToGLtexture(&this->image_data_cv_);
}

void BinarizationNode::disconnect(NodeBase* node)
{
    glDeleteTextures(1, &this->image_data_gl_);
}

void BinarizationNode::binarization()
{
    // Grayscaling
    cv::cvtColor(this->image_data_cv_, this->image_data_cv_, cv::COLOR_BGR2GRAY);

    // Binarization
    cv::threshold(this->image_data_cv_, this->image_data_cv_, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
}
