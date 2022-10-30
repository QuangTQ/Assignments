#ifndef H_ARRAY_H
#define H_ARRAY_H

/* Create an array */
void Input(int *Array,int *length);

/*  Print array  */
void Print(int *Array, int length);

/* insert one element at the end of array */ 
void Insert_at_the_end(int *Array, int *length,int insertValue);

/* delete the element in x position */
void Delete_one_element(int *Array,int *length,int pos);

/* sort element of array ascending */
void Sort_ascending(int *Array, int length);

/* sort element of array decrease */
void Sort_decrease(int *Array, int length);

/* function to search the value exist in the array or NOT */
void search(int *Array, int length);

/* swap 2 number pass via agrument */
static int swap(int *num1, int *num2);

#endif/*H_ARRAY_H*/
