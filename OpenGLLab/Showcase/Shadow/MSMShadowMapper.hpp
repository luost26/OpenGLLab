
#ifndef SHOWCASE_SHADOW_MSMSHADOWMAPPER
#define SHOWCASE_SHADOW_MSMSHADOWMAPPER

#include "ShadowMapper.hpp"
#include "ShadowMap.hpp"
#include "../Utility/GaussianBlurFilter.hpp"
#include "../Utility/helper.hpp"

namespace Showcase {
	 
	class MSMShadowMapper : public ShadowMapper {
	private:
		GLenum shadowMapInternalFormat;
		GLenum shadowMapFormat;
		int mapSize;

		FrameBuffer * FBOMS;
		Texture2DMultisample *mapMS, *auxMapMS;
		Texture2D * auxMap;

		Program * defaultProgram;
	public:
		MSMShadowMapper(
			ShadowStorage * store,
			int size=2048, GLenum format=GL_RGBA, GLenum intformat=GL_RGBA32F, 
			FrameBuffer * fb = NULL) :
			mapSize(size), shadowMapFormat(format), shadowMapInternalFormat(intformat),
			ShadowMapper(store, fb) {
		
			/* Create an auxiliary framebuffer for multisampling */
			FBOMS = new FrameBuffer();
			auxMapMS = new Texture2DMultisample();
			auxMapMS->bind()->empty(mapSize, mapSize, 8, GL_DEPTH_COMPONENT)->unbind();
			mapMS = new Texture2DMultisample();
			mapMS->bind()->empty(mapSize, mapSize, 8, shadowMapInternalFormat)->unbind();

			FBOMS->bind()
				->attachTexture2D(auxMapMS, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE)
				->attachTexture2D(mapMS, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE)
				->unbind();

			/* Create an auxiliary texture for depth data */
			auxMap = new Texture2D();
			auxMap->bind()->empty(mapSize, mapSize, GL_DEPTH_COMPONENT, GL_FLOAT, GL_DEPTH_COMPONENT)
				->minFilter(GL_NEAREST)->magFilter(GL_NEAREST)
				->wrapS(GL_CLAMP_TO_EDGE)->wrapT(GL_CLAMP_TO_EDGE)
				->borderColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

			/* Create the MSM shadow mapping program */
			defaultProgram = simple_shader_program(
				shader_path("shadow/msm.vert"),
				shader_path("shadow/msm.frag")
			);
		}

		~MSMShadowMapper() {
			delete FBOMS;
			delete mapMS;
			delete auxMapMS;
			delete auxMap;
		}

		int mapForSpotLight(SpotLight * spotlight, Scene * scene) {
			Program * prog = defaultProgram;

			/* Preprocess 1: Generate empty shadow map */
			ShadowMap * shadow_map = new ShadowMap(mapSize, shadowMapFormat, shadowMapInternalFormat);

			/* Preprocess 2: Calculate light space matrix*/
			glm::mat4 light_proj, light_view, light_space;
			float aspect = 1.0f;
			float _near = 1.0f;
			float _far = 25.0f;
			light_proj = glm::perspective(glm::radians(120.0f), aspect, _near, _far);
			glm::vec3 light_pos = glm::vec3(spotlight->position);
			glm::vec3 light_dir = glm::normalize(glm::vec3(spotlight->direction));
			glm::vec3 top_vec = light_dir == glm::vec3(1.0f, 0.0f, 0.0f) || light_dir == glm::vec3(-1.0f, 0.0f, 0.0f) ?
				glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), light_dir) :
				glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), light_dir);
			light_view = glm::lookAt(
				light_pos,
				light_pos + light_dir,
				top_vec
			);
			light_space = light_proj * light_view;

			/* Preprocess 3: check shadow maps limit and update light's attributes */
			if (spotlight->shadow_enabled && shadowStorage->getShadowMap(spotlight->shadow_map_index) != NULL) {
				delete shadowStorage->getShadowMap(spotlight->shadow_map_index);
				shadowStorage->updateShadow(spotlight->shadow_map_index, shadow_map, light_space);
			} else {
				if (shadowStorage->isFull())
					return -1;
				spotlight->shadow_enabled = 1;
				spotlight->shadow_map_index = shadowStorage->addShadow(shadow_map, light_space);
			}


			/* End Preprocess */


			/* Step 1: Generate multisampled moment shadow map */
			
			FBOMS->bind();
			GL::enable(GL_BLEND);
			glBlendFunc(GL_SRC1_COLOR, GL_ONE_MINUS_SRC1_COLOR);
			saveViewport();
			GL::setViewport(0, 0, mapSize, mapSize);
			glClear(GL_DEPTH_BUFFER_BIT);
			prog->use()->setMatrix4("lightSpace", light_space);  // upload light space matrix to depth map generation shader
			scene->draw(prog, SceneDrawConfig::configForShadowMapping());
			FBOMS->unbind();
			GL::disable(GL_BLEND);
			 

			/* Step 2: Downsample the multisampled map */
			FBO->bind()->attachTexture2D(auxMap, GL_DEPTH_ATTACHMENT)
				->attachTexture2D(shadow_map->texture(), GL_COLOR_ATTACHMENT0)
				->unbind();
			FBOMS->bind(GL_READ_FRAMEBUFFER);
			FBO->bind(GL_DRAW_FRAMEBUFFER);
			glBlitFramebuffer(0, 0, mapSize, mapSize, 0, 0, mapSize, mapSize, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

			FBOMS->unbind(GL_READ_FRAMEBUFFER);
			FBO->unbind(GL_DRAW_FRAMEBUFFER);

			/* Step 3: apply two-pass gaussian blur */
			two_pass_gaussian_blur(
				shadow_map->texture(),
				glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f),
				9, FBO
			);

			resumeViewport();
			return spotlight->shadow_map_index;
		}

	};

}

#endif // !SHOWCASE_SHADOW_MSMSHADOWMAPPER
