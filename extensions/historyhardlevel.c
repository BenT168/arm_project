#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "historyhardlevel.h"
#include "calendargame.h"
extern int score;

void hardQues(int i) {
	switch(i) {
		case 1 :
		printf("                        -- Question 1 --                       \n");
			printf("                                                               \n");
			printf("               What date was the Magna Carta signed?           \n");
			printf("                                                               \n");
			printf("--------------------------------TIP---------------------------------\n");
			printf("                                                               \n");
			printf("                           It's in June                  \n");
			printf("                                                               \n");
			printf("--------------------------------------------------------------------\n");
			break;
		case 2 :
			printf("                        -- Question 2 --                       \n");
			printf("                                                               \n");
			printf("           What date did Shakespeare marry Anne Hathaway?     \n");
			printf("                                                               \n");
			printf("--------------------------------TIP---------------------------------\n");
			printf("                                                               \n");
			printf("    In the same month as Gunpowder plot and in the 16th century   \n");
			printf("                                                               \n");
			printf("--------------------------------------------------------------------\n");
			break;
    default:
      break;
	}
}


void hardlevel() {
	hardQues(1);
  /* correct answer */
	int day = 15;
	char* month = "June";
	int year = 1215;

  /* read input */
	int buffInt;
	char buffer[20];
	int buffyear;
	scanf(" %i %s %i", &buffInt, buffer, &buffyear);

  /* check answer */
	if(day == buffInt && strcasecmp(month, buffer) == 0 && buffyear == year)
	{
		score++;
		printf("               Good job! That's the correct answer.              \n");
		printf("                                                               \n");
		progress();
	} else {
		printf("                   You have the wrong answer.                    \n");
    printf("               %i %s %i is the correct date               \n", day, month, year);
		printf("                                                               \n");
		progress();
	}

	nextques();
	printf("                                                               \n");


	hardQues(2);
  /* correct answer */
	day = 14;
	month = "November";
	year = 1582;

  /* read input */
	int buffInt4;
	char buffer4[20];
	int buffyear2;
	scanf(" %i %s %i", &buffInt4, buffer4, &buffyear2);

  /* check answer */
	if(day == buffInt4 && strcasecmp(month, buffer4) == 0 && buffyear2 == year)
	{
		score++;
		printf("     Congratulations on answering the hardest question correctly!   \n");
		printf("                       You are a champion!!!  \n");
		printf("                                                               \n");
	} else {
		printf("                   You have the wrong answer.                    \n");
    printf("               %i %s %i is the correct date               \n", day, month, year);
		printf("                                                               \n");
	}
}
