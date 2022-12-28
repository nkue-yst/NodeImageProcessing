#pragma once

#include "GL/gl.h"

#include "ImageNode.hpp"

class GrayScalingNode : public ImageNode
{
public:
    GrayScalingNode();

    void updateData() override;

private:
    // Execute grayscaling
    void grayscaling();
};
