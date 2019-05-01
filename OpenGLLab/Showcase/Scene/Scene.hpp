//
// Created by luost on 2019/5/1.
//

#ifndef OPENGLLAB_SCENE_HPP
#define OPENGLLAB_SCENE_HPP

#include <wglfw/wglfw.hpp>

using namespace wglfw;

namespace Showcase {

    struct SceneDrawConfig {
        bool illumination;
        SceneDrawConfig(bool illum=true):
            illumination(illum)
        {}

		static SceneDrawConfig configForShadowMapping() {
			SceneDrawConfig config;
			config.illumination = false;
			return config;
		}

    };

    class Scene {
    public:
        virtual void draw(Program * program, SceneDrawConfig config=SceneDrawConfig()) = 0;
    };

}

#endif //OPENGLLAB_SCENE_HPP
