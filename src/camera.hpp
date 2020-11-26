#ifndef GUARD_CAMERA_H
#define GUARD_CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "vec.hpp"

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera {
public:
    Camera(vec3 position = vec3(0, 0, 0), vec3 up = vec3(0, 1, 0), float yaw = YAW, float pitch = PITCH);

    glm::mat4 GetViewMatrix();
    void ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    void updateCameraVectors();

public:
    vec3 position;
    vec3 up;
    glm::vec3 Up;
    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
};
#endif
