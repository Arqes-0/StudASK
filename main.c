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
char *home;
int getLastID();
void welcome() {
  printf("  _____   _                 _               _____   _  __\n");
  printf(" / ____| | |               | |     /\\      / ____| | |/ /\n");
  printf("| (___   | |_   _   _    __| |    /  \\    | (___   | ' / \n");
  printf(" \\___ \\  | __| | | | |  / _ |   / /\\ \\    \\___ \\  |  <  \n");
  printf(" ____) | | |_  | |_| | | (_| |  / ____ \\   ____) | | . \\ \n");
  printf("|_____/   \\__|  \\__,_|  \\__,_| /_/    \\_\\ |_____/  |_|\\_\\ \n");
  printf("**********************************************************"
         "\n\n\n\n");
}
void createFolder(char name[255]) {
  char path[400];
  if (home != NULL) {
    snprintf(path, sizeof(path), "%s/StudAsk/", home);
    DIR *dir = opendir(path);
    if (!dir) {

      printf("Main StudAsk Folder, not FOUND!\n creating...\n");
      if (mkdir(path, 0777) == 0) {
        printf("Folder created in: %s\n", path);
      }
    }
    closedir(dir);
    strcat(path, name);

    if (mkdir(path, 0777) == 0) {
      printf("Folder created in: %s\n", path);

    } else
      printf("Error creating folder, %s", path);
  } else
    printf("Error Obtainining $HOME env variable");
  // maybe add option to get manually StudAsk folder
  strcat(path, "/Storage");
  fileptr = fopen(path, "a");
  fclose(fileptr);
}

char *concatString(char *s1, char *s2, char *s3, char *s4) {
  size_t totalLength = strlen(s1) + strlen(s2) + strlen(s3) + strlen(s4) + 5;
  char *text = "!::";
  char *result = malloc(sizeof(char) * totalLength);
  if (result == NULL) {
    printf("error asigning malloc result ConcatString");
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

void writeAsk(char *topicWA) {
  char path[500];
  snprintf(path, sizeof(path), "%s/StudAsk/", home);
  strcat(path, topicWA);
  strcat(path, "/Storage");
another:
  sleep(0);
  char *s1 = NULL, *s2 = NULL, *s3 = NULL, *s4 = NULL;
  size_t fbufferSize = 0;
  ssize_t c1, c2, c3, c4;
  int id = getLastID(path);
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
  // add ! to substitute \n or \0
  free(s1);
  free(s2);
  free(s3);
  free(s4);

  fileptr = fopen(path, "a");
  if (NULL == fileptr) {
    printf("Error opening the file :(");
    exit(0);
  }

  fprintf(fileptr, "%04d::!!%s:!!%s\n", id + 1, line,
          result); // Add ::!! to easier to parse

  fclose(fileptr);
  free(result);
  char AnotherQuestion;
  printf("Desea añadir otra pregunta (N/Y):");
  scanf(" %c", &AnotherQuestion);
  getchar();
  if (AnotherQuestion == 'y' || AnotherQuestion == 'Y') {
    goto another;
  }
}

void shuffle(int arr[], int n) {
  srand(time(NULL));
  for (int i = n - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
  }
}
void cleanString(char *str) {
  size_t len = strlen(str);
  // Si el último carácter es un salto de línea o espacio, lo eliminamos
  if (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
    str[len - 1] = '\0';
  }
}

void readAsk(char *topicRA) {
  int counter = 0;
  char path[500];
  snprintf(path, sizeof(path), "%s/StudAsk/", home);
  strcat(path, topicRA);
  strcat(path, "/Storage");
  int end = getLastID(path);
  int index[end];
  for (int i = 0; i < end; i++) {
    index[i] = i + 1;
  }
  shuffle(index, end);
  fileptr = fopen(path, "r");
  if (NULL == fileptr) {
    printf("Error opening the file :(");
    sleep(2);
    exit(0);
  }
  for (int I = 0; I < end; I++) {
    rewind(fileptr);
    int lineToRead = index[I];
    int lineRed = -1;
    char ID[4] = {0};
    int status = 0;
    while (status != -1 && lineToRead != lineRed) {
      status = getline(&line, &bufferSize, fileptr);
      for (int i = 0; i < 4; i++) {
        ID[i] = line[i];
      }
      lineRed = atoi(ID);
    }

    // Extract answers with strtok()
    strtok(line, "::!!");
    char *question = strtok(NULL, "::!!");
    printf("*Q: %s\n\n", question);
    char *answers[MAX_RESPUESTAS];
    answers[0] = strtok(NULL, "!!::"); // Right correcta
    answers[1] = strtok(NULL, "!!::");
    answers[2] = strtok(NULL, "!!::");
    answers[3] = strtok(NULL, "!!::");
    answers[3][strlen(answers[3]) - 1] = '\0';

    if (!answers[0] || !answers[1] || !answers[2] || !answers[3]) {
      printf("Error parsing answers.\n");
      free(line);
      fclose(fileptr);
      return;
    }

    // shuffle answers
    int indices[MAX_RESPUESTAS] = {0, 1, 2, 3};

    shuffle(indices, MAX_RESPUESTAS);

    // print answers in random positions
    for (int i = 0; i < MAX_RESPUESTAS; i++) {
      printf("-(%d): %s\n\n", i + 1, answers[indices[i]]);
    }

    // Get users choice
  Ganswer:
    int userChoice;
    printf("Your Choice: ");
    scanf("%d", &userChoice);
    // Validar la respuesta
    if (userChoice >= 1 && userChoice <= MAX_RESPUESTAS) {
      if (strcmp(answers[indices[userChoice - 1]], answers[0]) == 0) {
        printf("¡YOU ARE RIGHT!\n");
      } else {
        printf("Wrong answer. The right answer is: %s\n", answers[0]);
      }
    } else {
      printf("Invalid option, please try again\n");
      getchar();
      goto Ganswer;
    }
    counter++;
    if (counter != end) {
      getchar();
      printf("Press any key to next question!...");
      getchar();
      printf("\n\n");
    }
  }
  fclose(fileptr);
  free(line);
  getchar();
  printf("END OF QUESTIONS!\nPress any key to continue!");
  getchar();
}

void printDirectories() {
  char path[400];
  home = getenv("HOME");
  if (home != NULL) {
    snprintf(path, sizeof(path), "%s/StudAsk/", home);
    DIR *d = opendir(path);
    struct dirent *dir;
    if (d) {
      while ((dir = readdir(d)) != NULL) {
        // to dont print "." and ".."
        if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
          printf("---%s\n\n", dir->d_name);
        }
      }
      closedir(d);
    }
  }
}

int getLastID(char *path) { // This function is to get the last id of the
                            // questions maybe better with getchar
  fileptr = fopen(path, "r");
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
void AskTopic() {
  char topicVar[256];
  printf("Plase select one of the following topics:\n\n");
  printDirectories();
  printf("Your selection: ");
  scanf("%s", topicVar);
  getchar();
  system("clear");
  printf("Starting random %s Questions!!!\n", topicVar);
  sleep(1);
  system("clear");
  readAsk(topicVar);
}
void WriteTopic() {
  char topicVar[256];
  printf("Please select one of the following topics:\n");
  printDirectories();
  printf("Your selection: ");
  scanf("%s", topicVar);
  getchar();
  writeAsk(topicVar);
}
int main() {
  home = getenv("HOME");
  if (home == NULL) {
    printf("Error Obtainining $HOME ENV Variable");
  }

  sleep(1);
a:
  system("clear");
  welcome();
  printf("Please Select one of the following options: \n\n");
  printf(
      "Ask about a topic(1)\tCreate new Topic(2)\tAdd Questions(3)\tEXIT(0)\n");
  printf("Your choice: ");
  // Main Menu Code
  int selector = -1;
  scanf("%d", &selector);
  getchar();
  switch (selector) {
  case 0:
    system("clear");
    break;
  case 1:
    system("clear");
    AskTopic();
    goto a;
    break;
  case 2:
    system("clear");
    char folder[255];
    printf("Please enter the name of the folder: ");
    scanf("%s", folder);
    createFolder(folder);
    goto a;
    break;
  case 3:
    system("clear");
    WriteTopic();
    sleep(3);
    goto a;
    break;
  default:
    printf("Error in selection\nPress any key to continuePress any key to "
           "continue!!!");

    getchar();
    goto a;
  }
  free(line);
  return 0;
}
