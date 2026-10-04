#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	//Clears old bricks before new ones
	bricks.clear();

	//New game not lost
	lost = false;
	Box brick;

	// TODO #2 - Add this brick and 4 more bricks to the vector
	brick.width = 10;
	brick.height = 2;
	brick.x_position = 0;
	brick.y_position = 5;
	brick.doubleThick = true;
	brick.color = ConsoleColor::DarkGreen;

	//Adds 5 bricks to the vector
	for (int i = 0; i < 5; i++) {
		//Moves each brick to the right, evenly spaces them
		brick.x_position = i * 12; //Since the width is 10 I did i * 12 to get the spacing between each 10 char wide brick
		//This adds the copy of the brick vector
		bricks.push_back(brick);
	}
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	ball.Update();
	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	//This will draw each brick in the vector
	for (int i = 0; i < bricks.size(); i++) {
		bricks[i].Draw();
	}

	//If theres no bricks left
	if (bricks.size() == 0) {
		std::cout << "You win! Please, press R to play again!";
	}
	else if (lost) {
		std::cout << "You lost, press 'R' to play again.";
	}
	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	for (int i = 0; i < bricks.size(); i++) {
		if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			//Changes the brick color each time it hits
			bricks[i].color = ConsoleColor(bricks[i].color - 1);
			ball.y_velocity *= -1; //Ball bounces

			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			if (bricks[i].color == ConsoleColor::Black) {
				bricks.erase(bricks.begin() + i);
				//After the bricks broken the vector shifts each box to the left
				i--;
			}
		}
	}

	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	//If no bricks remain, the ball will pause and display victory
	if (bricks.size() == 0) {
		ball.moving == false;
		return;
	}

	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
	{
		ball.y_velocity *= -1;
	}

	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	if (ball.y_position + ball.y_velocity >= WINDOW_HEIGHT - 1) {
		ball.moving = false; //Stops the ball
		lost = true; //Player lost
		return;
	}
}
