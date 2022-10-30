/*----------------------------------------------------------------------*/
/*								 ASSIGMENT1			      				*/
/*								By QuangND58							*/
/*----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "array.h"

#define MAX_LENGTH 100U

/* list ERROR */
enum error_type{
	SUCCESS=0,
	ERROR_LENGTH_CHARACTER,
	ERROR_LENGTH_ARRAY,
	ERROR_UNINITIALIZE,
	ERROR_POSITION
};
/* show menu user */
static void show_menu();

/*show your option*/
static void show_user_option(char userOption);

/*check input length of array */
int check_length_array(int inputLength, int maxLength);

/*show detail error */
static void show_error(int errorType);

/* check delete position */
int check_position(int inputPos, int maxlength);

/*----------------------------------------------------------------------------------------------------------*/
int main() 
{
	int Arr[MAX_LENGTH]={0};
	char userOption[10];
	int inputLength=0;
	int checkLengthOption=0;
	int checkRangeArray=0;
	int returnError=0;
	int insertValue=0;
	int deletePositon=0;
	int checkPositon=0;
	
	do
	{
		 /* clear consle */
			system("cls");
			show_menu();
			printf("\nEnter your option:");
			gets(userOption);
			/* delete buffer */
			fflush(stdin);
			checkLengthOption=strlen(userOption);
			/* more than one character enter the keyboard */
			if(checkLengthOption>1)
			{
				returnError=ERROR_LENGTH_CHARACTER;
			}
			else
			{
				show_user_option(userOption[0]);
				switch(userOption[0])
				{
					case 'c':
					case 'C':
						printf("\nEnter the length of Array:");
						scanf("%d",&inputLength);
						fflush(stdin);
						checkRangeArray=check_length_array(inputLength,MAX_LENGTH);
						if(checkRangeArray==SUCCESS)
						{
							printf("\nEnter the value of each element (integer value):");
							Input(Arr,&inputLength);
						}
						else
						{
							returnError=ERROR_LENGTH_ARRAY;
						}
						break;
					case 'p':
					case 'P':
						checkRangeArray=check_length_array(inputLength,MAX_LENGTH);
						if(checkRangeArray==SUCCESS)
						{
							Print(Arr,inputLength);
						}
						else
						{
							returnError=ERROR_UNINITIALIZE;
						}
						break;
					case 'i':
					case 'I':
						checkRangeArray=check_length_array(inputLength,MAX_LENGTH);
						if(checkRangeArray==SUCCESS)
						{
							printf("\nEnter the input value from keyboard:");
							scanf("%d",&insertValue);
							fflush(stdin);
							Insert_at_the_end(Arr,&inputLength,insertValue);
						}
						else
						{
							returnError=ERROR_UNINITIALIZE;
						}
						break;	
					case 'd':
					case 'D':
						checkRangeArray=check_length_array(inputLength,MAX_LENGTH);
						if(checkRangeArray==SUCCESS)
						{
							printf("\nEnter the delete position from keyboard:");
							scanf("%d",&deletePositon);
							fflush(stdin);
							checkPositon=check_position(deletePositon,inputLength);
							if(checkPositon==SUCCESS)
							{
								Delete_one_element(Arr,&inputLength,deletePositon);
							}
							else
							{
								returnError=ERROR_POSITION;
							}
						}
						else
						{
							returnError=ERROR_UNINITIALIZE;
						}
						break;	
					case 's':
					case 'S':
						checkRangeArray=check_length_array(inputLength,MAX_LENGTH);
						if(checkRangeArray==SUCCESS)
						{
							Sort_ascending(Arr,inputLength);
						}
						else
						{
							returnError=ERROR_UNINITIALIZE;
						}
						break;		
					case 'x':
					case 'X':
						checkRangeArray=check_length_array(inputLength,MAX_LENGTH);
						if(checkRangeArray==SUCCESS)
						{
							Sort_decrease(Arr,inputLength);
						}
						else
						{
							returnError=ERROR_UNINITIALIZE;
						}
						break;
					case 't':
					case 'T':
						checkRangeArray=check_length_array(inputLength,MAX_LENGTH);
						if(checkRangeArray==SUCCESS)
						{
							search(Arr,inputLength);
						}
						else
						{
							returnError=ERROR_UNINITIALIZE;
						}
						break;
					case 'e':
					case 'E':
						exit(0);
						break;
					default:
						printf("\nYour option invalid, plase try again!\n");
				}
			}
			
		/*show error from user */
		if(returnError!=SUCCESS)
		{
			show_error(returnError);
		}
		/* keep console */
		else if(userOption[0]!='e'||userOption[0]!='E')
		{
			printf("\nPress eny key to continues...");
			/* keep console */
	        getch();
		}
		/*reset ERROR */
		returnError=SUCCESS;
	}while(userOption[0]!='e'||userOption[0]!='E');

	
return 0;
}

/*-------------------------------------------------------------------------------------------------------------------*/

static void show_menu()
{
			printf("\n\n****PROGRAM TO MANAGE NUMBERS USING ARRAY*****");
			printf("\n*   OPTIONS:                                 *");
			printf("\n*   C: Enter array                           *");
			printf("\n*   P: Print array                           *");
			printf("\n*   I: INSERT ONE ELEMENT IN LAST POSITION   *");
			printf("\n*   D: DELETE ONE ELEMENT IN K POSITION      *");
			printf("\n*   S: SORT ELEMENTS OF ARRAY ASCENDING      *");
			printf("\n*   X: SORT ELEMENTS OF ARRAY DECREASE       *");
			printf("\n*   T: SEARCH ONE NUMBER X IN ARRAY          *");
			printf("\n*   E: EXIT                                  *");
			printf("\n**********************************************\n");
	
}

static void show_user_option(char userOption)
{
	switch(userOption)
	{
		case 'c':
		case 'C':
				printf("\n**************CREATE ARRAY**************");
				break;
		case 'p':
		case 'P':
				printf("\n**************PRINT ARRAY***************");
				break;
		case 'i':
		case 'I':
				printf("\n*******INSERT AN ELEMENT AT THE END OF THE ARRAY*******");
				break;	
		case 'd':
		case 'D':
				printf("\n*******DELETE AN ELEMENT AT K POSITION OF THE ARRAY *******");
				break;
		case 's':
		case 'S':
				printf("\n***********SORT ELEMENTS ASCENDING *********");
				break;	
		case 'x':
		case 'X':
				printf("\n***********SORT ELEMENTS DECREASE***********");
				break;
		case 't':
		case 'T':
				printf("\n*******SEARCH AN ELEMENT IN THE ARRAY*******");
			break;

	}
}

int check_length_array(int inputLength, int maxLength)
{
	int check=0;
	
	if(inputLength<=0||inputLength>maxLength)
	{
		check=ERROR_LENGTH_ARRAY;
	}
	else
	{
		check=SUCCESS;
	}
	return check;
}

static void show_error(int errorType)
{
	switch(errorType)
	{
		case ERROR_LENGTH_CHARACTER:
			printf("\n->[ERROR]: More than 1 character!!!");
			break;
		case ERROR_LENGTH_ARRAY:
			printf("\n->[ERROR]: Range of Array invalid!!!");
			break;
		case ERROR_UNINITIALIZE:
			printf("\n->[ERROR]: Array Uninitialize!!!");
			break;
		case ERROR_POSITION:
			printf("\n->[ERROR]: Position Invalid!!!");
			break;
	}
	printf("\nPress any key to continuos....");
	getch();
}

int check_position(int inputPos, int maxlength)
{
	int check=0;
	if(inputPos<0||inputPos>maxlength)
	{
		check=ERROR_POSITION;
	}
	else
	{
		check=SUCCESS;
	}
	return check;
}


