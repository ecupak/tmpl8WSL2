﻿#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
	Camera();
	~Camera();
	glm::mat4 LookAt() const;
	void Init();
	void SetPosition(const glm::vec3& pos);
	void MoveZ(float multiplier);
	void MoveX(float multiplier);
	void Update(float deltaTime);
	//void SetViewMatrix(Shader* shader);
	void RotateMouse(const glm::vec2& p);

private:
	//void GetCameraAxis(const float3& cameraTarget);
	//	float lastX = 0, lastY = 0;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 dir = glm::vec3(0.0f);
	glm::vec3 translation = glm::vec3(0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	const float camSpeed = 20.0f;

	//euler angles
	float yaw = -90.0f;
	float pitch = 0.0f;
};
