#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
//functions used
// strcmp and atoi

int stack[100];
int *s = stack;
char line[100];
char *l = line;

int push(int value);
int pop(void);
void read(void);
void read_line(char separator);
void clear_line(void);
void put_number(int n);
void myprintf(const char *format, ...);

int main(void) {
  myprintf("*************************\n");
  myprintf("*          Stack        *\n");
  myprintf("*************************\n");
  int exit = 0;
  while (!exit) {
    myprintf("Enter command: ");
    read_line(',');
    if (strcmp(line, "Push") == 0) {
      clear_line();
      myprintf("Enter value: ");
      read_line(',');
      myprintf("%d stored at %d\n", atoi(line), push(atoi(line)));
      clear_line();
    } else if (strcmp(line, "Pop") == 0) {
      clear_line();
      myprintf("Value at %d\n", pop());
    } else if (strcmp(line, "Read") == 0) {
      clear_line();
      read();
    } else if (strcmp(line, "Exit") == 0) {
      clear_line();
      exit = 1;
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

void read_line(char separator) {
  int temp_val = 0;
  int sep = (int)separator;
  while (((temp_val = getchar()) != '\n') && temp_val != sep)
    *l++ = temp_val;
  *l = '\0';
}

void clear_line(void) {
  while (l > line && l < &line[99])
    *l-- = '\0';
  l = line;
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

