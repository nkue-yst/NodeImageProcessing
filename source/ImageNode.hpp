#pragma once

#include "NodeBase.hpp"

class ImageNode : public NodeBase
{
public:
    ImageNode(int32_t id);

    void draw() override;
};
