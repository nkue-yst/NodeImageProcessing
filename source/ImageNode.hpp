#pragma once

#include "GL/gl.h"

#include <opencv2/opencv.hpp>

#include "NodeBase.hpp"

class ImageNode : public NodeBase
{
public:
    static ImageNode* create(NodeType type);

    void draw() override;

protected:
    GLuint convertCVmatToGLtexture(cv::Mat* mat);

public:
    // Image data for drawing
    GLuint image_data_gl_;

    // Image data for processing
    cv::Mat image_data_cv_;
};
