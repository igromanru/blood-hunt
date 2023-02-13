#pragma once
#include <variant>
#include <unordered_map>

#include "../../engine/structures/structures.hpp"

#include "../../dependencies/imgui/imgui.h"

extern std::unordered_map<const char*, std::variant<bool, int, float, engine::structs::vector4_t>> features;
