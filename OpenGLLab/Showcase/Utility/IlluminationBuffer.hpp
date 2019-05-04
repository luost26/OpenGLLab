
#ifndef SHOWCASE_UTILITY_ILLUMINATIONBUFFER
#define SHOWCASE_UTILITY_ILLUMINATIONBUFFER

#include <wglfw/wglfw.hpp>
#include "ScreenQuad.hpp"

using namespace wglfw;

namespace Showcase {

	class IlluminationBuffer {
	protected:
		FrameBuffer * FBO;
		Texture2D *ambientTexture, *diffuseTexture, *specularTexture;
		RenderBuffer * depthRenderBuffer;
		unsigned int width, height;

		FragmentShader * mergeFragmentShader;
		ScreenQuad * displayQuad;
	
		void initAmbientTexture() {
			ambientTexture = new Texture2D();
			ambientTexture->bind()
				->empty(width, height, GL_RGBA, GL_FLOAT, GL_RGBA)
				->minFilter(GL_NEAREST)->magFilter(GL_NEAREST)
				->unbind();
			FBO->attachTexture2D(ambientTexture, GL_COLOR_ATTACHMENT0);
		}

		void initDiffuseTexture() {
			diffuseTexture = new Texture2D();
			diffuseTexture->bind()
				->empty(width, height, GL_RGBA, GL_FLOAT, GL_RGBA)
				->minFilter(GL_NEAREST)->magFilter(GL_NEAREST)
				->unbind();
			FBO->attachTexture2D(diffuseTexture, GL_COLOR_ATTACHMENT1);
		}

		void initSpecularTexture() {
			specularTexture = new Texture2D();
			specularTexture->bind()
				->empty(width, height, GL_RGBA, GL_FLOAT, GL_RGBA)
				->minFilter(GL_NEAREST)->magFilter(GL_NEAREST)
				->unbind();
			FBO->attachTexture2D(specularTexture, GL_COLOR_ATTACHMENT2);
		}

		void initDepthRenderBuffer() {
			depthRenderBuffer = new RenderBuffer();
			depthRenderBuffer->bind()
				->storage(width, height, GL_DEPTH_COMPONENT)
				->unbind();
			FBO->attachRenderBuffer(depthRenderBuffer, GL_DEPTH_ATTACHMENT);
		}

		void initDisplayQuad() {
			mergeFragmentShader = load_fragment_shader(shader_path("utility/illum_buffer.frag"));
			displayQuad = new ScreenQuad(NULL, mergeFragmentShader);
		}
	public:
		IlluminationBuffer(unsigned int w, unsigned int h) {
			width = w; 
			height = h;

			FBO = new FrameBuffer();
			FBO->bind();

			initAmbientTexture();
			initDiffuseTexture();
			initSpecularTexture();

			GLuint attachments[4] = {
				GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
				GL_COLOR_ATTACHMENT2
			};
			FBO->setDrawBuffers(3, attachments);

			initDepthRenderBuffer();

			FBO->unbind();

			initDisplayQuad();
		}

		~IlluminationBuffer() {
			delete ambientTexture;
			delete diffuseTexture;
			delete specularTexture;
			delete FBO;
			delete mergeFragmentShader;
			delete displayQuad;
		}

		void bind(CleanerCollection * cleaners) {
			FBO->bind();
			GL::clear(cleaners);
		}

		void unbind() {
			FBO->unbind();
		}

		void mergeDisplay() {
			Program * prog = displayQuad->program();
			ambientTexture->bindToTextureUnit(TextureUnit::get(0));
			diffuseTexture->bindToTextureUnit(TextureUnit::get(1));
			specularTexture->bindToTextureUnit(TextureUnit::get(2));
			prog->use()
				->setTexture("ambient", TextureUnit::get(0))
				->setTexture("diffuse", TextureUnit::get(1))
				->setTexture("specular", TextureUnit::get(2));
			displayQuad->draw();
		}
	};




	class IlluminationBufferMultisample {
	protected:
		FrameBuffer * FBO;
		Texture2DMultisample *ambientTextureMS, *diffuseTextureMS, *specularTextureMS;
		RenderBuffer * depthRenderBuffer;
		unsigned int samples;

		unsigned int width, height;
		Program * drawProgram;
		FragmentShader * mergeFragmentShader;
		ScreenQuad * displayQuad;

		void initAmbientTextureMS() {
			ambientTextureMS = new Texture2DMultisample();
			ambientTextureMS->bind()->empty(width, height, samples, GL_RGBA)->unbind();
			FBO->attachTexture2D(ambientTextureMS, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE);
		}

		void initDiffuseTextureMS() {
			diffuseTextureMS = new Texture2DMultisample();
			diffuseTextureMS->bind()->empty(width, height, samples, GL_RGBA)->unbind();
			FBO->attachTexture2D(diffuseTextureMS, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D_MULTISAMPLE);
		}

		void initSpecularTextureMS() {
			specularTextureMS = new Texture2DMultisample();
			specularTextureMS->bind()->empty(width, height, samples, GL_RGBA)->unbind();
			FBO->attachTexture2D(specularTextureMS, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D_MULTISAMPLE);
		}

		void initDepthRenderBuffer() {
			depthRenderBuffer = new RenderBuffer();
			depthRenderBuffer->bind()
				->storageMultisample(width, height, samples, GL_DEPTH_COMPONENT)
				->unbind();
			FBO->attachRenderBuffer(depthRenderBuffer, GL_DEPTH_ATTACHMENT);
		}

		void initDisplayQuad() {
			mergeFragmentShader = load_fragment_shader(shader_path("utility/illum_buffer_ms.frag"));
			displayQuad = new ScreenQuad(NULL, mergeFragmentShader);
		}
	public:
		IlluminationBufferMultisample(unsigned int w, unsigned int h, unsigned int s) {
			width = w;
			height = h;
			samples = s;

			/* Multisampled FBO */
			FBO = new FrameBuffer();
			FBO->bind();

			initAmbientTextureMS();
			initDiffuseTextureMS();
			initSpecularTextureMS();

			GLuint attachments[3] = {
				GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
				GL_COLOR_ATTACHMENT2
			};
			FBO->setDrawBuffers(3, attachments);

			initDepthRenderBuffer();

			FBO->unbind();

			initDisplayQuad();
		}

		void bind(CleanerCollection * cleaners) {
			FBO->bind();
			GL::clear(cleaners);
		}

		void unbind() {
			FBO->unbind();
		}

		void mergeDisplay() {
			Program * prog = displayQuad->program();
			ambientTextureMS->bindToTextureUnit(TextureUnit::get(0));
			diffuseTextureMS->bindToTextureUnit(TextureUnit::get(1));
			specularTextureMS->bindToTextureUnit(TextureUnit::get(2));
			prog->use()
				->setTexture("ambient", TextureUnit::get(0))
				->setTexture("diffuse", TextureUnit::get(1))
				->setTexture("specular", TextureUnit::get(2))
				->setInt("samples", samples);
			displayQuad->draw();
		}
	};

}

#endif // !SHOWCASE_UTILITY_ILLUMINATIONBUFFER
