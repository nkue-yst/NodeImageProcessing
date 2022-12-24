#pragma once

#include "GL/gl.h"

#include "NodeBase.hpp"

class ImageNode : public NodeBase
{
public:
    static ImageNode* create(NodeType type);

    void draw() override;

protected:
    GLuint image_data_;
};
