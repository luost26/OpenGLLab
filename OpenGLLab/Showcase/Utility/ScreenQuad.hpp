#ifndef UTILITY_SCREENQUAD_HPP
#define UTILITY_SCREENQUAD_HPP

#include <wglfw/wglfw.hpp>

using namespace wglfw;

namespace Showcase {

	class ScreenQuad {
	private:
		static char SCREEN_QUAD_VERT[];
		static char SCREEN_QUAD_FRAG[];

		static float vertices[30];

		Program * _program;
		VertexArray * _VAO;
		ArrayBuffer * _VBO;


		static VertexShader * createDefaultVertexShader() {
			ShaderSource * vshader_src = ShaderSource::fromCString(SCREEN_QUAD_VERT);
			VertexShader * vshader = VertexShader::make()->source(vshader_src)->compile();
			delete vshader_src;
			return vshader;
		}

		static FragmentShader * createDefaultFragmentShader() {
			ShaderSource * fshader_src = ShaderSource::fromCString(SCREEN_QUAD_FRAG);
			FragmentShader * fshader = FragmentShader::make()->source(fshader_src)->compile();
			delete fshader_src;
			return fshader;
		}

		void setTexture(Texture2D * tex, const char * uniform = "screenTexture") {
			_program->use();
			tex->bindToTextureUnit(TextureUnit::get(0));
			_program->setTexture(uniform, TextureUnit::get(0));
		}

		void setUpVertices() {
			/* Set up VAO */
			_VAO = new VertexArray();
			_VAO->bind();

			/* Set up VBO */
			_VBO = new ArrayBuffer();
			_VBO->bind()->load(sizeof(vertices), vertices, GL_STATIC_DRAW);
			_VBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
				->index(0)
				->size(3)
				->type(GL_FLOAT)
				->normalized(GL_FALSE)
				->stride(5 * sizeof(float))
				->pointer(0)
			)->enable();
			_VBO->getVertexAttributePointer(VertexAttributePointerConfiguration::make()
				->index(1)
				->size(2)
				->type(GL_FLOAT)
				->normalized(GL_FALSE)
				->stride(5 * sizeof(float))
				->pointer((void *)(3 * sizeof(float)))
			)->enable();
		}

	public:

		ScreenQuad(VertexShader * vshader = NULL, FragmentShader * fshader = NULL) {
			/* Set up shader program */
			vshader = vshader == NULL ? createDefaultVertexShader() : vshader;
			fshader = fshader == NULL ? createDefaultFragmentShader() : fshader;
			_program = new Program();
			_program->attach(vshader)->attach(fshader)->link();
			delete vshader;
			delete fshader;

			setUpVertices();
		}

		ScreenQuad(Program * prog) {
			_program = prog;

			setUpVertices();
		}

		Program * program() {
			return _program;
		}

		ScreenQuad * displayTexture(Texture2D * tex, const char * uniform = "screenTexture") {
			glDisable(GL_DEPTH_TEST);
			glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			_program->use();
			setTexture(tex, uniform);
			_VAO->bind();
			GL::drawArrays(GL_TRIANGLES, 0, 6);
			GL::enableDepthTest();
			return this;
		}

		ScreenQuad * draw() {
			_program->use();
			_VAO->bind();
			GL::drawArrays(GL_TRIANGLES, 0, 6);
			return this;
		}

	};


	char ScreenQuad::SCREEN_QUAD_VERT[] = "#version 410 core\n"
		"\n"
		"layout (location=0) in vec3 aPos;\n"
		"layout (location=1) in vec2 aTexCoord;\n"
		"\n"
		"out vec2 TexCoord;\n"
		"\n"
		"void main() {\n"
		"    gl_Position = vec4(aPos, 1.0);\n"
		"    TexCoord = aTexCoord;\n"
		"}";
	char ScreenQuad::SCREEN_QUAD_FRAG[] = "#version 410 core\n"
		"out vec4 color;\n"
		"\n"
		"in vec2 TexCoord;\n"
		"\n"
		"uniform sampler2D screenTexture;\n"
		"\n"
		"void main()\n"
		"{\n"
		"        color = vec4(texture(screenTexture, TexCoord).rgb, 1.0);\n"
		"}";

	float ScreenQuad::vertices[30] = {
			-1.0f,  1.0f,0,  0.0f, 1.0f,
			-1.0f, -1.0f,0,  0.0f, 0.0f,
			1.0f, -1.0f,0,  1.0f, 0.0f,

			-1.0f, 1.0f,0,  0.0f, 1.0f,
			1.0f, -1.0f,0,  1.0f, 0.0f,
			1.0f,  1.0f,0,  1.0f, 1.0f
	};
}

#endif
