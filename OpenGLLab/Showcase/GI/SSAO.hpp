
#ifndef SHOWCASE_GI_SSAO
#define SHOWCASE_GI_SSAO

#include <wglfw/wglfw.hpp>
#include <random>

#include "../Utility/ScreenQuad.hpp"
#include "../Utility/GBuffer.hpp"
#include "../Utility/GaussianBlurFilter.hpp"

using namespace wglfw;

namespace Showcase {

	class SSAO {
	public:
		FrameBuffer *FBO;
		Texture2D *colorBuffer, *noiseTexture;
		unsigned int width, height, sampleSize, noiseTextureSize;
		std::vector<glm::vec3> kernel, noise;
		FragmentShader * fragmentShader;
		ScreenQuad * renderQuad;

		inline static float lerp(float a, float b, float f) {
			return a + f * (b - a);
		}


		void initSampleKernel() {
			std::uniform_real_distribution<float> random_floats(0.0f, 1.0f);
			std::default_random_engine generator;
			for (unsigned int i = 0; i < sampleSize; ++i) {
				glm::vec3 sample(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, random_floats(generator));
				sample = glm::normalize(sample);
				sample *= random_floats(generator);
				float scale = (float)i / (float)sampleSize;

				scale = lerp(0.1f, 1.0f, scale * scale);
				sample *= scale;
				kernel.push_back(sample);
			}
		}

		void initNoiseTexture() {
			std::uniform_real_distribution<float> random_floats(0.0f, 1.0f);
			std::default_random_engine generator;
			for (unsigned int i = 0; i < noiseTextureSize*noiseTextureSize; i++)
			{
				glm::vec3 n(random_floats(generator) * 2.0 - 1.0, random_floats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
				noise.push_back(n);
			}

			noiseTexture = new Texture2D();
			noiseTexture->bind()->load(noiseTextureSize, noiseTextureSize, &noise[0], GL_RGB, GL_FLOAT, GL_RGB16F)
				->minFilter(GL_NEAREST)->magFilter(GL_NEAREST)->wrapS(GL_REPEAT)->wrapT(GL_REPEAT);
		}

		void initRenderQuad() {
  			fragmentShader = load_fragment_shader(shader_path("gi/ssao.frag"));
			renderQuad = new ScreenQuad(NULL, fragmentShader);
		}
	public:

		SSAO(unsigned int w, unsigned int h, unsigned int ss = 64, unsigned nts=4) {
			width = w;
			height = h;
			sampleSize = ss;
			noiseTextureSize = nts;
			  
			FBO = new FrameBuffer();
			FBO->bind();

			colorBuffer = new Texture2D();
			colorBuffer->bind()
				->empty(width, height, GL_RGB, GL_FLOAT, GL_RED)
				->minFilter(GL_NEAREST)->magFilter(GL_NEAREST);
			FBO->attachTexture2D(colorBuffer, GL_COLOR_ATTACHMENT0);

			FBO->unbind();

			initSampleKernel();
			initNoiseTexture();
			initRenderQuad();
		}

		Texture2D * create(GBuffer * gbuf, UniformBuffer * cam_ubo) {
			FBO->bind(); 
			glClear(GL_COLOR_BUFFER_BIT);

			gbuf->getPositionTexture()->bindToTextureUnit(TextureUnit::get(0));
			gbuf->getNormalTexture()->bindToTextureUnit(TextureUnit::get(1));
			noiseTexture->bindToTextureUnit(TextureUnit::get(2));
			
			Program * prog = renderQuad->program()->use()
				->setTexture("gPosition", TextureUnit::get(0))
				->setTexture("gNormal", TextureUnit::get(1))
				->setTexture("texNoise", TextureUnit::get(2));

			for (unsigned int i = 0; i < sampleSize; ++i) {
				prog->setVec3(("samples[" + std::to_string(i) + "]").c_str(), kernel[i]); 
			}
	 		prog->setUniformBlockBinding("Camera", UniformBufferRangeManager::global()->getRange(cam_ubo));

			renderQuad->draw();

			FBO->unbind();

			two_pass_gaussian_blur(colorBuffer);

			return colorBuffer;
		}

	};

}

#endif // !SHOWCASE_GI_SSAO
