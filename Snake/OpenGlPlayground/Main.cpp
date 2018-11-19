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

bool pause = false;
bool bw = false;
int headX = 25;
int headY = 25;
int grow = INITIAL_SIZE;
int head2X = 30;
int head2Y = 30;
int grow2 = INITIAL_SIZE;

list<int> snakeX;
list<int> snakeY;
list<int> snake2X;
list<int> snake2Y;



char board[gridWidth][gridHeight];
char direction = 'E';
char direction2 = 'W';
bool gameOver = true;




void DrawElement(double i, double j, char element) {
	double x = (i+.5) / gridWidth * 2.0 - 1.0;
	double y = (j+.5) / gridHeight * 2.0 - 1.0;
	double r = 1.0 / gridHeight;
	switch (element) {
	case 'T':
		glColor3f(1.0, !bw*.65, bw*.56);
		DrawCircle(x, y, r, 10);
		break;
	case 't':
		glColor3f(1.0, bw*.65, !bw*.57);
		DrawCircle(x, y, r, 10);
		break;
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
	if (pause)
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
	/*for (int i = 20; i < 40; i++) {
		board[20][i] = 'W';
	}*/
	//Find out where the head will be...
	switch (direction) {
	case 'N': headY++; break;
	case 'E': headX++; break;
	case 'S': headY--; break;
	case 'W': headX--; break;
	}

	switch (direction2) {
	case 'N': head2Y++; break;
	case 'E': head2X++; break;
	case 'S': head2Y--; break;
	case 'W': head2X--; break;
	}

	if (headX < 0) headX = gridWidth - 1;
	if (headY < 0) headY = gridHeight - 1;
	if (headX >= gridWidth)  headX = 0;
	if (headY >= gridHeight) headY = 0;
	if (head2X < 0) head2X = gridWidth - 1;
	if (head2Y < 0) head2Y = gridHeight - 1;
	if (head2X >= gridWidth)  head2X = 0;
	if (head2Y >= gridHeight) head2Y = 0;
	
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

	//Move the snake2's tail..
	if (grow2 == 0) {
		int tail2X = snake2X.back();
		int tail2Y = snake2Y.back();
		snake2X.pop_back();
		snake2Y.pop_back();
		board[tail2X][tail2Y] = ' ';
	}
	else if (grow2 < 0)
	{
		for (int i = 0; i < 2; i++) {
			int tail2X = snake2X.back();
			int tail2Y = snake2Y.back();
			snake2X.pop_back();
			snake2Y.pop_back();
			board[tail2X][tail2Y] = ' ';
		}

		grow2 += 1;
	}
	else {
		grow2--;
	}
	// Check what snake 1 hits
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
		case 'W':case 'S': case 's':case 'T': case 't':  gameOver = true; break;
	}
	// check what snake 2 hits
	switch (board[head2X][head2Y])
	{
	case 'A': grow2 += 4; break;
	case 'G': grow2 += 8; break;
	case 'B': {
		if (snake2X.size() <= 4)
		{
			gameOver = true; break;
		}
		else
		{
			grow2 -= 4; break;
		}
	}
	case 'W':case 'S': case 's': case 'T': case 't': gameOver = true; break;
	}

	//Move the snake's head.
	board[headX][headY] = bw ? 'S' : 's';
	snakeX.push_front(headX);
	snakeY.push_front(headY);
	//Move the 2nd snake's head.
	board[head2X][head2Y] = bw ? 'T' : 't';
	snake2X.push_front(head2X);
	snake2Y.push_front(head2Y);
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
		case 'w': if (direction == 'E' || direction == 'W' ) direction = 'N'; break;
		case 'a': if (direction == 'N' || direction == 'S' )	direction = 'W'; break;
		case 's': if (direction == 'E' || direction == 'W' )	direction = 'S'; break;
		case 'd': if (direction == 'N' || direction == 'S' ) direction = 'E'; break;
		case 'i': if (direction2 == 'E' || direction2 == 'W') direction2 = 'N'; break;
		case 'j': if (direction2 == 'N' || direction2 == 'S' )	direction2 = 'W'; break;
		case 'k': if (direction2 == 'E' || direction2 == 'W' )	direction2 = 'S'; break;
		case 'l': if (direction2 == 'N' || direction2 == 'S' ) direction2 = 'E'; break;
		case 'n': saveGame(); break;
		case 'm': loadGame(); break;
		case 'p': pause = !pause; break;
		case ' ': 
			if (gameOver) {
				gameOver = false;
				snakeX.clear();
				snakeY.clear();
				snake2X.clear();
				snake2Y.clear();
				headX = gridWidth / 2.0;
				headY = gridHeight / 2.0;
				head2X = gridWidth / 2.0 + 10;
				head2Y = gridHeight / 2.0 + 10;
				for (int i = 0; i < gridWidth; i++) {
					for (int j = 0; j < gridHeight; j++) {
						board[i][j] = ' ';
					}
				}
				grow = grow2 =INITIAL_SIZE;
				direction = direction2 = 'E';
			}
			break;
	}
}

void saveGame()
{
	ofstream snakeFileOut;
	string fileName;
	cin >> ("What name would you like to save the file under?", fileName);
	snakeFileOut.open(fileName);
	for (int i = 0; i < gridWidth; i++) {
		for (int j = 0; j < gridHeight; j++) {
			snakeFileOut << board[i][j];
		}
	}
	snakeFileOut.close();
	return;
	
}

void loadGame()
{
	ifstream snakeFileIn;
	string fileName;
	cin >> ("What filename would you like to load?", fileName);
	snakeFileIn.open(fileName);
	for (int i = 0; i < gridWidth; i++) {
		for (int j = 0; j < gridHeight; j++) {
			snakeFileIn >> board[i][j];
		}
	}
		
}



int main(int argc, char** argv) {
	srand(time(0));
	RunOpenGL(argc, argv, draw, NULL, keyboard);
}