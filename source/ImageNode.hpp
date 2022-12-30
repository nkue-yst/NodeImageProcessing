#pragma once

#include "GL/gl.h"

#include <opencv2/opencv.hpp>

#include "NodeBase.hpp"

class ImageNode : public NodeBase
{
public:
    static ImageNode* create(NodeType type);

    virtual void draw() override;

protected:
    void drawImage();

    GLuint convertCVmatToGLtexture(cv::Mat* mat);

public:
    // Image data for drawing
    GLuint image_data_gl_ = (GLuint)(uintptr_t)nullptr;

    // Image data for processing
    cv::Mat image_data_cv_;

    int32_t width_ = 100;
    int32_t height_ = 100;
};
