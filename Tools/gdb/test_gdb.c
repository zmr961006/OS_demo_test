/*************************************************************************
	> File Name: test_gdb.c
	> Author: 
	> Mail: 
	> Created Time: 2016年06月04日 星期六 20时26分58秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

#define   Max(a,b)  a > b ? a : b

static char buf[256];
static char *string;

void fun(){

    printf("welcome to xiyou!\n");

}


int main(){
    int a = 1;
    int b = 2;
    Max(1,2);
    printf("welcome to xiyou Linux !\n");
    fun();
    printf("please enter something\n");
    gets(string);
    printf("the string is %s\n",string);


}
