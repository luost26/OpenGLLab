//
//  Triangle.hpp
//
//  Created by 罗 世通 on 2019/3/19.
//

#ifndef Triangle_hpp
#define Triangle_hpp

namespace wglfw {
    
    template<class T>
    struct Triangle {
        T a, b, c;
        Triangle(const T & _a, const T & _b, const T & _c): a(_a), b(_b), c(_c) {}
    };
    
}

#endif /* Triangle_hpp */
