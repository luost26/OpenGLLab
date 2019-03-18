//
//  GL.hpp
//
//  Created by 罗 世通 on 2019/3/19.
//

#ifndef GL_hpp
#define GL_hpp

#include "support.hpp"

namespace wglfw {
    
    class GL {
    public:
        static void unbindVertexArray() {
            glBindVertexArray(0);
        }
    };
    
}

#endif /* GL_hpp */
