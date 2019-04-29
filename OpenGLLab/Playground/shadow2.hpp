//
// Created by 罗 世通 on 2019-04-28.
//

#ifndef OPENGLLAB_SHADOW2_HPP
#define OPENGLLAB_SHADOW2_HPP

namespace playground {

	struct GymSpotLight {
		glm::aligned_vec3 ambient;
		glm::aligned_vec3 diffuse;
		glm::aligned_vec3 specular;

		glm::aligned_vec3 position;
		glm::aligned_vec3 direction;

		glm::aligned_vec3 attenuation;
		glm::aligned_vec2 cutoff;


		GymSpotLight(const glm::vec3 & pos) {
			ambient = glm::vec3(0.3f, 0.3f, 0.3f);
			diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
			specular = glm::vec3(0.3f, 0.3f, 0.3f);

			position = pos;
			direction = glm::vec3(0.0f, -1.0f, 0.0f);

			attenuation = glm::vec3(1.0f, 0.007f, 0.0002f);
			cutoff = glm::vec2(glm::cos(glm::radians(30.0f)), glm::cos(glm::radians(60.0f)));
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
			lights.push_back(GymSpotLight(glm::vec3(20.67f, 10.83f, -9.63f)));
		}

		void initUBO(int ubo_range) {
			UBO = new UniformBuffer();
			uboRange = ubo_range;
			UBO->bind()->load(lights.size() * sizeof(GymSpotLight), &lights[0], GL_STATIC_DRAW)->bindRange(ubo_range)->unbind();
		}

		void apply(Program * prog) {
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
			spotLights->initUBO(1);
			pointLights = new GymPointLightArray();
			pointLights->initUBO(2);
        }

        void draw(Program * prog) {

			spotLights->apply(prog);
			pointLights->apply(prog);

            prog->setMatrix4("model", glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f)) );
            model->draw(prog);

        }
    };

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

            getCamera()->setPosition(glm::vec3(17.3f, 2.2f, -9.5f));

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

                common_program->use();
                common_program->setUniformBlockBinding("Camera", 0);

                scene->draw(common_program);

                GLFW::swapBuffers(window);
                GLFW::pollEvents();

            }

            return 0;
        }

    };

}


#endif //OPENGLLAB_SHADOW2_HPP
