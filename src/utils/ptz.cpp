#include "ptz.h"


PTZPreset::PTZPreset(std::string name, std::string token)
  : name(name),
    token(token)
{}


PTZNode::PTZNode(std::string name, std::string token)
  : name(name),
    token(token)
{}

PTZNode::~PTZNode()
{}
