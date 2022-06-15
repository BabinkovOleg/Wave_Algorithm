#include <malloc.h>

#include "raylib.h"

#include "functions.h"

#define MAX_INPUT_LENGTH 3

#define MAX_FIELD_SIZE 20

int main(void)
{
	const Vector2 screenSize = (Vector2) { 550, 550 };

	line line1, line2;
	
	LineConstruct(&line1);
	LineConstruct(&line2);

	line1.position = (Vector2) { screenSize.x * 2 / 3 - screenSize.x / 20, screenSize.y / 20 };
	line1.boxSize = (Vector2) { screenSize.x / 3, screenSize.y / 10 };
	line2.position = (Vector2) { screenSize.x * 2 / 3 - screenSize.x / 20, screenSize.y * 4 / 20 };
	line2.boxSize = (Vector2) { screenSize.x / 3, screenSize.y / 10 };
	Vector2 fieldSize = { -1, -1 };

	Vector2 startPos, finishPos;

	int** field = (int**)malloc(sizeof(int*) * MAX_FIELD_SIZE);
	for (int i = 0; i < MAX_FIELD_SIZE; ++i)
		field[i] = (int*)malloc(sizeof(int) * MAX_FIELD_SIZE);

	for (int i = 0; i < MAX_FIELD_SIZE; ++i) {
		for (int j = 0; j < MAX_FIELD_SIZE; ++j) {
			field[i][j] = 0;
		}
	}

	bool getCorrectSize = false;

	bool isWallsSet = false;

	bool isStartSet = false;
	bool isFinishSet = false;
	bool isStartFinishSet = false;
	bool isPathFound = false;

	InitWindow(screenSize.x, screenSize.y * 1.15, "Wave algorithm");

	SetTargetFPS(60);

	while (!WindowShouldClose())
	{
		BeginDrawing();

		ClearBackground(RAYWHITE);

		if (!getCorrectSize) {
			FirstStage(&line1, &line2, &fieldSize, screenSize, &getCorrectSize);
		}
		else {
			if (!isWallsSet)
				SecondStage(fieldSize, screenSize, field, &isWallsSet);
			else {
				if(!isStartFinishSet)
					ThirdStage(fieldSize, screenSize, field, &isStartSet, &isFinishSet, &isStartFinishSet, &startPos, &finishPos);
				else {
					if (!isPathFound) {
						FourthStage(fieldSize, screenSize, field, startPos, finishPos);
						isPathFound = true;
					}
					else {
						DrawField(fieldSize, screenSize, field);
					}
				}
			}
		}

		EndDrawing();
	}

	CloseWindow();

	return 0;
}