#include "BinarizationNode.hpp"

#include "imgui.h"
#include "imnodes.h"

BinarizationNode::BinarizationNode()
{
    this->setPinNum(1, 1);
}

void BinarizationNode::connect()
{
    this->binarization();
}

void BinarizationNode::binarization()
{

}
