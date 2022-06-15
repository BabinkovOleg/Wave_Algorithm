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
	DrawText(str, _line->position.x + 5, _line->position.y + (_line->boxSize.y - _line->fontSize) / 2, _line->fontSize, _line->textColor);
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
			Rectangle rec;
			rec.x = screenSize.x / 20 + j * screenSize.x * 18 / 20 / fieldSize.y + 1;
			rec.y = screenSize.y / 20 + i * screenSize.y * 18 / 20 / fieldSize.x + 1;
			rec.width = screenSize.x * 18 / 20 / fieldSize.y - 2;
			rec.height = screenSize.y * 18 / 20 / fieldSize.x - 2;
			if (field[i][j] != 0) {
				if(field[i][j] == -1)
					DrawRectangleRec(rec, BROWN);
				if (field[i][j] == 1)
					DrawRectangleRec(rec, GREEN);
				if (field[i][j] == MAX_FIELD_SIZE * MAX_FIELD_SIZE)
					DrawRectangleRec(rec, RED);
				//if (field[i][j] > 1 && field[i][j] < MAX_FIELD_SIZE * MAX_FIELD_SIZE) {
				//	DrawRectangleRec(rec, MAGENTA);
				//	DrawText(TextFormat("%d", field[i][j] - 1), rec.x, rec.y, 10, BLACK);
				//}
				if (field[i][j] == -2)
					DrawRectangleRec(rec, BLACK);
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

	DrawText("LMB on field to enter value [2 ; 20]\n\nEnter to next step", screenSize.x / 20, screenSize.y * 3 / 4, 25, RED);
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

void ThirdStage(Vector2 fieldSize, Vector2 screenSize, int** field, bool* isStartSet, bool* isFinishSet, bool* isStartFinishSet, Vector2* starPos, Vector2* finishPos) {
	DrawField(fieldSize, screenSize, field);

	for (int i = 0; i < fieldSize.x; ++i) {
		for (int j = 0; j < fieldSize.y; ++j) {
			Rectangle rec;
			rec.x = screenSize.x / 20 + j * screenSize.x * 18 / 20 / fieldSize.y + 1;
			rec.y = screenSize.y / 20 + i * screenSize.y * 18 / 20 / fieldSize.x + 1;
			rec.width = screenSize.x * 18 / 20 / fieldSize.y - 2;
			rec.height = screenSize.y * 18 / 20 / fieldSize.x - 2;
			if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), rec) && field[i][j] == 0) {
				if (!*isFinishSet && *isStartSet) {
					field[i][j] = MAX_FIELD_SIZE * MAX_FIELD_SIZE;
					*isFinishSet = true;
					*finishPos = (Vector2) { i, j };
				}
				if (!*isStartSet) {
					field[i][j] = 1;
					*isStartSet = true;
					*starPos = (Vector2) { i, j };
				}
			}
			if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), rec)) {
				if (field[i][j] == 1) {
					field[i][j] = 0;
					*isStartSet = false;
				}
				if (field[i][j] == MAX_FIELD_SIZE * MAX_FIELD_SIZE) {
					field[i][j] = 0;
					*isFinishSet = false;
				}
			}
		}
	}

	DrawText("LMB/RMB to set/delete start/finish\nEnter to next step", (int)screenSize.x / 20, (int)screenSize.y, 20, RED);

	if (IsKeyPressed(KEY_ENTER) && *isStartSet && *isFinishSet)
		* isStartFinishSet = true;
}

void FourthStage(Vector2 fieldSize, Vector2 screenSize, int** field, Vector2 startPos, Vector2 finishPos) {
	WaveAlg(fieldSize, screenSize, field, startPos, finishPos, startPos);
	BackPath(fieldSize, screenSize, field, startPos, finishPos, finishPos);
}

void WaveAlg(Vector2 fieldSize, Vector2 screenSize, int** field, Vector2 startPos, Vector2 finishPos, Vector2 currentPos) {
	if (currentPos.x > 0)
		if (field[(int)currentPos.x - 1][(int)currentPos.y] == 0 || field[(int)currentPos.x - 1][(int)currentPos.y] - 1 > field[(int)currentPos.x][(int)currentPos.y] && field[(int)currentPos.x - 1][(int)currentPos.y] < MAX_FIELD_SIZE * MAX_FIELD_SIZE) {
			field[(int)currentPos.x - 1][(int)currentPos.y] = field[(int)currentPos.x][(int)currentPos.y] + 1;
			WaveAlg(fieldSize, screenSize, field, startPos, finishPos, (Vector2) { currentPos.x - 1, currentPos.y });
		}

	if (currentPos.y > 0)
		if (field[(int)currentPos.x][(int)currentPos.y - 1] == 0 || field[(int)currentPos.x][(int)currentPos.y - 1] - 1 > field[(int)currentPos.x][(int)currentPos.y] && field[(int)currentPos.x][(int)currentPos.y - 1] < MAX_FIELD_SIZE * MAX_FIELD_SIZE) {
			field[(int)currentPos.x][(int)currentPos.y - 1] = field[(int)currentPos.x][(int)currentPos.y] + 1;
			WaveAlg(fieldSize, screenSize, field, startPos, finishPos, (Vector2) { currentPos.x, currentPos.y - 1 });
		}

	if (currentPos.x < fieldSize.x - 1)
		if (field[(int)currentPos.x + 1][(int)currentPos.y] == 0 || field[(int)currentPos.x + 1][(int)currentPos.y] - 1 > field[(int)currentPos.x][(int)currentPos.y] && field[(int)currentPos.x + 1][(int)currentPos.y] < MAX_FIELD_SIZE * MAX_FIELD_SIZE) {
			field[(int)currentPos.x + 1][(int)currentPos.y] = field[(int)currentPos.x][(int)currentPos.y] + 1;
			WaveAlg(fieldSize, screenSize, field, startPos, finishPos, (Vector2) { currentPos.x + 1, currentPos.y });
		}

	if (currentPos.y < fieldSize.y - 1)
		if (field[(int)currentPos.x][(int)currentPos.y + 1] == 0 || field[(int)currentPos.x][(int)currentPos.y + 1] - 1 > field[(int)currentPos.x][(int)currentPos.y] && field[(int)currentPos.x][(int)currentPos.y + 1] < MAX_FIELD_SIZE * MAX_FIELD_SIZE) {
			field[(int)currentPos.x][(int)currentPos.y + 1] = field[(int)currentPos.x][(int)currentPos.y] + 1;
			WaveAlg(fieldSize, screenSize, field, startPos, finishPos, (Vector2) { currentPos.x, currentPos.y + 1 });
		}
}

void BackPath(Vector2 fieldSize, Vector2 screenSize, int** field, Vector2 startPos, Vector2 finishPos, Vector2 currentPos) {
	int min = MAX_FIELD_SIZE * MAX_FIELD_SIZE - 1;
	if (currentPos.x > 0)
		if (field[(int)currentPos.x - 1][(int)currentPos.y] < min && field[(int)currentPos.x - 1][(int)currentPos.y] > 0) min = field[(int)currentPos.x - 1][(int)currentPos.y];
	if (currentPos.y > 0)
		if (field[(int)currentPos.x][(int)currentPos.y - 1] < min && field[(int)currentPos.x][(int)currentPos.y - 1] > 0) min = field[(int)currentPos.x][(int)currentPos.y - 1];
	if (currentPos.x < fieldSize.x - 1)
		if (field[(int)currentPos.x + 1][(int)currentPos.y] < min && field[(int)currentPos.x + 1][(int)currentPos.y] > 0) min = field[(int)currentPos.x + 1][(int)currentPos.y];
	if (currentPos.y < fieldSize.y - 1)
		if (field[(int)currentPos.x][(int)currentPos.y + 1] < min && field[(int)currentPos.x][(int)currentPos.y + 1] > 0) min = field[(int)currentPos.x][(int)currentPos.y + 1];

	if (min > 1) {
		if (currentPos.x > 0 && field[(int)currentPos.x - 1][(int)currentPos.y] == min) {
			field[(int)currentPos.x - 1][(int)currentPos.y] = -2;
			BackPath(fieldSize, screenSize, field, startPos, finishPos, (Vector2) { currentPos.x - 1, currentPos.y });
		}
		else
			if (currentPos.y > 0 && field[(int)currentPos.x][(int)currentPos.y - 1] == min) {
				field[(int)currentPos.x][(int)currentPos.y - 1] = -2;
				BackPath(fieldSize, screenSize, field, startPos, finishPos, (Vector2) { currentPos.x, currentPos.y - 1 });
			}
			else
				if (currentPos.x < fieldSize.x - 1 && field[(int)currentPos.x + 1][(int)currentPos.y] == min) {
					field[(int)currentPos.x + 1][(int)currentPos.y] = -2;
					BackPath(fieldSize, screenSize, field, startPos, finishPos, (Vector2) { currentPos.x + 1, currentPos.y });
				}
				else
					if (currentPos.y < fieldSize.y - 1 && field[(int)currentPos.x][(int)currentPos.y + 1] == min) {
						field[(int)currentPos.x][(int)currentPos.y + 1] = -2;
						BackPath(fieldSize, screenSize, field, startPos, finishPos, (Vector2) { currentPos.x, currentPos.y + 1 });
					}
	}
}