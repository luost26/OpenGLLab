//
//  Texture.hpp
//
//  Created by 罗 世通 on 2019/3/19.
//

#ifndef Texture_hpp
#define Texture_hpp

#include "support.hpp"
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace wglfw {
    
    class TextureNotBoundException : Exception {
        const char * what () const throw () {
            return "The given texture is not bound.";
        }
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
            instance->_level = 0;
            switch (instance->channels) {
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
        
        virtual Texture * bind() {
            return this;
        }
        
        virtual bool isBound() {
            return false;
        }
    };
    
    class Texture2D : public Texture {
    public:
        Texture2D() : Texture() {}
        
        Texture2D * active(int unit) {
            glActiveTexture(unit);
            return this;
        }
        
        Texture2D * bind() {
            glBindTexture(GL_TEXTURE_2D, _texture);
            return this;
        }
        
        Texture2D * bindToTextureUnit(int unit) {
            active(unit);
            bind();
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
