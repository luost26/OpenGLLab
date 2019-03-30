//
//  Texture.hpp
//
//  Created by 罗 世通 on 2019/3/19.
//

#ifndef Texture_hpp
#define Texture_hpp

#include "support.hpp"
#include "Exception.hpp"
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace wglfw {
    
    class TextureNotBoundException : Exception {
        const char * what () const throw () {
            return "The given texture is not bound.";
        }
    };
    
    class TextureImageNotOpenedException : Exception {
        const char * what () const throw () {
            return "Texture image cannot be opened.";
        }
    };
    
    class TextureUnit {
    private:
        const static GLenum units[32];
        static TextureUnit * instances[32];
        
        int _unit;
        
        TextureUnit(int u) {
            _unit = u;
        }
    public:
        static TextureUnit * get(int u) {
            if (u < 0 || u >= 32) {
                return NULL;
            }
            if (!instances[u])
                instances[u] = new TextureUnit(u);
            return instances[u];
        }
        
        GLenum getGLenumValue() {
            return units[_unit];
        }
        
        int getUnitNumber() {
            return _unit;
        }
    };
    
    const GLenum TextureUnit::units[32] = {
        GL_TEXTURE0,
        GL_TEXTURE1,
        GL_TEXTURE2,
        GL_TEXTURE3,
        GL_TEXTURE4,
        GL_TEXTURE5,
        GL_TEXTURE6,
        GL_TEXTURE7,
        GL_TEXTURE8,
        GL_TEXTURE9,
        GL_TEXTURE10,
        GL_TEXTURE11,
        GL_TEXTURE12,
        GL_TEXTURE13,
        GL_TEXTURE14,
        GL_TEXTURE15,
        GL_TEXTURE16,
        GL_TEXTURE17,
        GL_TEXTURE18,
        GL_TEXTURE19,
        GL_TEXTURE20,
        GL_TEXTURE21,
        GL_TEXTURE22,
        GL_TEXTURE23,
        GL_TEXTURE24,
        GL_TEXTURE25,
        GL_TEXTURE26,
        GL_TEXTURE27,
        GL_TEXTURE28,
        GL_TEXTURE29,
        GL_TEXTURE30,
        GL_TEXTURE31
    };
    
    TextureUnit * TextureUnit::instances[32] = {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    };
    
    class TextureImage {
    private:
        TextureImage() {};
    public:
        unsigned char * data;
        int width, height, channels, _format, _level;
        
        static TextureImage * fromPath(const char * path) {
            TextureImage * instance = new TextureImage;
            stbi_set_flip_vertically_on_load(true);
            instance->data = stbi_load(path, &instance->width, &instance->height, &instance->channels, 0);
            if (!instance->data)
                throw TextureImageNotOpenedException();
            instance->_level = 0;
            switch (instance->channels) {
                case 1:
                    instance->_format = GL_RED;
                    break;
                case 3:
                    instance->_format = GL_RGB;
                    break;
                case 4:
                    instance->_format = GL_RGBA;
                    break;
            }
            return instance;
        }
        
        TextureImage * format(int f) {
            _format = f;
            return this;
        }
        
        TextureImage * level(int l) {
            _level = l;
            return this;
        }
        
        ~TextureImage() {
            stbi_image_free(data);
        }
    };
    
    class Texture {
    protected:
        unsigned int _texture;
        Texture(unsigned int t): _texture(t) {}
        
        void assertIsBound() {
            if (!isBound()) {
                throw TextureNotBoundException();
            }
        }
    public:
        Texture() {
            glGenTextures(1, &_texture);
        }
        
        static std::vector<Texture*> createMany(int n) {
            unsigned int * ts = new unsigned int[n+1];
            glGenVertexArrays(n, ts);
            std::vector<Texture*> arrays;
            while (n > 0) {
                arrays.push_back(new Texture(*ts));
                ++ ts; -- n;
            }
            delete [] ts;
            return arrays;
        }
        
        unsigned int texture() {
            return _texture;
        }
        
        static void activateTextureUnit(TextureUnit * unit) {
            glActiveTexture(unit->getGLenumValue());
        }
        
        Texture * activate(TextureUnit * unit) {
            activateTextureUnit(unit);
            return this;
        }
        
        virtual Texture * bind() {
            throw MethodNotImplementedException();
            return this;
        }
        
        virtual Texture * bindToTextureUnit(TextureUnit * unit) {
            activate(unit);
            bind();
            return this;
        }
        
        virtual bool isBound() {
            throw MethodNotImplementedException();
            return false;
        }
    };
    
    class Texture2D : public Texture {
    public:
        Texture2D() : Texture() {}
        
        Texture2D * bind() {
            glBindTexture(GL_TEXTURE_2D, _texture);
            return this;
        }
        
        bool isBound() {
            int b;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &b);
            return b == _texture;
        }
        
        Texture2D * wrapS(int v) {
            assertIsBound();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, v);
            return this;
        }
        
        Texture2D * wrapT(int v) {
            assertIsBound();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, v);
            return this;
        }
        
        Texture2D * minFilter(int v) {
            assertIsBound();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, v);
            return this;
        }
        
        Texture2D * magFilter(int v) {
            assertIsBound();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, v);
            return this;
        }
        
        Texture2D * loadImage(TextureImage * loader) {
            assertIsBound();
            glTexImage2D(GL_TEXTURE_2D, loader->_level, loader->_format, loader->width, loader->height,
                         0, loader->_format, GL_UNSIGNED_BYTE, loader->data);
            return this;
        }
        
        Texture2D * generateMipmap() {
            assertIsBound();
            glGenerateMipmap(GL_TEXTURE_2D);
            return this;
        }
        
    };
    
}

#endif /* Texture_hpp */
