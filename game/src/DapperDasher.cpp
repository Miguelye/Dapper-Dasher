#include "raylib.h"

// ***Global Variables***
//Window Data
const int WINDOW_DIMENSIONS[2]{ 512, 380 };

struct AnimData
{
	int columns;
	int rows;
	Rectangle rect;
	Vector2 pos;
	int frame;
	float updateTime;
	float runningTime;
};

void InitializeScarfy(Texture2D, AnimData&);
void InitializeNebula(Texture2D, AnimData&);
void UpdatePositionScarfy(AnimData&, int, const float& dT);
void UpdatePositionNebula(AnimData&, int, const float& dT);
void UpdateRunningTime(AnimData&, const float& dT);
bool IsGrounded(AnimData& data);

int main()
{
	// Initialiaze Window
	InitWindow(WINDOW_DIMENSIONS[0], WINDOW_DIMENSIONS[1], "Dapper Dasher");

	//Sprites
	// ***SCURFY***
	
	Texture2D scarfy = LoadTexture("textures/scarfy.png");
	AnimData scarfyData;
	InitializeScarfy(scarfy, scarfyData);

	// *** NEBULA ***
	Texture2D nebula = LoadTexture("textures/nebula.png");

	const int NUMBER_OF_NEBULAS = 8;
	AnimData nebulas[NUMBER_OF_NEBULAS]{};

	for (size_t i = 0; i <  NUMBER_OF_NEBULAS; i++)
	{
		InitializeNebula(nebula, nebulas[i]);
		nebulas[i].pos.x = WINDOW_DIMENSIONS[0] + i * 300;
	}

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

		//ground check
		if (IsGrounded(scarfyData))
		{
			//rectangle is grounded
			scarfyVelocity = 0;

			//Check for jumping
			if (IsKeyPressed(KEY_SPACE))
			{
				scarfyVelocity = jumpForce;
			}

			UpdateRunningTime(scarfyData, dT);
		}
		else
		{
			//rectangle is on the air
			//Apply gravity
			scarfyVelocity += gravity * dT;
		}



		//update positions
		UpdatePositionScarfy(scarfyData, scarfyVelocity, dT);
		for (size_t i = 0; i < NUMBER_OF_NEBULAS; i++)
		{
			UpdatePositionNebula(nebulas[i], nebulaVel, dT);
		}

		//Update Running Time
		for (size_t i = 0; i < NUMBER_OF_NEBULAS; i++)
		{
			UpdateRunningTime(nebulas[i], dT);
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

void InitializeNebula(Texture2D nebula, AnimData& nebulaData)
{
	nebulaData.columns = 8;
	nebulaData.rows = 1;
	nebulaData.rect.x = 0.0;
	nebulaData.rect.y = 0.0;
	nebulaData.rect.width = nebula.width / nebulaData.columns;
	nebulaData.rect.height = nebula.height / nebulaData.columns;
	nebulaData.pos.x = WINDOW_DIMENSIONS[0];
	nebulaData.pos.y = WINDOW_DIMENSIONS[1] - nebulaData.rect.height;
	nebulaData.frame = 0;
	nebulaData.runningTime = 0.0;
	nebulaData.updateTime = 1.0 / 16.0;
}

void InitializeScarfy(Texture2D scarfy, AnimData& scarfyData)
{
	scarfyData.columns = 6;
	scarfyData.rows = 1;
	scarfyData.rect.x = 0.0;
	scarfyData.rect.y = 0.0;
	scarfyData.rect.width = scarfy.width / scarfyData.columns;
	scarfyData.rect.height = scarfy.height;
	scarfyData.pos.x = WINDOW_DIMENSIONS[0] / 2 - scarfyData.rect.width / 2;
	scarfyData.pos.y = WINDOW_DIMENSIONS[1] - scarfyData.rect.height;
	scarfyData.frame = 0;
	scarfyData.updateTime = 1.0 / 12.0;
	scarfyData.runningTime = 0;
}

void UpdatePositionNebula(AnimData& nebulaData, int velocity, const float& dT)
{
	nebulaData.pos.x += -velocity * dT;
}

void UpdatePositionScarfy(AnimData& scarfyData, int velocity, const float& dT)
{
	scarfyData.pos.y += velocity * dT;
}

void UpdateRunningTime(AnimData& data, const float& dT)
{
	data.runningTime += dT;
	if (data.runningTime >= data.updateTime)
	{
		data.runningTime = 0.0;
		//Update animation frame
		data.rect.x = data.rect.width * data.frame;
		data.frame++;
		if (data.frame > data.columns - 1)
		{
			data.frame = 0;
		}
	}
}

bool IsGrounded(AnimData& data)
{
	return data.pos.y >= WINDOW_DIMENSIONS[1] - data.rect.height;
}