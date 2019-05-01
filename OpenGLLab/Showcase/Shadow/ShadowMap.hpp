
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

	class ShadowStorage {
	private:
		std::vector<ShadowMap*> maps;
		std::vector<glm::mat4> lightSpaces;
		int shadowMapsLimit;
		int shadowMapsUnitOffset;
	public:
		ShadowStorage(int limit = 12, int unit = 4) {
			shadowMapsLimit = limit;
			shadowMapsUnitOffset = unit;
		}

		ShadowMap * getShadowMap(int idx) {
			if (idx >= maps.size())
				return NULL;
			return maps[idx];
		}

		int addShadow(ShadowMap * map, glm::mat4 mat) {
			maps.push_back(map);
			lightSpaces.push_back(mat);
			return maps.size() - 1;
		}

		void updateShadow(int idx, ShadowMap * map, glm::mat4 mat) {
			maps[idx] = map;
			lightSpaces[idx] = mat;
		}

		bool isFull() {
			return maps.size() >= shadowMapsLimit;
		}

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

#endif // !SHOWCASE_SHADOW_SHADOWMAP
