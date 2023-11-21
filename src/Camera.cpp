﻿#include "Camera.h"

#include "template.h"


//void Camera::GetCameraAxis(const float3& cameraTarget)
//{
//	cameraFront = normalize(position - cameraTarget);
//	cameraRight = normalize(cross(float3(0.0f, 1.0f, 0.0f), cameraFront));
//	cameraUp = cross(cameraFront, cameraRight);
//}

Camera::Camera()
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::LookAt() const
{
	return glm::lookAt(position, position + cameraFront, cameraUp);
}

void Camera::Init()
{
}

void Camera::SetPosition(const glm::vec3& pos)
{
	position = pos;
}


void Camera::MoveZ(float multiplier)
{
	cameraFront.y = 0;
	translation += camSpeed * cameraFront * multiplier;
}

void Camera::MoveX(float multiplier)
{
	translation += normalize(cross(cameraFront, cameraUp)) * camSpeed * multiplier;
}


void Camera::Update(const float deltaTime, const KeyboardManager& km)
{
	HandleInput(deltaTime, km);

	dir.x = cos(TO_RADIANS * (yaw)) * cos(TO_RADIANS * (pitch));
	dir.y = sin(TO_RADIANS * (pitch));
	dir.z = sin(TO_RADIANS * (yaw)) * cos(TO_RADIANS * (pitch));

	cameraFront = normalize(dir);
	position = position + translation * deltaTime;
	translation = { 0, 0, 0 };
}


void Camera::HandleInput(const float deltaTime, const KeyboardManager& km)
{
	// Get inputs.
	int xMovement{ km.IsPressed(Action::MoveLeft) ? 1 : 0 + km.IsPressed(Action::MoveRight) ? -1 : 0 };
	int zMovement{ km.IsPressed(Action::MoveForward) ? 1 : 0 + km.IsPressed(Action::MoveBackward) ? -1 : 0 };

	int yawAmount{ km.IsPressed(Action::YawLeft) ? 1 : 0 + km.IsPressed(Action::YawRight) ? -1 : 0 };
	int pitchAmount{ km.IsPressed(Action::PitchUp) ? 1 : 0 + km.IsPressed(Action::PitchDown) ? -1 : 0 };

	// Apply inputs.
	MoveX(static_cast<float>(xMovement));
	MoveZ(static_cast<float>(zMovement));

	yaw += static_cast<float>(yawAmount) * yawSpeed * deltaTime;
	pitch += static_cast<float>(pitchAmount) * pitchSpeed * deltaTime;
	PreventPitchFlip();
}


void Camera::RotateMouse(const glm::vec2& p)
{
	float xoffset = p.x;
	float yoffset = -p.y; // reversed since y-coordinates range from bottom to top

	const float sensitivity = 0.3f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	PreventPitchFlip();
}


void Camera::PreventPitchFlip()
{
	// Prevent flipping.
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	else if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
}


const float Camera::GetFOV() const
{
	return fov;
}