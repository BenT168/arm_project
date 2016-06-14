#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "historymediumlevel.h"
#include "calendargame.h"
extern int score;

void mediumQues(int i) {
	switch(i) {
		case 1 :
			printf("                        -- Question 1 --                       \n");
			printf("                                                               \n");
			printf("             Which year did the Victorian era begin?         \n");
			printf("                                                               \n");
			printf("--------------------------------TIP---------------------------------\n");
			printf("                                                               \n");
			printf("               Answer of form: -Year e.g. 2010                  \n");
			printf("                                                               \n");
			printf("--------------------------------------------------------------------\n");
			break;
		case 2 :
			printf("                        -- Question 2 --                       \n");
			printf("                                                               \n");
			printf("           In which year was Abraham Lincoln assassinated?     \n");
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
			printf("               Which year was the Wall Street crash?          \n");
			printf("                                                               \n");
			printf("--------------------------------TIP---------------------------------\n");
			printf("                                                               \n");
			printf("               Answer of form: -Year e.g. 2010  \n");
			printf("                                                               \n");
			printf("--------------------------------------------------------------------\n");
			break;
		case 4 :
			printf("                        -- Question 4 --                       \n");
			printf("                                                               \n");
			printf("               What date is the Boston Tea Party?              \n");
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


void mediumlevel() {

	mediumQues(1);
	/* correct answer */
	int year = 1837;

	/* read input */
	int buffyear;
	scanf(" %i", &buffyear);

	/* check answer */
	if(year == buffyear) {
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


	mediumQues(2);
	/* correct answer */
  year = 1865;

	/* read input */
	int buffInt2;
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

	mediumQues(3);
	/* correct answer */
	year = 1929;

	/* read input */
	int buffyear2;
	scanf(" %i", &buffyear2);

	/* check answer */
	if(year == buffyear2)
	{
		score++;
		printf("               Good job! That's the correct answer.              \n");
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


	mediumQues(4);
	/* correct answer */
	int day = 16;
	char* month = "December";
	year = 1773;

	/* read input */
	int buffInt4;
	char buffer4[20];
	int buffyear3;
	scanf(" %i %s %i", &buffInt4, buffer4, &buffyear3);

	/* check answer */
	if(day == buffInt4 && strcasecmp(month, buffer4) == 0 && buffyear3 == year)
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
