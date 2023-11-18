#include "Camera.h"


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

void Camera::Init()
{
}

void Camera::SetPosition(const float3& pos)
{
	position = pos;
}

void Camera::MoveZ(float multiplier)
{
	translation += camSpeed * cameraFront * multiplier;
}

void Camera::MoveX(float multiplier)
{
	translation += normalize(cross(cameraFront, cameraUp)) * camSpeed * multiplier;
}

void Camera::Update(float deltaTime)
{
	dir.x = cos(TO_RADIANS * (yaw)) * cos(TO_RADIANS * (pitch));
	dir.y = sin(TO_RADIANS * (pitch));
	dir.z = sin(TO_RADIANS * (yaw)) * cos(TO_RADIANS * (pitch));
	cameraFront = normalize(dir);
	position = position + translation * deltaTime;
	translation = 0;
}

mat4 Camera::LookAt() const
{
	return mat4::LookAt(position, position + cameraFront, cameraUp);
}

void Camera::RotateMouse(const int2& p)
{
	float xoffset = static_cast<float>(p.x) - lastX;
	float yoffset = lastY - static_cast<float>(p.y); // reversed since y-coordinates range from bottom to top
	lastX = static_cast<float>(p.x);
	lastY = static_cast<float>(p.y);

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;


	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}
