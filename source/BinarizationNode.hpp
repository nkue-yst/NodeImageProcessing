#pragma once

#include "GL/gl.h"

#include "ImageNode.hpp"

class BinarizationNode : public ImageNode
{
public:
    BinarizationNode();

    void connect() override;

private:
    // Execute binarization
    void binarization();
};
