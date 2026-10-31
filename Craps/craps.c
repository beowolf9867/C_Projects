#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>

int roll_dice(void);
bool play_game(void);

#define DICE 7

int main(void) {
  srand((unsigned) time(NULL)); //set the seed based on current time
  int win_count = 0;
  int lose_count = 0;
  int game_result = 0;
  int continue_game = 0;
  printf("*********************************\n");
  printf("* Welcome to the game of Craps! *\n");
  printf("*********************************\n");

  printf("Do you want to play: ");
  scanf("%d", &continue_game);

  while (continue_game) {
    game_result = play_game();
    if (game_result == true) {
      win_count++;
    } if (game_result == false) {
      lose_count++;
    }
    continue_game = 0;
    printf("Do you want to continue: ");
    scanf("%d", &continue_game);
  }

  printf("You won %d times and lost %d times\n", win_count, lose_count);
  return 0; 
}

int roll_dice(void) {
  int roll1, roll2;
  roll1 = rand() % DICE;
  roll2 = rand() % DICE;
  return roll1 + roll2;
}

bool play_game(void) {
  int win = 0;
  int lose = 0;
  int game1 = 0;
  int game2 = 0;
  int point = 0;
  int cont_game = 0;

  game1 = roll_dice();
  if (game1 == 7 || game1 == 11) {
    win = 1;
    cont_game = 0;
    printf("You rolled: %d\nYou win\n", game1);
  } else if (game1 == 2 || game1 == 3 || game1 == 12) {
    lose = 1;
    cont_game = 0;
    printf("You rolled: %d\nYou lose\n", game1);
  } else {
    cont_game = 1;
    point = game1;
    printf("You rolled: %d\n", game1);
    while (cont_game) {
      game2 = roll_dice();
      win = 0;
      lose = 0;
      if (game2 == point) {
	win = 1;
	lose = 0;
	cont_game = 0;
	printf("You rolled: %d\nYou win\n", game2);
      } else if (game2 == 7) {
	lose = 1;
	win = 0;
	cont_game = 0;
	printf("You rolled: %d\nYou lose\n", game2);
      } else {
	cont_game = 1;
      }
    }
  }

  if (win) {
    return true;
  } if (lose) {
    return false;
  }
  return false;
}

