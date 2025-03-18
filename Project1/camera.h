#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    NONE = 0,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Default Camera Values
const float YAW = -90.0f; // rotates over y axis (up and down)
const float PITCH = 0.0f; // rotates over x axis (left and right)
const float ROLL = 0.0f; // rotates over z axis (sideways, not necessary for default use)
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class Camera 
{
public:
    // camera Attributes
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldup;

    // euler angles
    float yaw;
    float pitch;

    // camera options
    float speed;
    float sensitivity;
    float zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(speed), sensitivity(sensitivity), zoom(zoom)
    {
        position = position;
        worldup = up;
        yaw = yaw;
        pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(speed), sensitivity(sensitivity), zoom(zoom)
    {
        position = glm::vec3(posX, posY, posZ);
        worldup = glm::vec3(upX, upY, upZ);
        yaw = yaw;
        pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using euler angles and the lookAt Matrix
    mat4 GetViewMatrix()
    {
        return lookAt(position, position + front, up);

        // first parameter is the current camera position
        // second parameter is the target point the camera should be looking at
        // the up vector on the y axis (vec3(0,1,0))
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = speed * deltaTime;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainpitch = true)
    {
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainpitch)
        {
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }

        // update front, right and up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        zoom -= (float)yoffset;
        if (zoom < 1.0f)
            zoom = 1.0f;
        if (zoom > 45.0f)
            zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new front vector
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);
        // also re-calculate the right and up vector
        right = normalize(glm::cross(front, worldup));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up = normalize(glm::cross(right, front));
    }

};
#endif