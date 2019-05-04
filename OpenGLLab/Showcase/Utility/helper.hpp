
#ifndef SHOWCASE_UTILITY_HELPER_HPP
#define SHOWCASE_UTILITY_HELPER_HPP

#include <string>

namespace Showcase {

	std::string shader_path(const char * fn) {
		return std::string("./Resources/Shaders/") + fn;
	}

	std::string texture_path(const char * fn) {
		return  std::string("./Resources/Textures/") + fn;
	}

	std::string model_path(const char * fn) {
		return std::string("./Resources/Models/") + fn;
	}

}

#endif // !SHOWCASE_UTILITY_HELPER_HPP
