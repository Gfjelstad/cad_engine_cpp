#pragma once

#include <string>
#include "nlohmann/json.hpp"

namespace rpc {
    nlohmann::json handle_request(const std::string& request);
}