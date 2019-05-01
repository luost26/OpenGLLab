
#ifndef SHOWCASE_SHADOW_SHADOWMAP
#define SHOWCASE_SHADOW_SHADOWMAP

#include <wglfw/wglfw.hpp>

namespace Showcase {

	class ShadowMap {
	protected:
		Texture2D * map;
		bool _release_map;
	public:
		ShadowMap(Texture2D * tex) {
			map = tex;
			_release_map = false;
		}

		ShadowMap(unsigned int size, GLenum format, GLenum internal_format) {
			map = new Texture2D();
			map->bind()->empty(size, size, format, GL_FLOAT, internal_format)
				->minFilter(GL_NEAREST)->magFilter(GL_NEAREST)
				->wrapS(GL_CLAMP_TO_EDGE)->wrapT(GL_CLAMP_TO_EDGE)
				->borderColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
				->unbind();
			_release_map = true;
		}

		~ShadowMap() {
			if (_release_map) delete map;
		}

		Texture2D * texture() {
			return map;
		}
	};

}

#endif // !SHOWCASE_SHADOW_SHADOWMAP
