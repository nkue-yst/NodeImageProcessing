#pragma once

#include <cstdint>
#include <vector>

class NodeBase
{
public:
    NodeBase(int32_t id, std::vector<int32_t> pins)
        : id_(id)
        , pin_list_(pins)
    {
    }

    virtual void draw() = 0;

    int32_t getID() const { return this->id_; }

protected:
    std::vector<int32_t> pin_list_;

private:
    int32_t id_;
};
