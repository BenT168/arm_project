#ifndef CALENDARGAME_H
#define CALENDARGAME_H

int determineleapyear(int year);
int randomGen(int range);
int randomGenYear();
int calculateDay(int day, int month, int year);
int dayToInt(char* day);
char* intToDay(int day);
void Dateprogram() ;
void Holidayrogram();
char* int_to_holiday(int);
int get_month(int);
int get_day(int);
void progress();
void nextques();
void nextlevel();
void Historyprogram();
void Hangmanprogram();

#endif
