//
//  FrameBuffer.hpp
//
//  Created by 罗 世通 on 2019/4/1.
//

#ifndef FrameBuffer_hpp
#define FrameBuffer_hpp

#include "support.hpp"
#include "RenderBuffer.hpp"

namespace wglfw {
    
    class FrameBufferNotBoundException : Exception {
        const char * what () const throw () {
            return "The given frame buffer is not bound.";
        }
    };
    
    class FrameBufferNotCompleteException : Exception {
        const char * what () const throw () {
            return "The given frame buffer is not complete.";
        }
    };
    
    class FrameBuffer {
    private:
        unsigned int _FBO;
        GLenum _boundTo;
    public:
        
        FrameBuffer() {
            glGenFramebuffers(1, &_FBO);
        }
        
        FrameBuffer * bind(GLenum target=GL_FRAMEBUFFER) {
            _boundTo = target;
            glBindFramebuffer(target, _FBO);
            return this;
        }
        
        bool isBound() {
            int draw_buffer=0, read_buffer=0;
            glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &draw_buffer);
            glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &read_buffer);
            if (_boundTo == GL_FRAMEBUFFER) {
                return _FBO == draw_buffer && _FBO == read_buffer;
            } else if (_boundTo == GL_DRAW_FRAMEBUFFER) {
                return _FBO == draw_buffer;
            } else if (_boundTo == GL_READ_FRAMEBUFFER) {
                return _FBO == read_buffer;
            }
            return false;
        }
        
        void assertIsBound() {
            if (!isBound()) {
                throw FrameBufferNotBoundException();
            }
        }
        
        bool isComplete() {
            assertIsBound();
            return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        }
        
        void assertIsComplete() {
            if (!isComplete()) {
                throw FrameBufferNotCompleteException();
            }
        }
        
        FrameBuffer * unbind() {
            assertIsBound();
            glBindFramebuffer(_boundTo, 0);
            return this;
        }
        
        FrameBuffer * attachTexture2D(Texture * tex, GLenum attachment, GLenum tex_target=GL_TEXTURE_2D, int level=0) {
            assertIsBound();
            glFramebufferTexture2D(_boundTo, attachment, tex_target, tex->texture(), level);
            return this;
        }
        
        FrameBuffer * attachRenderBuffer(RenderBuffer * rb, GLenum attach=GL_DEPTH_STENCIL_ATTACHMENT) {
            assertIsBound();
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, attach, GL_RENDERBUFFER, rb->RBO());
            return this;
        }

        FrameBuffer * setDrawBuffer(GLenum buf) {
            assertIsBound();
            glDrawBuffer(buf);
            return this;
        }

        FrameBuffer * setReadBuffer(GLenum buf) {
            assertIsBound();
            glReadBuffer(buf);
            return this;
        }

        unsigned int FBO() const {
            return _FBO;
        }
        
    };
    
}

#endif /* FrameBuffer_hpp */
