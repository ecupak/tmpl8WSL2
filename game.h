#pragma once
#include "Timer.h"
#include "Triangle.h"

class Camera;

namespace Tmpl8
{
	class Game
	{
	public:
		// game flow methods
		void Init();
		void HandleInput(float deltaTime);
		void Tick(float deltaTime);

		void Shutdown()
		{
			/* implement if you want to do something on exit */
		}

		/*void SetTarget(Surface* s)
		{
			screen = s;
		}*/

		// data members
		//Surface* screen;
		//TODO less big array just to store input
		int keystate[66666];
		int2 mousePos;
		float vertices[9] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f, 0.5f, 0.0f
		};
		Shader* simpleShader = nullptr;
		Triangle triangle;
		Camera* camera = nullptr;
		inline static Timer timer;
	};
} // namespace Tmpl8
