#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_FLASHCARDS 100
#define MAX_LINE 1024
#define MAX_SECTIONS 20

typedef struct {
    char question[MAX_LINE];
    char answer[MAX_LINE];
} Flashcard;

typedef struct {
    char name[100];
    Flashcard flashcards[MAX_FLASHCARDS];
    int flashcard_count;
} Section;

Section sections[MAX_SECTIONS];
int section_count = 0;
int current_section = 0;

void trim_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

void input_box(const char *prompt, char *output, int maxlen) {
    int row, col;
    getmaxyx(stdscr, row, col);
    WINDOW *win = newwin(5, col - 4, row / 2 - 3, 2);
    box(win, 0, 0);
    mvwprintw(win, 1, 2, "%s", prompt);
    echo();
    curs_set(1);
    mvwgetnstr(win, 2, 2, output, maxlen - 1);
    noecho();
    curs_set(0);
    delwin(win);
}

void save_sections() {
    FILE *fp = fopen("flashcards.dat", "w");
    if (!fp) return;
    fprintf(fp, "%d\n", section_count);
    for (int i = 0; i < section_count; i++) {
        fprintf(fp, "SECTION:%s\n", sections[i].name);
        fprintf(fp, "%d\n", sections[i].flashcard_count);
        for (int j = 0; j < sections[i].flashcard_count; j++) {
            fprintf(fp, "Q:%s\n", sections[i].flashcards[j].question);
            fprintf(fp, "A:%s\n", sections[i].flashcards[j].answer);
        }
    }
    fclose(fp);
}

void load_sections() {
    FILE *fp = fopen("flashcards.dat", "r");
    if (!fp) return;

    char line[MAX_LINE];
    fgets(line, MAX_LINE, fp);
    section_count = atoi(line);

    for (int i = 0; i < section_count; i++) {
        fgets(line, MAX_LINE, fp);
        trim_newline(line);
        strncpy(sections[i].name, line + 8, sizeof(sections[i].name));

        fgets(line, MAX_LINE, fp);
        sections[i].flashcard_count = atoi(line);
        for (int j = 0; j < sections[i].flashcard_count; j++) {
            fgets(line, MAX_LINE, fp);
            trim_newline(line);
            strncpy(sections[i].flashcards[j].question, line + 2, MAX_LINE);
            fgets(line, MAX_LINE, fp);
            trim_newline(line);
            strncpy(sections[i].flashcards[j].answer, line + 2, MAX_LINE);
        }
    }

    fclose(fp);
}

void extract_clozes(const char *answer, char processed[MAX_LINE], char clozes[10][MAX_LINE], int *count) {
    const char *start = answer;
    int i = 0;
    processed[0] = '\0';
    *count = 0;

    while (*start) {
        const char *open = strstr(start, "{{");
        if (!open) {
            strcat(processed, start);
            break;
        }
        strncat(processed, start, open - start);
        const char *close = strstr(open, "}}");
        if (!close) break;

        strcat(processed, "[____]");
        strncpy(clozes[i], open + 2, close - open - 2);
        clozes[i][close - open - 2] = '\0';
        i++;
        (*count)++;
        start = close + 2;
    }
}

void practice_flashcards() {
    if (section_count == 0) return;
    Section *sec = &sections[current_section];

    for (int i = 0; i < sec->flashcard_count; ++i) {
        clear();
        char clozes[10][MAX_LINE], processed[MAX_LINE];
        int cloze_count = 0;
        extract_clozes(sec->flashcards[i].answer, processed, clozes, &cloze_count);

        mvprintw(2, 2, "Q: %s", sec->flashcards[i].question);
        mvprintw(4, 2, "A: %s", processed);

        for (int j = 0; j < cloze_count; j++) {
            char guess[MAX_LINE];
            mvprintw(6 + j, 2, "Blank #%d: ", j + 1);
            echo();
            getnstr(guess, MAX_LINE);
            noecho();

            if (strcmp(guess, clozes[j]) == 0) {
                attron(COLOR_PAIR(2));
                mvprintw(6 + j, 20, "✔ Correct");
                attroff(COLOR_PAIR(2));
            } else {
                attron(COLOR_PAIR(1));
                mvprintw(6 + j, 20, "✘ Wrong: %s", clozes[j]);
                attroff(COLOR_PAIR(1));
            }
        }
        mvprintw(LINES - 2, 2, "Press any key to continue...");
        getch();
    }
}

void create_flashcard() {
    if (section_count == 0) return;
    Flashcard *fc = &sections[current_section].flashcards[sections[current_section].flashcard_count];
    input_box("Enter question:", fc->question, MAX_LINE);
    input_box("Enter answer (use {{...}} for blanks):", fc->answer, MAX_LINE);
    sections[current_section].flashcard_count++;
    save_sections();
}

void create_section() {
    if (section_count >= MAX_SECTIONS) return;
    input_box("New section name:", sections[section_count].name, sizeof(sections[section_count].name));
    sections[section_count].flashcard_count = 0;
    section_count++;
    save_sections();
}

void delete_section() {
    if (section_count == 0) return;
    for (int i = current_section; i < section_count - 1; i++) {
        sections[i] = sections[i + 1];
    }
    section_count--;
    if (current_section >= section_count) current_section = section_count - 1;
    save_sections();
}

void switch_section() {
    int choice = current_section;
    while (1) {
        clear();
        mvprintw(1, 2, "Choose a section:");
        for (int i = 0; i < section_count; i++) {
            if (i == choice) attron(A_REVERSE);
            mvprintw(3 + i, 4, "%s", sections[i].name);
            if (i == choice) attroff(A_REVERSE);
        }
        int ch = getch();
        if (ch == KEY_UP) choice = (choice - 1 + section_count) % section_count;
        else if (ch == KEY_DOWN) choice = (choice + 1) % section_count;
        else if (ch == 10) {
            current_section = choice;
            return;
        } else if (ch == 27) return;
    }
}

void print_centered(int y, const char *text, int selected) {
    int x = (COLS - strlen(text)) / 2;
    if (selected)
        attron(A_REVERSE);
    mvprintw(y, x, "%s", text);
    if (selected)
        attroff(A_REVERSE);
}

void main_menu() {
    const char *choices[] = {
        "Create Section", "Switch Section", "Delete Section",
        "Add Flashcard", "Practice Flashcards",
        "Quit"
    };
    int choice = 0;
    int n_choices = sizeof(choices) / sizeof(char *);

    while (1) {
        clear();
        mvprintw(1, 2, "Current Section: %s", section_count > 0 ? sections[current_section].name : "None");
        for (int i = 0; i < n_choices; i++)
            print_centered(3 + i * 2, choices[i], i == choice);
        int ch = getch();
        if (ch == KEY_UP) choice = (choice - 1 + n_choices) % n_choices;
        else if (ch == KEY_DOWN) choice = (choice + 1) % n_choices;
        else if (ch == 10) {
            switch (choice) {
                case 0: create_section(); break;
                case 1: switch_section(); break;
                case 2: delete_section(); break;
                case 3: create_flashcard(); break;
                case 4: practice_flashcards(); break;
                case 5: return;
            }
        } else if (ch == 27) return;
    }
}

int main() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    load_sections();
    main_menu();
    endwin();
    save_sections();
    return 0;
}
