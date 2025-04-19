// TODO: APLICAR LO DE ALATORIO DE PREGUNTAS CON EL INDICE DESORDENADO
// CORREGIR ESTETICA Y POR ULTIMO AÑADIR TOPICS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// to create subdirectories
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
// GENERATION RANDOM NUMBER
#include <time.h>
#define MAX_RESPUESTAS 4
// global variables
char *line = NULL;
size_t bufferSize = 0;
ssize_t characters;
FILE *fileptr;

int getLastID();

void createFolder(char name[255]) {
  char path[400];
  const char *home = getenv("HOME");
  if (home != NULL) {
    snprintf(path, sizeof(path), "%s/StudAsk/", home);
    DIR *dir = opendir(path);
    if (dir) {
      closedir(dir);
      printf("Topic alredy created\n");
    } else {

      if (mkdir(path, 0777) == 0) {
        printf("Folder created in: %s\n", path);
      }
    }
    strcat(path, name);

    if (mkdir(path, 0777) == 0) {
      printf("Folder created in: %s\n", path);

    } else
      printf("Error creating folder, %s", path);
  } else
    printf("Error Obtainining $HOME env variable");
}

char *concatString(char *s1, char *s2, char *s3, char *s4) {
  size_t totalLength = strlen(s1) + strlen(s2) + strlen(s3) + strlen(s4) + 5;
  char *text = "!::";
  char *result = malloc(sizeof(char) * totalLength);
  if (result == NULL) {
    printf("error asigning malloc result");
    exit(1);
  }
  strcpy(result, s1);
  strcat(result, text);
  strcat(result, s2);
  strcat(result, text);
  strcat(result, s3);
  strcat(result, text);
  strcat(result, s4);

  return result;
}

void writeAsk() {
  char *s1 = NULL, *s2 = NULL, *s3 = NULL, *s4 = NULL;
  size_t fbufferSize = 0;
  ssize_t c1, c2, c3, c4;
  int id = getLastID();
  printf("Write the ask:");
  characters = getline(&line, &bufferSize, stdin); // get ask line

  line[characters - 1] = ':'; // Erase \n character with :
  printf("Please enter the right answer: ");
  c1 = getline(&s1, &fbufferSize, stdin);
  printf("Please enter the seccond answer: ");
  c2 = getline(&s2, &fbufferSize, stdin);
  printf("Please enter the third answer: ");
  c3 = getline(&s3, &fbufferSize, stdin);
  printf("Please enter the fourth answer: ");
  c4 = getline(&s4, &fbufferSize, stdin);
  s1[c1 - 1] = '!';
  s2[c2 - 1] = '!';
  s3[c3 - 1] = '!';
  s4[c4 - 1] = '\0';
  char *result = concatString(s1, s2, s3, s4);
  // agregar ! a todas menos la 4 para sustituir \n
  free(s1);
  free(s2);
  free(s3);
  free(s4);

  fileptr = fopen("Storage", "a");
  if (NULL == fileptr) {
    printf("Error opening the file :(");
    exit(0);
  }

  fprintf(fileptr, "%04d::!!%s:!!%s\n", id + 1, line,
          result); // Add ::!! to easier to parse

  fclose(fileptr);
}

void shuffle(int arr[], int n) {
  for (int i = n - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}

void readAsk() {

  int end = getLastID();
  int index[end];

  for (int i = 0; i < end; i++) {
    index[i] = i + 1;
  }
  shuffle(index, end);
  fileptr = fopen("Storage", "r");
  if (NULL == fileptr) {
    printf("Error opening the file :(");
    exit(0);
  }
  printf("Starting random questions");
  sleep(2);
  for (int I = 0; I < end; I++) {
    int lineToRead = index[I];
    int lineRed = -1;
    /// MODIFICAR LO ANTERIOR PARA ALEATORIZAR LAS PREGUNTAS

    char ID[4] = {0};
    int status = 0;
    while (status != -1 && lineToRead != lineRed) {
      status = getline(&line, &bufferSize, fileptr);
      for (int i = 0; i < 4; i++) {
        ID[i] = line[i];
      }
      lineRed = atoi(ID);
    }

    // Extraer respuestas con strtok()
    strtok(line, "::!!");
    char *question = strtok(NULL, "::!!");
    printf("Question: %s\n", question);
    char *answers[MAX_RESPUESTAS];
    answers[0] = strtok(NULL, "!!::"); // Respuesta correcta
    answers[1] = strtok(NULL, "!!::");
    answers[2] = strtok(NULL, "!!::");
    answers[3] = strtok(NULL, "!!::");

    if (!answers[0] || !answers[1] || !answers[2] || !answers[3]) {
      printf("Error parsing answers.\n");
      free(line);
      fclose(fileptr);
      return;
    }

    // Mezclar respuestas
    int indices[MAX_RESPUESTAS] = {0, 1, 2, 3};
    srand(time(NULL));
    shuffle(indices, MAX_RESPUESTAS);

    // Imprimir respuestas en orden aleatorio
    printf("Seleccione la respuesta correcta:\n");
    for (int i = 0; i < MAX_RESPUESTAS; i++) {
      printf("%d) %s\n", i + 1, answers[indices[i]]);
    }

    // Obtener la respuesta del usuario
    int userChoice;
    printf("Ingrese el número de su respuesta: ");
    scanf("%d", &userChoice);

    // Validar la respuesta
    if (userChoice >= 1 && userChoice <= MAX_RESPUESTAS) {
      if (strcmp(answers[indices[userChoice - 1]], answers[0]) == 0) {
        printf("¡Correcto!\n");
      } else {
        printf("Incorrecto. La respuesta correcta era: %s\n", answers[0]);
      }
    } else {
      printf("Opción inválida.\n");
    }
  }
  fclose(fileptr);
  free(line);
}

void printDirectories() {
  char path[400];
  const char *home = getenv("HOME");
  if (home != NULL) {
    snprintf(path, sizeof(path), "%s/StudAsk/", home);
    DIR *d = opendir(path);
    struct dirent *dir;
    if (d) {
      while ((dir = readdir(d)) != NULL) {
        printf("%s\n", dir->d_name);
      }
      closedir(d);
    }
  }
}

int getLastID() { // This function is to get the last id of the questions
  fileptr = fopen("Storage", "r");
  while (getline(&line, &bufferSize, fileptr) != -1) {
  }
  fclose(fileptr);
  char ID[4] = {0};
  for (int i = 0; i < 4 && line[i] != '\0'; i++) {
    ID[i] = line[i];
  }
  int id = atoi(ID);
  return id;
}

int main() {

  /* inicialize of the code, checking for storage file */
  if ((fileptr = fopen("Storage", "r")) == NULL) {
    printf("Error opening main storage file\n");
    printf("Creating main storage file...\n");
    fileptr = fopen("Storage", "w");
    printf("Done!\n");
    fclose(fileptr);
  } else
    printf("Main storage file found\n");

a:
  printf("please select write or read from storage: \n");
  printf("Write(1)\tCreate new folder(2)\tRead(3)\n");

  // Main Menu Code
  int selector = 0;
  scanf("%d", &selector);
  getchar();
  switch (selector) {
  case 1:
    writeAsk();
    break;
  case 2:
    char folder[255];
    printf("Please enter the name of the folder: ");
    scanf("%s", &folder);
    createFolder(folder);
    break;
  case 3:
    readAsk();
    int coso = getLastID();
    printf("%d\n", coso);
    break;
  default:
    printf("Error in selection\n");
    goto a;
  }

  return 0;
}
