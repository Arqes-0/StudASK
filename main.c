#include <stdio.h>
#include <stdlib.h>

//global variables
char *line = NULL;
size_t bufferSize = 0;
ssize_t characters;
FILE* fileptr;

void writeAsk(){
	printf("Write the ask:");
	characters = getline(&line, &bufferSize, stdin);

	line[characters-1]=':'; //Erase \n character

	fileptr = fopen("Storage","a");
	if (NULL == fileptr) {printf ("Error opening the file :(");exit(0);}
	fprintf(fileptr, "%s:!!\n",line); //Add ::!! to easier to parse


	fclose(fileptr);
}


void readAsk(){
	fileptr = fopen("Storage","r");
	if (NULL == fileptr) {printf ("Error opening the file :(");exit(0);}
	while (getline(&line,&bufferSize,fileptr)!=-1){

		printf("%s",line);

	}
	printf("test2\n"); 
	fclose(fileptr);

}


int getID(){
	printf("test1");
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
int selector = 0;
scanf("%d",&selector);
getchar();
switch (selector){
	case 1:
	writeAsk();
	break;
	case 2:
	readAsk();
	printf("test3");
	int coso = getID();
	printf("%d\n",coso);
	break;
	default: printf("Error in selection\n"); goto a;


}
	
return 0;
}
