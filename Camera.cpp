#include "Camera.h"

Camera::Camera(glm::vec3 startPosition, glm::vec3 startWorldUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed) {
	position = startPosition;
	worldUp = startWorldUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	trunSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime) {
	GLfloat velocity = deltaTime * moveSpeed;

	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
		position += front * velocity;
	}
	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
		position -= front * velocity;
	}
	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
		position -= right * velocity;
	}
	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
		position += right * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange, GLfloat deltaTime) {
	GLfloat velocity = deltaTime * trunSpeed;

	yaw += xChange * velocity;
	pitch += yChange * velocity;

	if (pitch > 90.0f)
		pitch = 90.0f;
	if (pitch < -90.0f)
		pitch = -90.0f;
	
	update();
}

void Camera::update() {
	front.x = cos(glm::radians(yaw))* cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::calculateViewMatrix() {
	return glm::lookAt(position, position + front, up);
}


Camera::Camera(){}
Camera::~Camera(){}