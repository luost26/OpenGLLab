#ifndef APP_GYMAPP
#define APP_GYMAPP

#include "App.hpp"
#include "../Scene/Gym.hpp"

namespace Showcase {
	class GymApp : public App {
	public:
		GymApp(): App(1024, 768, "Gym", 
			WindowHintList::create()
			->contextVersionMajor(4)
			->contextVersionMinor(1)
			->openGLProfile(GLFW_OPENGL_CORE_PROFILE)
			->openGLForwardCompatability(GL_TRUE)
			->samples(8)
		) {
			
			GL::enableDepthTest();
			GL::enable(GL_MULTISAMPLE);
			GL::enable(GL_CULL_FACE);
			glCullFace(GL_BACK);

		}

		int run() {
			const int camera_ubo_id = 0;
			const int spotlight_ubo_id = 1;

			UniformBuffer * cameraUBO = new UniformBuffer();
			cameraUBO->bind()
				->load(2*sizeof(glm::mat4)+sizeof(glm::aligned_vec3), NULL, GL_STATIC_DRAW)
				->bindRange(camera_ubo_id)->unbind();

			Gym * scene = new Gym(spotlight_ubo_id);
			Program * common_program = simple_shader_program(
				shader_path("object/common.vert"),
				shader_path("object/common_msm.frag")
			);

			
			camera->setPosition(glm::vec3(17.3f, 2.2f, -9.5f));

			SceneDrawConfig draw_config;
			draw_config.illumination = true;

			while (!window->shouldClose()) {
				stopwatch();
				processInput(window);
				GL::clear(cleaners);

				/* Upload camera parameters */
				cameraUBO->bind()
					->loadSub(0, sizeof(glm::mat4), glm::value_ptr(camera->projectionMatrix()))
					->loadSub(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera->viewMatrix()))
					->loadSub(2*sizeof(glm::mat4), sizeof(glm::aligned_vec3), glm::value_ptr(camera->position()))
					->unbind();
				common_program->use()->setUniformBlockBinding("Camera", camera_ubo_id);

				/* Draw scene */
				scene->draw(common_program, draw_config);

				GLFW::swapBuffers(window);
				GLFW::pollEvents();
			}

			return 0;
		}
	};
}

#endif // !APP_GYMAPP
