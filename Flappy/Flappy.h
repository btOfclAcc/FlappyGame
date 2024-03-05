#include <vector>
#include "raylib.h"

#pragma once

#ifndef FLAPPY_H
#define FLAPPY_H

using namespace std;


class Flappy
{
public:
	
	static const int screenWidth = 800; // 
	static const int screenHeight = 450;
	static const int GAP = 250;

	struct Bird
	{
		Vector2 position;
		int radius = 20;
		Color color = BLACK;

		void Draw()
		{
			DrawCircleV(position, radius, color);
		}

		void Move()
		{
			if (IsKeyDown(KEY_SPACE))
				position.y -= 3;
			else
				position.y += 2;
		}
	};

	struct Column 
	{
		float x; 
		float width = 80;
		float heightTop;
		float heightBottom;
		float yBottom;

		Color color = GRAY;

		Rectangle topRect, botRect; 

		bool gained = false;

		// constructor -> Custom with minimum number of args
		Column(float x)
		{
			this->x = x;

			this->heightTop = GetRandomValue(175, 275);

			this->yBottom = heightTop + 100;
			this->heightBottom = screenHeight - yBottom;


			this->topRect = Rectangle{ x, 0, width, heightTop };
			this->botRect = Rectangle{ x, yBottom, width, heightBottom };
		}
	
		void Draw()
		{
			// draw it if within bounds
			if (x + width > 0 && x < screenWidth)
			{
				DrawRectangleRec(topRect, color);
				DrawRectangleRec(botRect, color);
			}
		}

		void Move()
		{
			x -= 1;
			topRect.x = x;
			botRect.x = x;
		}

		bool Passed(Bird bird)
		{
			if (x + width < bird.position.x - bird.radius && !gained)
			{
				color = RED;
				gained = true;
				return true;
			}

			return false;
		}

		bool CheckCollisionWithBird(Bird bird)
		{
			return CheckCollisionCircleRec(bird.position, bird.radius, topRect) ||
				CheckCollisionCircleRec(bird.position, bird.radius, botRect);
		}

		bool IsOutOfBounds()
		{
			return x + width < 0;
		}
	};

	bool gameOver = false;

	int score;
	int highScore;

	Bird bird;

	vector<Column> columns;

	bool cheats = true;

	void Main();
	void Start();
	void EvalCurFrame();
	void DrawCurFrame();
	void Update();
};
#endif // !FLAPPY_H