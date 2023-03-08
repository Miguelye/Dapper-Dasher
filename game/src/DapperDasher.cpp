#include "raylib.h"


int main()
{
	//Window Data
	const int WindowHeight = 380;
	const int WindowWidth = 512;
	// Initialiaze Window
	InitWindow(WindowWidth, WindowHeight, "Dapper Dasher");

	//Sprites
	// ***SCURFY***
	Texture2D scarfy = LoadTexture("textures/scarfy.png");
	Rectangle scarfyRect;
	scarfyRect.width = scarfy.width / 6;
	scarfyRect.height = scarfy.height;
	scarfyRect.x = 0;
	scarfyRect.y = 0;

	Vector2 scarfyPos;
	scarfyPos.x = WindowWidth / 2 - scarfyRect.width / 2;
	scarfyPos.y = WindowHeight - scarfyRect.height;

	int scarfyFrame = 0;

	// *** NEBULA ***
	Texture2D nebula = LoadTexture("textures/nebula.png");
	Rectangle nebulaRect;
	nebulaRect.width = nebula.width / 8;
	nebulaRect.height = nebula.height / 8;
	nebulaRect.x = 0.0;
	nebulaRect.y = 0.0;
	Vector2 nebulaPos{ WindowWidth, WindowHeight - nebulaRect.height};
	int nebulaVel = 400; //pixels per second
	


	//Bools
	bool isCharacterGrounded;

	//Forces
	const int gravity = 1'000; // pixel per second per second (p/s^2)
	const int jumpForce = -600; //pixels/second
	int scarfyVelocity = 0;	

	//Time
	const float updateTime = 1.0 / 12.0;
	float runningTime = 0;

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		//Delta Time
		const float dT = GetFrameTime();

		//Start Drawing
		BeginDrawing();
		ClearBackground(WHITE);


		isCharacterGrounded = scarfyPos.y >= WindowHeight - scarfyRect.height;

		//ground check
		if (isCharacterGrounded)
		{
			//rectangle is grounded
			scarfyVelocity = 0;
		}
		else
		{
			//rectangle is on the air
			//Apply gravity
			scarfyVelocity += gravity * dT;
		}

		//Check for jumping
		if (IsKeyPressed(KEY_SPACE) && isCharacterGrounded)
		{
			scarfyVelocity = jumpForce;
		}

		//update positions
		scarfyPos.y += scarfyVelocity * dT;
		nebulaPos.x += -nebulaVel * dT;

		//Update Running Time
		runningTime += dT;
		if (runningTime >= updateTime && isCharacterGrounded)
		{
			runningTime = 0.0;
			//Update animation frame
			scarfyRect.x = scarfyRect.width * scarfyFrame;
			scarfyFrame++;
			if (scarfyFrame > 5)
			{
				scarfyFrame = 0;
			}
		}



		//Drawing sprites

		DrawTextureRec(scarfy, scarfyRect, scarfyPos, WHITE);
		DrawTextureRec(nebula, nebulaRect, nebulaPos, WHITE);

		//Stop Drawing
		EndDrawing();
	}
	UnloadTexture(scarfy);
	UnloadTexture(nebula);
	CloseWindow();
}