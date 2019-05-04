#ifndef APP_GYMAPP
#define APP_GYMAPP

#include "App.hpp"
#include "../Scene/Gym.hpp"
#include "../Shadow/MSMShadowMapper.hpp"
#include "../Utility/IlluminationBuffer.hpp"

namespace Showcase {
	class GymApp : public App {
	public:
		GymApp(): App(1024, 768, "Gym", 
			WindowHintList::create()
			->contextVersionMajor(4)
			->contextVersionMinor(1)
			->openGLProfile(GLFW_OPENGL_CORE_PROFILE)
			->openGLForwardCompatability(GL_TRUE)
		) {
			
			GL::enableDepthTest();
			GL::enable(GL_MULTISAMPLE);
			GL::enable(GL_CULL_FACE);
			glCullFace(GL_BACK);

		}

		int run() {

			UniformBuffer * cameraUBO = new UniformBuffer();
			int camera_ubo_range = UniformBufferRangeManager::global()->getRange(cameraUBO);
			cameraUBO->bind()
				->load(2*sizeof(glm::mat4)+sizeof(glm::aligned_vec3), NULL, GL_STATIC_DRAW)
				->bindRange(camera_ubo_range)->unbind();

			Gym * scene = new Gym();
			Program * common_program = simple_shader_program(
				shader_path("object/common.vert"),
				shader_path("object/common_msm_illum.frag")
			);

			/* Framebuffers should be created early (before uploading textures and others to shader program) otherwise weired bugs will appear */
			IlluminationBufferMultisample * illum_buffer = new IlluminationBufferMultisample(width, height, 8);
			CleanerCollection * illum_buffer_cleaners = new CleanerCollection();
			illum_buffer_cleaners
				->add(new ColorBufferCleaner(glm::vec4(0.1f/3.0f, 0.1f/3.0f, 0.1f/3.0f, 1.0f/3.0f)))
				->add(new DepthBufferCleaner);


			ScreenQuad * debug_quad = new ScreenQuad(NULL, load_fragment_shader(shader_path("debug_quad.frag")));

			ShadowStorage * shadow_storage = new ShadowStorage();
			MSMShadowMapper * shadow_mapper = new MSMShadowMapper(shadow_storage);
			shadow_mapper->mapForSpotLight(scene->spotLightArray()->lightPtr(0), scene);
			shadow_mapper->mapForSpotLight(scene->spotLightArray()->lightPtr(1), scene);

			scene->uploadLights();
			shadow_storage->uploadAllShadowMaps(common_program);

			delete shadow_mapper;

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
				common_program->use()->setUniformBlockBinding("Camera", camera_ubo_range);

				/* Draw scene */
				illum_buffer->bind(illum_buffer_cleaners);
				scene->draw(common_program, draw_config);
				illum_buffer->unbind();


				illum_buffer->mergeDisplay();

				//debug_quad->displayTexture(shadow_mapper->maps[0]->texture());

				printInfo();
				GLFW::swapBuffers(window);
				GLFW::pollEvents();
			}

			return 0;
		}
	};
}

#endif // !APP_GYMAPP
