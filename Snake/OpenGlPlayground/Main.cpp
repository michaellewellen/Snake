#include "OpenGL.h"
#include "GL/glut.h"
#include "GL/GL.h"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <list>
#include <ctime>
#include <fstream>
#include <Windows.h>

using namespace std;
const int gridHeight = 50;
const int gridWidth = 60;
const int INITIAL_SIZE = 5;
const double PGenerateFruit = .01;

ofstream snakeFile;
bool bw = false;
int headX = 25;
int headY = 25;
int grow = INITIAL_SIZE;

list<int> snakeX;
list<int> snakeY;

char board[gridWidth][gridHeight];
char direction = 'E';
bool gameOver = true;

void DrawElement(double i, double j, char element) {
	double x = (i+.5) / gridWidth * 2.0 - 1.0;
	double y = (j+.5) / gridHeight * 2.0 - 1.0;
	double r = 1.0 / gridHeight;
	switch (element) {
	case 'S':
		glColor3f(0, bw*.5, !bw*.5);
		DrawCircle(x, y, r, 10);
		break;
	case 's':
		glColor3f(0, !bw*.5, bw*.5);
		DrawCircle(x, y, r, 10);
		break;
	case 'G':
		glColor3f(1.0, 1.0, 0);
		DrawCircle(x, y, r, 10);
		glColor3f(0, 1.0, 0);
		DrawCircle(x + r * .15, y + r * .9, r*.4, 10);
		break;
	case 'A':
		glColor3f(1.0, 0.0, 0);
		DrawCircle(x, y, r, 10);
		glColor3f(0, 1.0, 0);
		DrawCircle(x+r*.15, y+r*.9, r*.4, 10);
		break;
	case 'W':
		glColor3f(0.0,0.0,0.0);
		DrawRectangle(x-r, y-r, x +r, y+r);
		break;
	case 'B':
		glColor3f(0.0, 0.0, 0.0);
		DrawCircle(x, y, r, 10);
		glColor3f(.5, .5, 0);
		DrawCircle(x + r * .15, y + r * .9, r*.4, 10);
		break;
	case ' ':
		break;
	default:
		break;
	}
}

void update() {
	bw = !bw;
	if (direction == 'P')
		return;
	//Generate Fruit...
	if (rand() / (double)RAND_MAX < PGenerateFruit) {
		int x = rand() % gridWidth;
		int y = rand() % gridHeight;
		int type = rand() % 25;
		if (type <= 5 && board[x][y] != 'S' && board[x][y] != 's')
			board[x][y] = 'G';
		else if (type > 5 && type <= 10 && board[x][y] != 'S' && board[x][y] != 's')
			board[x][y] = 'B';
		else if (board[x][y] != 'S' && board[x][y] != 's')
			board[x][y] = 'A';
	}
	//Generate Walls..
	for (int i = 20; i < 40; i++) {
		board[20][i] = 'W';
	}
	//Find out where the head will be...
	switch (direction) {
		case 'N': headY++; break;
		case 'E': headX++; break;
		case 'S': headY--; break;
		case 'W': headX--; break;
	}
	if (headX < 0) headX = gridWidth - 1;
	if (headY < 0) headY = gridHeight - 1;
	if (headX >= gridWidth)  headX = 0;
	if (headY >= gridHeight) headY = 0;

	//Move the snake's tail..
	if (grow == 0) {
		int tailX = snakeX.back();
		int tailY = snakeY.back();
		snakeX.pop_back();
		snakeY.pop_back();
		board[tailX][tailY] = ' ';
	}
	else if (grow < 0)
	{
		for (int i = 0; i < 2; i++) {
			int tailX = snakeX.back();
			int tailY = snakeY.back();
			snakeX.pop_back();
			snakeY.pop_back();
			board[tailX][tailY] = ' ';
		}

		grow += 1;
	}
	else {
		grow--;
	}

	switch (board[headX][headY])
	{
		case 'A': grow += 4; break;
		case 'G': grow += 8; break;
		case 'B': {
			if (snakeX.size() <= 4)
			{
				gameOver = true; break;
			}
		    else
			{
				grow -= 4; break;
			}
		}
		case 'W':case 'S': case 's': gameOver = true; break;
	}

	//Move the snake's head.
	board[headX][headY] = bw ? 'S' : 's';
	snakeX.push_front(headX);
	snakeY.push_front(headY);

}

void draw() {

	for (int i = 0; i < gridWidth; i++) {
		for (int j = 0; j < gridHeight; j++) {
			DrawElement(i, j, board[i][j]);
		}
	}
	Sleep(50);
	if (!gameOver)
		update();
}

void keyboard(int key) {
	switch (key) {
		case 'w': if (direction == 'E' || direction == 'W' || direction == 'P') direction = 'N'; break;
		case 'a': if (direction == 'N' || direction == 'S' || direction == 'P')	direction = 'W'; break;
		case 's': if (direction == 'E' || direction == 'W' || direction == 'P')	direction = 'S'; break;
		case 'd': if (direction == 'N' || direction == 'S' || direction == 'P') direction = 'E'; break;
		case 'n': saveGame();
		case 'p': if (direction = 'P');
		case ' ': 
			if (gameOver) {
				gameOver = false;
				snakeX.clear();
				snakeY.clear();
				headX = gridWidth / 2.0;
				headY = gridHeight / 2.0;
				for (int i = 0; i < gridWidth; i++) {
					for (int j = 0; j < gridHeight; j++) {
						board[i][j] = ' ';
					}
				}
				grow = INITIAL_SIZE;
			}
			break;
	}
}

void saveGame()
{
	string fileName;
	cin >> ("What name would you like to save the file under?", fileName);
	snakeFile.open(fileName);
	for (int i = 0; i < gridWidth; i++) {
		for (int j = 0; j < gridHeight; j++) {
			snakeFile << board[i][j];
		}
	}
	snakeFile.close();
	return;
	
}



int main(int argc, char** argv) {
	srand(time(0));
	RunOpenGL(argc, argv, draw, NULL, keyboard);
}