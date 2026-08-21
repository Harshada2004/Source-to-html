#ifndef UITILTY_H
#define UTILITY_H
#define MAX_TOKEN_LEN 256
#define MAX_LINE_LEN 1024
#include<stdio.h>
#include<string.h>
#include<ctype.h>
typedef enum
{
    STATE_IDLE =0,
    STATE_MULTILINE_COMMENT,
    STATE_SINGLELINE_COMMENT,
    STATE_PREPROCESSOR,
    STATE_STRING_LITERAL,
    STATE_CHAR_LITERAL
}ParserState;

typedef enum {
    TOKEN_TYPE_DEFAULT = 0,
    TOKEN_TYPE_KEYWORD,
    TOKEN_TYPE_DATATYPE,
    TOKEN_TYPE_COMMENT,
    TOKEN_TYPE_PREPROCESSOR,
    TOKEN_TYPE_STRING
} TokenType;


void print_usage(const char *program_name);
void source2html(char *input_file,char *output_file);
void html_begin(FILE *outptr,char *title);
void write_token(FILE *outptr,char *token ,TokenType type);
TokenType get_token_type(char *token);
void html_end(FILE *outptr);
#endif