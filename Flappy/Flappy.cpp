#include "Flappy.h"
#include <raylib.h>
#include <algorithm>
#include <iostream>

void Flappy::Main()
{
	InitWindow(screenWidth, screenHeight, "Flappy");

	Start();

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		Update();
	}

	CloseWindow();
}


void Flappy::Start()
{
	gameOver = false;
	columns.clear();
	score = 0;

	for (int i = 0; i < 5; i++)
	{
		float x = 300 + GAP * i;
		Column column = Column(x);
		columns.push_back(column);
	}

	bird.position = Vector2{ 100, (float)GetRandomValue(100, screenHeight - 100) };
}

void Flappy::EvalCurFrame()
{
	if (gameOver)
	{
		if (IsKeyPressed(KEY_ENTER))
			Start();

		return;
	}

	bird.Move();

	if (cheats) {
		int id;
		if (columns[0].gained) {
			id = 1;
		}
		else {
			id = 0;
		}
		if (bird.position.y > (columns[id].heightTop + 50)) {
			bird.position.y -= 3;
		}
	}

	for (int i = 0; i < columns.size(); i++)
	{
		columns[i].Move();

		if (columns[i].CheckCollisionWithBird(bird))
			gameOver = true;

		if (!columns[i].gained)
			if (columns[i].Passed(bird)) {
				score++;
				if (score > highScore)
					highScore = score;
			}
	}
}

void Flappy::DrawCurFrame()
{
	BeginDrawing();
	ClearBackground(RAYWHITE);


	if (!gameOver)
	{
		for (int i = 0; i < columns.size(); i++)
		{
			if (columns[i].IsOutOfBounds())
			{
				columns[i].x = columns[columns.size() - 1].x + GAP;
				columns[i].heightTop = GetRandomValue(175, 275);
				columns[i].yBottom = columns[i].heightTop + 100;
				columns[i].heightBottom = screenHeight - columns[i].yBottom;
				columns[i].topRect = Rectangle{ columns[i].x, 0, columns[i].width, columns[i].heightTop };
				columns[i].botRect = Rectangle{ columns[i].x, columns[i].yBottom, columns[i].width, columns[i].heightBottom };
				columns[i].color = GRAY;
				columns[i].gained = false;

			std::rotate(columns.begin() + i, columns.begin() + 1 + i, columns.end());
			}

			columns[i].Draw();
		}

		bird.Draw();


		DrawText(TextFormat("Score: %03i", score), 20, 20, 16, DARKBLUE);
		DrawText(TextFormat("High Score: %03i", highScore), 20, 40, 16, DARKGREEN);
	}
	else
		DrawText("Game Over, Press Enter to Restart",
			screenWidth / 2 - MeasureText("Game Over, Press Enter to Restart", 24) / 2,
			screenHeight / 2 - 12,
			24, BLACK);


	EndDrawing();
}

void Flappy::Update()
{
	EvalCurFrame();
	DrawCurFrame();
}