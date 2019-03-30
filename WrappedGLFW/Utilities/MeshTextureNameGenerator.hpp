//
//  MeshTextureNameGenerator.hpp
//
//  Created by 罗 世通 on 2019/3/30.
//

#ifndef MeshTextureNameGenerator_hpp
#define MeshTextureNameGenerator_hpp

#include "../Exception.hpp"

namespace wglfw {
    
    class TextureTypeUnknownException : public Exception {
    public:
        const char * what() throw () {
            return "Unknown texture type.";
        }
    };
    
    class MeshTextureNameGenerator {
    public:
        enum TextureType {
            DIFFUSE, SPECULAR, NORMAL, HEIGHT
        };
    protected:
        TextureType _forType;
    public:
        
        virtual MeshTextureNameGenerator * forType(TextureType t) {
            _forType = t;
            return this;
        }
        virtual std::string generate() {
            throw MethodNotImplementedException();
            return "";
        }
    };
    
    class MeshTextureNameGeneratorFactory {
    public:
        virtual MeshTextureNameGenerator * generator() {
            throw MethodNotImplementedException();
            return NULL;
        }
    };
    
    
    class DefaultMeshTextureNameGenerator : public MeshTextureNameGenerator {
    private:
        std::string _prefix, _diffuse, _specular, _normal, _height;
        int _countDiffuse, _countSpecular, _countNormal, _countHeight;
    public:
        
        DefaultMeshTextureNameGenerator(const std::string & _pf = "material.", const std::string & _d = "texture_diffuse", const std::string & _s = "texture_specular", const std::string & _n = "texture_normal", const std::string & _h = "texture_height") {
            _prefix = _pf;
            _diffuse = _d;
            _specular = _s;
            _normal = _n;
            _height = _h;
            _countDiffuse = 0;
            _countSpecular = 0;
            _countNormal = 0;
            _countHeight = 0;
        }
        
        static DefaultMeshTextureNameGenerator * create() {
            DefaultMeshTextureNameGenerator * instance = new DefaultMeshTextureNameGenerator();
            return instance;
        }
        
        DefaultMeshTextureNameGenerator * forType(TextureType t) {
            _forType = t;
            return this;
        }
        
        std::string generate() {
            std::string * type_prefix;
            int * type_counter;
            switch (_forType) {
                case DIFFUSE:
                    type_prefix = &_diffuse;
                    type_counter = &_countDiffuse;
                    break;
                case SPECULAR:
                    type_prefix = &_specular;
                    type_counter = &_countSpecular;
                    break;
                case NORMAL:
                    type_prefix = &_normal;
                    type_counter = &_countNormal;
                    break;
                case HEIGHT:
                    type_prefix = &_height;
                    type_counter = &_countHeight;
                    break;
                default:
                    throw TextureTypeUnknownException();
                    break;
            }
            
            std::string name = _prefix + (*type_prefix) + std::to_string(*type_counter);
            ++(*type_counter);
            return name;
        }
    };
    
    class DefaultMeshTextureNameGeneratorFactory : public MeshTextureNameGeneratorFactory {
    private:
        std::string _prefix, _diffuse, _specular, _normal, _height;
    public:
        
        DefaultMeshTextureNameGeneratorFactory(const std::string & _pf = "material.", const std::string & _d = "texture_diffuse", const std::string & _s = "texture_specular", const std::string & _n = "texture_normal", const std::string & _h = "texture_height") {
            _prefix = _pf;
            _diffuse = _d;
            _specular = _s;
            _normal = _n;
            _height = _h;
        }
        
        DefaultMeshTextureNameGenerator * generator() {
            DefaultMeshTextureNameGenerator * generator = new DefaultMeshTextureNameGenerator(_prefix, _diffuse, _specular, _normal, _height);
            return generator;
        }
        
        DefaultMeshTextureNameGeneratorFactory * prefix(const std::string & s) {
            _prefix = s;
            return this;
        }
        
        DefaultMeshTextureNameGeneratorFactory * diffuse(const std::string & s) {
            _diffuse = s;
            return this;
        }
        
        DefaultMeshTextureNameGeneratorFactory * specular(const std::string & s) {
            _specular = s;
            return this;
        }
        
        DefaultMeshTextureNameGeneratorFactory * normal(const std::string & s) {
            _normal = s;
            return this;
        }
        
        DefaultMeshTextureNameGeneratorFactory * height(const std::string & s) {
            _height = s;
            return this;
        }
        
    };
    
}

#endif /* MeshTextureNameGenerator_hpp */
