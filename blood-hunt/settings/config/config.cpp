#include "config.hpp"

std::unordered_map<const char*, std::variant<bool, int, float, engine::structs::vector4_t>> features {
	{"esp", false},
	{"health_esp", false},
	{"distance", false},
	{"tracers", false},
	{"draw_fov", false},
	{"draw_name", false},

	{"aimbot", false},
	{"smoothing", 1},
	{"aimbot_fov", 1},
	{"antiaim", false},
	{"slidefly", false},

	{"esp_color", engine::structs::vector4_t{1.f,1.f,1.f,1.f}},
	{"health_color", engine::structs::vector4_t{0.f,1.f,0.f,1.f}},
	{"distance_color", engine::structs::vector4_t{1.f,1.f,1.f,1.f}},
	{"tracers_color", engine::structs::vector4_t{1.f,1.f,1.f,1.f}},
	{"fov_color", engine::structs::vector4_t{1.f,1.f,1.f,1.f}},
	{"name_color", engine::structs::vector4_t{1.f,1.f,1.f,1.f}},

	{"team_check", false}
};