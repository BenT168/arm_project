#ifndef HANGMAN_H
#define HANGMAN_H

///////////////////////////////////function/////////////////////////////////////

void showHangman(int choice);
void questions(int i);
char* answers(int ans);
void hangman();
char* checkanswer(char c, char* answer, char* updateDash);
char* dashes(char* answer);
char* checkanswer(char c, char* answer, char* updateDash);
void nextquesHang();


#endif
