#pragma once

#include <cstdint>

class NodeBase
{
public:
    NodeBase(int32_t id)
        : id_(id)
    {
    }

    virtual void draw() = 0;

    int32_t getID() const { return this->id_; }

private:
    int32_t id_;
};
