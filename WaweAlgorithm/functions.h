#pragma once

#include"raylib.h"


typedef struct line {
	Vector2 position;
	Vector2 boxSize;
	int fontSize;
	Color boxColor;
	Color textColor;
	Color contourColor;
	int length;
	char* currentStr;
	bool isActive;
} line;


int GetNumber(line* _line);

void DrawTextLine(line* _line);

void InputText(line* _line);

void LineActivation(line* _line);

void LineConstruct(line* _line);

void DrawField(Vector2 fieldSize, Vector2 screenSize, int** field);

void FirstStage(line* line1, line* line2, Vector2* fieldSize, Vector2 screenSize, bool* getCorrectSize);

void SecondStage(Vector2 fieldSize, Vector2 screenSize, int** field, bool* isWallsSet);

void ThirdStage(Vector2 fieldSize, Vector2 screenSize, int** field, bool* isStartSet, bool* isFinishSet, bool* isStartFinishSet);

void FourthStage(Vector2 fieldSize, Vector2 screenSize, int** field);