//
//  PointLight.hpp
//
//  Created by 罗 世通 on 2019/3/30.
//

#ifndef PointLight_hpp
#define PointLight_hpp

#include "../../../WrappedGLFW/loader.hpp"
#include <glm/glm.hpp>
#include <string>

namespace playground {

    class PointLight {
    private:
        glm::vec3 _position, _ambient, _diffuse, _specular;
        float _constant, _linear, _quadratic;
        PointLight() {}
    public:
        static PointLight * create() {
            PointLight * instance = new PointLight;
            instance->_constant = 1;
            instance->_linear = 0;
            instance->_quadratic = 0;
            return instance;
        }
        
        PointLight * color(glm::vec3 c, const float am=0.1f, const float di=0.8f, const float sp=1.0f) {
            return this->ambient(c * am)->diffuse(c * di)->specular(c * sp);
        }
        
        PointLight * position(glm::vec3 v) {
            _position = v;
            return this;
        }
        
        PointLight * ambient(glm::vec3 v) {
            _ambient = v;
            return this;
        }
        
        PointLight * diffuse(glm::vec3 v) {
            _diffuse = v;
            return this;
        }
        
        PointLight * specular(glm::vec3 v) {
            _specular = v;
            return this;
        }
        
        PointLight * attenuation(const float c=1.0f, const float l=0.09f, const float q=0.032f) {
            _constant = c;
            _linear = l;
            _quadratic = q;
            return this;
        }
        
        PointLight * apply(Program * prog, glm::vec3 view_pos,const std::string & uniform, const std::string & view_pos_uniform) {
            prog->setVec3((uniform+".ambient").c_str(), _ambient)
                ->setVec3((uniform+".diffuse").c_str(), _diffuse)
                ->setVec3((uniform+".specular").c_str(), _specular)
                ->setVec3((uniform+".position").c_str(), _position)
                ->setFloat((uniform+".constant").c_str(), _constant)
                ->setFloat((uniform+".linear").c_str(), _linear)
                ->setFloat((uniform+".quadratic").c_str(), _quadratic)
                ->setVec3(view_pos_uniform.c_str(), view_pos);
            return this;
        }
    };
    
    
}

#endif /* PointLight_hpp */
