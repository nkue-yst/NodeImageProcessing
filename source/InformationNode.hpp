#pragma once

#include "ImageNode.hpp"

class InformationNode : public ImageNode
{
public:
    InformationNode();

    void draw() override;
};
