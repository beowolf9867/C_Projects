// WORDSORT USING 2D ARRAY
/*
#include <stdio.h>
#include <stdlib.h>

char word[100];
char *w = word;
char *wi = word;
char words[3][20];
char *w1 = &words[0][0];
char *w2 = &words[1][0];
char *w3 = &words[2][0];

int read_word();
void sort_word();
int num_of_words();

int main(void) {
  int exit = 1;
  printf("*********************\n");
  printf("*     Word Sorter   *\n");
  printf("*********************\n");
  while (exit == 1) {
    printf("Enter word: ");
    exit = read_word();
    printf("Unsorted: %s\n", word);
    printf("-----------------------------\n");
  }
  sort_word();
  printf("Sorted words:\n 1. %s,\n 2. %s,\n 3. %s\n", words[0], words[1], words[2]);
  printf("You wrote %d words\n", num_of_words());
}

int read_word() {
  int temp_val = 0;
  while ((temp_val = getchar()) != '\n' && temp_val != ' ')
    *w++ = temp_val;
  *w++ = ',';
  if (temp_val == ' ')
    return 0;
  else 
    return 1;
}

int num_of_words() {
  int n = 0;
  wi = word;
  while (wi != &word[99]) {
    if (*wi == ',')
      n++;
    wi++;
  }
  return n;
}

void sort_word() {
  while (*wi != ',')
    *w1++ = *wi++;
  wi++;
  for (; *wi != ',';)
    *w2++ = *wi++;
  wi++;
  for (; *wi != ',';)
    *w3++ = *wi++;
  
  *w1 = '\0';
  *w2 = '\0';
  *w3 = '\0';
}
*/
// WORDSORT USING MALLOC (N-number of words)
#include <stdio.h>
#include <stdlib.h>

char word[100];
char *w = word; //used in getchar
char *wi = word; //used in copying to other arrays
char *wp = NULL; //used in arrays of pointers

int read_word();
void sort_word();
int num_of_words();

int main(void) {
  int exit = 1;
  int i = 0;
  printf("*********************\n");
  printf("*     Word Sorter   *\n");
  printf("*********************\n");
  while (exit == 1) {
    printf("Enter word: ");
    exit = read_word();
    printf("Unsorted: %s\n", word);
    printf("-----------------------------\n");
  }
  int n = num_of_words();
  char **words = malloc(n*sizeof(char *));
  for (int i = 0; i < n; i++)
    *wp++ = *wi++;
  sort_word();
  printf("Sorted words:");
  while (i < n) {
    printf("%s", words[i]);
  }
}

int read_word() {
  int temp_val = 0;
  while ((temp_val = getchar()) != '\n' && temp_val != ' ')
    *w++ = temp_val;
  *w++ = ',';
  if (temp_val == ' ')
    return 0;
  else 
    return 1;
}

int num_of_words() {
  int n = 0;
  wi = word;
  while (wi != &word[99]) {
    if (*wi == ',')
      n++;
    wi++;
  }
  return n;
}

void sort_word() {
  while (wi != &word[99]) {
    while (*wi != ',')
      *wp++ = *wi++;
    wi++;
  }
}

