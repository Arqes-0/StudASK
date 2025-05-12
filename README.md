README.md

# StudAsk

StudAsk is a command-line tool for creating and managing topic-based multiple-choice questions. It allows users to create topics, store questions, and quiz themselves on various subjects.

## Features

- Create custom topics
- Store multiple-choice questions
- Shuffle and randomize answers
- Review previous questions

## Installation

Clone the repository (or download from releases the .deb file):
```bash
git clone https://github.com/arqes-0/StudAsk.git
cd StudAsk
```
Compile the program:
```bash
gcc StudAsk.c -o StudAsk
```
## Usage

Run the program:
```bash
./StudAsk
```
### Main Menu Options:

1. Ask about a topic - Answer random questions from a selected topic.
2. Create a new topic - Add a new folder to store questions.
3. Add questions - Input multiple-choice questions into an existing topic.
4. Exit - Close the program.

### Example:
```bash
./StudAsk
```
Follow the instructions on the screen to create topics and store questions.

## Dependencies

This program uses standard C libraries, including:

- stdio.h
- stdlib.h
- string.h
- unistd.h
- dirent.h
- sys/stat.h
- sys/types.h
- time.h

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Contributing

Feel free to submit issues or pull requests to improve StudAsk!
