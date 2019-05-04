
#ifndef APP_APP
#define APP_APP

#include <string>
#include <wglfw/wglfw.hpp>

using namespace wglfw;

namespace Showcase {

	class App {
	protected:
		static App * instance;
		Camera * camera;
		CleanerCollection * cleaners;
		float deltaTime, lastFrame;
		int width, height;
		Window * window;
		WindowHintList * windowHints;
		std::string windowTitle;
	public:
		App(int w=1024, int h=768, std::string title="Showcase", WindowHintList * hints=NULL) {
			/* Basic */
			width = w;
			height = h;
			windowTitle = title;

			/* Drawing */
			camera = new Camera(
				glm::vec3(0, 0.0, 3.0), 
				glm::vec3(0.0, 0.0, 0.0), 
				glm::vec3(0.0, 1.0, 0.0), 
				width, height);
			cleaners = new CleanerCollection;
			cleaners->add(new ColorBufferCleaner(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)))
				->add(new DepthBufferCleaner);

			/* GLFW */
			if (hints == NULL) {
				hints = WindowHintList::create()
					->contextVersionMajor(4)
					->contextVersionMinor(1)
					->openGLProfile(GLFW_OPENGL_CORE_PROFILE)
					->openGLForwardCompatability(GL_TRUE);
			}
			windowHints = hints;
			initEnvWindow();

			instance = this;
		}

		virtual int run() = 0;

		virtual void initEnvWindow() {
			GLFW::initialize();

			GLFW::setWindowHints(windowHints);

			WindowConfiguration * config = new WindowConfiguration(
				width, height, windowTitle.c_str()
			);

			window = new Window(config);
			GLFW::makeContextCurrent(window);
			window->setFramebufferSizeCallback(framebuffer_size_callback);
			
			GLFW::loadOpenGLUsingGLAD();
		}

		void stopwatch() {
			float current_frame = GLFW::getTime();
			deltaTime = current_frame - lastFrame;
			lastFrame = current_frame;
		}

		float getFPS() {
			return 1 / deltaTime;
		}

		virtual void printInfo() {
			printf("CameraPosition:(%.3f, %.3f, %.3f)  FPS:%d               \r",
				camera->position().x,
				camera->position().y,
				camera->position().z,
				(int)getFPS()
			);
		}

		virtual void processInput(Window *window) {
			if (window->getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
				window->setShouldClose(true);
			}

			Camera * cam = camera;

			if (!cam)
				return;
			float speed = 2.5f * deltaTime;
			if (window->getKey(GLFW_KEY_W) == GLFW_PRESS)
				cam->moveAhead(speed);
			if (window->getKey(GLFW_KEY_S) == GLFW_PRESS)
				cam->moveBack(speed);
			if (window->getKey(GLFW_KEY_A) == GLFW_PRESS || window->getKey(GLFW_KEY_LEFT) == GLFW_PRESS)
				cam->moveLeft(speed);
			if (window->getKey(GLFW_KEY_D) == GLFW_PRESS || window->getKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
				cam->moveRight(speed);
			if (window->getKey(GLFW_KEY_UP) == GLFW_PRESS)
				cam->moveUp(speed);
			if (window->getKey(GLFW_KEY_DOWN) == GLFW_PRESS)
				cam->moveDown(speed);

			float degspeed = 1 * deltaTime;
			if (window->getKey(GLFW_KEY_Q) == GLFW_PRESS)
				cam->lookLeft(degspeed);
			if (window->getKey(GLFW_KEY_E) == GLFW_PRESS)
				cam->lookRight(degspeed);
			if (window->getKey(GLFW_KEY_R) == GLFW_PRESS)
				cam->lookUp(degspeed);
			if (window->getKey(GLFW_KEY_F) == GLFW_PRESS)
				cam->lookDown(degspeed);

			if (window->getKey(GLFW_KEY_EQUAL) == GLFW_PRESS)
				cam->zoomIn(10 * degspeed);
			if (window->getKey(GLFW_KEY_MINUS) == GLFW_PRESS)
				cam->zoomOut(10 * degspeed);
		}

		virtual void framebufferSizeCallback(int w, int h) {
			width = w; 
			height = h;
			GL::setViewport(0, 0, width, height);
			App::instance->camera->setViewportWidth(width);
			App::instance->camera->setViewportHeight(height);
		}

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
			App::instance->framebufferSizeCallback(width, height);
		}
	};

	App * App::instance = NULL;

}

#endif // !APP_APP
