#pragma once

#include "ImageNode.hpp"

class ImageSourceNode : public ImageNode
{
public:
    ImageSourceNode();

    void draw() override;

    void loadSource(const char* file_path);
};
