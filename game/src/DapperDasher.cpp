#include "raylib.h"

struct AnimData
{
	Rectangle rect;
	Vector2 pos;
	int frame;
	float updateTime;
	float runningTime;
};

int main()
{
	//Window Data
	const int WINDOW_DIMENSIONS[2]{ 512, 380 };
	// Initialiaze Window
	InitWindow(WINDOW_DIMENSIONS[0], WINDOW_DIMENSIONS[1], "Dapper Dasher");

	//Sprites
	// ***SCURFY***
	
	Texture2D scarfy = LoadTexture("textures/scarfy.png");
	AnimData scarfyData;

	scarfyData.rect.x = 0.0;
	scarfyData.rect.y = 0.0;
	scarfyData.rect.width = scarfy.width / 6;
	scarfyData.rect.height = scarfy.height;
	scarfyData.pos.x = WINDOW_DIMENSIONS[0] / 2 - scarfyData.rect.width / 2;
	scarfyData.pos.y = WINDOW_DIMENSIONS[1] - scarfyData.rect.height;
	scarfyData.frame = 0;
	scarfyData.updateTime = 1.0 / 12.0;
	scarfyData.runningTime = 0;

	// *** NEBULA ***
	Texture2D nebula = LoadTexture("textures/nebula.png");

	const int NUMBER_OF_NEBULAS = 8;
	AnimData nebulas[NUMBER_OF_NEBULAS]{};

	for (size_t i = 0; i < NUMBER_OF_NEBULAS; i++)
	{
		nebulas[i].rect.x = 0.0;
		nebulas[i].rect.y = 0.0;
		nebulas[i].rect.width = nebula.width / 8;
		nebulas[i].rect.height = nebula.height / 8;
		nebulas[i].pos.x = WINDOW_DIMENSIONS[0] + (i * 300);
		nebulas[i].pos.y = WINDOW_DIMENSIONS[1] - nebulas[i].rect.height;
		nebulas[i].frame = 0;
		nebulas[i].runningTime = 0.0;
		nebulas[i].updateTime = 1.0 / 16.0;
	}

	//Bools
	bool isCharacterGrounded;

	//Forces
	const int gravity = 1'000; // pixel per second per second (p/s^2)
	const int jumpForce = -600; //pixels/second
	int scarfyVelocity = 0;	
	int nebulaVel = 200;

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		//Delta Time
		const float dT = GetFrameTime();

		//Start Drawing
		BeginDrawing();
		ClearBackground(WHITE);


		isCharacterGrounded = scarfyData.pos.y >= WINDOW_DIMENSIONS[1] - scarfyData.rect.height;

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
		scarfyData.pos.y += scarfyVelocity * dT;

		for (size_t i = 0; i < NUMBER_OF_NEBULAS; i++)
		{
			nebulas[i].pos.x += -nebulaVel * dT;
		}

		//Update Running Time
		//Scarfy
		scarfyData.runningTime += dT;
		if (scarfyData.runningTime >= scarfyData.updateTime && isCharacterGrounded)
		{
			scarfyData.runningTime = 0.0;
			//Update animation frame
			scarfyData.rect.x = scarfyData.rect.width * scarfyData.frame;
			scarfyData.frame++;
			if (scarfyData.frame > 5)
			{
				scarfyData.frame = 0;
			}
		}

		//Nebula
		for (size_t i = 0; i < NUMBER_OF_NEBULAS; i++)
		{
			nebulas[i].runningTime += dT;
			if (nebulas[i].runningTime >= nebulas[i].updateTime)
			{
				nebulas[i].runningTime = 0.0;
				//Update animation frame
				nebulas[i].rect.x = nebulas[i].rect.width * nebulas[i].frame;
				nebulas[i].frame++;
				if (nebulas[i].frame > 7)
				{
					nebulas[i].frame = 0;
				}
			}
		}



		//Drawing sprites

		DrawTextureRec(scarfy, scarfyData.rect, scarfyData.pos, WHITE);
		for (size_t i = 0; i < NUMBER_OF_NEBULAS; i++)
		{
			DrawTextureRec(nebula, nebulas[i].rect, nebulas[i].pos, WHITE);
		}

		//Stop Drawing
		EndDrawing();
	}
	UnloadTexture(scarfy);
	UnloadTexture(nebula);
	CloseWindow();
}