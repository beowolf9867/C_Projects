#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
//functions used: 
//strcmp and atoi and isdigit

int stack[100];
int *s = stack;
char line[100][100];
char *line_ptr[100];
char *w = NULL;

int push(int value);
int pop(void);
void read(void);
int read_line(char separator);
void put_number(int n);
void myprintf(const char *format, ...);

int main(void) {
  //point the pointers to line array
  for (int i = 0; i < 100; i++)
    line_ptr[i] = &line[i][0];
  myprintf("*************************\n");
  myprintf("*          Stack        *\n");
  myprintf("*************************\n");
  int exit = 0;
  while (!exit) {
    myprintf("Enter string: ");
    int x = read_line(',');
    for (int i = 0; i < x; i++) {
      if (isdigit(line_ptr[i][0]))
	push(atoi(line_ptr[i]));
      if (line_ptr[i][0] == '+') {
	int right = pop();
	int left = pop();
	int num = push(right+left);
	printf("%d\n", stack[num-1]);
      }
      if (line_ptr[i][0] == '-') {
	int right = pop();
	int left = pop();
	int num = push(left-right);
	printf("%d\n", stack[num-1]);
      }
    }
  }
  return 0;
}

int push(int value) {
  if (s >= &stack[99])
    myprintf("Overflow, address not stored.\n");
  else
    *s++ = value;
  return s-stack;
}

int pop(void) {
  if (s == stack) {
    myprintf("Underflow, no addresses\n");
    return 0;
  } else
    return *(--s); //IMPORTANT
}

void read(void) {
  int *r = stack;
  for (r = s-1; r >= stack;)
    myprintf("%d, ", *r--);
  myprintf("\n");
}

int read_line(char separator) {
  int temp_val = 0;
  int x = 0;
  int sep = (int)separator;
  while (temp_val != '\n') {
    w = line_ptr[x];
    while (((temp_val = getchar()) != sep) && temp_val != '\n') {
      if (temp_val == ' ')
	continue;
      *w++ = temp_val;
    }
    *w = '\0';
    w = line_ptr[++x];
  }
  return x;
}

void put_number(int n) {
  if (n == 0) {
    putchar('0');
    return;
  }
  if (n < 0) {
    putchar('-');
    n = -n;
  }
  char store[12];    
  char *s = store;
  while (n > 0) {
    *s++ = ((n % 10) + '0');
    n /= 10;
  }
  s--;
  for (; s >= store; s--)
    putchar(*s);
}

void myprintf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  char *f = format;
  for (;*f != '\0'; f++) {
    if (*f != '%')
      putchar(*f);
    else {
      if (*(f+1) == 's') {
	fputs(va_arg(args, char *), stdout);
	f++;
      } else if (*(f+1) == 'd') {
	f++;
	int num = va_arg(args, int);
	put_number(num);
      } else if (*(f+1) == 'c') {
	f++;
	putchar(va_arg(args, int));
      } else if (*(f+1) == '%') {
	putchar('%');
	f++;
      }
    }
  }
  va_end(args);
}

