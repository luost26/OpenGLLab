
#ifndef SHOWCASE_UTILITY_GBUFFER
#define SHOWCASE_UTILITY_GBUFFER

#include <wglfw/wglfw.hpp>
#include "../Scene/Scene.hpp"

using namespace wglfw;

namespace Showcase {

	class GBuffer {
	private:
		FrameBuffer * gBuffer;
		Texture2D * positionTexture, normalTexture, albedoTexture, specularTexture;
		RenderBuffer * depthRenderBuffer;
		unsigned int width, height;
		CleanerCollection * bufferCleaners;
		Program * shaderProgram;

		void initPositionTexture() {
			positionTexture = new Texture2D();
			positionTexture->bind()
				->empty(width, height, GL_RGB, GL_FLOAT, GL_RGB16F)
				->minFilter(GL_NEAREST)->magFilter(GL_NEAREST);
			gBuffer->attachTexture2D(positionTexture, GL_COLOR_ATTACHMENT0);
		}

		void initNormalTexture() {
			normalTexture = new Texture2D();
			normalTexture->bind()
				->empty(width, height, GL_RGB, GL_FLOAT, GL_RGB16F)
				->minFilter(GL_NEAREST)->magFilter(GL_NEAREST);
			gBuffer->attachTexture2D(normalTexture, GL_COLOR_ATTACHMENT1);
		}

		void initAlbedoTexture() {
			albedoTexture = new Texture2D();
			albedoTexture->bind()
				->empty(width, height, GL_RGB, GL_FLOAT, GL_RGB)
				->minFilter(GL_NEAREST)->magFilter(GL_NEAREST);
			gBuffer->attachTexture2D(albedoTexture, GL_COLOR_ATTACHMENT2);
		}

		void initSpecularTexture() {
			specularTexture = new Texture2D();
			specularTexture->bind()
				->empty(width, height, GL_RGB, GL_FLOAT, GL_RGB)
				->minFilter(GL_NEAREST)->magFilter(GL_NEAREST);
			gBuffer->attachTexture2D(specularTexture, GL_COLOR_ATTACHMENT3);
		}

		void initDepthRenderBuffer() {
			depthRenderBuffer = new RenderBuffer();
			depthRenderBuffer->bind()->storage(width, height, GL_DEPTH_COMPONENT);
			gBuffer->attachRenderBuffer(depthRenderBuffer, GL_DEPTH_ATTACHMENT);

		}

		void initBufferCleaners() {
			cleaners = new CleanerCollection;
			cleaners->add(new ColorBufferCleaner(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f)))
				->add(new DepthBufferCleaner);
		}

		void initShaderProgram() {
			shaderProgram = simple_shader_program(
				shader_path("utility/g_buffer.vert"),
				shader_path("utility/g_buffer.frag")
			);
		}
	public:
		GBuffer(int w, int h) {
			width = w;
			height = h;

			gBuffer = new FrameBuffer();
			gBuffer->bind();

			initPositionTexture();
			initNormalTexture();
			initAlbedoTexture();
			initSpecularTexture();

			GLuint attachments[4] = { 
				GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, 
				GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3
			};
			gBuffer->setDrawBuffers(4, attachments);

			initDepthRenderBuffer();

			gBuffer->unbind();

			initBufferCleaners();

			initShaderProgram();
		}

		~GBuffer() {
			delete positionTexture;
			delete normalTexture;
			delete albedoTexture;
			delete specularTexture;
			delete depthRenderBuffer;
			delete gBuffer;
			delete bufferCleaners;
			delete shaderProgram;
		}

		void bind() {
			gBuffer->bind();
			GL::clear(bufferCleaners);
		}

		void unbind() {
			gBuffer->unbind();
		}

		Texture2D * getPositionTexture() {
			return positionTexture;
		}

		Texture2D * getNormalTexture() {
			return normalTexture;
		}

		Texture2D * getAlbedoTexture() {
			return albedoTexture;
		}

		Texture2D * getSpecularTexture() {
			return specularTexture;
		}

		Program * getShaderProgram() {
			return shaderProgram;
		}
	};

}

#endif // !SHOWCASE_UTILITY_GBUFFER
