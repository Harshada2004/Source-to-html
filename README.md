# Source to HTML Converter Using C

## 📌 Overview

Source to HTML Converter is a C-based project that converts source code into an HTML document with syntax highlighting.

The project reads a source file, identifies different elements of the program such as **keywords, identifiers, constants, operators, strings, comments, and special symbols**, and generates an HTML file where these elements are displayed with different formatting.

This project demonstrates practical implementation of **file handling, string processing, lexical analysis, and HTML generation using C**.

## ✨ Features

* Converts source code into an HTML file
* Identifies different source-code elements
* Highlights keywords and other tokens
* Handles comments and string literals
* Preserves source-code formatting
* Generates a browser-readable HTML file
* Uses file handling for input and output

## 🛠️ Technologies Used

* **Language:** C
* **Concepts:** File Handling, Strings, Pointers, Structures
* **Compiler:** GCC
* **Output:** HTML
* **Platform:** Linux / Windows

## 🧠 Working Principle

The program reads the source file character by character and identifies different components of the source code.

### Source Code

```text
#include <stdio.h>

int main()
{
    printf("Hello World");
    return 0;
}
```

### Processing

```text
Source File
     ↓
Read Character by Character
     ↓
Identify Tokens
     ↓
Classify Source Elements
     ↓
Apply HTML Tags / Formatting
     ↓
Generate HTML File
```

### Generated HTML

The identified elements are wrapped with appropriate HTML formatting so that the source code can be displayed with syntax highlighting in a web browser.

## 🔍 Source Elements Identified

The converter processes elements such as:

* Keywords
* Identifiers
* Constants
* Operators
* Special symbols
* String literals
* Character constants
* Comments
* Preprocessor directives

## ⚙️ Working

### 1. Read Input File

The program opens the source-code file in read mode.

### 2. Analyze Source Code

The source code is processed character by character to identify different lexical elements.

### 3. Identify Tokens

The program determines whether the current characters represent a keyword, identifier, constant, operator, comment, string, or another source-code element.

### 4. Generate HTML

HTML tags are added around the identified elements to apply formatting.

### 5. Create Output File

The formatted content is written into an `.html` file.

## 📂 Project Structure

```text
Source-to-HTML/
│
├── main.c
├── source.c
├── source.h
├── html.c
├── html.h
├── keywords.c
├── keywords.h
└── README.md
```

> File names can be adjusted according to your actual repository structure.

## ▶️ Compilation

Compile the project using GCC:

```bash
gcc *.c -o source_to_html
```

## 🚀 Execution

```bash
./source_to_html
```

Enter the source file and output HTML file as required by the program.

## 💻 Example

### Input

```c
#include <stdio.h>

int main()
{
    int num = 10;

    printf("Number = %d", num);

    return 0;
}
```

### Output

The program generates an HTML file containing the formatted source code, which can be opened in any web browser.

```text
Source Code
     ↓
   Parser
     ↓
Token Identification
     ↓
HTML Formatting
     ↓
   output.html
```

## 🧠 Concepts Learned

* File handling in C
* Character and string processing
* Lexical analysis
* Token identification
* Structures and pointers
* Conditional processing
* HTML tags and formatting
* Modular programming
* Dynamic memory management

## 🎯 Learning Outcomes

* Learned how source code can be processed at the lexical level.
* Gained practical experience with file handling in C.
* Implemented token identification and classification.
* Learned how to generate HTML dynamically from C.
* Improved understanding of strings, pointers, and character processing.
* Strengthened debugging and problem-solving skills.

## ⚡ Advantages

* Simple and lightweight implementation
* Converts source code into a browser-readable format
* Improves source-code readability
* Demonstrates practical application of lexical analysis
* Implemented without relying on external syntax-highlighting libraries

## ⚠️ Limitations

* Syntax highlighting depends on the tokens supported by the implementation.
* Complex language constructs may require additional parsing logic.
* The current implementation is primarily designed around C source code.

## 🔮 Future Enhancements

* Support for multiple programming languages
* Improved syntax highlighting
* Line numbering
* Custom HTML/CSS themes
* Support for nested or complex language constructs
* Command-line arguments for input and output files
* Improved lexical analysis

## 👩‍💻 Author

Harshada Wani

### Project

**Source to HTML Converter Using C**

A C programming project demonstrating File Handling, Lexical Analysis, Token Identification, String Processing, and HTML Generation.
