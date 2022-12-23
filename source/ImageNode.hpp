#pragma once

#include "SDL.h"

#include "GL/gl.h"

#include "NodeBase.hpp"

class ImageNode : public NodeBase
{
public:
    ImageNode(int32_t id);

    void draw() override;

private:
    GLuint image_data_;
};
