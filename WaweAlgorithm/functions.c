#include "functions.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_INPUT_LENGTH 3

#define MAX_FIELD_SIZE 20

int GetNumber(line* _line) {
	return atoi(_line->currentStr);
}

void DrawTextLine(line* _line) {
	DrawRectangleV(_line->position, _line->boxSize, _line->contourColor);
	DrawRectangle(_line->position.x + 1, _line->position.y + 1, _line->boxSize.x - 2, _line->boxSize.y - 2, _line->boxColor);
	
	char* str = _line->currentStr;
	if (_line->isActive) {
		str = TextFormat("%s_", _line->currentStr);
	}
	DrawText(str, _line->position.x + 5, _line->position.y, _line->fontSize, _line->textColor);
}

void InputText(line* _line) {
	if (_line->isActive) {
		int keyChar = GetCharPressed();
		int key = GetKeyPressed();

		if (keyChar >= 48 && keyChar <= 57 && _line->length < MAX_INPUT_LENGTH) {
			_line->currentStr[_line->length] = (char)keyChar;
			_line->currentStr[_line->length + 1] = '\0';
			_line->length += 1;
		}

		if (key == 259 && _line->length > 0) {
			_line->length -= 1;
			_line->currentStr[_line->length] = '\0';
		}
	}
}

void LineActivation(line* _line) {
	if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
		if (CheckCollisionPointRec(GetMousePosition(), (Rectangle) { _line->position.x, _line->position.y, _line->boxSize.x, _line->boxSize.y }))
			_line->isActive = true;
		else
			_line->isActive = false;
	}
}

void LineConstruct(line* _line) {
	_line->position.x = 0;
	_line->position.y = 0;
	_line->boxSize.y = 40;
	_line->boxSize.x = 200;
	_line->currentStr = (char*)malloc(sizeof(char) * (MAX_INPUT_LENGTH + 1));
	_line->currentStr[0] = '\0';
	_line->fontSize = 40;
	_line->length = 0;
	_line->boxColor = LIGHTGRAY;
	_line->textColor = RED;
	_line->contourColor = RED;
	_line->isActive = false;
}

void DrawField(Vector2 fieldSize, Vector2 screenSize, int** field) {
	for (int i = 0; i <= fieldSize.x; ++i) {
		DrawLine(screenSize.x / 20, screenSize.y / 20 + i * screenSize.y * 18 / 20 / fieldSize.x, screenSize.x * 19 / 20, screenSize.y / 20 + i * screenSize.y * 18 / 20 / fieldSize.x, BLACK);
	}
	for (int i = 0; i <= fieldSize.y; ++i) {
		DrawLine(screenSize.x / 20 + i * screenSize.x * 18 / 20 / fieldSize.y, screenSize.y / 20, screenSize.x / 20 + i * screenSize.x * 18 / 20 / fieldSize.y, screenSize.y * 19 / 20, BLACK);
	}

	for (int i = 0; i < fieldSize.x; ++i) {
		for (int j = 0; j < fieldSize.y; ++j) {
			if (field[i][j] != 0) {
				Rectangle rec;
				rec.x = screenSize.x / 20 + j * screenSize.x * 18 / 20 / fieldSize.y + 1;
				rec.y = screenSize.y / 20 + i * screenSize.y * 18 / 20 / fieldSize.x + 1;
				rec.width = screenSize.x * 18 / 20 / fieldSize.y - 2;
				rec.height = screenSize.y * 18 / 20 / fieldSize.x - 2;
				if(field[i][j] == -1)
					DrawRectangleRec(rec, BROWN);
				if (field[i][j] == -2)
					DrawRectangleRec(rec, GREEN);
				if (field[i][j] == -3)
					DrawRectangleRec(rec, RED);
			}
		}
	}
}

void FirstStage(line* line1, line* line2, Vector2* fieldSize, Vector2 screenSize, bool* getCorrectSize) {
	LineActivation(line1);
	LineActivation(line2);

	InputText(line1);
	InputText(line2);

	if (IsKeyPressed(KEY_ENTER)) {
		fieldSize->x = GetNumber(line1);
		fieldSize->y = GetNumber(line2);
		if (fieldSize->x <= MAX_FIELD_SIZE && fieldSize->y <= MAX_FIELD_SIZE && fieldSize->x > 1 && fieldSize->y > 1)
			*getCorrectSize = true;
		else
			*getCorrectSize = false;
	}

	DrawTextLine(line1);
	DrawTextLine(line2);

	if (!*getCorrectSize && fieldSize->x != -1 && fieldSize->y != -1)
		DrawText("Invalid field size", screenSize.x / 20, line2->position.y + line2->boxSize.y * 2, line2->fontSize, RED);

	DrawText("Number of rows:", screenSize.x / 20, screenSize.y * 3 / 40, 27, DARKGRAY);
	DrawText("Number of columns:", screenSize.x / 20, screenSize.y * 9 / 40, 27, DARKGRAY);

	DrawText("LMB on field to enter value (2; 20)\n\nEnter to next step", screenSize.x / 20, screenSize.y * 3 / 4, 25, RED);
}

void SecondStage(Vector2 fieldSize, Vector2 screenSize, int** field, bool* isWallsSet) {
	DrawField(fieldSize, screenSize, field);

	for (int i = 0; i < fieldSize.x; ++i) {
		for (int j = 0; j < fieldSize.y; ++j) {
			Rectangle rec;
			rec.x = screenSize.x / 20 + j * screenSize.x * 18 / 20 / fieldSize.y + 1;
			rec.y = screenSize.y / 20 + i * screenSize.y * 18 / 20 / fieldSize.x + 1;
			rec.width = screenSize.x * 18 / 20 / fieldSize.y - 2;
			rec.height = screenSize.y * 18 / 20 / fieldSize.x - 2;
			if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), rec))
				field[i][j] = -1;
			if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && CheckCollisionPointRec(GetMousePosition(), rec))
				field[i][j] = 0;
		}
	}

	if (IsKeyPressed(KEY_ENTER))
		*isWallsSet = true;
	DrawText("LMB to set walls\nEnter to next step", screenSize.x / 20, screenSize.y, 20, RED);
	DrawText("RMB to delete walls", screenSize.x / 2, screenSize.y, 20, RED);
}

void ThirdStage(Vector2 fieldSize, Vector2 screenSize, int** field, bool* isStartSet, bool* isFinishSet, bool* isStartFinishSet) {
	DrawField(fieldSize, screenSize, field);

	for (int i = 0; i < fieldSize.x; ++i) {
		for (int j = 0; j < fieldSize.y; ++j) {
			if (field[i][j] == 0) {

			}
			Rectangle rec;
			rec.x = screenSize.x / 20 + j * screenSize.x * 18 / 20 / fieldSize.y + 1;
			rec.y = screenSize.y / 20 + i * screenSize.y * 18 / 20 / fieldSize.x + 1;
			rec.width = screenSize.x * 18 / 20 / fieldSize.y - 2;
			rec.height = screenSize.y * 18 / 20 / fieldSize.x - 2;
			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), rec) && field[i][j] == 0) {
				if (!*isFinishSet && *isStartSet) {
					field[i][j] = -3;
					*isFinishSet = true;
				}
				if (!*isStartSet) {
					field[i][j] = -2;
					*isStartSet = true;
				}
			}
			if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), rec)) {
				if (field[i][j] == -2) {
					field[i][j] = 0;
					*isStartSet = false;
				}
				if (field[i][j] == -3) {
					field[i][j] = 0;
					*isFinishSet = false;
				}
			}
		}
	}

	DrawText("LMB/RMB to set/delete start/finish\nEnter to next step", screenSize.x / 20, screenSize.y, 20, RED);

	if (IsKeyPressed(KEY_ENTER) && *isStartSet && *isFinishSet)
		* isStartFinishSet = true;
}

void FourthStage(Vector2 fieldSize, Vector2 screenSize, int** field) {
	DrawField(fieldSize, screenSize, field);
}