//
// Created by luost on 2019/5/1.
//

#ifndef OPENGLLAB_SPOTLIGHT_HPP
#define OPENGLLAB_SPOTLIGHT_HPP

namespace Showcase {

    struct SpotLight {
        glm::aligned_vec3 ambient;
        glm::aligned_vec3 diffuse;
        glm::aligned_vec3 specular;

        glm::aligned_vec3 position;
        glm::aligned_vec3 direction;

        glm::aligned_vec3 attenuation;
        glm::aligned_vec2 cutoff;

        int shadow_enabled;
        int shadow_map_index;

		SpotLight() {}

    };

    class SpotLightArray {
	private:
		std::vector<SpotLight> lights;
		UniformBuffer * UBO;
		int uboRange;

		void initUBO(UniformBufferRangeManager * ubo_range_manager) {
			UBO = new UniformBuffer();
			uboRange = ubo_range_manager->getRange((void*)this);
		}
    public:
		SpotLightArray(std::vector<SpotLight> _lights, UniformBufferRangeManager * ubo_range_manager=UniformBufferRangeManager::global()) {
			lights = _lights;
			initUBO(ubo_range_manager);
        }

		~SpotLightArray() {
			delete UBO;
		}

		void uploadAllLights() {
			UBO->bind()
				->load(lights.size() * sizeof(SpotLight), &lights[0], GL_STATIC_DRAW)
				->bindRange(uboRange)
				->unbind();
		}

		void uploadLight(int idx) {
			UBO->bind()
				->loadSub(idx*sizeof(SpotLight), sizeof(SpotLight), &lights[idx])
				->unbind();
		}

		SpotLight * lightPtr(int idx) {
			return &lights[idx];
		}

        void apply(Program * prog) {
            prog->use();
            prog->setInt("numberOfSpotLights", lights.size());
            prog->setUniformBlockBinding("SpotLightsBlock", uboRange);
        }

    };

}

#endif //OPENGLLAB_SPOTLIGHT_HPP
