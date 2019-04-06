//
//  skybox.hpp
//
//  Created by 罗 世通 on 2019/4/5.
//

#ifndef skybox_hpp
#define skybox_hpp

#include "base.hpp"
#include "Objects/Skybox.hpp"

namespace playground {
    
    class SkyboxWorld : public Base {
    public:
        static int main() {
            Window * window = initializeEnvAndCreateWindow();
            GL::enableDepthTest();
            glEnable(GL_CULL_FACE);
            
            SkyboxCubeMap * cubemap = SkyboxCubeMap::create()
                ->right(texture_path("skybox/right.jpg"))
                ->left(texture_path("skybox/left.jpg"))
                ->top(texture_path("skybox/top.jpg"))
                ->bottom(texture_path("skybox/bottom.jpg"))
                ->front(texture_path("skybox/front.jpg"))
                ->back(texture_path("skybox/back.jpg"));
            Skybox * skybox = new Skybox(cubemap);
            delete cubemap;
            
            while (!window->shouldClose()) {
                stopwatch();
                processInput(window);
                GL::clear(getDefaultCleanerCollection());
                
                skybox->draw(getCamera());
                
                GLFW::swapBuffers(window);
                GLFW::pollEvents();
            }
            
            return 0;
        }
    };
    
}

#endif /* skybox_hpp */
