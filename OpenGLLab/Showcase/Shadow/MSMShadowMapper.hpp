
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
			int size=2048, GLenum format=GL_RGBA, GLenum intformat=GL_RGBA32F, 
			int limit = 12, int unit = 4, FrameBuffer * fb = NULL) :
			mapSize(size), shadowMapFormat(format), shadowMapInternalFormat(intformat),
			ShadowMapper(limit, unit, fb) {
		
			/* Create an auxiliary framebuffer for multisampling */
			FBOMS = new FrameBuffer();
			auxMapMS = new Texture2DMultisample();
			auxMapMS->bind()->empty(mapSize, mapSize, 8, GL_DEPTH_COMPONENT)->unbind();
			mapMS = new Texture2DMultisample();
			mapMS->bind()->empty(mapSize, mapSize, 8, shadowMapInternalFormat)->unbind();

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

			/* Preprocess 1: Generate empty shadow map and bind to Frame Buffer */
			ShadowMap * shadow_map = new ShadowMap(mapSize, shadowMapFormat, shadowMapInternalFormat);

			/* Preprocess 2: Calculate light space matrix*/
			glm::mat4 light_proj, light_view, light_space;
			float aspect = 1.0f;
			float _near = 1.0f;
			float _far = 25.0f;
			light_proj = glm::perspective(glm::radians(120.0f), aspect, _near, _far);
			light_view = glm::lookAt(
				glm::vec3(light->position.x, light->position.y, light->position.z),
				glm::vec3(light->position.x, 0.0f, light->position.z),
				glm::vec3(0.0f, 0.0f, -1.0f)
			);
			light_space = light_proj * light_view;


			/* Preprocess 3: check shadow maps limit and update light's attributes */
			if (spotlight->shadow_enabled) {
				delete maps[spotlight->shadow_map_index];
				maps[spotlight->shadow_map_index] = shadow_map;
				lightSpaces[spotlight->shadow_map_index] = light_space;
			} else {
				if (maps.size() >= shadowMapsLimit) {
					delete shadow_map;
					return -1;
				}
				maps.push_back(shadow_map);
				lightSpaces.push_back(light_space);
				spotlight->shadow_enabled = 1;
				spotlight->shadow_map_index = maps.size() - 1;
			}


			/* End Preprocess */


			/* Step 1: Generate multisampled moment shadow map */
			FBOMS->bind()
				->attachTexture2D(auxMapMS, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE)
				->attachTexture2D(mapMS, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE);
			glClear(GL_DEPTH_BUFFER_BIT);
			prog->use()->setMatrix4("lightSpace", light_space);  // upload light space matrix to depth map generation shader
			scene->draw(prog, SceneDrawConfig::configForShadowMapping());


			/* Step 2: Downsample the multisampled map */
			FBO->bind()->attachTexture2D(auxMap, GL_DEPTH_ATTACHMENT)
				->attachTexture2D(shadow_map->texture(), GL_COLOR_ATTACHMENT0)
				->unbind();
			FBOMS->bind(GL_READ_FRAMEBUFFER);
			FBO->bind(GL_DRAW_FRAMEBUFFER);
			glBlitFramebuffer(0, 0, mapSize, mapSize, 0, 0, mapSize, mapSize, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

			FBOMS->unbind(GL_FRAMEBUFFER);


			/* Step 3: apply two-pass gaussian blur */
			two_pass_gaussian_blur(
				shadow_map->texture(),
				glm::vec2(1.0f, 0.0f), glm::vec2(0.0f, 1.0f),
				9, FBO
			);

			return spotlight->shadow_map_index;
		}

	};

}

#endif // !SHOWCASE_SHADOW_MSMSHADOWMAPPER
