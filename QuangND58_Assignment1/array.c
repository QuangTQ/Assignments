#include <stdio.h>
#include "array.h"

/* Create an array */
void Input(int *Array,int *length)
{
	int index=0;

		for(index=0;index<(*length);index++)
		{
			printf("\nArray[%d]=",index);
			scanf("%d",&Array[index]);
			fflush(stdin);
		}
}

/*  Print array  */
void Print(int *Array, int length)
{
	int index=0;
	printf("\nArray[%d]=",length);
	for(index=0;index<length;index++)
		{
			printf("  %d",Array[index]);
		}
}

/* insert one element at the end of array */ 
void Insert_at_the_end(int *Array, int *length, int insertValue)
{
	(*length)++;
	Array[*length-1]=insertValue;
}
 
/* delete the element in x position */
void Delete_one_element(int *Array,int *length,int pos)
{
	int index=0;
		for(index=pos-1;index<*length;index++)
			{
				 Array[index]=Array[index+1];		
			}
	(*length)--;
}

/* sort element of array ascending */
void Sort_ascending(int *Array, int length)
{
	int index1=0;
	int index2=0;
	for(index1=0;index1<length;index1++)
	{
		for(index2=index1+1;index2<length;index2++)	
		{
			if(Array[index1]>Array[index2])
			{
				swap(&Array[index1],&Array[index2]);
			}
		}
	}
	
}

/* sort element of array decrease */
void Sort_decrease(int *Array, int length)
{
	int index1=0;
	int index2=0;
	for(index1=0;index1<length;index1++)
	{
		for(index2=index1+1;index2<length;index2++)	
		{
			if(Array[index1]<Array[index2])
			{
				swap(&Array[index1],&Array[index2]);
			}
		}
	}
}

/* function to search the value exist in the array or NOT */
void search(int *Array, int length)
{
	int value=0;
	int index=0;
	int check=0;
	int pos=0;
	printf("\nEnter the value do you want check:");
	scanf("%d",&value);
	fflush(stdin); 
	for(index=0;index<length;index++)
	{
		if(Array[index]==value)
		{
			check=1;
			/* get positon of value */
			pos=index+1;
		}
	}
	if(1==check)
	{
		printf("\n%d on %dth position in the array",value,pos);
	}
	else
	{
		printf("\n%d NOT found in the array",value);
	}
}

/* swap 2 number pass via agrument */
static int swap(int *num1, int *num2)
{
	int temp=0;
	temp=*num1;
	*num1=*num2;
	*num2=temp;
}

