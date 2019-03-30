//
//  helper.hpp
//
//  Created by 罗 世通 on 2019/3/30.
//

#ifndef helper_hpp
#define helper_hpp

#include <string>
#include "../Shader.hpp"
#include "../Program.hpp"

namespace wglfw {
    Program * simple_shader_program(const std::string & vpath, const std::string & fpath) {
        Program * program = new Program();
        try {
            VertexShader * vshader = VertexShader::make()->source(ShaderSource::fromFile(vpath.c_str()))->compile();
            FragmentShader * fshader = FragmentShader::make()->source(ShaderSource::fromFile(fpath.c_str()))->compile();
            program->attach(vshader)->attach(fshader)->link();
            delete vshader;
            delete fshader;
            
        } catch (ShaderCompilationException e) {
            std::cerr << e.what() << std::endl;
            exit(-1);
        } catch (ProgramLinkException e) {
            std::cerr << e.what() << std::endl;
            exit(-1);
        }
        return program;
    }
}

#endif /* helper_hpp */
