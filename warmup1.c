#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#include "warmup1.h"

#define TRUE 1
#define FALSE 0
#define BUFLEN 1026

/*Define Structure Transaction used to store information from the tfile*/
/*typedef struct tagTransaction
{
	char Type[1];
	long Time;
	int Amount;
	char *Descrpt;
}Transaction;*/

extern int Insert(Transaction *Tran, My402List *List); 
extern int Parse(char *buf, Transaction *Tran);
extern int ReadInput(FILE *fp, char *buf);
extern int Amount_Format(int amount_b, char *amount_a, char *type);
extern int Date_Format(long Time, char *date_after);
extern int Balance(int prev, Transaction *tran);
extern int Output(My402List *list);
extern FILE *Command(int argc, char *argv[]);

My402List list;  //global variable list used to instore the transaction information

/******************Function Insert: insert structure Tran into list at the right place***********/
int Insert(Transaction *Tran, My402List *List)  
{
		
	if(My402ListEmpty(List))   //if list is empty insert at the very beginning
	{
		My402ListAppend(List, (void *)Tran);

		return TRUE;
	}
	
	if(My402ListEmpty(List)==FALSE)  //if list is not empty, find the right place to insert
	{

		My402ListElem *elem=NULL;
		elem=My402ListFirst(List);
		Transaction *tmpTran=elem->obj;

		if((Tran->Time)==(tmpTran->Time))
		{
				fprintf(stderr, "Insert Error: Time field is the same. \n");
				return FALSE;
		}
   
    if((Tran->Time)<(tmpTran->Time))
		{
			if(My402ListInsertBefore(List, (void *)Tran, My402ListFirst(List))==FALSE)
			{
				fprintf(stderr, "Insert Error: Insert unsuccessful. \n");
				return FALSE;
			}
		}
   
		else
		{My402ListElem *r=My402ListFirst(List);
		My402ListElem *q=My402ListNext(List, r);
		while(q!=NULL)
		{
			tmpTran=q->obj;
			if((tmpTran->Time)<(Tran->Time))
			{
				r=q;
				q=My402ListNext(List, q);
			}
			else
				break;
		}
    if((tmpTran->Time)==(Tran->Time))
			{
					fprintf(stderr, "Insert Error: Time field is the same. \n");
				return FALSE;
			}
		My402ListInsertAfter(List, (void *)Tran, r);}
	}
	
	return TRUE;
}


/****************Function Parse: parse one line string into structure transaction*****************/
int Parse(char *buf, Transaction *Tran)
{	
	int field=0, total;
	char type, time1[25], *amount=NULL, *descrpt;
	char *start_ptr=buf;		
	char *tab_ptr=strchr(start_ptr, '\t');

	while(tab_ptr!=NULL)
	{
		*tab_ptr++='\0';
	
		if(field==0)  //type field
		{
			strncpy(&type, start_ptr, sizeof(start_ptr));

			if((type!='+')&&(type!='-'))
			{
				fprintf(stderr, "Malformed Error: Type.\n");
				return FALSE;
			}
		}

		else if(field==1)  //time field
		{
			strncpy(time1, start_ptr, strlen(start_ptr));
			time1[strlen(start_ptr)]='\0';
      time_t now=time(NULL);
			if((atol(time1)<0)||(atol(time1)>now)) 
			{
				fprintf(stderr, "Invalid Error: Time.\n");
				return FALSE;
			}
		}

		else if(field==2)  //amount field
		{			
			amount=start_ptr;
			char *dot_ptr=strchr(amount, '.');
			*dot_ptr++='\0';

			int dollar, cent;
			if((dollar=atoi(amount))>=10000000)
			{
				fprintf(stderr, "Invalid Error: Amount.\n");
				return FALSE;
			}
			if((dollar)<0)
			{
				fprintf(stderr, "Invalid Error: Amount.\n");
				return FALSE;
			}
			amount=dot_ptr;
			if((cent=atoi(amount))>100)
			{
				fprintf(stderr, "Malformed Error: Amount.\n");
				return FALSE;
			}
			total=dollar*100+cent;
		}
		
      field=field+1;
			start_ptr=tab_ptr;
			tab_ptr=strchr(start_ptr, '\t');
	}
 
  if(field<3)
  {
   fprintf(stderr, "Malformed Error: missing field.\n");
				return FALSE;
  }
    if(field>3)
  {
   fprintf(stderr, "Malformed Error: too many fields.\n");
				return FALSE;
  }
  
	descrpt=start_ptr;  //descrpt field
 
	if((strlen(descrpt))>=25)
	{
		descrpt[24]='\0';
	}
 
                                                                                  
	Tran=malloc(sizeof(Transaction));
 
	strncpy(Tran->Type, &type, sizeof(type));

	Tran->Time=atol(time1);
	Tran->Amount=total;
	Tran->Descrpt = malloc(24);	
 
	strncpy(Tran->Descrpt, descrpt, strlen(descrpt));

  Tran->Descrpt[strlen(descrpt)] = '\0';

	if(Insert(Tran, &list)==FALSE)
                  return FALSE;

	return TRUE;
}


/******************Function ReadInput: read one line from tfile and parse it into structure Tran*********/
int ReadInput(FILE *fp, char *buf)
{	
	int i=1;
	while(fgets(buf, BUFLEN, fp)!=NULL)
	{
		if(strlen(buf)>1024)
   {
    fprintf(stderr, "Error: transaction line is to long.\n");
				return FALSE;
   }
    if(strlen(buf)>1)
		{
			buf[strlen(buf)-1]='\0';
			i=i+1;
			Transaction Tran;
			if(Parse(buf, &Tran)==FALSE)
                    exit(1);	
		}
	}
	
	return TRUE;
}


/*******************Function Amount_Format: change integer amount into char* and print**************/
int Amount_Format(int amount_b, char *amount_a, char *type)
{
	char type1;
	strcpy(&type1, type);	

	if(amount_b<1000000000)
	{
		char amount_cent[4], amount_dollar[20];
		char amount_before[30];
	
		sprintf(amount_before, "%d", amount_b);
		int length= strlen(amount_before);
		int i;
		
		amount_cent[3]='\0';
		amount_cent[2]=amount_before[length-1];
		amount_cent[1]=amount_before[length-2];
		amount_cent[0]='.';
		
		for(i=0; i<length-2; i++)
		{
			amount_dollar[i]=amount_before[i];
		}

		amount_dollar[length-2]='\0';
		int amount_dollar_before=atoi(amount_dollar);
		int F1=amount_dollar_before/1000000;
		int S3=(amount_dollar_before-F1*1000000)/1000;
		int T3=amount_dollar_before-F1*1000000-S3*1000;
	
		if(F1>0)
		{sprintf(amount_dollar, "%d,%03d,%03d", F1, S3, T3);	}
		else if(F1==0&&S3>0)
		{sprintf(amount_dollar, "%d,%03d", S3, T3);}
		else if(F1==0&&S3==0)
		{sprintf(amount_dollar, "%d", T3);}
	
		strcpy(amount_a, strcat(amount_dollar, amount_cent));	
	
		if(type1=='+')
			{
				printf("%14s  |", amount_a); 
			}
		else
			{
				printf(" (");
				printf("%12s) |", amount_a); 
			}
		return TRUE;
	}
		
	if (type1=='+')
	{
		printf("  ?,???,???.??  |");
		return TRUE;
	}
	else
	{
		printf(" (");
		printf("?,???,???.??");
		printf(") |");
		return TRUE;
	}
}

/***********************Function Date_Format: change long date into char* ***************************/
int Date_Format(long Time, char *date_after)
{
	char date_before[25];
	strcpy(date_before, ctime(&Time));
	int i;
	for(i=0; i<=9;i++)
	{
		date_after[i]=date_before[i];
	}

	for(i=10; i<=14;i++)
	{
		date_after[i]=date_before[i+9];
	}
	date_after[15]='\0';

	return TRUE;
}


/******Function Balance: keep track of the balance field for each Tran, change into right format and print******************/
int Balance(int prev, Transaction *tran)
{
	char type1;
	int symbol;
	strcpy(&type1, tran->Type);
	if(type1=='+')
		prev=prev+(tran->Amount);
	else
		prev=prev-(tran->Amount);

	if(prev<=-1000000000)
	{
		printf(" (");
		printf("?,???,???.??");
		printf(") |");
		return prev;
	}
	else if(prev>=1000000000)
	{
		printf("  ?,???,???.??  |");
		return prev;
	}
	
	if(prev>=0)
		symbol=1;
	else
	{
		symbol=0;
		prev=0-prev;
	}
	char balance_cent[4], balance_dollar[20];
	char balance_before[30], balance_after[30];

	sprintf(balance_before, "%d", prev);
	int length= strlen(balance_before);
	int i;
	
	balance_cent[3]='\0';
	balance_cent[2]=balance_before[length-1];
	balance_cent[1]=balance_before[length-2];
	balance_cent[0]='.';

	for(i=0; i<length-2; i++)
	{
		balance_dollar[i]=balance_before[i];
	}
	balance_dollar[length-2]='\0';
	int balance_dollar_before=atoi(balance_dollar);
	int F1=balance_dollar_before/1000000;
	int S3=(balance_dollar_before-F1*1000000)/1000;
	int T3=balance_dollar_before-F1*1000000-S3*1000;

	if(F1>0)
	{sprintf(balance_dollar, "%d,%03d,%03d", F1, S3, T3);	}
	else if(F1==0&&S3>0)
	{sprintf(balance_dollar, "%d,%03d", S3, T3);}
	else if(F1==0&&S3==0)
	{sprintf(balance_dollar, "%d", T3);}
	
	strcpy(balance_after, strcat(balance_dollar, balance_cent));

	if(symbol==1)	
		printf("%14s  |\n", balance_after); 
	else if(symbol==0)
		{
       printf(" (%12s) |\n", balance_after);
       prev=0-prev;
    }
	return prev;
}


/****************Function Output: print out sorted elements in list********************************/
int Output(My402List *list)
{
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	printf("|       Date      | Description              |         Amount |        Balance |\n");
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	My402ListElem *elem=My402ListFirst(list);
	Transaction *Tran=elem->obj;
	char date[20];
	char amount[30];
	int balance=0;
	int i;
	for(i=0; i<My402ListLength(list)-1; i++)
	{
		//print date
		Date_Format(Tran->Time, date);
		printf("| %s |", date);

		//description		
		printf(" %-24s |", Tran->Descrpt); 

		//amount
		Amount_Format(Tran->Amount, amount, Tran->Type);

		//balance		
		balance=Balance(balance, Tran);
		
		elem=My402ListNext(list, elem);
		Tran=elem->obj; 
	}
	elem=My402ListLast(list);
	Tran=elem->obj;
	//print date
	Date_Format(Tran->Time, date);
	printf("| %s |", date);

	//description		
	printf(" %-24s |", Tran->Descrpt); 

	//amount
	Amount_Format(Tran->Amount, amount, Tran->Type);

	//balance		
	balance=Balance(balance, Tran);
	printf("+-----------------+--------------------------+----------------+----------------+\n");
	return TRUE;
} 

/****************Function Command(): read command line and return right *fp**************/
FILE *Command(int argc, char *argv[])
{
 	FILE *fp;
 
  if(argc<2||argc>3)
  {
   fprintf(stderr, "Command Error: Malformed command.\n");
				exit(1);
  }
	else if(argc==3)   //three fields
	{
   if(strcmp(argv[1], "sort")!=0)  //3 fields and second is not sort
   {
	      fprintf(stderr, "Command Error: Malformed command.\n");
				exit(1);
   }
   else                             //3 fields and second is sort 
   {
    char *filename=argv[2];
    
    struct stat info;
    stat(argv[2], &info);
    if(S_ISDIR(info.st_mode))
    {
      fprintf(stderr,"Command Error: file name cannot be a directory.\n");
      exit(1);
    }
    if(*argv[2]=='-')   ////////////not sure
    {
      fprintf(stderr, "Command Error: in file name field.\n");
				exit(1);
    }
		else
    {
     fp=fopen(filename, "r");
     if(fp==NULL)
     {
     perror("File Error");
     exit(1);
     }
////////////////////////////////////////////////////
    }
   }
  }
  else                    //2 fields
  {
   if(strcmp(argv[1], "sort")!=0)   //2 fields and second is not sort
    {
     fprintf(stderr, "Command Error: Malformed command.\n");
				exit(1);                  
    }
   else                          //2 fields and second is sort
   {
    fp=stdin;
    if(fp==NULL)
        {
            fprintf(stderr,"Input Error: no input.\n");
            exit(1);
        }
   }
  }
  return fp;
}

/*************************************main ()*****************************************************/
int main(int argc, char *argv[])
{
	if(My402ListInit(&list)==FALSE)
		fprintf(stderr, "Initial unsuccessful.\n");
   
   FILE *fp=Command(argc, argv);

  char buf[BUFLEN];
	if(ReadInput(fp, buf)==FALSE)
                   exit(1);	
 
	Output(&list);
	return TRUE;
}
