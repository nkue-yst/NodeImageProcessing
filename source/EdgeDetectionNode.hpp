#pragma once

#include "ImageNode.hpp"

class EdgeDetectionNode : public ImageNode
{
public:
    EdgeDetectionNode();

    void updateData() override;

    void drawOptions() override;

private:
    // Execute edge detection
    void edgeDetection();

    // Original image data
    cv::Mat original_image_;

    // edge detection parameter
    int32_t threshold[2];
};
