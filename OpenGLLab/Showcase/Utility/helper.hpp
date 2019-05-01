
#ifndef SHOWCASE_UTILITY_HELPER_HPP
#define SHOWCASE_UTILITY_HELPER_HPP

#include <string>

namespace Showcase {

	std::string shader_path(const char * fn) {
		std::string base = SHOWCASE_SHADER_DIR;
		return base + "/" + fn;
	}

	std::string texture_path(const char * fn) {
		std::string base = SHOWCASE_TEXTURE_DIR;
		return base + "/" + fn;
	}

	std::string model_path(const char * fn) {
		std::string base = SHOWCASE_MODEL_DIR;
		return base + "/" + fn;
	}

}

#endif // !SHOWCASE_UTILITY_HELPER_HPP
