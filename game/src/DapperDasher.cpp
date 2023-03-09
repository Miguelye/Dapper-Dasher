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
void UpdatePosition(AnimData&, int, int, const float& dT);
void UpdatePosition(float, int, const float& dT);
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
		nebulas[i].pos.x = WINDOW_DIMENSIONS[0] + (i * 700);
	}

	float finishLine = nebulas[NUMBER_OF_NEBULAS - 1].pos.x + 400;
	// *** BACKGROUND ***
	Texture2D background = LoadTexture("textures/far-buildings.png");
	Texture2D midground = LoadTexture("textures/back-buildings.png");
	Texture2D foreground = LoadTexture("textures/foreground.png");
	float bgX{ 0 };
	float mgX{ 0 };
	float fgX{ 0 };

	//Forces
	const int gravity = 1'000; // pixel per second per second (p/s^2)
	const int jumpForce = -600; //pixels/second
	int scarfyJump = 0;	
	int nebulaVel = 300;

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		//Delta Time
		const float dT = GetFrameTime();

		//Start Drawing
		BeginDrawing();
		ClearBackground(WHITE);
		//Draw background
		bgX -= 20 * dT;
		if (bgX <= -background.width * 2)
		{
			bgX = 0;
		}

		mgX -= 40 * dT;
		if (mgX <= -midground.width * 2)
		{
			mgX = 0;
		}

		fgX -= 80 * dT;
		if (fgX <= -foreground.width * 2)
		{
			fgX = 0;
		}

		//background
		Vector2 bg1Pos{ bgX, 0.0 };
		DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);

		Vector2 bg2Pos{ bgX + background.width * 2, 0.0 };
		DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

		//midground
		Vector2 mg1Pos{ mgX, 0.0 };
		DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);

		Vector2 mg2Pos{ mgX + midground.width * 2, 0.0 };
		DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

		//foreground
		Vector2 fg1Pos{ fgX, 0.0 };
		DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);

		Vector2 fg2Pos{ fgX + foreground.width * 2, 0.0 };
		DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

		//ground check
		if (IsGrounded(scarfyData))
		{
			//rectangle is grounded
			scarfyJump = 0;

			//Check for jumping
			if (IsKeyPressed(KEY_SPACE))
			{
				scarfyJump = jumpForce;
			}

			UpdateRunningTime(scarfyData, dT);
		}
		else
		{
			//rectangle is on the air
			//Apply gravity
			scarfyJump += gravity * dT;
		}

		//update positions
		UpdatePosition(scarfyData, 0, scarfyJump, dT);
		UpdatePosition(finishLine, -nebulaVel, dT);
		for (size_t i = 0; i < NUMBER_OF_NEBULAS; i++)
		{
			UpdatePosition(nebulas[i], -nebulaVel, 0, dT);
		}

		//Update Running Time
		for (size_t i = 0; i < NUMBER_OF_NEBULAS; i++)
		{
			UpdateRunningTime(nebulas[i], dT);
		}


		bool IsCollision{};
		for each (AnimData nebula in nebulas)
		{
			float pad = 20;
			Rectangle nebRect
			{
				nebula.pos.x + pad,
				nebula.pos.y + pad,
				nebula.rect.width - 2*pad,
				nebula.rect.height - 2*pad
			};
			Rectangle scarfyRect
			{
				scarfyData.pos.x,
				scarfyData.pos.y,
				scarfyData.rect.width,
				scarfyData.rect.height
			};
			if (CheckCollisionRecs(nebRect, scarfyRect))
			{
				IsCollision = true;
			}
		}

		if (IsCollision)
		{

		}
		else 
		{
			//Drawing sprites
			DrawTextureRec(scarfy, scarfyData.rect, scarfyData.pos, WHITE);
			for (size_t i = 0; i < NUMBER_OF_NEBULAS; i++)
			{
				DrawTextureRec(nebula, nebulas[i].rect, nebulas[i].pos, WHITE);
			}
		}

		//Stop Drawing
		EndDrawing();
	}
	UnloadTexture(scarfy);
	UnloadTexture(nebula);
	UnloadTexture(background);
	UnloadTexture(midground);
	UnloadTexture(foreground);
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

void UpdatePosition(AnimData& data, int velocityX, int velocityY, const float& dT)
{
	data.pos.x += velocityX * dT;
	data.pos.y += velocityY * dT;
}

void UpdatePosition(float data, int velocity, const float& dT)
{
	data += velocity * dT;
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