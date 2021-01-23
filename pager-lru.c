/*
 * File: pager-lru.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains an lru pageit
 *      implmentation.
 */

#include <stdio.h> 
#include <stdlib.h>

#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) { 
    
	/* This file contains the stub for an LRU pager */
	/* You may need to add/remove/modify any part of this file */

	/* Static vars */
	static int initialized = 0;
	static int tick = 1; // artificial time
	static int timestamps[MAXPROCESSES][MAXPROCPAGES]; //used to track the life of a page
	//[[timestamp of pages, timestamp...]]
	/* Local vars */
	int proctmp;
	int pagetmp;
	int small;
	int swap;
	int proc;
	int pc, page, i;

	/* initialize static vars on first run */
	if(!initialized){
	for(proctmp=0; proctmp < MAXPROCESSES; proctmp++){
	    for(pagetmp=0; pagetmp < MAXPROCPAGES; pagetmp++){
		timestamps[proctmp][pagetmp] = 0; 
	    }
	}
	initialized = 1;
	}
    	
    /* TODO: Implement LRU Paging */
	
	for(proc=0; proc<MAXPROCESSES; proc++){
		//for loop
		//which process is currently running
		if(q[proc].active){
			//determine current page
			pc = q[proc].pc; 
			page = pc/PAGESIZE;
			//is it not in memory? 
			if(!q[proc].pages[page]) {
				//try to swap in
				if(!pagein(proc,page)) {
				//if fails, evict LRU page from memory
					swap = 0;
					small = tick;
					for(i =0; i< MAXPROCPAGES; i++){
						//get smallest tick value that is in memory
						if(timestamps[proc][i] < small && q[proc].pages[i]){
							
							//Make sure page isn't current
							if(i!=page){
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
			
		}
	}	
						
	/* advance time for next pageit iteration */
	tick++; 
	

} 
