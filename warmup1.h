#ifndef _WARMUP1_H_
#define _WARMUP1_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "my402list.h"

#define TRUE 1
#define FALSE 0
#define BUFLEN 1026

typedef struct tagTransaction
{
	char Type[1];
	long Time;
	int Amount;
	char *Descrpt;
}Transaction;

/******************Function Insert: insert structure Tran into list at the right place***********/
extern int Insert(Transaction *Tran, My402List *List);  


/****************Function Parse: parse one line string into structure transaction*****************/
extern int Parse(char *buf, Transaction *Tran);


/******************Function ReadInput: read one line from tfile and parse it into structure Tran*********/
extern int ReadInput(FILE *fp, char *buf);


/*******************Function Amount_Format: change integer amount into char* and print**************/
extern int Amount_Format(int amount_b, char *amount_a, char *type);


/***********************Function Date_Format: change long date into char* ***************************/
extern int Date_Format(long Time, char *date_after);


/******Function Balance: keep track of the balance field for each Tran, change into right format and print******************/
extern int Balance(int prev, Transaction *tran);


/****************Function Output: print out sorted elements in list********************************/
extern int Output(My402List *list);


/****************Function Command(): read command line and return right *fp**************/
extern FILE *Command(int argc, char *argv[]);

#endif
