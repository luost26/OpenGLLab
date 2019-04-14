//
//  Exception.hpp
//
//  Created by 罗 世通 on 2019/3/18.
//

#ifndef Exception_hpp
#define Exception_hpp

#include "support.hpp"

namespace wglfw {
    struct Exception : public std::exception
    {
        const char * what () const throw () {
            return "WrappedGLFW Exception";
        }
    };
    
    struct MethodNotImplementedException : public Exception
    {
        const char * what () const throw () {
            return "MethodNotImplementException";
        }
    };
}

#endif /* Exception_hpp */
