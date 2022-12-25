#pragma once

#include "GL/gl.h"

#include "ImageNode.hpp"

class GrayScalingNode : public ImageNode
{
public:
    GrayScalingNode();

    void connect(NodeBase* node) override;
    void disconnect(NodeBase* node) override;

private:
    // Execute grayscaling
    void grayscaling();
};
