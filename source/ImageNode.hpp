#pragma once

#include "GL/gl.h"

#include "NodeBase.hpp"

class ImageNode : public NodeBase
{
public:
    ImageNode(int32_t id, std::vector<int32_t> pins);

    void draw() override;

private:
    GLuint image_data_;
};
