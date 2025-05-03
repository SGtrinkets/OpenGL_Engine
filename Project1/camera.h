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

    // posiiton of the camera
    vec3 position;
	// points forward according to the cartesian coordiante system (where x is right, y is up, -z is forward)
    vec3 Front;
	// points up to where the camera is (positive y axis)
    vec3 up;
	// points right to where the camera is (positive x axis)
    vec3 right;
	// always points in the direction of the world up vector (positive y axis)
    vec3 worldup;

    // euler angles
    float yaw;
    float pitch;

    // camera options
    float speed;
    float sensitivity;
    float zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM)
    {
        this->position = position;
        this->worldup = up;
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(double posX, double posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM)
    {
        position = glm::vec3(posX, posY, posZ);
        worldup = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using euler angles and the lookAt Matrix
    mat4 GetViewMatrix()
    {
        return lookAt(position, position + Front, up);

        // first parameter is the current camera position
        // second parameter is the target point the camera should be looking at
        // the up vector on the y axis (vec3(0,1,0))
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = speed * deltaTime;
        if (direction == FORWARD)
            position += Front * velocity;
        if (direction == BACKWARD)
            position -= Front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
        if (direction == UP)
            position += worldup * velocity;
        if (direction == DOWN)
            position -= worldup * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(double xoffset, double yoffset, GLboolean constrainpitch = true)
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
    void ProcessMouseScroll(double yoffset)
    {
        zoom -= (float)yoffset;
        if (zoom < 45.0f)
            zoom = 45.0f;
        if (zoom > 135.0f)
            zoom = 135.0f;
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
        Front = glm::normalize(front);
        // also re-calculate the right and up vector
        right = normalize(glm::cross(Front, worldup));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up = normalize(glm::cross(right, Front));
    }

};
#endif