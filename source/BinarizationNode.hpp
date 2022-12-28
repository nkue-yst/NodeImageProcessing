#pragma once

#include "ImageNode.hpp"

class BinarizationNode : public ImageNode
{
public:
    BinarizationNode();

    void updateData() override;

private:
    // Execute binarization
    void binarization();
};
