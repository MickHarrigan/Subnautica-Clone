//Title: Project 2 for Spring 2020
//Author: Michael Harrigan
//Date: 3/10/2020
//Description: This is main for project 2

#include "Game.h"

#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
using namespace std;

int main () {
  srand(time(NULL));
  Game newGame;
  newGame.StartGame();
  return 0;
}
