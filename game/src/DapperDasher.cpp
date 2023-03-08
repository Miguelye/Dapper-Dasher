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
	const int WindowHeight = 380;
	const int WindowWidth = 512;
	// Initialiaze Window
	InitWindow(WindowWidth, WindowHeight, "Dapper Dasher");

	//Sprites
	// ***SCURFY***
	
	Texture2D scarfy = LoadTexture("textures/scarfy.png");
	AnimData scarfyData;

	scarfyData.rect.x = 0.0;
	scarfyData.rect.y = 0.0;
	scarfyData.rect.width = scarfy.width / 6;
	scarfyData.rect.height = scarfy.height;
	scarfyData.pos.x = WindowWidth / 2 - scarfyData.rect.width / 2;
	scarfyData.pos.y = WindowHeight - scarfyData.rect.height;
	scarfyData.frame = 0;
	scarfyData.updateTime = 1.0 / 12.0;
	scarfyData.runningTime = 0;

	// *** NEBULA ***
	Texture2D nebula = LoadTexture("textures/nebula.png");
	AnimData nebulaData
	{
		{0.0, 0.0, nebula.width / 8, nebula.height / 8}, //Rectangle Rect
		{WindowWidth, WindowHeight - nebulaData.rect.height}, //Vector2 pos
		0, //int frame
		1.0 / 12.0, //float updateTime
		0.0  //float runningTime
	};

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


		isCharacterGrounded = scarfyData.pos.y >= WindowHeight - scarfyData.rect.height;

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
		nebulaData.pos.x += -nebulaVel * dT;

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
		nebulaData.runningTime += dT;
		if (nebulaData.runningTime >= nebulaData.updateTime)
		{
			nebulaData.runningTime = 0.0;
			//Update animation frame
			nebulaData.rect.x = nebulaData.rect.width * nebulaData.frame;
			nebulaData.frame++;
			if (nebulaData.frame > 7)
			{
				nebulaData.frame = 0;
			}
		}


		//Drawing sprites

		DrawTextureRec(scarfy, scarfyData.rect, scarfyData.pos, WHITE);
		DrawTextureRec(nebula, nebulaData.rect, nebulaData.pos, WHITE);

		//Stop Drawing
		EndDrawing();
	}
	UnloadTexture(scarfy);
	UnloadTexture(nebula);
	CloseWindow();
}