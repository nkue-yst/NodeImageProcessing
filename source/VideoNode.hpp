#pragma once

#include "GL/gl.h"

#include <opencv2/opencv.hpp>

#include "NodeBase.hpp"

class VideoNode : public NodeBase
{
public:
    static VideoNode* create(NodeType type);

    virtual void draw() override;

protected:
    GLuint convertCVmatToGLtexture(cv::Mat* mat);

public:
    // Video data
    cv::VideoCapture video_;

    // Frame data for drawing
    GLuint frame_data_gl_;

    // Frame data for processing
    cv::Mat frame_data_cv_;
};
