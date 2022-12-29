#pragma once

#include "VideoNode.hpp"

class VideoSourceNode : public VideoNode
{
public:
    VideoSourceNode();

    void draw() override;

    void loadSource(const char* file_path);
};
