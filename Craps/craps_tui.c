#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <dirent.h>

#define y_TITLE (menu_y_Max / 4) - 1
#define x_TITLE (menu_x_Max - 28) / 2

#define MENU0_Y (menu_y_Max / 4) + 1
#define MENU0_X ((menu_x_Max - 10) / 2) + 1

#define MENU1_Y menu_y_Max / 2
#define MENU1_X (menu_x_Max - 9) / 2

#define MENU2_Y (menu_y_Max / 2) + 2
#define MENU2_X (menu_x_Max - 4) / 2

void main_screen();
void newgamescreen(WINDOW *win);
void loadgamescreen(WINDOW *win);
void gameplayscreen(WINDOW *win);

char name[20];
char player_name[20];
char filename[30];

int main(void) {
  main_screen();
  endwin();
  return 0;
}

// MAINSCREEN

void main_screen() {
  initscr();
  noecho();
  curs_set(0);
  refresh();

  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);
  
  WINDOW *menuwin = newwin(yMax / 2, xMax / 2, yMax / 4, xMax / 4);
  box(menuwin, 0, 0);
  int menu_y_Max, menu_x_Max; 
  getmaxyx(menuwin, menu_y_Max, menu_x_Max);
  wattron(menuwin, A_BOLD);
  mvwprintw(menuwin, y_TITLE, x_TITLE, "Welcome to the Game of Craps!");
  wattroff(menuwin, A_BOLD);

  mvwprintw(menuwin, MENU0_Y, MENU0_X, "New Game");
  mvwprintw(menuwin, MENU1_Y, MENU1_X, "Load Game");
  mvwprintw(menuwin, MENU2_Y, MENU2_X, "Exit");
  wrefresh(menuwin);
  keypad(menuwin, TRUE);
  
  int ch;
  int menu_pos = 0;
  
  // SELECTING MENUS
  int running = 1;
  while ((ch=wgetch(menuwin)) != 0 && running == 1) {
    werase(menuwin);
    box(menuwin, 0, 0);
    wrefresh(menuwin);
    mvwprintw(menuwin, y_TITLE, x_TITLE, "Welcome to the Game of Craps!");
    mvwprintw(menuwin, MENU0_Y, MENU0_X, "New Game");
    mvwprintw(menuwin, MENU1_Y, MENU1_X, "Load Game");
    mvwprintw(menuwin, MENU2_Y, MENU2_X, "Exit");
    switch (ch) {
    case KEY_UP:
      if (menu_pos == 0) {
	wattron(menuwin, A_STANDOUT);
	mvwprintw(menuwin, MENU0_Y, MENU0_X, "New Game");
       wattroff(menuwin, A_STANDOUT);	
       break;
     }
     menu_pos--;
     if (menu_pos == 0) {
       wattron(menuwin, A_STANDOUT);
       mvwprintw(menuwin, MENU0_Y, MENU0_X, "New Game");
       wattroff(menuwin, A_STANDOUT);	
     } else if (menu_pos == 1) {
       wattron(menuwin, A_STANDOUT);
       mvwprintw(menuwin, MENU1_Y, MENU1_X, "Load Game");
       wattroff(menuwin, A_STANDOUT);
     } else if (menu_pos == 2) {
       wattron(menuwin, A_STANDOUT);
       mvwprintw(menuwin, MENU2_Y, MENU2_X, "Exit");
       wattroff(menuwin, A_STANDOUT);
     }
     wrefresh(menuwin);
     break;
   case KEY_DOWN:
     if (menu_pos == 2) {
	wattron(menuwin, A_STANDOUT);
	mvwprintw(menuwin, MENU2_Y, MENU2_X, "Exit");
	wattroff(menuwin, A_STANDOUT);
	break;
     }
     menu_pos++;
     if (menu_pos == 2) {
       wattron(menuwin, A_STANDOUT);
       mvwprintw(menuwin, MENU2_Y, MENU2_X, "Exit");
       wattroff(menuwin, A_STANDOUT);
     } else if (menu_pos == 0) {
       wattron(menuwin, A_STANDOUT);
       mvwprintw(menuwin, MENU0_Y, MENU0_X, "New Game");
       wattroff(menuwin, A_STANDOUT);
     } else if (menu_pos == 1) {
       wattron(menuwin, A_STANDOUT);
       mvwprintw(menuwin, MENU1_Y, MENU1_X, "Load Game");
       wattroff(menuwin, A_STANDOUT);
     }
     wrefresh(menuwin);
     break;
   case 10:
     if (menu_pos == 0) {
       newgamescreen(menuwin);
     } else if (menu_pos == 1) {
       loadgamescreen(menuwin);
       break;
     } else if (menu_pos == 2) {
       running = 0;
       refresh();
       break;
     }
     break;
   default:
     break;
    }
 }
 wrefresh(menuwin);
}

// NEW_GAME SCREEN

void newgamescreen(WINDOW *win) {
  werase(win);
  box(win, 0, 0);
  wrefresh(win);
  echo();
  curs_set(1);
  mvwprintw(win, 2, 2, "File Name: ");
  wrefresh(win);
  mvwgetnstr(win, 2, 14, name, 10);
  noecho();
  curs_set(0);
  
  werase(win);
  box(win, 0, 0);
  wrefresh(win);
  echo();
  curs_set(1);
  mvwprintw(win, 2, 2, "User Name: ");
  wrefresh(win);
  mvwgetnstr(win, 2, 14, player_name, 10);
  noecho();
  curs_set(0);
  
  snprintf(filename, sizeof(filename), "saves/%s.txt", name);
  FILE *fp = fopen(filename, "w");
  fprintf(fp, "Name: %s\nWins: 0\nLosses: 0\n", player_name);
  fclose(fp);
}

//LOAD_GAME SCREEN

void loadgamescreen(WINDOW *win) {
  werase(win);
  box(win, 0, 0);
  wrefresh(win);
  int save_num = 0;
  int x_save = 2;
  int y_save = 4;
  int x_savename = 10;
  int y_savename = 4;
  char savefiles[10][20];
  struct dirent *file;
  DIR *dir = opendir("./saves/");
  while ((file=readdir(dir)) != NULL) {
    mvwprintw(win, y_save, x_save, "Save %d: ", save_num);
    mvwprintw(win, y_savename, x_savename, "%s", file->d_name);
    strcpy(savefiles[save_num],file->d_name);
    y_save = y_save + 1;
    y_savename = y_savename + 1;
    save_num = save_num + 1;
  }
  closedir(dir);
  char savename[20];
  echo();
  curs_set(1);
  wattron(win, A_BOLD);
  mvwprintw(win, 2, 2, "Enter Name: ");
  mvwprintw(win, 3, 2, "------------");
  wattroff(win, A_BOLD);
  wrefresh(win);
  mvwgetnstr(win, 2, 14, savename, 10);
  noecho();
  curs_set(0);
  int pos = 0;
  char savefiles_string[20] = {'\0'};
  strcpy(savefiles_string, savefiles[0]);
  while (strcmp(savename,savefiles_string) != 0)
    strcpy(savefiles_string,savefiles[pos++]);
  werase(win);
  box(win, 0, 0);
  wrefresh(win);
  mvwprintw(win, 2, 2, "Save %d: ", --pos);
  mvwprintw(win, 2, 10, "Selected");
  char temp_name[100] = {'\0'};
  char s[20] = "./saves/";
  memcpy(s + strlen(s), savename, strlen(savename) + 1);  
  strcpy(temp_name, s);
  char buffer[10][100];
  FILE *fp = fopen(temp_name, "r");
  int size_buffer = 0;
  while ((fgets(buffer[size_buffer++], sizeof(buffer), fp)) != NULL)
    ;
  fclose(fp);
  mvwprintw(win, 4, 2, "You have: ");
  mvwprintw(win, 5, 2, "-------------");
  for (int i = 0; i < size_buffer; i++) {
    for (int j = 0; buffer[i][j] != '\0'; j++)
      if (buffer[i][j] == '\n') {
	buffer[i][j] = '\0';
	break;
      }
  }
  for (int i = 0; i < size_buffer; i++)
    mvwprintw(win, 6+i, 2, "%s", buffer[i]);
  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);
  WINDOW *gamewin = newwin(yMax, xMax, 0, 0);
  gameplayscreen(gamewin);
  werase(win);
}

void gameplayscreen(WINDOW *win) {
  werase(win);
  box(win, 0, 0);
  int game_y_Max, game_x_Max; 
  getmaxyx(win, game_y_Max, game_x_Max);
  wattron(win, A_BOLD);
  mvwprintw(win, 10, (game_x_Max/2)-4, "Game");
  wattroff(win, A_BOLD);
  wrefresh(win);
  keypad(win, TRUE);

  
  werase(win);
}
