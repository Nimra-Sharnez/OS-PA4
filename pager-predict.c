/*
 * File: pager-predict.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains a predictive pageit
 *      implmentation.
 */

#include <stdio.h> 
#include <stdlib.h>
#include "simulator.h"
#include "stack.c"



void pageit(Pentry q[MAXPROCESSES]) { 
    
	/* This file contains the stub for an LRU pager */
	/* You may need to add/remove/modify any part of this file */

	/* Static vars */
	static int initialized = 0;
	static int tick = 1; // artificial time
	static int timestamps[MAXPROCESSES][MAXPROCPAGES]; //used to track the life of a page
	//[[timestamp of pages, timestamp...]]
	static int markov[MAXPROCESSES][MAXPROCPAGES][MAXPROCPAGES];
	static int nextpage[MAXPROCESSES][1];
	static int pcTracker[MAXPROCESSES][1]; 
	static int maxPC[MAXPROCESSES][MAXPROCPAGES];
	static int ind[MAXPROCESSES][1];
	static int prevpage[MAXPROCESSES][1];
	static int jump[MAXPROCESSES][1];
	
	
	/* Local vars */
	int proctmp;
	int pagetmp;
	int small;
	int swap;
	int proc;
	int pc, page, i, pgtmp, j;
	//int checkpoint[MAXPROCESSES][1];
	//int prevPC[MAXPROCESSES][1];
	
	
	/* initialize static vars on first run */
	if(!initialized){
	for(proctmp=0; proctmp < MAXPROCESSES; proctmp++){
		nextpage[proctmp][0] = 0;
		top[proctmp][0] = -1;
		prevpage[proctmp][0] = 0;
		pcTracker[proctmp][0] = 0;
		ind[proctmp][0] = 0;
	    for(pagetmp=0; pagetmp < MAXPROCPAGES; pagetmp++){
		timestamps[proctmp][pagetmp] = 0; 
		stack[proctmp][pagetmp] = 0;
		maxPC[proctmp][pagetmp] = 0;
		for(pgtmp = 0; pgtmp < MAXPROCPAGES; pgtmp++){
			markov[proctmp][pagetmp][pgtmp] = 0;
		}
	    }
	}
	initialized = 1;
	}



    	
    /* TODO: Implement LRU Paging */

	
	for(proc=0; proc<MAXPROCESSES; proc++){
		
		//which process is currently running
		if(q[proc].active){
			//determine current page
			
			pc = q[proc].pc;
			page = pc/PAGESIZE; 

			//if we have entered a new page
			if (page != prevpage[proc][0]){ 


				if ((prevpage[proc][0] == 3 && page == 10) || (prevpage[proc][0] == 13 && page == 9)){
					jump[proc][0] = 1;
				}
				
				//process at previous keep track of it's next process
				markov[proc][prevpage[proc][0]][page]++;
				push(page, proc);
	
				
				//prevpage[proc][0] = page;
				j = -1;
				for(i=0; i<MAXPROCPAGES;i++){
					if(markov[proc][page][i] > j){
						j = markov[proc][page][i]; 
						nextpage[proc][0] = i;
					 }	
				}
				//for initially starting
				if (j == 0){ nextpage[proc][0] = page+1; markov[proc][page][nextpage[proc][0]]++; /*printf("%d\n", pc);*/}			
			
				if (prevpage[proc][0]!= page-1){
					for(i=0; i <MAXPROCPAGES; i++){
						if (pcTracker[proc][0] == maxPC[proc][i]){
							i = 99;
							break;
						}
						
					}
					if(i != 99){ //made it through entire list and none were same
						ind[proc][0]++;
						maxPC[proc][ind[proc][0]] = pcTracker[proc][0];
					}
				}
					
			}
		
			
			 

			
			
			
			
			prevpage[proc][0] = page;
			pcTracker[proc][0] = pc;
			//is it not in memory? 
			if(!q[proc].pages[page]) {
				//page out something
				//try to swap in
				if(!pagein(proc,page)) { 
				//if fails, evict LRU page from memory
					swap = 0;
					small = tick;
					for(i =0; i< MAXPROCPAGES; i++){
						//get smallest tick value that is in memory
						if(timestamps[proc][i] < small && q[proc].pages[i]){
							
							//Make sure page isn't current and isn't next
							if(!(i==page || i==nextpage[proc][0])){
								small = timestamps[proc][i];
								swap = i;
								
							}
						}
					}
					if(pageout(proc,swap)) {
						
						continue;
					}


					
				}

			}
			
			timestamps[proc][page] = tick;

			


			

			//if it is not a jumping process page out the previous page
			if(jump[proc][0] == 0){
				if (q[proc].pages[page-1] && (page-1 >= 0)){
					pageout(proc, page-1);
				}

			}


			//if it is a jumping process this is hardcode the pattern
			else{
				if (page == 3){//3-->10
					
					if (q[proc].pages[6]){
						pageout(proc, 6);
					}
					if (q[proc].pages[7]){
						pageout(proc, 7);
					}
					if (q[proc].pages[8]){
						pageout(proc, 8);
					}
					if (q[proc].pages[9]){
						pageout(proc, 9);
					}
					
					if(!q[proc].pages[nextpage[proc][0]]){ //it'll be 4 if 13->9 and 10 if 3->10
						if(pagein(proc,nextpage[proc][0])){
							timestamps[proc][nextpage[proc][0]] = tick;
						}
					}
					

				}
				else if (page == 13){//9, 10, 11, 12, 13 :||
					
					if (q[proc].pages[8]){
						pageout(proc, 8);
					}
					if (q[proc].pages[7]){
						pageout(proc, 7);
					}
					if (q[proc].pages[6]){
						pageout(proc, 6);
					}
					if (q[proc].pages[5]){
						pageout(proc, 5);
					}
					if (q[proc].pages[4]){
						pageout(proc, 4);
					}
					if (q[proc].pages[3]){
						pageout(proc, 3);
					}
					if (q[proc].pages[2]){
						pageout(proc, 2);
					}

					if(!q[proc].pages[nextpage[proc][0]]){ //it'll be 9 if 13->9 and 14 if 3->10
						if(pagein(proc,nextpage[proc][0])){
							timestamps[proc][nextpage[proc][0]] = tick;
						}
					}

					
				}

			}
				
			
			//if the pc is at 0 hard code the pattern
			if (pc == 0){
				if (q[proc].pages[14]){
					pageout(proc, 14);
				}
				if (q[proc].pages[13]){
					pageout(proc, 13);
				}
				if (q[proc].pages[12]){
					pageout(proc, 12);
				}
				if (q[proc].pages[11]){
					pageout(proc, 11);
				}
				if (q[proc].pages[10]){
					pageout(proc, 10);
				}
				if (q[proc].pages[9]){
					pageout(proc, 9);
				}
				if (q[proc].pages[8]){
					pageout(proc, 8);
				}
				if (q[proc].pages[7]){
					pageout(proc, 7);
				}
				if (q[proc].pages[6]){
					pageout(proc, 6);
				}//?
				if (q[proc].pages[5]){
					pageout(proc, 5);
				}
				if (q[proc].pages[4]){
					pageout(proc, 4);
				}
				if (q[proc].pages[3]){
					pageout(proc, 3);
				}

				
				if(!q[proc].pages[nextpage[proc][0]]){
					if(pagein(proc,nextpage[proc][0])){
						timestamps[proc][nextpage[proc][0]] = tick;
					}
				}

				

			}
			j = 0;
			for (i=0; i<MAXPROCPAGES; i++){
				if (maxPC[proc][i] > j){
					j = maxPC[proc][i];
				}
			}
			if (pc == j){ //this means we reached the end of the loop's global max
				if (page == 3 && jump[proc][0] == 0){
					if(!q[proc].pages[nextpage[proc][0]]){
						if(pagein(proc,nextpage[proc][0])){
							timestamps[proc][nextpage[proc][0]] = tick;
						}
					}
				}
				if (page == 8){
	
					if(!q[proc].pages[nextpage[proc][0]]){
						if(pagein(proc,nextpage[proc][0])){
							timestamps[proc][nextpage[proc][0]] = tick;
						}
					}
				}
				if (page == 11  && jump[proc][0] == 0){
					
					if(!q[proc].pages[nextpage[proc][0]]){
						if(pagein(proc,nextpage[proc][0])){
							timestamps[proc][nextpage[proc][0]] = tick;
						}
					}
				}

				
			}
			//staying 100-200 ticks ahead of the end
			if (pc<=j-100 && pc>=j-200 && jump[proc][0] == 0){

				if(!q[proc].pages[nextpage[proc][0]]){
					if(pagein(proc,nextpage[proc][0])){
						timestamps[proc][nextpage[proc][0]] = tick;
					}
				}
			}


			
			//page in next page
			else if(!q[proc].pages[nextpage[proc][0]]){
					if(pagein(proc,nextpage[proc][0])){
						timestamps[proc][nextpage[proc][0]] = tick;
					}
			}

			
			
			
		}


	}

					
	/* advance time for next pageit iteration */
	tick++;
	//printf("proc: %d num-> %d num-> %d num-> %d num-> %d\n",1, maxPC[1][0], maxPC[1][1], maxPC[1][2], maxPC[1][3] );
	
}

//R -g Tk &^C
