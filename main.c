#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//global variables
char *line = NULL;
size_t bufferSize = 0;
ssize_t characters;
FILE* fileptr;

int getLastID();

char* concatString(char *s1, char *s2, char *s3, char *s4){
	size_t totalLength = strlen(s1) + strlen (s2) + strlen (s3) + strlen (s4) + 5;
	char *text="!::";
	char *result = malloc(sizeof(char)*totalLength);
	if (result == NULL){printf("error asigning malloc result");exit(1);}
	strcpy(result,s1);
	strcat(result,text);
	strcat(result,s2);
	strcat(result,text);
	strcat(result,s3);
	strcat(result,text);
	strcat(result,s4);
	
	return result;
}


void writeAsk(){
	char *s1=NULL,*s2=NULL,*s3=NULL, *s4=NULL;
	size_t fbufferSize = 0;
	ssize_t c1,c2,c3,c4;
	int id =getLastID();
	printf("Write the ask:");
	characters = getline(&line, &bufferSize, stdin); //get ask line

	line[characters-1]=':'; //Erase \n character with :
	printf("Please enter the right answer: ");
	c1=getline(&s1,&fbufferSize,stdin);
	 printf("Please enter the seccond answer: ");
        c2=getline(&s2,&fbufferSize,stdin);
 	printf("Please enter the third answer: ");
        c3=getline(&s3,&fbufferSize,stdin);
	 printf("Please enter the fourth answer: ");
        c4=getline(&s4,&fbufferSize,stdin);
	s1[c1-1]='!';
	s2[c2-1]='!';
	s3[c3-1]='!';
	s4[c4-1]='\0';
	char *result = concatString(s1,s2,s3,s4);
	//agregar ! a todas menos la 4 para sustituir \n
	free(s1);
	free(s2);
	free(s3);
	free(s4);	

	fileptr = fopen("Storage","a");
	if (NULL == fileptr) {printf ("Error opening the file :(");exit(0);}
	
	


	fprintf(fileptr, "%04d%s:!!%s\n",id+1,line,result); //Add ::!! to easier to parse


	fclose(fileptr);
}


void readAsk(){
	fileptr = fopen("Storage","r");
	if (NULL == fileptr) {printf ("Error opening the file :(");exit(0);}
	printf("What line do you want to read? ");
	int lineToRead=0;
	int lineRed=-1;
	scanf("%d",&lineToRead);
	getchar();
	char ID[4]={0};
	int status=0;
	while (status!=-1&&lineToRead!=lineRed){
		status = getline(&line,&bufferSize,fileptr);
		for (int i=0;i<4;i++){
				ID[i]=line[i];
				}
		lineRed= atoi(ID);
		}
	printf("%s" ,line);
	fclose(fileptr);
	free(line);

}


int getLastID(){ //This function is to get the last id of the questions
	fileptr = fopen("Storage","r");
	while (getline(&line,&bufferSize,fileptr)!=-1){}
	fclose(fileptr);
	char ID[4]={0};
	for (int i = 0; i<4&& line[i]!='\0'; i++){ID[i]=line[i];}
	int id = atoi(ID);
	return id;
	}



int main(){

/* inicialize of the code, checking for storage file */
if ((fileptr = fopen("Storage","r"))== NULL){
	printf("Error opening main storage file\n");
	printf("Creating main storage file...\n");
	fileptr = fopen("Storage","w");
	printf("Done!\n");
	fclose(fileptr);
}
else printf("Main storage file found\n");

a:
printf("please select write or read from storage: \n");
printf("Write(1)\tRead(2)\n");

//Main Menu Code
int selector = 0;
scanf("%d",&selector);
getchar();
switch (selector){
	case 1:
	writeAsk();
	break;
	case 2:
	readAsk();
	int coso = getLastID();
	printf("%d\n",coso);
	break;
	default: printf("Error in selection\n"); goto a;


}
	
return 0;
}
