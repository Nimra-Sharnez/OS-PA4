#include <stdio.h>
#define MAXPROCESSES 20
#define MAXSIZE 20  
 
static int stack[MAXPROCESSES][MAXSIZE];     
static int top[MAXPROCESSES][1];            


int isempty(int p) {

   if(top[p][0] == -1)
      return 1;
   else
      return 0;
}
   
int isfull(int p) {

   if(top[p][0] == MAXSIZE){
      return 1;
}
   else{
      return 0;
}
}

int peek(int p) {
   return stack[p][top[p][0]];
}

int pop(int p) {
   int data;
   int i;
   if(!isempty(p)) {
      data = stack[p][0];
      top[p][0]--;  
      for (i = 1; i<MAXSIZE+1; i++){
	stack[p][i-1] = stack[p][i];
      } 
      return data;
   } else {
      return 0;
   }
}

int push(int data, int p) {

   if(!isfull(p)) {
      top[p][0]++;
      stack[p][top[p][0]] = data;
      return 1;
   } else {
      return 0;
   }
}
