/*
 * file name: test.c
 * author   : Yu Liu
 * email    : <ilhanwnz@hotmail.com>
 * time     : Sat 09 Mar 2019 11:41:25 AM CST
 */

#include <stdio.h>

int main(void)
{
	int *arr[10];
	printf("%ld\n", sizeof(arr) / sizeof(*arr));
	return 0;
}
