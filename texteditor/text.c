#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// read from stdin 
// can insert lines
// delete lines
// do above based on where the cursor is 
// determine cursor position using a pointer

char word[100];
char *l = word;

typedef struct node {
  char string[100];
  struct node *next;
} node;

node *first = NULL;
node *cursor = NULL;

int read_line();
void read_text();
void insert_line();
void delete_line();
int num_of_nodes();

int main(void) { 
  printf("**************************\n");
  printf("*       Text editor      *\n");
  printf("**************************\n");
  int choice = 1;
  while (choice) {
    printf("1. Insert line\n");
    printf("2. Delete line\n");
    printf("3. Read line\n");
    printf("4. Exit\n");
    printf("-------------------------------------\n");
    printf("Choose one of the above commands: ");  
    int choice = read_line();
    if (choice == '1') {
      insert_line();
    }
    if (choice == '2') {
      delete_line();
    }
    if (choice == '3') {
      read_text();
    }
  }
  return 0;
}

int read_line() {
  int temp_val = 0;
  l = word;
  while ((temp_val = getchar()) != '\n') 
    *l++ = temp_val;
  l--;
  if (*l-- == '4')
    return 0;
  else
    return *l;
}

void read_text() {
  int line_num = 1;
  node *temp = first;
  printf("--------Text---------\n");
  while (temp != NULL) {
    printf("[%d] %s", line_num++, temp->string);
    temp = temp->next;
  }
  printf("-------------------------\n");
}

void insert_line() {
  int node_pos = 0;
  char string2[100];
  char string3[50];
  
  int n = num_of_nodes();
  printf("There are [%d] lines\n", n);
  printf("Where do you wish to insert your node: ");
  scanf("%d", &node_pos);
  if (node_pos == 0) {
    first = (node *) malloc(sizeof(node));
    printf("What do you wish to write: ");
    getchar();
    fgets(string2, sizeof(string2), stdin);
    strcpy(first->string, string2);
    first->next = NULL;
    cursor = first;
    printf("Successfully written to node\n");
    printf("------------------------------------\n");
  }
  else if  (node_pos > n) {
    printf("Not enough lines\n");
    getchar();
    printf("------------------------------------\n");
  } else {
    cursor = first;
    for (int i = 0; i < node_pos-2; i++)
      cursor = cursor->next; 
    node *new_node = (node *) malloc(sizeof(node));
    printf("What do you wish to write: ");
    getchar();
    fgets(string2, sizeof(string2), stdin);
    strcpy(new_node->string, string2); 
    new_node->next = cursor->next;
    cursor->next = new_node;
    printf("Successfully written to node\n");
    printf("------------------------------------\n");
  }
}

void delete_line() {
  
}

int num_of_nodes() {
  int line_num = 0;
  node *temp = first;
  while (temp != NULL) {
    line_num++;
    
    temp = temp->next;
  }
  return line_num;
}
