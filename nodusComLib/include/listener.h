#pragma once

#include <string>
#include "nlohmann/json.hpp"

namespace com
{
    nlohmann::json handle_request(const std::string &request);
}