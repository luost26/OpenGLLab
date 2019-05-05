//
// Created by luost on 2019/5/1.
//

#ifndef OPENGLLAB_GYM_HPP
#define OPENGLLAB_GYM_HPP

#include <wglfw/wglfw.hpp>
#include <vector>

#include "Scene.hpp"
#include "../Light/SpotLight.hpp"
#include "../Utility/helper.hpp"

using namespace wglfw;

namespace Showcase {

    class Gym : public Scene {
	private:
		SpotLightArray * spotLights;
		AssimpModel * gymModel;

		void initSpotLights() {
			glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
			glm::vec3 diffuse = glm::vec3(0.75f, 0.75f, 0.75f);
			glm::vec3 specular = glm::vec3(0.8f, 0.8f, 0.8f);

			glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

			glm::vec3 attenuation = glm::vec3(1.0f, 0.014f, 0.0007f);
			glm::vec2 cutoff = glm::vec2(glm::cos(glm::radians(30.0f)), glm::cos(glm::radians(60.0f)));

			std::vector<glm::vec3> positions{
				glm::vec3(27.358, 9.783, -16.737),
				glm::vec3(14.046, 9.783, -16.626),
				glm::vec3(20.751, 9.783, -29.489)
			};

			std::vector<SpotLight> lights;

			for (auto pos : positions) {
				SpotLight light;
				light.ambient = ambient;
				light.diffuse = diffuse;
				light.specular = specular;

				light.direction = direction;

				light.attenuation = attenuation;
				light.cutoff = cutoff;

				light.position = pos;

				light.shadow_enabled = 0;

				lights.push_back(light);
			}

			spotLights = new SpotLightArray(lights);
		}
	
		void initGymModel() {
			gymModel = AssimpModel::fromFile(model_path("gym/gym.obj").c_str());
		}

	public:

		Gym() {
			initGymModel();
			initSpotLights();
		}

		void uploadLights() {
			spotLights->uploadAllLights();
		} 

		SpotLightArray * spotLightArray() {
			return spotLights;
		}

		void draw(Program * prog, SceneDrawConfig config) {
			
			prog->use();

			if (config.illumination) {
				spotLights->apply(prog);
			}
			prog->setMatrix4("model", glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
			gymModel->draw(prog);
		}
	

    };

}

#endif //OPENGLLAB_GYM_HPP
