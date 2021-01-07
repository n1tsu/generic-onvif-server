#pragma once

#include "utils.h"

tt__PTZNode *to_gsoap(soap *soap, PTZNode *node);

tt__PTZConfiguration *to_gsoap(soap *soap, std::shared_ptr<PTZConfiguration> configuration);

tt__VideoEncoderConfiguration *to_gsoap(soap *soap, std::shared_ptr<EncoderConfiguration> configuration);

tt__VideoSourceConfiguration *to_gsoap(soap *soap, std::shared_ptr<VideoConfiguration> configuration);

tt__Profile *to_gsoap(soap *soap, Profile *profile);

tt__PTZPreset *to_gsoap(soap *soap, PTZPreset *preset);
