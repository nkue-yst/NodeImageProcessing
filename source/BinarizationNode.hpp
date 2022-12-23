#pragma once

#include "GL/gl.h"

#include "NodeBase.hpp"

class BinarizationNode : public NodeBase
{
public:
    BinarizationNode(int32_t id, std::vector<int32_t> pins);

    void draw() override;

private:
    GLuint image_data_;
};
