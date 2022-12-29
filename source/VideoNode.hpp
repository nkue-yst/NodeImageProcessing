#pragma once

#include "ImageNode.hpp"

class VideoNode : public ImageNode
{
public:
    static VideoNode* create(NodeType type);

    virtual void draw() override;

public:
    // Video data
    cv::VideoCapture video_;
};
