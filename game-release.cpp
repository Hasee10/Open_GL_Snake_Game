/*
Muhammad Haseeb Arshad
23I-2578
DS-A
PROJECT (PF SNAKE GAME)
*/


//============================================================================
// Name        : Tetris.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Tetris...
//============================================================================

#ifndef TETRIS_CPP_
#define TETRIS_CPP_
#include "util.h"
#include <iostream>
#include <vector>
#include <algorithm>
//#include<cstdlib>
#include <ctime>
#include <string>
//#include<sys/wait.h>
//#include<stdlib.h>
//#include<stdio.h>
#include <unistd.h>
#include <sstream>
#include <cmath>      // for basic math functions such as cos, sin, sqrt
#include <fstream> // For high scores (future step)
using namespace std;


void SetCanvasSize(int width, int height) 
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 64, 0, 64, -1, 1);
    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();
}

//We declare the global variables over here 

const int num = 2;

double startx = 320,starty = 400;

int width = 640, height = 640; // The dimensions of the game window

const int snake_max_size = 200; // The snake will attain this maximum size when it absorbs the balls  

int snake_initial_size = 4; // At the start the snake is 3 units long have  a circle as its head and 2 squares that comprise of its body

int snake[snake_max_size][num]; // According to the hint a 2-D array was declared for the snake

char dir_of_mov = 'W'; // Through the initialization of a character now we can indicate the keys where the movement of the snake for each key is to be done i.e W (Up key) S (Down Key) A (Left Key) D (Right key).

int x_food1, y_food1, x_food2, y_food2, x_food3, y_food3, x_food4, y_food4, x_food5, y_food5; // According to the guidelines 5 randomly populated foods should be present at a time that should reappear every 15 seconds along with a power food that has a large size 

int x_big_food, y_big_food; // Variables for x and y axis co-ordinates of big food

int food_timer_1 = 0; // Timer for the first food

int food_timer_2 = 0; // Timer for the second food

int food_timer_3 = 0; // Timer for the third food

int food_timer_4 = 0; // Timer for the fourth food

int food_timer_5 = 0; // Timer for the fifth food

int big_food_timer = 0; // Timer for the big food

int score = 0; // The variable for score of the player

bool screen = true; // The variable to check if the condition is true or not

string DisplayScore; // Variable to declare the score of the game

// Game states for menu and gameplay
enum GameState {
    STATE_MENU,
    STATE_GAME,
    STATE_INSTRUCTIONS,
    STATE_LEVELS,
    STATE_HIGHSCORES,
    STATE_GAMEOVER
};

GameState currentState = STATE_MENU;

// Menu navigation variables
int menuSelection = 0;
const int MENU_OPTIONS = 5; // Start, Instructions, Levels, High Scores, Exit

int gameOverSelection = 0; // 0 = Play Again, 1 = Back to Menu

// Obstacle struct and vector
struct Obstacle {
    enum Type { SQUARE, CIRCLE, TRIANGLE, DIAMOND } type;
    int x, y, size;
    float* color;
};
vector<Obstacle> obstacles;

// High scores handling
vector<int> highScores;
const string HIGHSCORES_FILE = "highscores.txt";
const int MAX_HIGHSCORES = 5;

void loadHighScores() {
    highScores.clear();
    ifstream fin(HIGHSCORES_FILE);
    int s;
    while (fin >> s) highScores.push_back(s);
    fin.close();
    sort(highScores.rbegin(), highScores.rend());
    if (highScores.size() > MAX_HIGHSCORES) highScores.resize(MAX_HIGHSCORES);
}

void saveHighScores() {
    ofstream fout(HIGHSCORES_FILE);
    for (int s : highScores) fout << s << endl;
    fout.close();
}

void tryAddHighScore(int score) {
    loadHighScores();
    highScores.push_back(score);
    sort(highScores.rbegin(), highScores.rend());
    if (highScores.size() > MAX_HIGHSCORES) highScores.resize(MAX_HIGHSCORES);
    saveHighScores();
}

// Function to initialize obstacles for the current level
void initObstacles(int level = 1) {
    obstacles.clear();
    if (level == 1) {
        obstacles.push_back({Obstacle::SQUARE, 10, 40, 8, colors[DEEP_SKY_BLUE]});
        obstacles.push_back({Obstacle::CIRCLE, 40, 30, 6, colors[YELLOW]});
        obstacles.push_back({Obstacle::DIAMOND, 25, 20, 7, colors[MAGENTA]});
        obstacles.push_back({Obstacle::TRIANGLE, 50, 50, 8, colors[GREEN]});
        // Rectangle (draw as a wide square)
        obstacles.push_back({Obstacle::SQUARE, 20, 10, 12, colors[OLIVE]});
        // Hexagon (draw as a circle for now, for visual variety)
        obstacles.push_back({Obstacle::CIRCLE, 55, 15, 7, colors[LIGHT_SKY_BLUE]});
    } else if (level == 2) {
        obstacles.push_back({Obstacle::SQUARE, 10, 40, 5, colors[DEEP_SKY_BLUE]});
        obstacles.push_back({Obstacle::CIRCLE, 40, 30, 3, colors[YELLOW]});
        obstacles.push_back({Obstacle::DIAMOND, 25, 20, 4, colors[MAGENTA]});
        obstacles.push_back({Obstacle::TRIANGLE, 50, 50, 6, colors[GREEN]});
    }
    // Add more for higher levels if desired
}

// Function to draw obstacles
void drawObstacles() {
    for (const auto& obs : obstacles) {
        switch (obs.type) {
            case Obstacle::SQUARE:
                DrawSquare(obs.x, obs.y, obs.size, obs.color);
                break;
            case Obstacle::CIRCLE:
                DrawCircle(obs.x + obs.size/2.0, obs.y + obs.size/2.0, obs.size/2.0, obs.color);
                break;
            case Obstacle::DIAMOND:
                DrawLine(obs.x, obs.y + obs.size/2, obs.x + obs.size/2, obs.y + obs.size, 2, obs.color);
                DrawLine(obs.x + obs.size/2, obs.y + obs.size, obs.x + obs.size, obs.y + obs.size/2, 2, obs.color);
                DrawLine(obs.x + obs.size, obs.y + obs.size/2, obs.x + obs.size/2, obs.y, 2, obs.color);
                DrawLine(obs.x + obs.size/2, obs.y, obs.x, obs.y + obs.size/2, 2, obs.color);
                break;
            case Obstacle::TRIANGLE:
                DrawLine(obs.x, obs.y, obs.x + obs.size, obs.y, 2, obs.color);
                DrawLine(obs.x, obs.y, obs.x + obs.size/2, obs.y + obs.size, 2, obs.color);
                DrawLine(obs.x + obs.size, obs.y, obs.x + obs.size/2, obs.y + obs.size, 2, obs.color);
                break;
        }
    }
}

// The drawing of the grid would make the making of the game comparatively easier
void food_1()
{      
	srand(time(0) + 100);
	
	int food_length; // The length of the food will be according to the pixels set for each block
	food_length = 0;
	int a;
	
	validfood:
	a = rand() % 64 + 1; // Randomly generated numbers for the foods to fill the game window
	
	if(food_timer_1 == 0)
	{
	  int i = 0;
	
	  while (i < 64)  
	  {
	    if (a == i)
	    {
	      x_food1 = food_length; 
	    }
	    food_length = food_length + 1; // Increasing the value of each food for each iteration 
	    i++;
	  }
	
	  food_length = 0; // Resetting the food length for the y axis of the first food as zero so that it does not disturb the other co-ordinates of the food that is generated
	  
	  int b;
	  b = rand() % 64 + 1; // randomly generated numbers for the y-co-ordinates of the food
	
	  int j = 0; 
	  while(j < 59)
	  {
	    if (b == j)
	    {
	      y_food1 = food_length; ; // Increasing the value of each food for each iteration
	    }
	      food_length = food_length + 1;
	      j++;
	  }
	}
	
	if (food_timer_1 < 80)
	{
	 food_timer_1++;
	 DrawCircle(x_food1, y_food1, 0.7, colors[WHITE]);
	 DrawCircle(x_food1, y_food1, 0.5, colors[DEEP_SKY_BLUE]);
	}
	
	else
	{
	 food_timer_1 = 0;
	 DrawCircle(x_food1, y_food1, 0.7, colors[WHITE]);
	 DrawCircle(x_food1, y_food1, 0.5, colors[DEEP_SKY_BLUE]);
	}
	
	// Checks for the spawning of food on the top of the obstacles
	
	if ((x_food1 >= 10 && x_food1 <= 18) && (y_food1 >= 50 && y_food1 <= 50))
	  {
	   goto validfood;  
	  }
	
	if ((x_food1 >= 10 && x_food1 <= 18) && (y_food1 >= 59 && y_food1 <= 59))
	  {
	   goto validfood;  
	  }
        
	if ((x_food1 >= 19 && x_food1 <= 24) && (y_food1 >= 19 && y_food1 <= 19))
	  {
	   goto validfood;  
	  } 
	  
	if ((x_food1 >= 10 && x_food1 <= 10) && (y_food1 >= 50 && y_food1 <= 59))
	  {
	   goto validfood;  
	  }  
  
      if ((x_food1 >= 50 && x_food1 <= 59) && (y_food1 >= 52 && y_food1 <= 52))
	  {
	   goto validfood;  
	  }  

      if ((x_food1 >= 50 && x_food1 <= 59) && (y_food1 >= 43 && y_food1 <= 52))
	  {
	   goto validfood;  
	  }  
     
     if ((x_food1 >= 50 && x_food1 <= 59) && (y_food1 >= 43 && y_food1 <= 43))
	  {
	   goto validfood;  
	  }
    
    if ((x_food1 >= 19 && x_food1 <= 44) && (y_food1 >= 19 && y_food1 <= 19))
	  {
	   goto validfood;  
	  }

    if ((x_food1 >= 19 && x_food1 <= 19) && (y_food1 >= 12 && y_food1 <= 19))
	  {
	   goto validfood;  
	  }
	  
    if ((x_food1 >= 44 && x_food1 <= 44) && (y_food1 >= 12 && y_food1 <= 19))
	  {
	   goto validfood;  
	  }
	  
    if ((x_food1 >= 28 && x_food1 <= 28) && (y_food1 >= 25 && y_food1 <= 38))
	  {
	   goto validfood;  
	  }
	
    if ((x_food1 >= 28 && x_food1 <= 36) && (y_food1 >= 38 && y_food1 <= 38))
	  {
	   goto validfood;  
	  }	
	  
    if ((x_food1 >= 20 && x_food1 <= 28) && (y_food1 >= 38 && y_food1 <= 38))
	  {
	   goto validfood;  
	  }

    if ((x_food1 >= 28 && x_food1 <= 36) && (y_food1 >= 25 && y_food1 <= 25))
	  {
	   goto validfood;  
	  }

    if ((x_food1 >= 20 && x_food1 <= 28) && (y_food1 >= 25 && y_food1 <= 25))
	  {
	   goto validfood;  
	  }
	  
    if ((x_food1 >= 58 && x_food1 <= 58) && (y_food1 >= 10 && y_food1 <= 40))
	  {
	   goto validfood;  
	  }	  
	  
    if ((x_food1 >= 4 && x_food1 <= 50) && (y_food1 >= 4 && y_food1 <= 4))
	  {
	   goto validfood;  
	  }	  
	  
    if ((x_food1 >= 4 && x_food1 <= 4) && (y_food1 >= 10 && y_food1 <= 50))
	  {
	   goto validfood;  
	  }	  
	  
}

void food_2()
{
        srand(time(0) + 300);
        
	int food_length; // The length of the food will be according to the pixels set for each block
	food_length = 0;
	int a;
	
	validfood:
	a = rand() % 64 + 1; // Randomly generated numbers for the foods to fill the game window
	
	
	if (food_timer_2 == 0)
	{
	  int i = 0;
	  while (i < 64)
	  {
	    if (a == i)
	      {
	        x_food2 = food_length; 
	      }
	    food_length = food_length + 1; // Increasing the value of each food for each iteration 
	    i++;
	  }
	
	  food_length = 0; // Resetting the food length for the y axis of the first food as zero so that it does not disturb the other co-ordinates of the food that is generated
	  int b;
	  b = rand() % 64 + 1; // randomly generated numbers for the y-co-ordinates of the food
	  
	  int j = 0; 
	  while (j < 59)
	  {
	    if (b == j)
	    {
	      y_food2 = food_length; ; // Increasing the value of each food for each iteration
	    }
	      food_length = food_length + 1;
	      j++;
	  }
	}  
	
	  if (food_timer_2 < 80)
	  {
	    food_timer_2++;
	    DrawCircle(x_food2, y_food2, 0.7, colors[WHITE]);
	    DrawCircle(x_food2, y_food2, 0.5, colors[DEEP_SKY_BLUE]);
	  }
	
	  else
	  {
	    food_timer_2 = 0;
	    DrawCircle(x_food2, y_food2, 0.7, colors[WHITE]);
	    DrawCircle(x_food2, y_food2, 0.5, colors[DEEP_SKY_BLUE]);
	  }
	
	  // Checks for the spawning of food on the top of the obstacles
	  
	  if ((x_food2 >= 10 && x_food2 <= 18) && (y_food2 >= 50 && y_food2 <= 50))
	    {
	     goto validfood;  
	    }
  
	  if ((x_food2 >= 10 && x_food2 <= 18) && (y_food2 >= 59 && y_food2 <= 59))
	    {
	     goto validfood;  
	    }

	  if ((x_food2 >= 19 && x_food2 <= 24) && (y_food2 >= 19 && y_food2 <= 19))
	    {
	     goto validfood;  
	    }
	    
	  if ((x_food2 >= 10 && x_food2 <= 10) && (y_food2 >= 50 && y_food2 <= 59))
	    {
	     goto validfood;  
	    }   
	    
	  if ((x_food2 >= 50 && x_food2 <= 59) && (y_food2 >= 52 && y_food2 <= 52))
	    {
	     goto validfood;  
	    }
	  
	  if ((x_food2 >= 50 && x_food2 <= 59) && (y_food2 >= 43 && y_food2 <= 52))
	    {
	     goto validfood;  
	    }
	  
	  if ((x_food2 >= 50 && x_food2 <= 59) && (y_food2 >= 43 && y_food2 <= 43))
	    {
	     goto validfood;  
	    }
	    
	  if ((x_food2 >= 19 && x_food2 <= 44) && (y_food2 >= 19 && y_food2 <= 19))
	    {
	     goto validfood;  
	    }  
	    
         if ((x_food2 >= 19 && x_food2 <= 19) && (y_food2 >= 12 && y_food2 <= 19))
	    {
	     goto validfood;  
	    }   
	  
         if ((x_food2 >= 44 && x_food2 <= 44) && (y_food2 >= 12 && y_food2 <= 19))
	    {
	     goto validfood;  
	    } 
	 
	 if ((x_food2 >= 28 && x_food2 <= 28) && (y_food2 >= 25 && y_food2 <= 38))
	    {
	     goto validfood;  
	    } 
	   	
         if ((x_food2 >= 28 && x_food2 <= 36) && (y_food2 >= 38 && y_food2 <= 38))
	    {
	     goto validfood;  
	    } 
	    
	 if ((x_food2 >= 20 && x_food2 <= 28) && (y_food2 >= 38 && y_food2 <= 38))
	    {
	     goto validfood;  
	    }    
	    
	 if ((x_food2 >= 28 && x_food2 <= 36) && (y_food2 >= 25 && y_food2 <= 25))
	    {
	     goto validfood;  
	    }    
	  
	 if ((x_food2 >= 20 && x_food2 <= 28) && (y_food2 >= 25 && y_food2 <= 25))
	    {
	     goto validfood;  
	    }  
	    
        if ((x_food2 >= 58 && x_food2 <= 58) && (y_food2 >= 10 && y_food2 <= 40))
	  {
	   goto validfood;  
	  }	  
	  
        if ((x_food2 >= 4 && x_food2 <= 50) && (y_food2 >= 4 && y_food2 <= 4))
	  {
	   goto validfood;  
	  }	  
	  
        if ((x_food2 >= 4 && x_food2 <= 4) && (y_food2 >= 10 && y_food2 <= 50))
	  {
	   goto validfood;  
	  }	  
   
}

void food_3()
{
        srand(time(0) + 500);
        
	int food_length; // The length of the food will be according to the pixels set for each block
	food_length = 0;
	int a;
	
	validfood:
	a = rand() % 64 + 1; // Randomly generated numbers for the foods to fill the game window
	
	if (food_timer_3 == 0)
	{
	  int i = 0;
	  while (i < 64)
	  {
	    if(a == i)
	      {
	        x_food3 = food_length; 
	      }
	    food_length = food_length + 1; // Increasing the value of each food for each iteration 
	    i++;
	  }
	
	  food_length = 0; // Resetting the food length for the y axis of the first food as zero so that it does not disturb the other co-ordinates of the food that is generated
	
	  int b;
	  b = rand() % 64 + 1; // randomly generated numbers for the y-co-ordinates of the food
	
	  int j = 0; 
	  while(j < 59)
	  {
	    if(b == j)
	    {
	      y_food3 = food_length; ; // Increasing the value of each food for each iteration
	    }
	      food_length = food_length + 1;
	      j++;
	  }
	}
	
	 if (food_timer_3 < 80)
	  {
	    food_timer_3++;
	    DrawCircle(x_food3, y_food3, 0.7, colors[WHITE]);
	    DrawCircle(x_food3, y_food3, 0.5, colors[DEEP_SKY_BLUE]);
	  }
	
	  else
	  {
	    food_timer_3 = 0;
	    DrawCircle(x_food3, y_food3, 0.7, colors[WHITE]);
	    DrawCircle(x_food3, y_food3, 0.5, colors[DEEP_SKY_BLUE]);
	  }

	// Checks for the spawning of food on the top of the obstacles
	
	if ((x_food3 >= 10 && x_food3 <= 18) && (y_food3 >= 50 && y_food3 <= 50))
	  {
	   goto validfood;  
	  }
	
	if ((x_food3 >= 10 && x_food3 <= 18) && (y_food3 >= 59 && y_food3 <= 59))
	  {
	   goto validfood;  
	  }
	
	if ((x_food3 >= 19 && x_food3 <= 24) && (y_food3 >= 19 && y_food3 <= 19))
	  {
	   goto validfood;  
	  }

        if ((x_food3 >= 10 && x_food3 <= 10) && (y_food3 >= 50 && y_food3 <= 59))
	  {
	   goto validfood;  
	  }

        if ((x_food3 >= 50 && x_food3 <= 59) && (y_food3 >= 52 && y_food3 <= 52))
	  {
	   goto validfood;  
	  }
	  
       if ((x_food3 >= 50 && x_food3 <= 59) && (y_food3 >= 43 && y_food3 <= 52))
	  {
	   goto validfood;  
	  }  
	  
       if ((x_food3 >= 50 && x_food3 <= 59) && (y_food3 >= 43 && y_food3 <= 43))
	  {
	   goto validfood;  
	  } 
	  
       if ((x_food3 >= 19 && x_food3 <= 44) && (y_food3 >= 19 && y_food3 <= 19))
	  {
	   goto validfood;  
	  }   
	  
      if ((x_food3 >= 19 && x_food3 <= 19) && (y_food3 >= 12 && y_food3 <= 19))
	  {
	   goto validfood;  
	  }   

     if ((x_food3 >= 44 && x_food3 <= 44) && (y_food3 >= 12 && y_food3 <= 19))
	  {
	   goto validfood;  
	  } 
	  
      if ((x_food3 >= 28 && x_food3 <= 28) && (y_food3 >= 25 && y_food3 <= 38))
	    {
	     goto validfood;  
	    } 
	   	
     if ((x_food3 >= 28 && x_food3 <= 36) && (y_food3 >= 38 && y_food3 <= 38))
	    {
	     goto validfood;  
	    } 
	    
     if ((x_food3 >= 20 && x_food3 <= 28) && (y_food3 >= 38 && y_food3 <= 38))
	    {
	     goto validfood;  
	    }    
	    
     if ((x_food3 >= 28 && x_food3 <= 36) && (y_food3 >= 25 && y_food3 <= 25))
	    {
	     goto validfood;  
	    }    
	  
     if ((x_food3 >= 20 && x_food3 <= 28) && (y_food3 >= 25 && y_food3 <= 25))
	    {
	     goto validfood;  
	    }  	  

    if ((x_food3 >= 58 && x_food3 <= 58) && (y_food3 >= 10 && y_food3 <= 40))
	  {
	   goto validfood;  
	  }	  
	  
    if ((x_food3 >= 4 && x_food3 <= 50) && (y_food3 >= 4 && y_food3 <= 4))
	  {
	   goto validfood;  
	  }	  
	  
    if ((x_food3 >= 4 && x_food3 <= 4) && (y_food3 >= 10 && y_food3 <= 50))
	  {
	   goto validfood;  
	  }	  
    
}

void food_4()
{
	srand(time(0) + 700);
	
	int food_length; // The length of the food will be according to the pixels set for each block
	food_length = 0;
	int a;
	
	validfood:
	a = rand() % 64 + 1; // Randomly generated numbers for the foods to fill the game window
	
	if (food_timer_4 == 0)
	{
	  int i = 0;
	  while (i < 64)
	  {
	    if(a == i)
	      {
	        x_food4 = food_length; 
	      }
	    food_length = food_length + 1; // Increasing the value of each food for each iteration 
	    i++;
	  }
	
	  food_length = 0; // Resetting the food length for the y axis of the first food as zero so that it does not disturb the other co-ordinates of the food that is generated
	  int b;
	  b = rand() % 64 + 1; // randomly generated numbers for the y-co-ordinates of the food
	  
	  int j = 0; 
	  while(j < 59)
	    {
	      if(b == j)
	      {
	        y_food4 = food_length; ; // Increasing the value of each food for each iteration
	      }
	        food_length = food_length + 1;
	        j++;
	    }
	}   
	
	if (food_timer_4 < 80)
	  {
	    food_timer_4++;
	    DrawCircle(x_food4, y_food4, 0.7, colors[WHITE]);
	    DrawCircle(x_food4, y_food4, 0.5, colors[DEEP_SKY_BLUE]);
	  }
	
	  else
	  {
	    food_timer_4 = 0;
	    DrawCircle(x_food4, y_food4, 0.7, colors[WHITE]);
	    DrawCircle(x_food4, y_food4, 0.5, colors[DEEP_SKY_BLUE]);
	  }

	// Checks for the spawning of food on the top of the obstacles
	
	if ((x_food4 >= 10 && x_food4 <= 18) && (y_food4 >= 50 && y_food4 <= 50))
	  {
	   goto validfood;  
	  }

        if ((x_food4 >= 10 && x_food4 <= 18) && (y_food4 >= 59 && y_food4 <= 59))
	  {
	   goto validfood;  
	  }
        
	if ((x_food4 >= 19 && x_food4 <= 24) && (y_food4 >= 19 && y_food4 <= 19))
	  {
	   goto validfood;  
	  }
	  
	if ((x_food4 >= 10 && x_food4 <= 10) && (y_food4 >= 50 && y_food4 <= 54))
	  {
	   goto validfood;  
	  }  
	  
	if ((x_food4 >= 50 && x_food4 <= 59) && (y_food4 >= 52 && y_food4 <= 52))
	  {
	   goto validfood;  
	  }
	  
  	if ((x_food4 >= 50 && x_food4 <= 59) && (y_food4 >= 43 && y_food4 <= 52))
	  {
	   goto validfood;  
	  }  
	  
	if ((x_food4 >= 50 && x_food4 <= 59) && (y_food4 >= 43 && y_food4 <= 43))
	  {
	   goto validfood;  
	  }
	  
	if ((x_food4 >= 19 && x_food4 <= 44) && (y_food4 >= 19 && y_food4 <= 19))
	  {
	   goto validfood;  
	  }

	if ((x_food4 >= 19 && x_food4 <= 19) && (y_food4 >= 12 && y_food4 <= 19))
	  {
	   goto validfood;  
	  }

       if ((x_food4 >= 44 && x_food4 <= 44) && (y_food4 >= 12 && y_food4 <= 19))
	  {
	   goto validfood;  
	  }
 	 
      if ((x_food4 >= 28 && x_food4 <= 28) && (y_food4 >= 25 && y_food4 <= 38))
	    {
	     goto validfood;  
	    } 
	   	
      if ((x_food4 >= 28 && x_food4 <= 36) && (y_food4 >= 38 && y_food4 <= 38))
	    {
	     goto validfood;  
	    } 
	    
     if ((x_food4 >= 20 && x_food4 <= 28) && (y_food4 >= 38 && y_food4 <= 38))
	    {
	     goto validfood;  
	    }    
	    
     if ((x_food4 >= 28 && x_food4 <= 36) && (y_food4 >= 25 && y_food4 <= 25))
	    {
	     goto validfood;  
	    }    
	  
     if ((x_food4 >= 20 && x_food4 <= 28) && (y_food4 >= 25 && y_food4 <= 25))
	    {
	     goto validfood;  
	    }  
 
    if ((x_food4 >= 58 && x_food4 <= 58) && (y_food4 >= 10 && y_food4 <= 40))
	  {
	   goto validfood;  
	  }	  
	  
    if ((x_food4 >= 4 && x_food4 <= 50) && (y_food4 >= 4 && y_food4 <= 4))
	  {
	   goto validfood;  
	  }	  
	  
    if ((x_food4 >= 4 && x_food4 <= 4) && (y_food4 >= 10 && y_food4 <= 50))
	  {
	   goto validfood;  
	  }	  


}

void food_5()
{
        srand(time(0) + 1200);
        
	int food_length; // The length of the food will be according to the pixels set for each block
	food_length = 0;
	int a;
	
	validfood:
	a = rand() % 64 + 1; // Randomly generated numbers for the foods to fill the game window
	
	if (food_timer_5 == 0)
	{
	
	  int i = 0;
	  while (i < 64)
	  {
	    if(a == i)
	    {
	      x_food5 = food_length; 
	    }
	      food_length = food_length + 1; // Increasing the value of each food for each iteration 
	      i++;
	  }
	
	  food_length = 0; // Resetting the food length for the y axis of the first food as zero so that it does not disturb the other co-ordinates of the food that is generated
	  int b;
	  b = rand() % 64 + 1; // randomly generated numbers for the y-co-ordinates of the food
	
	  int j = 0; 
	  while(j < 59)
	  {
	    if(b == j)
	    {
	      y_food5 = food_length; ; // Increasing the value of each food for each iteration
	    }
	      food_length = food_length + 1;
	      j++;
	  }
        }
        
          if (food_timer_5 < 80)
	  {
	    food_timer_5++;
	    DrawCircle(x_food5, y_food5, 0.7, colors[WHITE]);
	    DrawCircle(x_food5, y_food5, 0.5, colors[DEEP_SKY_BLUE]);
	  }
	
	  else
	  {
	    food_timer_5 = 0;
	    DrawCircle(x_food5, y_food5, 0.7, colors[WHITE]);
	    DrawCircle(x_food5, y_food5, 0.5, colors[DEEP_SKY_BLUE]);
	  }
        
        // Checks for the spawning of food on the top of the obstacles
	
	if ((x_food5 >= 10 && x_food5 <= 18) && (y_food5 >= 50 && y_food5 <= 50))
	  {
	   goto validfood;  
	  }
	  
	if ((x_food5 >= 10 && x_food5 <= 18) && (y_food5 >= 59 && y_food5 <= 59))
	  {
	   goto validfood;  
	  }  
	
	if ((x_food5 >= 19 && x_food5 <= 24) && (y_food5 >= 19 && y_food5 <= 19))
	  {
	   goto validfood;  
	  }
	  
	if ((x_food5 >= 10 && x_food5 <= 10) && (y_food5 >= 50 && y_food5 <= 59))
	  {
	   goto validfood;  
	  }  
        
       if ((x_food5 >= 50 && x_food5 <= 59) && (y_food5 >= 52 && y_food5 <= 52))
	  {
	   goto validfood;  
	  }
	  
       if ((x_food5 >= 50 && x_food5 <= 59) && (y_food5 >= 43 && y_food5 <= 52))
	  {
	   goto validfood;  
	  }  
	
      if ((x_food5 >= 50 && x_food5 <= 59) && (y_food5 >= 43 && y_food5 <= 43))
	  {
	   goto validfood;  
	  }
	  
      if ((x_food5 >= 19 && x_food5 <= 44) && (y_food5 >= 19 && y_food5 <= 19))
	  {
	   goto validfood;  
	  }	  
	  
      if ((x_food5 >= 19 && x_food5 <= 19) && (y_food5 >= 12 && y_food5 <= 19))
	  {
	   goto validfood;  
	  }
	  
      if ((x_food5 >= 44 && x_food5 <= 44) && (y_food5 >= 12 && y_food5 <= 19))
	  {
	   goto validfood;  
	  }	  

     	 
     if ((x_food5 >= 28 && x_food5 <= 28) && (y_food5 >= 25 && y_food5 <= 38))
	    {
	     goto validfood;  
	    } 
	   	
    if ((x_food5 >= 28 && x_food5 <= 36) && (y_food5 >= 38 && y_food5 <= 38))
	    {
	     goto validfood;  
	    } 
	    
     if ((x_food5 >= 20 && x_food5 <= 28) && (y_food5 >= 38 && y_food5 <= 38))
	    {
	     goto validfood;  
	    }    
	    
     if ((x_food5 >= 28 && x_food5 <= 36) && (y_food5 >= 25 && y_food5 <= 25))
	    {
	     goto validfood;  
	    }    
	  
     if ((x_food5 >= 20 && x_food5 <= 28) && (y_food5 >= 25 && y_food5 <= 25))
	    {
	     goto validfood;  
	    }  
	    
    if ((x_food5 >= 58 && x_food5 <= 58) && (y_food5 >= 10 && y_food5 <= 40))
	  {
	   goto validfood;  
	  }	  
	  
    if ((x_food5 >= 4 && x_food5 <= 50) && (y_food5 >= 4 && y_food5 <= 4))
	  {
	   goto validfood;  
	  }	  
	  
    if ((x_food5 >= 4 && x_food5 <= 4) && (y_food5 >= 10 && y_food5 <= 50))
	  {
	   goto validfood;  
	  }	  
	    

}

void big_food()
{
        srand(time(0) + 4000);
        
	int food_length; // The length of the food will be according to the pixels set for each block
	food_length = 0;
	int a;
	
	validfood:
	a = rand() % 64 + 1; // Randomly generated numbers for the foods to fill the game window
	
	if (big_food_timer == 0)
	{
	  int i = 0;
	  while (i < 64)
	  {
	    if(a == i)
	    {
	      x_big_food = food_length; 
	    }
	      food_length = food_length + 1; // Increasing the value of each food for each iteration 
	      i++;
	  }
	
	  food_length = 0; // Resetting the food length for the y axis of the first food as zero so that it does not disturb the other co-ordinates of the food that is generated
	  int b;
	  b = rand() % 64 + 1; // randomly generated numbers for the y-co-ordinates of the food
	
	  int j = 0; 
	  while(j < 59)
	  {
	    if(b == j)
	    {
	      y_big_food = food_length; ; // Increasing the value of each food for each iteration
	    }
	      food_length = food_length + 1;
	      j++;
	  }
        }
        
          if (big_food_timer < 300)
	  {
	    big_food_timer++;
	    DrawCircle(x_big_food, y_big_food, 1.2, colors[WHITE]);
	    DrawCircle(x_big_food, y_big_food, 1.0, colors[RED]);
	  }
	
	  else
	  {
	    big_food_timer = 0;
	    DrawCircle(x_big_food, y_big_food, 1.2, colors[WHITE]);
	    DrawCircle(x_big_food, y_big_food, 1.0, colors[RED]);
	  }

	// Checks for the spawning of food on the top of the obstacles
	
	if ((x_big_food >= 10 && x_big_food <= 18) && (y_big_food >= 50 && y_big_food <= 50))
	  {
	   goto validfood;  
	  }

        if ((x_big_food >= 10 && x_big_food <= 18) && (y_big_food >= 59 && y_big_food <= 59))
	  {
	   goto validfood;  
	  }
        
	if ((x_big_food >= 19 && x_big_food <= 24) && (y_big_food >= 19 && y_big_food <= 19))
	  {
	   goto validfood;  
	  }
	  
	if ((x_big_food >= 10 && x_big_food <= 10) && (y_big_food >= 50 && y_big_food <= 59))
	  {
	   goto validfood;  
	  }	  
        
        if ((x_big_food >= 50 && x_big_food <= 59) && (y_big_food >= 52 && y_big_food <= 52))
	  {
	   goto validfood;  
	  }
	  
        if ((x_big_food >= 50 && x_big_food <= 59) && (y_big_food >= 43 && y_big_food <= 52))
	  {
	   goto validfood;  
	  }  
	  
       if ((x_big_food >= 50 && x_big_food <= 59) && (y_big_food >= 43 && y_big_food <= 43))
	  {
	   goto validfood;  
	  }
	  
       if ((x_big_food >= 19 && x_big_food <= 44) && (y_big_food >= 19 && y_big_food <= 19))
	  {
	   goto validfood;  
	  }
	
       if ((x_big_food >= 19 && x_big_food <= 19) && (y_big_food >= 12 && y_big_food <= 19))
	  {
	   goto validfood;  
	  }	
	  
       if ((x_big_food >= 44 && x_big_food <= 44) && (y_big_food >= 12 && y_big_food <= 19))
	  {
	   goto validfood;  
	  }	  
	  
       if ((x_big_food >= 28 && x_big_food <= 28) && (y_big_food >= 25 && y_big_food <= 38))
	  {
	   goto validfood;  
	  }
	  
       if ((x_big_food >= 28 && x_big_food <= 36) && (y_big_food >= 38 && y_big_food <= 38))
	  {
	   goto validfood;  
	  }	  
	  
      if ((x_big_food >= 20 && x_big_food <= 28) && (y_big_food >= 38 && y_big_food <= 38))
	  {
	   goto validfood;  
	  }

      if ((x_big_food >= 28 && x_big_food <= 36) && (y_big_food >= 25 && y_big_food <= 25))
	  {
	   goto validfood;  
	  }
	  
      if ((x_big_food >= 20 && x_big_food <= 28) && (y_big_food >= 25 && y_big_food <= 25))
	  {
	   goto validfood;  
	  }
	  
	  
     if ((x_big_food >= 58 && x_big_food <= 58) && (y_big_food >= 10 && y_big_food <= 40))
	  {
	   goto validfood;  
	  }	  
	  
    if ((x_big_food >= 4 && x_big_food <= 50) && (y_big_food >= 4 && y_big_food <= 4))
	  {
	   goto validfood;  
	  }	  
	  
    if ((x_big_food >= 4 && x_big_food <= 4) && (y_big_food >= 10 && y_big_food <= 50))
	  {
	   goto validfood;  
	  }	 
}

/* * Main Canvas drawing function.* */
 
void snake_movement()
{
  for (int i = 0; i < snake_initial_size; i++) 
    {
      switch (i) 
      {
        case 0:
        if (snake[0][0] == 64)
        {
         snake[0][0] = 0;
        }
        else if (snake[0][0] == -1)
        {
         snake[0][0] = 63;
        }
        else if (snake[0][1] == 60)
        {
         snake[0][1] = 0;
        }
        else if (snake[0][1] == -1)
        {
         snake[0][1] = 60;
        }
        // Neon glowing head: white inner, red outer
        DrawCircle(snake[i][0] + 0.5, snake[i][1] + 0.5, 0.6, colors[RED]);
        break;
       
        default:
        // Neon body: alternate colors for gradient effect
        float* bodyColor;
        if (i % 3 == 1) bodyColor = colors[GREEN];
        else if (i % 3 == 2) bodyColor = colors[DEEP_SKY_BLUE];
        else bodyColor = colors[MAGENTA];
        DrawSquare(snake[i][0], snake[i][1], 2, bodyColor);
        break;
      }
    }
    
    for(int i = snake_initial_size - 1; i > 0;i--) // Updates the positions of the snake's body segments and is responsible for making each segment of the snake follow the part of the body in front of it
    { // snake[i] is the current body part of the snake and snake[i-1] is the body part of the snake that follows it
        snake[i][0] = snake[i - 1][0];
        snake[i][1] = snake[i - 1][1];
    }   
    
    switch(dir_of_mov) 
	{
          case 'W': // When the user enters the upward key the snake starts to move upward with an increment of 10 in its 2-D array along the y-axis i.e in its column 
          snake[0][1] = snake[0][1] + 1; 
          break;

          case 'A': // When the user enters the left key the snake starts to move leftwards with a decrement of 10 in its 2-D array along the x-axis i.e in its column
          snake[0][0] = snake[0][0] - 1;
          break;

          case 'S': // When the user enters the downward key the snake starts to move downwards with a decrement of 10 in its 2-D array along the y-axis i.e in its row
          snake[0][1] = snake[0][1] - 1;
          break;

          case 'D': // When the user enters the rightward key the snake starts to move leftwards with a increment of 10 in its 2-D array along the x-axis i.e in its row
          snake[0][0] = snake[0][0] + 1;
          break;

          default: // If the user enters any other key then the 4 arrows. the code would set it as default and the movement would not be valid
          break;
        }  
        
        
     for (int i = 1; i < snake_initial_size; i++) // Loop for checking if the snake hits itself or not if it does the game will end and a message will be displayed saying you lost
     {
      if ((snake[0][0] == snake[i][0]) && (snake[0][1] == snake[i][1]))
      {
       screen = 0;
       DrawString (40,45,"Your Score = ", colors[BLACK]);
       DrawString (40, 40, DisplayScore, colors[BLACK]);
       DrawString (40,35,"You Lost. Game Over!", colors[BLACK]);
       tryAddHighScore(score);
      }
     } 
     
     if (snake[0][0] == x_food1 && snake[0][1] == y_food1) // Condition for checking if the co-ordinates of the 1st food particle match with those of the head. If they do the snake will increase in size
     {
      score += 5;
      snake_initial_size += 1;
      food_timer_1 = 0;
      food_1();
     }
     food_1();
 
     if (snake[0][0] == x_food2 && snake[0][1] == y_food2) // Condition for checking if the co-ordinates of the 2nd food particle match with those of the head. If they do the snake will increase in size
     {
      score += 5;
      snake_initial_size += 1 ;
      food_timer_2 = 0;
      food_2();
     }
     food_2();
     
    if (snake[0][0] == x_food3 && snake[0][1] == y_food3) // Condition for checking if the co-ordinates of the 3rd food particle match with those of the head. If they do the snake will increase in size
     {
      score += 5;
      snake_initial_size += 1;
      food_timer_3 = 0;
      food_3();
     }
      food_3();
     
    if (snake[0][0] == x_food4 && snake[0][1] == y_food4) // Condition for checking if the co-ordinates of the 4th food particle match with those of the head. If they do the snake will increase in size
     {
      score += 5;
      snake_initial_size += 1 ;
      food_timer_4 = 0;
      food_4();
     }
     food_4();
      
    if (snake[0][0] == x_food5 && snake[0][1] == y_food5) // Condition for checking if the co-ordinates of the 5th food particle match with those of the head. If they do the snake will increase in size
     {
      score += 5;
      snake_initial_size += 1 ;
      food_timer_5 = 0;
      food_5();
     }
      food_5();
   
   if (snake[0][0] == x_big_food && snake[0][1] == y_big_food) // Condition for checking if the co-ordinates of the big food particle matches with those of the head. If they do the snake will increase in size
     {
      score += 20;
      snake_initial_size += 8;
      big_food_timer = 0;
      big_food();
     }
      big_food();
 } 

void collide() // Function for the collision of snake with the obstacles
{
    // Check collision with dynamic obstacles
    for (const auto& obs : obstacles) {
        int sx = snake[0][0];
        int sy = snake[0][1];
        switch (obs.type) {
            case Obstacle::SQUARE:
                if (sx >= obs.x && sx < obs.x + obs.size && sy >= obs.y && sy < obs.y + obs.size) {
                    screen = 0;
                    tryAddHighScore(score);
                    return;
                }
                break;
            case Obstacle::CIRCLE: {
                float cx = obs.x + obs.size/2.0, cy = obs.y + obs.size/2.0, r = obs.size/2.0;
                float dist = sqrt((sx + 0.5 - cx) * (sx + 0.5 - cx) + (sy + 0.5 - cy) * (sy + 0.5 - cy));
                if (dist < r) {
                    screen = 0;
                    tryAddHighScore(score);
                    return;
                }
                break;
            }
            case Obstacle::DIAMOND: {
                // Approximate as bounding box for simplicity
                if (sx >= obs.x && sx < obs.x + obs.size && sy >= obs.y && sy < obs.y + obs.size) {
                    screen = 0;
                    tryAddHighScore(score);
                    return;
                }
                break;
            }
            case Obstacle::TRIANGLE: {
                // Approximate as bounding box for simplicity
                if (sx >= obs.x && sx < obs.x + obs.size && sy >= obs.y && sy < obs.y + obs.size) {
                    screen = 0;
                    tryAddHighScore(score);
                    return;
                }
                break;
            }
        }
    }
    // (Self-collision is handled in snake_movement)
}

void Display()
{
    if (currentState == STATE_MENU) {
        glClearColor(0.05, 0.01, 0.1, 1.0); // Dark background for neon look
        glClear(GL_COLOR_BUFFER_BIT);
        // Neon glowing title, centered
        DrawString(22, 57, "NEON SNAKE GAME", colors[RED]);
        DrawString(21.7, 56.7, "NEON SNAKE GAME", colors[WHITE]); // subtle glow

        const char* menuItems[MENU_OPTIONS] = {"Start Game", "Instructions", "Levels", "High Scores", "Exit"};
        float menuStartY = 45;
        float menuX = 22;
        float buttonW = 20;
        float buttonH = 4;
        float spacing = 7;
        for (int i = 0; i < MENU_OPTIONS; ++i) {
            float y = menuStartY - i * spacing;
            float* borderColor = (i == menuSelection) ? colors[YELLOW] : colors[DEEP_SKY_BLUE];
            // Neon button border (rectangle outline)
            DrawLine(menuX, y, menuX + buttonW, y, 2, borderColor);
            DrawLine(menuX + buttonW, y, menuX + buttonW, y - buttonH, 2, borderColor);
            DrawLine(menuX + buttonW, y - buttonH, menuX, y - buttonH, 2, borderColor);
            DrawLine(menuX, y - buttonH, menuX, y, 2, borderColor);
            // Centered text
            float textX = menuX + buttonW / 2 - 4;
            float textY = y - buttonH / 2 + 1;
            float* textColor = (i == menuSelection) ? colors[YELLOW] : colors[WHITE];
            DrawString(textX, textY, menuItems[i], textColor);
        }
        // Neon snake icon (head and 2 body segments), positioned above menu
        DrawCircle(menuX + buttonW / 2 + 2, menuStartY + 7, 1.2, colors[RED]); // head
        DrawSquare(menuX + buttonW / 2 + 0.5, menuStartY + 7, 1.2, colors[GREEN]); // body 1
        DrawSquare(menuX + buttonW / 2 - 1, menuStartY + 7, 1.2, colors[DEEP_SKY_BLUE]); // body 2
        // Neon food icon, next to snake
        DrawCircle(menuX + buttonW / 2 + 5, menuStartY + 7, 0.7, colors[YELLOW]);
        // Menu instructions, centered at bottom
        DrawString(13, 10, "Use UP/DOWN arrows to navigate, ENTER to select", colors[LIGHT_SKY_BLUE]);
        glutSwapBuffers();
        return;
    }
    if (currentState == STATE_INSTRUCTIONS) {
        glClearColor(0.05, 0.01, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        DrawString(20, 55, "INSTRUCTIONS", colors[YELLOW]);
        DrawString(10, 48, "- Use arrow keys to move the snake.", colors[WHITE]);
        DrawString(10, 44, "- Eat neon food to grow and score points.", colors[WHITE]);
        DrawString(10, 40, "- Avoid obstacles and yourself!", colors[WHITE]);
        DrawString(10, 36, "- Eat the big red food for a huge bonus!", colors[WHITE]);
        DrawString(10, 20, "Press ESC or ENTER to return to menu.", colors[LIGHT_SKY_BLUE]);
        glutSwapBuffers();
        return;
    }
    if (currentState == STATE_LEVELS) {
        glClearColor(0.05, 0.01, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        DrawString(22, 55, "LEVELS", colors[DEEP_SKY_BLUE]);
        DrawString(10, 48, "Level 1: Easy - Fewer obstacles, slower snake.", colors[WHITE]);
        DrawString(10, 44, "Level 2: Medium - More obstacles, faster snake.", colors[WHITE]);
        DrawString(10, 40, "(More levels can be added easily!)", colors[WHITE]);
        DrawString(10, 20, "Press ESC or ENTER to return to menu.", colors[LIGHT_SKY_BLUE]);
        glutSwapBuffers();
        return;
    }
    if (currentState == STATE_GAME && screen) {
        // Visually appealing deep blue background
        glClearColor(0.07, 0.07, 0.2, 1.0); // Deep blue
        glClear(GL_COLOR_BUFFER_BIT);
        // Main game logic (draw snake, food, obstacles, score)
        DisplayScore = to_string((score));
        drawObstacles();
        // Score bar
        DrawSquare(0, 60, 65, colors[BLACK]);
        DrawString(10, 61, "SCORE ", colors[YELLOW]);
        DrawString(22, 61, DisplayScore, colors[WHITE]);
        snake_movement();
        collide();
        glutSwapBuffers();
        return;
    } 
    // Game Over screen
    if (currentState == STATE_GAME && !screen) {
        glClearColor(0.05, 0.01, 0.1, 1.0); // Match menu/game bg
        glClear(GL_COLOR_BUFFER_BIT);
        // Center coordinates
        float centerX = 32, topY = 50;
        // Neon 'Game Over' message (single, with glow)
        DrawString(centerX - 6, topY, "GAME OVER", colors[RED]);
        DrawString(centerX - 6.2, topY - 0.2, "GAME OVER", colors[WHITE]);
        // New High Score message
        loadHighScores();
        bool isHigh = (highScores.size() > 0 && score == highScores[0]);
        if (isHigh && score > 0) {
            DrawString(centerX - 5, topY - 4, "New High Score!", colors[YELLOW]);
        }
        // Neon snake/food icons above score
        DrawCircle(centerX - 1, topY - 7, 1.2, colors[RED]);
        DrawSquare(centerX - 2.5, topY - 7, 1.2, colors[GREEN]);
        DrawCircle(centerX + 1, topY - 7, 0.7, colors[YELLOW]);
        // Neon score box (full rectangle)
        float scoreBoxX = centerX - 8, scoreBoxY = topY - 16, scoreBoxW = 16, scoreBoxH = 8;
        // Neon border
        DrawLine(scoreBoxX, scoreBoxY, scoreBoxX + scoreBoxW, scoreBoxY, 3, colors[DEEP_SKY_BLUE]);
        DrawLine(scoreBoxX + scoreBoxW, scoreBoxY, scoreBoxX + scoreBoxW, scoreBoxY - scoreBoxH, 3, colors[DEEP_SKY_BLUE]);
        DrawLine(scoreBoxX + scoreBoxW, scoreBoxY - scoreBoxH, scoreBoxX, scoreBoxY - scoreBoxH, 3, colors[DEEP_SKY_BLUE]);
        DrawLine(scoreBoxX, scoreBoxY - scoreBoxH, scoreBoxX, scoreBoxY, 3, colors[DEEP_SKY_BLUE]);
        // Fill
        DrawSquare(scoreBoxX + 0.5, scoreBoxY - scoreBoxH + 0.5, scoreBoxW - 1, colors[BLACK]);
        // Score label and value, centered
        DrawString(centerX - 2, scoreBoxY - 2, "SCORE", colors[YELLOW]);
        DrawString(centerX + 1, scoreBoxY - 4, DisplayScore, colors[WHITE]);
        // Neon buttons, centered and spaced
        float buttonW = 12, buttonH = 3.5;
        float buttonX = centerX - buttonW/2, playY = scoreBoxY - scoreBoxH - 4, menuY = playY - 5;
        float* playAgainColor = (gameOverSelection == 0) ? colors[GREEN] : colors[DEEP_SKY_BLUE];
        float* backMenuColor = (gameOverSelection == 1) ? colors[YELLOW] : colors[DEEP_SKY_BLUE];
        // Play Again button
        DrawLine(buttonX, playY, buttonX + buttonW, playY, 3, playAgainColor);
        DrawLine(buttonX + buttonW, playY, buttonX + buttonW, playY - buttonH, 3, playAgainColor);
        DrawLine(buttonX + buttonW, playY - buttonH, buttonX, playY - buttonH, 3, playAgainColor);
        DrawLine(buttonX, playY - buttonH, buttonX, playY, 3, playAgainColor);
        DrawString(buttonX + 2, playY - buttonH/2 + 0.5, "Play Again", playAgainColor);
        // Back to Menu button
        DrawLine(buttonX, menuY, buttonX + buttonW, menuY, 3, backMenuColor);
        DrawLine(buttonX + buttonW, menuY, buttonX + buttonW, menuY - buttonH, 3, backMenuColor);
        DrawLine(buttonX + buttonW, menuY - buttonH, buttonX, menuY - buttonH, 3, backMenuColor);
        DrawLine(buttonX, menuY - buttonH, buttonX, menuY, 3, backMenuColor);
        DrawString(buttonX + 1, menuY - buttonH/2 + 0.5, "Back to Menu", backMenuColor);
        // Fun tip, smaller and subtle, centered
        DrawString(centerX - 12, menuY - 6, "Tip: Eat the big red food for a huge score boost!", colors[LIGHT_SKY_BLUE]);
        glutSwapBuffers();
        return;
    }
    if (currentState == STATE_HIGHSCORES) {
        glClearColor(0.05, 0.01, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        DrawString(22, 55, "HIGH SCORES", colors[YELLOW]);
        loadHighScores();
        for (int i = 0; i < highScores.size(); ++i) {
            DrawString(25, 48 - i * 4, to_string(i+1) + ".  " + to_string(highScores[i]), colors[DEEP_SKY_BLUE]);
        }
        DrawString(10, 20, "Press ESC or ENTER to return to menu.", colors[LIGHT_SKY_BLUE]);
        glutSwapBuffers();
        return;
    }
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) 
{
    if (currentState == STATE_MENU) {
        if (key == GLUT_KEY_UP) {
            menuSelection = (menuSelection - 1 + MENU_OPTIONS) % MENU_OPTIONS;
        } else if (key == GLUT_KEY_DOWN) {
            menuSelection = (menuSelection + 1) % MENU_OPTIONS;
        }
        glutPostRedisplay();
        return;
    }
    if (key == GLUT_KEY_LEFT)   /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/ 
    {
     if (dir_of_mov != 'D') // D for rightwards
     {
	dir_of_mov = 'A'; // A for leftwards 
     }  
    } 
    
    else if (key == GLUT_KEY_RIGHT)  /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/ 
    {
    if (dir_of_mov != 'A') // A for leftwards
     {
        dir_of_mov = 'D'; // D for Rightwards
     }
    } 
    
    else if (key == GLUT_KEY_UP) /*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/ 
    {
    if (dir_of_mov != 'S') // S for downwards 
     {
        dir_of_mov = 'W'; // W for upwards
     }
    }
    
    else if (key == GLUT_KEY_DOWN)  /*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/ 
    {
    if (dir_of_mov != 'W') // W for upwards
     {
	dir_of_mov = 'S'; // S for downwards	
     }    
    }
  
    /* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
     * this function*/
     glutPostRedisplay();

    if (currentState == STATE_GAME && !screen) {
        if (key == GLUT_KEY_UP || key == GLUT_KEY_DOWN) {
            gameOverSelection = 1 - gameOverSelection;
            glutPostRedisplay();
            return;
        }
    }
}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) 
{
    if (currentState == STATE_MENU) {
        if (int(key) == 13) { // ENTER key
            if (menuSelection == 0) {
                currentState = STATE_GAME; // Start Game
                screen = true;
                initObstacles(1);
            } else if (menuSelection == 1) {
                currentState = STATE_INSTRUCTIONS;
            } else if (menuSelection == 2) {
                currentState = STATE_LEVELS;
            } else if (menuSelection == 3) {
                currentState = STATE_HIGHSCORES;
            } else if (menuSelection == 4) {
                exit(0); // Exit
            }
            glutPostRedisplay();
        }
        return;
    }
    if (key == KEY_ESC/* Escape key ASCII*/) 
    {
        exit(1); // exit the program when escape key is pressed.
    }
    if (key == 'R' || key=='r'/* Escape key ASCII*/) 
    {
        //exit(1); // exit the program when escape key is pressed.
    	//aswangle+=90;
    }
  
    else if (int(key) == 13)
    {
    }
    
    glutPostRedisplay();
  
    if (currentState == STATE_GAME && !screen) {
        if (int(key) == 13) {
            if (gameOverSelection == 0) {
                // Play Again
                // Reset game state
                score = 0;
                snake_initial_size = 4;
                dir_of_mov = 'W';
                screen = true;
                // Optionally reset snake position and food timers
            } else {
                // Back to Menu
                currentState = STATE_MENU;
                screen = true;
            }
            glutPostRedisplay();
            return;
        }
    }
    if (currentState == STATE_INSTRUCTIONS || currentState == STATE_LEVELS) {
        if (key == KEY_ESC || int(key) == 13) {
            currentState = STATE_MENU;
            glutPostRedisplay();
            return;
        }
    }
    if (currentState == STATE_HIGHSCORES) {
        if (key == KEY_ESC || int(key) == 13) {
            currentState = STATE_MENU;
            glutPostRedisplay();
            return;
        }
    }
}

void Timer(int m) 
{
  // implement your functionality here
    glutPostRedisplay();
  
  // once again we tell the library to call our Timer function after next 1000/FPS
    glutTimerFunc(1000.0 / FPS, Timer, 0);
}

/*
 * our gateway main function
 * */
int main(int argc, char*argv[]) 
{
    snake_movement();
    food_1();
    food_2();
    food_3();
    food_4();
    food_5();
    collide();
    big_food();
    InitRandomizer(); 
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
    glutInitWindowPosition(50, 50); 
    glutInitWindowSize(width, height); 
    glutCreateWindow("PF's Snake Game"); 
    SetCanvasSize(width, height); 
    glutTimerFunc(10.0 / FPS, Timer, 0);
    
    glutDisplayFunc(Display);
    glutSpecialFunc(NonPrintableKeys);
    glutKeyboardFunc(PrintableKeys);
    glutMainLoop();
    return 1;
}
#endif /* Snake Game */
