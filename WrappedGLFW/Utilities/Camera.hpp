//
//  Camera.hpp
//
//  Created by 罗 世通 on 2019/3/24.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <glm/glm.hpp>

namespace wglfw {
    
    const float EPSILON = 0.001;
    
    class Camera {
    private:
        glm::vec3 _position, _worldUp;
        glm::vec3 _up, _front, _right;
        float _yaw, _pitch;
        
        void updateCameraVectors() {
            glm::vec3 front;
            front.x = cos(_yaw) * cos(_pitch);
            front.y = sin(_pitch);
            front.z = sin(_yaw) * cos(_pitch);

            _front = glm::normalize(front);
            _right = glm::normalize(glm::cross(_front, _worldUp));
            _up    = glm::normalize(glm::cross(_right, _front));
        }
        
        void updateYawPitch() {
            _pitch = glm::asin(_front.y);
            
            float cospitch = glm::sqrt(1 - (_front.y*_front.y));
            if (fabs(cospitch) < EPSILON) {
                _yaw = 0.0f;
            } else {
                _yaw = glm::acos(_front.x / cospitch);
                if (_front.z < 0) _yaw = - _yaw;
            }
        }
        
    public:
        Camera(const glm::vec3 & eye, const glm::vec3 & center, const glm::vec3 & top, const glm::vec3 & worldtop) {
            _position = eye;
            _front = glm::normalize(center - eye);
            _up = glm::normalize(top);
            _worldUp = glm::normalize(worldtop);
            updateYawPitch();
            updateCameraVectors();
        }
        
        Camera(const glm::vec3 & eye, const float yaw, const float pitch, const glm::vec3 & worldtop) {
            _position = eye;
            _worldUp = worldtop;
            updateCameraVectors();
        }
        
        Camera(const glm::vec3 & eye, const glm::vec3 & center, const glm::vec3 & top): Camera(eye, center, top, top) {}
        
        inline const glm::mat4 viewMatrix() const {
            return glm::lookAt(_position, _position+_front, _up);
        }
        
        operator glm::mat4 () const {
            return viewMatrix();
        }
        
        glm::vec3 position() {
            return _position;
        }
        
        glm::vec3 front() {
            return _front;
        }
        
        inline void moveUp(const float & speed) {
            _position += speed * _up;
        }
        
        inline void moveDown(const float & speed) {
            _position -= speed * _up;
        }
        
        inline void moveAhead(const float & speed) {
            _position += speed * _front;
        }
        
        inline void moveBack(const float & speed) {
            _position -= speed * _front;
        }
        
        inline void moveLeft(const float & speed) {
            _position -= speed * _right;
        }
        
        inline void moveRight(const float & speed) {
            _position += speed * _right;
        }
        
        inline void lookUp(const float & ddeg) {
            _pitch += ddeg;
            updateCameraVectors();
        }
        
        inline void lookDown(const float & ddeg) {
            _pitch -= ddeg;
            updateCameraVectors();
        }
        
        inline void lookRight(const float & ddeg) {
            _yaw += ddeg;
            updateCameraVectors();
        }
        
        inline void lookLeft(const float & ddeg) {
            _yaw -= ddeg;
            updateCameraVectors();
        }
        
    };
    
}

#endif /* Camera_hpp */
