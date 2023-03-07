#include "raylib.h"


int main()
{
	//Window Data
	const int WindowHeight = 380;
	const int WindowWidth = 512;
	// Initialiaze Window
	InitWindow(WindowWidth, WindowHeight, "Dapper Dasher");

	//rectangle data
	const int rectHeight = 80;
	const int rectWidth = 50;
	int posY = WindowHeight - rectHeight;
	int posX = WindowWidth / 2;
	int rectVelocity = 0; //pixel per frame
	bool IsRectGrounded = posY >= WindowHeight - rectHeight;

	//Gravity
	const int gravity = 1; // pixel per frame per frame

	SetTargetFPS(60);
	while (!WindowShouldClose())
	{
		//Start Drawing
		BeginDrawing();
		ClearBackground(WHITE);

		IsRectGrounded = posY >= WindowHeight - rectHeight;

		//ground check
		if (IsRectGrounded)
		{
			//rectangle is grounded
			rectVelocity = 0;
		}
		else
		{
			//rectangle is on the air
			//Apply gravity
			rectVelocity += gravity;
		}

		//Check for jumping
		if (IsKeyPressed(KEY_SPACE))
		{
			rectVelocity = -20;
		}

		//update position
		posY += rectVelocity;

		DrawRectangle(posX, posY, rectWidth, rectHeight, RED);
		//Stop Drawing
		EndDrawing();
	}
	CloseWindow();
}