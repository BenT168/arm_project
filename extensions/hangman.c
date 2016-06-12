#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


#include "hangman.h"

int longestWord = 23;
int fails = 0;
int ques = 1;
int passQues = 0;

void showHangman(int);

/* Create dash line for inputing answer */
char* dashes(char* answer) {
  char* dash = (char*)malloc(sizeof(char) * (strlen(answer)+1));
  dash[0] = '_';
  for(int i = 1; i < strlen(answer); i++) {
    if(answer[i] != ' ') {
      strcat(dash, "_");
    } else {
      strcat(dash, " ");
    }
  }
  return dash;
}


/* 10 perpared questions */
void questions(int i) {
  char* dash = (char*)malloc(sizeof(char) * longestWord);
	switch(i) {
		case 1 :
      printf("                                                               \n");
			printf("                        -- Question 1 --                       \n");
			printf("                                                               \n");
			printf("Name the most famous battle which took place between 1939 and 1945 \n");
			printf("                                                               \n");
      dash = dashes(answers(1));
      printf("                            %s                              \n   ", dash);
      printf("                                                              \n");
			break;

    case 2 :
			printf("                        -- Question 2 --                       \n");
			printf("                                                               \n");
			printf("               Name the second wife of Henry VIII     \n");
			printf("                                                               \n");
      dash = dashes(answers(2));
      printf("                            %s                              \n   ", dash);
      printf("                                                              \n");
			break;

    case 3 :
			printf("                        -- Question 3 --                       \n");
			printf("                                                               \n");
			printf("               Who was the king before Queen Victoria?          \n");
			printf("                                                               \n");
      dash = dashes(answers(3));
      printf("                            %s                              \n   ", dash);
      printf("                                                              \n");
			break;

    case 4 :
			printf("                        -- Question 4 --                       \n");
			printf("                                                               \n");
			printf("       Which killer was known as 'The Whitechapel Murderer'?         \n");
			printf("                                                               \n");
      dash = dashes(answers(4));
      printf("                            %s                              \n   ", dash);
      printf("                                                              \n");
      break;

    case 5 :
			printf("                        -- Question 5 --                       \n");
			printf("                                                               \n");
			printf("     Which disease killed thousands of people in Glasgow in 1832?    \n");
			printf("                                                               \n");
      dash = dashes(answers(5));
      printf("                            %s                              \n   ", dash);
      printf("                                                              \n");
      break;

  	case 6 :
			printf("                        -- Question 6 --                       \n");
			printf("                                                               \n");
			printf("       Which planet was discovered by William Herschel in 1781?  \n");
			printf("                                                               \n");
      dash = dashes(answers(6));
      printf("                            %s                              \n   ", dash);
      printf("                                                              \n");
      break;

    case 7 :
      printf("                        -- Question 7 --                       \n");
      printf("                                                               \n");
      printf("     Who was the the longest serving Prime minister of the UK?      \n");
      printf("                                                               \n");
      dash = dashes(answers(7));
      printf("                            %s                              \n   ", dash);
      printf("                                                              \n");
      break;

    case 8 :
      printf("                        -- Question 8 --                       \n");
      printf("                                                               \n");
      printf("       Which country was the first to allow women to vote?         \n");
      printf("                                                               \n");
      dash = dashes(answers(8));
      printf("                            %s                              \n   ", dash);
      printf("                                                              \n");
      break;

    case 9 :
      printf("                        -- Question 9 --                       \n");
      printf("                                                               \n");
      printf("       Which King of England was crowned on Christmas Day?         \n");
      printf("                                                               \n");
      dash = dashes(answers(9));
      printf("                            %s                              \n   ", dash);
      printf("                                                              \n");
      break;

    case 10 :
      printf("                        -- Question 10 --                       \n");
      printf("                                                               \n");
      printf("       Who made the first telephone call to the moon?         \n");
      printf("                                                               \n");
      dash = dashes(answers(10));
      printf("                            %s                              \n   ", dash);
      printf("                                                              \n");
      break;

		default:
		  break;
	}
  free(dash);
}




/* answers for the 10 questions */
char* answers(int ans) {
  char* answer = "world war two";
  switch(ans) {
    case 1 : break;
    case 2 : answer = "anne boleyn"; break;
    case 3 : answer = "william viii"; break;
    case 4 : answer = "jack the ripper"; break;
    case 5 : answer = "cholera"; break;
    case 6 : answer = "uranus"; break;
    case 7 : answer = "robert walpole"; break;
    case 8 : answer = "new zealand"; break;
    case 9 : answer = "william the conqueror"; break;
    case 10 : answer = "richard nixon"; break;
  }
  return answer;
}


/* check if the input is contained in correct answer */
int isinAnswer(char c, char* answer) {
  if(strchr(answer, c) != NULL) {
    return 1;
  }
  return 0;
}

/* update dash line */
char* checkanswer(char c, char* answer, char* updateDash) {
  for(int i = 0; i < strlen(answer); i++) {
    if(answer[i] == c) {
      updateDash[i] = c;
    }
  }
  return updateDash;
}


/* ask whether go to next question or not */
void nextquesHang() {
	printf("                                                               \n");
	printf("Would you like to go to the next question? [y/n]\n");
  char c;
	scanf(" %c", &c);
	if(tolower(c) == 'y') {
		system("clear");
	} else if(tolower(c) == 'n') {
		printf("Thank you for playing Hangman! We hope you enjoyed it!\n");
		exit(EXIT_SUCCESS);
	} else {
		printf("Sorry, you have typed in an invalid letter!\n");
		printf("Please type in y or n.\n");
		nextquesHang();
	}
}


/* Hangman Game */
void hangman() {
  /* show question */
  questions(ques);
  int count = 0;
  printf("Write a letter:\n ");
  repeat: ;
  /* create the dash line */
  char answer;
  if(count > 0) goto scan;
  char* dash = (char*)malloc((strlen(dashes(answers(ques))) + 1) * sizeof(char));
  dash = dashes(answers(ques));

  /* read the input */
  scan: ;
  scanf(" %c", &answer);
  char c = tolower(answer);

  /* check input */
  if(isinAnswer(c, answers(ques))) {
    char* result = checkanswer(c, answers(ques), dash);
    dash = result;
    printf("                           %s                                 \n",result);
  } else {  // case incorrect input
    printf("                                                               \n");
    fails++;
    /* show the new hangman when it's wrong */
    showHangman(5 - fails + 1);
      printf("                           %s                             \n",dash);
    /* if all lives are used GAME OVER */
    if(fails == 6) {
      printf("                                                                \n");
      printf("               Sorry! You have been hanged.                   \n");
      printf("---------------------------------------------------------------\n");
      printf("   To start again               TYPE ----------------  a \n");
    	printf("   To go to next question       TYPE ----------------  y \n");
    	printf("   To quit                      TYPE ----------------  n \n");
      printf("---------------------------------------------------------------\n");

      wrong : ;
      /* read input for decision */
      char input;
      scanf(" %c", &input);
      char d = tolower(input);

      if(d == 'a') {
        fails = 0;
        hangman();
      } else if(d == 'y') {
        ques++;
        fails = 0;
        hangman();
      } else if(d == 'n') {
        printf("                                                               \n");
      	printf("    Thank you for playing Hangman! We hope you enjoyed it!\n");
        exit(EXIT_SUCCESS);
      } else {
    		printf("Sorry, you have typed in an invalid letter!\n");
    		printf("Please type in a, y or n.\n");
    		goto wrong;
    	}
    }
  }


  /* we completed the word! */
  if(strcmp(dash, answers(ques)) == 0) {
    passQues++;
    printf("                                                               \n");
    printf("    Congratulations!!! You have gotten the correct answer!     \n");

    /* got all the questions correct */
    if(passQues == 10) {
      printf("                                                               \n");
      printf("        OMG!!! You passed all the questions!!!        \n");
      printf("        You are an amazing specimen of nature XD      \n");
      printf("                                                               \n");
      exit(EXIT_SUCCESS);
    }
    /* answered all questions but with some error */
    if(ques == 10) {
      printf("                                                               \n");
      printf("              You have done the last question!                \n");
      printf("             You have passed %i questions in total. \n", passQues);
      printf("    Thank you for playing Hangman! We hope you enjoyed it!\n");
      exit(EXIT_SUCCESS);
    }
    nextquesHang();
    ques++;
    fails = 0;
    hangman();
  }

  if(ques == 10) {
    printf("                                                               \n");
    printf("              You have done the last question!                \n");
    printf("             You have passed %i questions in total. \n", passQues);
    printf("    Thank you for playing Hangman! We hope you enjoyed it!\n");
    exit(EXIT_SUCCESS);
  }
  count++;
  goto repeat;
}




void showHangman(int choice) { //This function show the hangman after each wrong try

  switch(choice) {
    case 0:
    system("clear");
    printf("                  Wrong letter.                           \n");
	  printf("\n\t              ||===== ");
	  printf("\n\t              ||    | ");
	  printf("\n\t              ||   %cO/",'\\');
	  printf("\n\t              ||    | ");
	  printf("\n\t              ||   / %c",'\\');
	  printf("\n\t              ||      ");
    printf("                                                               \n");
	  break;

    case 1:
    system("clear");
    printf("                  Wrong letter.                           \n");
	  printf("\n\t              ||===== ");
	  printf("\n\t              ||    | ");
	  printf("\n\t              ||   %cO/",'\\');
	  printf("\n\t              ||    | ");
	  printf("\n\t              ||     %c",'\\');
	  printf("\n\t              ||      ");
    printf("                                                               \n");
	  break;

    case 2:
    system("clear");
    printf("                  Wrong letter.                           \n");
	  printf("\n\t              ||===== ");
	  printf("\n\t              ||    | ");
	  printf("\n\t              ||   %cO/",'\\');
	  printf("\n\t              ||    | ");
	  printf("\n\t              ||      ");
	  printf("\n\t              ||      ");
    printf("                                                               \n");
	  break;

    case 3:
    system("clear");
    printf("                  Wrong letter.                           \n");
	  printf("\n\t              ||===== ");
	  printf("\n\t              ||    | ");
	  printf("\n\t              ||   %cO/",'\\');
	  printf("\n\t              ||      ");
	  printf("\n\t              ||      ");
	  printf("\n\t              ||      ");
    printf("                                                               \n");
	  break;

    case 4:
    system("clear");
    printf("                  Wrong letter.                           \n");
	  printf("\n\t              ||===== ");
	  printf("\n\t              ||    | ");
	  printf("\n\t              ||   %cO ",'\\');
	  printf("\n\t              ||      ");
	  printf("\n\t              ||      ");
	  printf("\n\t              ||      ");
    printf("                                                               \n");
	  break;

    case 5:
    system("clear");
    printf("                  Wrong letter.                           \n");
	  printf("\n\t              ||===== ");
	  printf("\n\t              ||    | ");
	  printf("\n\t              ||    O ");
	  printf("\n\t              ||      ");
	  printf("\n\t              ||      ");
	  printf("\n\t              ||      ");
    printf("                                                               \n");
	  break;
  }
}
