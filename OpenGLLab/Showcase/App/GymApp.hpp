#ifndef APP_GYMAPP
#define APP_GYMAPP

#include "App.hpp"
#include "../Scene/Gym.hpp"
#include "../Shadow/MSMShadowMapper.hpp"
#include "../Utility/IlluminationBuffer.hpp"
#include "../Utility/GBuffer.hpp"
#include "../GI/SSAO.hpp"

namespace Showcase {
	class GymApp : public App {
	public:
		GymApp(): App(1366, 768, "Gym", 
			WindowHintList::create()
			->contextVersionMajor(4)
			->contextVersionMinor(1)
			->openGLProfile(GLFW_OPENGL_CORE_PROFILE)
			->openGLForwardCompatability(GL_TRUE)
			->set(GLFW_RESIZABLE, GL_FALSE)
		) {
			
			GL::enableDepthTest();
			GL::enable(GL_MULTISAMPLE);

		}

		void processInput(Window *window) {
			App::processInput(window);

			if (window->getKey(GLFW_KEY_Z) == GLFW_PRESS)
				SSAOEnabled = true;
			if (window->getKey(GLFW_KEY_X) == GLFW_PRESS)
				SSAOEnabled = false;
		}

		bool SSAOEnabled;

		int run() {
			SSAOEnabled = true;

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
			IlluminationBufferMultisample * illum_buffer = new IlluminationBufferMultisample(width, height, 16);
			CleanerCollection * illum_buffer_cleaners = new CleanerCollection();
			illum_buffer_cleaners
				->add(new ColorBufferCleaner(glm::vec4(0.1f/3.0f, 0.1f/3.0f, 0.1f/3.0f, 1.0f/3.0f)))
				->add(new DepthBufferCleaner);

			GBuffer * g_buffer = new GBuffer(width, height);
			
			SSAO * ssao = new SSAO(width, height);

			ScreenQuad * debug_quad = new ScreenQuad(NULL, load_fragment_shader(shader_path("debug_quad.frag")));

			ShadowStorage * shadow_storage = new ShadowStorage();
			MSMShadowMapper * shadow_mapper = new MSMShadowMapper(shadow_storage);
			shadow_mapper->mapForSpotLight(scene->spotLightArray()->lightPtr(0), scene);
			shadow_mapper->mapForSpotLight(scene->spotLightArray()->lightPtr(1), scene);
			shadow_mapper->mapForSpotLight(scene->spotLightArray()->lightPtr(2), scene);

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

				/* Fill GBuffer*/
				g_buffer->bind();
				scene->draw(g_buffer->getShaderProgram(), SceneDrawConfig::configForGBuffer());
				g_buffer->unbind();

				/* Create SSAO texture */
				Texture2D *ao_texture = ssao->create(g_buffer, cameraUBO);

				GL::enable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				/* Fill illum buffer */ 
				illum_buffer->bind(illum_buffer_cleaners);
				scene->draw(common_program, draw_config);
				illum_buffer->unbind(); 
				GL::disable(GL_BLEND);

				if (SSAOEnabled) illum_buffer->setAmbientOcclusion(ao_texture);
				else illum_buffer->disableAmbientOcclusion();
				 
				illum_buffer->mergeDisplay();

				//debug_quad->displayTexture(shadow_storage->getShadowMap(1)->texture());
			
				printInfo();
				GLFW::swapBuffers(window);
				GLFW::pollEvents();
			}

			return 0;
		}
	};
}

#endif // !APP_GYMAPP
