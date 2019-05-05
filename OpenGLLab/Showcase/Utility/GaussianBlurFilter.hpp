
#ifndef SHOWCASE_UTILITY_GAUSSIANBLURFILTER
#define SHOWCASE_UTILITY_GAUSSIANBLURFILTER

#include <wglfw/wglfw.hpp>
#include "ScreenQuad.hpp"

using namespace wglfw;

namespace Showcase {
	
	Texture2D * two_pass_gaussian_blur(
		Texture2D * map,
		glm::vec2 dir1 = glm::vec2(1.0f, 0.0f),
		glm::vec2 dir2 = glm::vec2(0.0f, 1.0f),
		int level = 9,
		FrameBuffer * FBO = NULL
	) {
		bool release_FBO = false;
		if (FBO == NULL) {
			FBO = new FrameBuffer();
			release_FBO = true;
		}

		Texture2D * auxMap = new Texture2D();

		auxMap->bind()->empty(map->width(), map->height(), map->format(), GL_FLOAT, map->internalFormat())
			->minFilter(GL_NEAREST)->magFilter(GL_NEAREST)
			->wrapS(GL_CLAMP_TO_EDGE)->wrapT(GL_CLAMP_TO_EDGE)
			->borderColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		FBO->bind()->attachTexture2D(auxMap, GL_COLOR_ATTACHMENT0)->unbind();

		static Program * gaussian_blur_program = simple_shader_program(
			shader_path("utility/gaussian_blur.vert"), 
			shader_path("utility/gaussian_blur.frag"));
		static ScreenQuad * quad = new ScreenQuad(gaussian_blur_program);
		// static modifier: share Program and ScreenQuad among calls to avoid memory leak

		gaussian_blur_program->use()
			->setVec2("direction", dir1)
			->setInt("level", level);

		FBO->bind();
		quad->displayTexture(map);
		FBO->unbind();

		FBO->bind()->attachTexture2D(map, GL_COLOR_ATTACHMENT0)->unbind();
		gaussian_blur_program->use()
			->setVec2("direction", dir2)
			->setInt("level", level);
		FBO->bind();
		quad->displayTexture(auxMap);
		FBO->unbind();

		delete auxMap;

		if (release_FBO)
			delete FBO;

		return map;
	}

}

#endif // !SHOWCASE_UTILITY_GAUSSIANBLURFILTER
