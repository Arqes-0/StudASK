#include <stdio.h>
#include <stdlib.h>

//global variables
char *line = NULL;
size_t bufferSize = 0;
ssize_t characters;
FILE* fileptr;

int getID();

void writeAsk(){
	int id =getID();
	printf("Write the ask:");
	characters = getline(&line, &bufferSize, stdin); //get ask line

	line[characters-1]=':'; //Erase \n character

	fileptr = fopen("Storage","a");
	if (NULL == fileptr) {printf ("Error opening the file :(");exit(0);}
	
	fprintf(fileptr, "%04d%s:!!\n",id+1,line); //Add ::!! to easier to parse


	fclose(fileptr);
}


void readAsk(){
	fileptr = fopen("Storage","r");
	if (NULL == fileptr) {printf ("Error opening the file :(");exit(0);}
	while (getline(&line,&bufferSize,fileptr)!=-1){

		printf("%s",line); //read all lines

	}
	fclose(fileptr);

}


int getID(){ //This function is to get the last id of the questions
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
	int coso = getID();
	printf("%d\n",coso);
	break;
	default: printf("Error in selection\n"); goto a;


}
	
return 0;
}
