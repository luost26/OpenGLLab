//
// Created by 罗 世通 on 2019-04-28.
//

#ifndef OPENGLLAB_SHADOW2_HPP
#define OPENGLLAB_SHADOW2_HPP

#include "base.hpp"
#include "Objects/ScreenQuad.hpp"

namespace playground {

	struct GymSpotLight {
		glm::aligned_vec3 ambient;
		glm::aligned_vec3 diffuse;
		glm::aligned_vec3 specular;

		glm::aligned_vec3 position;
		glm::aligned_vec3 direction;

		glm::aligned_vec3 attenuation;
		glm::aligned_vec2 cutoff;

		bool shadow_enabled;
		int shadow_map_index;

		GymSpotLight() {}

		GymSpotLight(const glm::vec3 & pos) {
			ambient = glm::vec3(0.3f, 0.3f, 0.3f);
			diffuse = glm::vec3(0.9f, 0.9f, 0.9f);
			specular = glm::vec3(1.0f, 1.0f, 1.0f);

			position = pos;
			direction = glm::vec3(0.0f, -1.0f, 0.0f);

			attenuation = glm::vec3(1.0f, 0.014f, 0.0007f);
			cutoff = glm::vec2(glm::cos(glm::radians(30.0f)), glm::cos(glm::radians(60.0f)));

			shadow_enabled = 0.0f;
		}
	};

	struct GymPointLight {
		glm::aligned_vec3 ambient;
		glm::aligned_vec3 diffuse;
		glm::aligned_vec3 specular;
		glm::aligned_vec3 position;

		glm::aligned_vec3 attenuation;

		GymPointLight(const glm::vec3 & pos) {
			ambient = glm::vec3(0.3f, 0.3f, 0.3f);
			diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
			specular = glm::vec3(0.3f, 0.3f, 0.3f);

			position = pos;

			attenuation = glm::vec3(1.0f, 0.007f, 0.0002f);
		}
	};

	class GymSpotLightArray {
	public:
		std::vector<GymSpotLight> lights;
		UniformBuffer * UBO;
		int uboRange;

		GymSpotLightArray() {
			lights.push_back(GymSpotLight(glm::vec3(24.092, 10.141, -13.476)));
		}

		void initUBO(int ubo_range) {
			UBO = new UniformBuffer();
			uboRange = ubo_range;
			UBO->bind()->load(lights.size() * sizeof(GymSpotLight), &lights[0], GL_STATIC_DRAW)->bindRange(ubo_range)->unbind();
		}

		void apply(Program * prog) {
			prog->use();
			prog->setInt("numberOfSpotLights", lights.size());
			prog->setUniformBlockBinding("SpotLightsBlock", uboRange);
		}
	};

	class GymPointLightArray {
	public:
		std::vector<GymPointLight> lights;
		UniformBuffer * UBO;
		int uboRange;

		GymPointLightArray() {
			//lights.push_back(GymPointLight(glm::vec3(17.3f, 2.2f, -9.5f)));
			//lights.push_back(GymPointLight(glm::vec3(10.605f, 2.159f, -3.2744f)));
		}

		void initUBO(int ubo_range) {
			UBO = new UniformBuffer();
			uboRange = ubo_range;
			UBO->bind()->load(lights.size() * sizeof(GymPointLight), &lights[0], GL_STATIC_DRAW)->bindRange(ubo_range)->unbind();
		}

		void apply(Program * prog) {
			prog->use();
			prog->setInt("numberOfPointLights", lights.size());
			prog->setUniformBlockBinding("PointLightsBlock", uboRange);
		}

	};

	class Shadow2Scene {
	public:
		AssimpModel * model;
		GymSpotLightArray * spotLights;
		GymPointLightArray * pointLights;

		Shadow2Scene() {
			model = AssimpModel::fromFile(model_path("gym/gym.obj").c_str());
			spotLights = new GymSpotLightArray();
			pointLights = new GymPointLightArray();
		}

		void uploadLights() {
			spotLights->initUBO(1);
			pointLights->initUBO(2);
		}

		void draw(Program * prog, bool illum = true) {

			prog->use();

			if (illum) {
				prog->setVec3("viewPos", Base::getCamera()->position());  // Important, dont forget to set viewPos, otherwise specular illumination will not show
				spotLights->apply(prog);
				pointLights->apply(prog);
			}

			prog->setMatrix4("model", glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
			model->draw(prog);

		}
	};

	class GymSpotLightShadowMapper {
	private:
		GymSpotLightShadowMapper(GymSpotLight * _light) {
			light = _light;
			index = countShadowMaps;
			++countShadowMaps;
		}
	public:
		FrameBuffer * FBO;
		Texture2D * map;
		glm::mat4 lightSpace;
		GymSpotLight * light;
		int mapSize;
		int index;

		const static int shadowMapsLimit = 12;
		static int countShadowMaps;

		static GymSpotLightShadowMapper * create(GymSpotLight * _light) {
			if (countShadowMaps >= shadowMapsLimit)
				return NULL;
			return new GymSpotLightShadowMapper(_light);
		}

		GymSpotLightShadowMapper * init(int shadow_map_size=8192) {
			mapSize = shadow_map_size;
			FBO = new FrameBuffer();
			map = new Texture2D();
			map->bind()->empty(mapSize, mapSize, GL_DEPTH_COMPONENT, GL_FLOAT)
				->minFilter(GL_NEAREST)->magFilter(GL_NEAREST)
				->wrapS(GL_CLAMP_TO_EDGE)->wrapT(GL_CLAMP_TO_EDGE)
				->borderColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			FBO->bind()->attachTexture2D(map, GL_DEPTH_ATTACHMENT)
				->setDrawBuffer(GL_NONE)
				->setReadBuffer(GL_NONE)
				->unbind();
			return this;
		}


		GymSpotLightShadowMapper * draw(Program * prog, Shadow2Scene * scene) {
			glm::mat4 light_proj, light_view, light_space;
			float aspect = 1.0f;
			float _near = 1.0f;
			float _far = 25.0f;
			light_proj = glm::perspective(glm::radians(120.0f), aspect, _near, _far);
			light_view = glm::lookAt(
				glm::vec3(light->position.x, light->position.y, light->position.z),
				glm::vec3(light->position.x, 0.0f, light->position.z),
				glm::vec3(0.0f,0.0f,-1.0f)
			);
			
			light_space = light_proj * light_view;
			lightSpace = light_space;

			glDisable(GL_CULL_FACE);

			GL::setViewport(0, 0, mapSize, mapSize);
			FBO->bind();
			glClear(GL_DEPTH_BUFFER_BIT);

			prog->use()->setMatrix4("lightSpace", light_space);  // upload light space matrix to depth map generation shader
			scene->draw(prog, false);

			FBO->unbind();

			GL::setViewport(0, 0, Base::getDefaultScreenWidth(), Base::getDefaultScreenHeight(), VIEWPORT_SCALE_FACTOR);

			glEnable(GL_CULL_FACE);

			return this;
		}

		GymSpotLightShadowMapper * bind(Program * prog) {
			light->shadow_enabled = true;
			light->shadow_map_index = index;
			
			int unit = 4 + index;
			map->bindToTextureUnit(TextureUnit::get(unit));
			char idxstr[5];
			itoa(index, idxstr, 10);

			prog->use();  // Notice! Activate the program before uploading data (especially texture)
			prog->setTexture((std::string("shadowMaps[") + idxstr + "]").c_str(), TextureUnit::get(unit));
			prog->setMatrix4((std::string("lightSpaces[") + idxstr + "]").c_str(), lightSpace);

			return this;
		}

	};

	int GymSpotLightShadowMapper::countShadowMaps = 0;

    class Shadow2 : public Base {
    public:
        static int main() {

            Window * window = initializeEnvAndCreateWindow(WindowHintList::create()
				->contextVersionMajor(4)
				->contextVersionMinor(1)
				->openGLProfile(GLFW_OPENGL_CORE_PROFILE)
				->openGLForwardCompatability(GL_TRUE)
				->samples(8));
            GL::enableDepthTest();

			glEnable(GL_MULTISAMPLE);
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

            UniformBuffer * cameraUBO = new UniformBuffer();
            cameraUBO->bind()->load(2*sizeof(glm::mat4), NULL, GL_STATIC_DRAW)->bindRange(0)->unbind();

            Shadow2Scene * scene = new Shadow2Scene();

            Program * common_program = simple_shader_program(shader_path("shadow2/object.vert"), shader_path("shadow2/object.frag"));
			Program * shadow_mapping_program = simple_shader_program(shader_path("shadow2/depth_mapping.vert"), shader_path("shadow2/depth_mapping.frag"));
			
			ScreenQuad * debug_quad = new ScreenQuad(NULL, load_fragment_shader(shader_path("shadow2/debug_quad.frag")));

			GymSpotLightShadowMapper * shadow_mapper = GymSpotLightShadowMapper::create(&scene->spotLights->lights[0]);
			shadow_mapper->init()->draw(shadow_mapping_program, scene)
				->bind(common_program);

			scene->uploadLights();

			getCamera()->setPosition(glm::vec3(17.3f, 2.2f, -9.5f));

			printf("Offset of shadow_enabled: %d\n", offsetof(GymSpotLight, shadow_enabled));

            while (!window->shouldClose()) {
                stopwatch();
                processInput(window);
                GL::clear(getDefaultCleanerCollection());

                printf("CamPos %.3f,%.3f,%.3f\r", getCamera()->position().x, getCamera()->position().y, getCamera()->position().z);

                /* Upload camera parameters */
                cameraUBO->bind()
                        ->loadSub(0, sizeof(glm::mat4), glm::value_ptr(getCamera()->projectionMatrix()))
                        ->loadSub(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(getCamera()->viewMatrix()))
                        ->unbind();

				scene->draw(common_program);

				// debug_quad->draw(shadow_mapper->map);

                GLFW::swapBuffers(window);
                GLFW::pollEvents();

            }

            return 0;
        }

    };

}


#endif //OPENGLLAB_SHADOW2_HPP
