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

