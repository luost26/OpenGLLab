
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

		std::vector<ShadowMap*> maps;
		std::vector<glm::mat4> lightSpaces;
		int shadowMapsLimit;
		int shadowMapsUnitOffset;
	public:
		ShadowMapper(int limit=12, int unit=4, FrameBuffer * fb = NULL) {
			shadowMapsLimit = limit;
			shadowMapsUnitOffset = unit;
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
			for (auto map : maps) {
				delete map;
			}
		}

		int mapForSpotLight(SpotLight * spotlight, Scene * scene) = 0;

		void uploadAllShadowMaps(Program * prog) {
			prog->use();

			for (int idx = 0; idx < maps.size(); ++idx) {
				int unit = shadowMapsUnitOffset + idx;
				ShadowMap * map = maps[idx];
				map->texture()->bindToTextureUnit(TextureUnit::get(unit));

				char idxstr[5];
				sprintf(idxstr, "%d", idx);

				prog->setTexture((std::string("shadowMaps[") + idxstr + "]").c_str(), TextureUnit::get(unit));
				prog->setMatrix4((std::string("lightSpaces[") + idxstr + "]").c_str(), lightSpaces[idx]);
			}
		}

		void uploadShadowMap(Program * prog, int idx) {
			prog->use();

			int unit = shadowMapsUnitOffset + idx;
			ShadowMap * map = maps[idx];
			map->texture()->bindToTextureUnit(TextureUnit::get(unit));

			char idxstr[5];
			sprintf(idxstr, "%d", idx);

			prog->setTexture((std::string("shadowMaps[") + idxstr + "]").c_str(), TextureUnit::get(unit));
			prog->setMatrix4((std::string("lightSpaces[") + idxstr + "]").c_str(), lightSpaces[idx]);
		}
	};

}

#endif // !SHOWCASE_SHADOW_SHADOWMAPPER
