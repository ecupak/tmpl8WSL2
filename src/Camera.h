#pragma once
#include "game.h"

class Camera
{
public:
	Camera();
	~Camera();

	void Init();
	void SetPosition(const float3& pos);
	void MoveZ(float multiplier);
	void MoveX(float multiplier);
	void Update(float deltaTime);
	mat4 LookAt() const;
	void RotateMouse(const int2& int2);
	//void SetViewMatrix(Shader* shader);

private:
	//void GetCameraAxis(const float3& cameraTarget);
	//	float lastX = 0, lastY = 0;
	float3 position = float3(0.0f, 0.0f, 3.0f);
	float3 cameraFront = float3(0.0f, 0.0f, -1.0f);
	float3 dir = 0;
	float3 translation = 0;
	float3 cameraUp = float3(0.0f, 1.0f, 0.0f);
	const float camSpeed = 0.2f;

	//euler angles
	float yaw = -90.0f;
	float pitch = 0.0f;
};
