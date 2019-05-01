
#ifndef SHOWCASE_SHADOW_SHADOWMAPPER
#define SHOWCASE_SHADOW_SHADOWMAPPER

#include <stdio.h>
#include <vector>
#include <wglfw/wglfw.hpp>

#include "ShadowMap.hpp"
#include "../Light/SpotLight.hpp"
#include "../Scene/Scene.hpp"

using namespace wglfw;

namespace Showcase {

	class ShadowMapper {
	protected:
		FrameBuffer * FBO;
		bool _release_FBO;

		ShadowStorage * shadowStorage;
	public:
		ShadowMapper(ShadowStorage * store, FrameBuffer * fb = NULL) {
			shadowStorage = store;
			if (fb == NULL) {
				fb = new FrameBuffer();
				_release_FBO = true;
			} else {
				_release_FBO = false;
			}
			FBO = fb;
		}

		~ShadowMapper() {
			if (_release_FBO) delete FBO;
		}

		virtual int mapForSpotLight(SpotLight * spotlight, Scene * scene) = 0;

		
	};

}

#endif // !SHOWCASE_SHADOW_SHADOWMAPPER
