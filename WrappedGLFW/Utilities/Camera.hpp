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
        float _fieldOfView, _fovNear, _fovFar;
        unsigned int _viewportWidth, _viewportHeight;
        glm::mat4 _projection;
        
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
        
        void updateProjectionMatrix() {
            _projection = glm::perspective(fieldOfViewRadians(), viewportAspectRatio(), _fovNear, _fovFar);
        }
        
    public:
        Camera(const glm::vec3 & eye, const glm::vec3 & center, const glm::vec3 & top, const glm::vec3 & worldtop, unsigned int & width, unsigned int & height, const float & fov=45.0, const float & fov_near=0.1, const float & fov_far=100.0) {
            _position = eye;
            _front = glm::normalize(center - eye);
            _up = glm::normalize(top);
            _worldUp = glm::normalize(worldtop);
            _viewportWidth = width;
            _viewportHeight = height;
            _fieldOfView = fov;
            _fovNear = fov_near;
            _fovFar = fov_far;
            updateYawPitch();
            updateCameraVectors();
            updateProjectionMatrix();
        }
        
        Camera(const glm::vec3 & eye, const float yaw, const float pitch, const glm::vec3 & worldtop, unsigned int & width, unsigned int & height, const float & fov=45.0, const float & fov_near=0.1, const float & fov_far=100.0) {
            _position = eye;
            _worldUp = worldtop;
            _viewportWidth = width;
            _viewportHeight = height;
            _fieldOfView = fov;
            _fovNear = fov_near;
            _fovFar = fov_far;
            updateCameraVectors();
            updateProjectionMatrix();
        }
        
        Camera(const glm::vec3 & eye, const glm::vec3 & center, const glm::vec3 & top, unsigned int & width, unsigned int & height, const float & fov=45.0, const float & fov_near=0.1, const float & fov_far=100.0): Camera(eye, center, top, top, width, height, fov, fov_near, fov_far) {}
        
        inline const glm::mat4 viewMatrix() const {
            return glm::lookAt(_position, _position+_front, _up);
        }
        
        inline const glm::mat4 projectionMatrix() const {
            return _projection;
        }
        
        operator glm::mat4 () const {
            return viewMatrix();
        }
        
        inline const glm::vec3 position() const {
            return _position;
        }
        
        inline const glm::vec3 front() const {
            return _front;
        }
        
        inline const float fieldOfViewDegrees() const {
            return _fieldOfView;
        }
        
        inline const float fieldOfViewRadians() const {
            return glm::radians(_fieldOfView);
        }
        
        inline const unsigned int viewportWidth() const {
            return _viewportWidth;
        }
        
        inline const unsigned int viewportHeight() const {
            return _viewportHeight;
        }
        
        inline const float viewportAspectRatio() const {
            return (float)_viewportWidth / (float)_viewportHeight;
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
        
        inline void zoomIn(const float & ddeg) {
            _fieldOfView -= ddeg;
            updateProjectionMatrix();
        }
        
        inline void zoomOut(const float & ddeg) {
            _fieldOfView += ddeg;
            updateProjectionMatrix();
        }
        
        inline void setViewportWidth(unsigned int s) {
            _viewportWidth = s;
            updateProjectionMatrix();
        }
        
        inline void setViewportHeight(unsigned int s) {
            _viewportHeight = s;
            updateProjectionMatrix();
        }
        
        inline void setFoVNear(const float & d) {
            _fovNear = d;
            updateProjectionMatrix();
        }
        
        inline void setFoVFar(const float & d) {
            _fovFar = d;
            updateProjectionMatrix();
        }
        
    };
    
}

#endif /* Camera_hpp */
