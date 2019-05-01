
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
		int previousViewport[4];
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

		void saveViewport() {
			glGetIntegerv(GL_VIEWPORT, previousViewport);
		}

		void resumeViewport() {
			GL::setViewport(previousViewport[0], previousViewport[1], previousViewport[2], previousViewport[3]);
		}
		
	};

}

#endif // !SHOWCASE_SHADOW_SHADOWMAPPER
