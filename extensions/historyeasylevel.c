#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "historyeasylevel.h"
#include "calendargame.h"
extern int score;


void easyQues(int i) {
	switch(i) {
		case 1 :
			printf("                        -- Question 1 --                       \n");
			printf("                                                               \n");
			printf("                     What date is Christmas?        \n");
			printf("                                                               \n");
			printf("--------------------------------TIP---------------------------------\n");
			printf("                                                               \n");
			printf("               Answer of form: -Date -Month e.g. 4 June           \n");
			printf("                                                               \n");
			printf("--------------------------------------------------------------------\n");
			break;
		case 2 :
			printf("                        -- Question 2 --                       \n");
			printf("                                                               \n");
			printf("                     What year did WW2 end?        \n");
			printf("                                                               \n");
			printf("--------------------------------TIP---------------------------------\n");
			printf("                                                               \n");
			printf("                  Answer of form: -Year e.g. 2010                 \n");
			printf("                                                               \n");
			printf("--------------------------------------------------------------------\n");
			break;
		case 3 :
			printf("                        -- Question 3 --                       \n");
			printf("                                                               \n");
			printf("                What date is the battle of Hastings?          \n");
			printf("                                                               \n");
			printf("--------------------------------TIP---------------------------------\n");
			printf("                                                               \n");
			printf("      Answer of form: -Date -Month -Year e.g. 12 March 2010  \n");
			printf("                                                               \n");
			printf("--------------------------------------------------------------------\n");
			break;
		case 4 :
			printf("                        -- Question 4 --                       \n");
			printf("                                                               \n");
			printf("                What date was the Gunpowder Plot?              \n");
			printf("                                                               \n");
			printf("--------------------------------TIP---------------------------------\n");
			printf("                                                               \n");
			printf("      Answer of form: -Date -Month -Year e.g. 12 March 2010     \n");
			printf("                                                               \n");
			printf("--------------------------------------------------------------------\n");
		default:
		  break;
	}
}

void easylevel() {
	easyQues(1);
	/* correct answer */
	int day = 25;
	char* month = "December";

	/* read input */
  int buffInt;
	char buffer[20];
	scanf(" %i %s", &buffInt, buffer);

	/* check answer */
	if(day == buffInt && strcasecmp(month, buffer) == 0) {
		score++;
		printf("               Good job! That's the correct answer.               \n");
		printf("                                                               \n");
		progress();
	} else {
		printf("                   You have the wrong answer.                    \n");
		printf("                 %i %s is the correct date                 \n", day, month);
		printf("                                                               \n");
		progress();
	}

	nextques();

  printf("                                                               \n");

	easyQues(2);
	/* correct answer */
	int year = 1945;
  int buffInt2;

	/* read input */
	scanf(" %i", &buffInt2);

	/* check answer */
	if(year == buffInt2) {
		score++;
		printf("               Good job! That's the correct answer.               \n");
		printf("                                                               \n");
		progress();
	} else {
		printf("                   You have the wrong answer.                    \n");
		printf("                  %i is the correct year                  \n", year);
		printf("                                                               \n");
		progress();
	}

  nextques();
  printf("                                                               \n");

  easyQues(3);
	/* correct answer */
	day = 14;
	month = "October";
	year = 1066;

	/* read input */
  int buffInt3;
	char buffer3[20];
	int buffyear;
	scanf(" %i %s %i", &buffInt3, buffer3, &buffyear);

	/* check answer */
	if(day == buffInt3 && strcasecmp(month, buffer3) == 0 && buffyear == year)
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

	easyQues(4);
	/* correct answer */
	day = 5;
	month = "November";
	year = 1605;

	/* read input */
	int buffInt4;
	char buffer4[20];
	int buffyear2;
	scanf(" %i %s %i", &buffInt4, buffer4, &buffyear2);

	/* check answer */
	if(day == buffInt4 && strcasecmp(month, buffer4) == 0 && buffyear2 == year)
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
}
