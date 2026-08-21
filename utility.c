#include"utility.h"

const char *KEYWORDS[] = {
    "return", "if", "else", "while", "for", "do", "break", "continue",
    "switch", "case", "default", "goto", "sizeof", "typedef", "struct", "union",
    "enum", "const", "volatile", "extern", "static", "auto", "register", "restrict",
    NULL
};

const char *DATATYPES[] = {
    "int", "char", "float", "double", "void", "long", "short", "signed", "unsigned",
    NULL
};

void print_usage(const char *program_name) 
{
    printf("Usage: %s <input_c_file>\n", program_name);
    printf("Example: %s my_program.c\n", program_name);
    printf("Output will be written to my_program.c.html\n");
}

void source2html(char *input_file,char *output_file)
{
    FILE *inptr = fopen(input_file,"r");
    if(!inptr)
    {
        perror("Error opening input file");
        return;
    }
    FILE *outptr = fopen(output_file,"w");
    if(!outptr)
    {
        perror("Error opening output file");
        return;
    }

    html_begin(outptr,input_file);

    ParserState state = STATE_IDLE;
    char c;
    char token[MAX_TOKEN_LEN];
    int token_idx = 0;
    #define RESET_TOKEN() do{token_idx=0;memset(token,0,MAX_TOKEN_LEN);}while(0)
    #define APPEND_CHAR(ch)do{if(token_idx<MAX_TOKEN_LEN-1)token[token_idx++]=(ch);}while(0)

    RESET_TOKEN();
    while((c =fgetc(inptr))!=EOF)
    {
        if(state == STATE_MULTILINE_COMMENT)
        {
            APPEND_CHAR(c);
            if(c=='*')
            {
                char nxt_c =fgetc(inptr);
                if(nxt_c =='/')
                {
                    APPEND_CHAR(nxt_c);
                    write_token(outptr,token,TOKEN_TYPE_COMMENT);
                    state = STATE_IDLE;
                    RESET_TOKEN();
                }
                else if(nxt_c !=EOF)
                {
                    ungetc(nxt_c,inptr);
                }
            }
            else if(c=='\n')
            {
                fprintf(outptr,"\n");
            }
        }

        else if(state == STATE_SINGLELINE_COMMENT)
        {
            if(c=='\n')
            {
                write_token(outptr,token,TOKEN_TYPE_COMMENT);
                fprintf(outptr,"\n");
                state=STATE_IDLE;
                RESET_TOKEN();
            }
            else
            {
                APPEND_CHAR(c);
            }
        }

        else if(state == STATE_PREPROCESSOR)
        {
            APPEND_CHAR(c);
            if(c=='\n')
            {
                write_token(outptr,token,TOKEN_TYPE_PREPROCESSOR);
                fprintf(outptr,"\n");
                state =STATE_IDLE;
                RESET_TOKEN();
            }
        }
        else if(state == STATE_STRING_LITERAL)
        {
            APPEND_CHAR(c);
            if(c=='\\')// handle escape character 
            {
                char nxt_c =fgetc(inptr);
                if(nxt_c !=EOF)
                {
                    APPEND_CHAR(nxt_c);
                }
            }

            else if(c=='"')
            {
                write_token(outptr,token,TOKEN_TYPE_STRING);
                state = STATE_IDLE;
                RESET_TOKEN();
            }
            

            else if(c=='\n')
            {
                printf("Warning unterminated string literal\n");
                write_token(outptr,token,TOKEN_TYPE_STRING);
                state = STATE_IDLE;
                RESET_TOKEN();
            }
        }

        else if(state == STATE_CHAR_LITERAL)
        {
            APPEND_CHAR(c);
            if(c=='\\')
            {
                char nxt_c=fgetc(inptr);
                if(nxt_c !=EOF)
                {
                    APPEND_CHAR(nxt_c);//append because \n,\t etc are string litterals
                }
            }
            else if(c=='\'')
            {
                write_token(outptr,token,TOKEN_TYPE_STRING);
                state = STATE_IDLE;
                RESET_TOKEN();
            }
            else if(c=='\n')
            {
                printf("Warning: unterminated char literal");
                write_token(outptr,token,TOKEN_TYPE_STRING);
                fprintf(outptr,"\n");
                state = STATE_IDLE;
                RESET_TOKEN();
            }
        }

        else 
        {
            if(c=='/')
            {
                char nxt_c = fgetc(inptr);
                if(nxt_c =='*')
                {
                    if(token_idx>0)//flushing pending not required
                    {
                        write_token(outptr,token,get_token_type(token));
                        RESET_TOKEN();
                    }
                    APPEND_CHAR(c);
                    APPEND_CHAR(nxt_c);
                    state = STATE_MULTILINE_COMMENT;
                    continue;
                }

                else if(nxt_c =='/')
                {
                    if(token_idx>0)//flushing pending
                    {
                        write_token(outptr,token,get_token_type(token));
                        RESET_TOKEN();
                    }
                    APPEND_CHAR(c);
                    APPEND_CHAR(nxt_c);
                    state = STATE_SINGLELINE_COMMENT;
                    continue;
                }
                else
                {
                    if(nxt_c !=EOF)
                    {
                        ungetc(nxt_c,inptr);
                    }

                    if (token_idx > 0) 
                    {
                        write_token(outptr, token, get_token_type(token));
                        RESET_TOKEN();
                    }
                    write_token(outptr,"/",TOKEN_TYPE_DEFAULT);
                    RESET_TOKEN();
                    continue;
                }
            }

            if (c=='#')
            {
                if (token_idx > 0) 
                {
                    write_token(outptr, token, get_token_type(token));
                    RESET_TOKEN();
                }
                APPEND_CHAR(c);
                state = STATE_PREPROCESSOR;
                continue;
            }

            if(c=='"')
            {
                if (token_idx > 0) 
                {
                    write_token(outptr, token, get_token_type(token));
                    RESET_TOKEN();
                }
                APPEND_CHAR(c);
                state = STATE_STRING_LITERAL;
                continue;
            }

            if(c=='\'')
            {
                if (token_idx > 0) 
                {
                    write_token(outptr, token, get_token_type(token));
                    RESET_TOKEN();
                }
                APPEND_CHAR(c);
                state = STATE_CHAR_LITERAL;
                continue;
            }

            if(c==' ' || c=='\t')
            {
                if (token_idx > 0) 
                {
                    write_token(outptr, token, get_token_type(token));
                    RESET_TOKEN();
                }
                fprintf(outptr,"%c",c==' '? ' ':'\t');
                continue;
            }

            if(c=='\n')
            {
                if (token_idx > 0) 
                {
                    write_token(outptr, token, get_token_type(token));
                    RESET_TOKEN();
                }
                fprintf(outptr,"\n");
                continue;
            }

            if(!isalnum(c) && c!= '_')
            {
                if (token_idx > 0) 
                {
                    write_token(outptr, token, get_token_type(token));
                    RESET_TOKEN();
                }
                fprintf(outptr,"<span class=\"operator\">%c</span>",c);
                continue;
            }

            APPEND_CHAR(c);

        }
    }

    if(token_idx>0)
    {
        if(state == STATE_IDLE)
        {
            write_token(outptr,token,get_token_type(token));
        }
        else
        {
            //printf("Warning: Unfinished block at end of file\n");
            TokenType type = TOKEN_TYPE_DEFAULT;
           if(state == STATE_MULTILINE_COMMENT || state == STATE_SINGLELINE_COMMENT)
           {
                type = TOKEN_TYPE_COMMENT;
           }
           else if(state == STATE_PREPROCESSOR)
           {
                type = TOKEN_TYPE_PREPROCESSOR;
           }
           else if (state == STATE_STRING_LITERAL || state == STATE_CHAR_LITERAL) 
            {
                type = TOKEN_TYPE_STRING;
            }
            write_token(outptr,token,type);
        }
    }
    html_end(outptr);
    fclose(inptr);
    fclose(outptr);
    printf("Successfully converted '%s' to '%s'\n", input_file, output_file);
}  

void html_end(FILE *outptr)
{
    fprintf(outptr, "    </code></pre>\n");
    fprintf(outptr, "</body>\n");
    fprintf(outptr, "</html>\n");
}
TokenType get_token_type(char *token)
{
    for(int i =0;KEYWORDS[i]!=NULL;i++)
    {
        if(strcmp(token,KEYWORDS[i])== 0)
        {
            return TOKEN_TYPE_KEYWORD;
        }
    }
    for(int i =0;DATATYPES[i]!=NULL;i++)
    {
        if(strcmp(token,DATATYPES[i])==0)
        {
            return TOKEN_TYPE_DATATYPE;
        }
    }
    return TOKEN_TYPE_DEFAULT;
}

void write_token(FILE *outptr,char *token ,TokenType type)
{
    char *class_name ="";
        switch (type) 
        {

            case TOKEN_TYPE_KEYWORD: 
            class_name = "keyword"; 
            break;

            case TOKEN_TYPE_DATATYPE: 
            class_name = "datatype"; 
            break;

            case TOKEN_TYPE_COMMENT: 
            class_name = "comment";
            break;

            case TOKEN_TYPE_PREPROCESSOR: 
            class_name = "preprocessor"; 
            break;

            case TOKEN_TYPE_STRING: 
            class_name = "string"; 
            break;

            case TOKEN_TYPE_DEFAULT: 
            class_name = ""; 
            break; 
        }
        if(class_name[0]!='\0')
        {
            fprintf(outptr,"<span class = \"%s\">",class_name);
        }

        for(int i =0;i<strlen(token);i++)
        {
            if(token[i]=='<')
            fprintf(outptr,"&lt;");
            
            else if(token[i]=='>')
            fprintf(outptr,"&gt;");
            
            else if(token[i]=='&')
            fprintf(outptr,"&amp;");

            else if(token[i]==' ')
            fprintf(outptr," ");

            else if(token[i]=='\t')
            fprintf(outptr,"    ");

            else
            putc(token[i],outptr);
        }
        if(class_name[0] !='\0')
        {
            fprintf(outptr,"</span>");
        }
}
void html_begin(FILE *outptr,char *title)
{
    fprintf(outptr, "<!DOCTYPE html>\n");
    fprintf(outptr, "<html lang=\"en\">\n");
    fprintf(outptr, "<head>\n");
    fprintf(outptr, "    <meta charset=\"UTF-8\">\n");
    fprintf(outptr, "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n");
    fprintf(outptr, "    <title>Syntax Highlighted: %s</title>\n", title);
    fprintf(outptr, "    <style>\n");
    fprintf(outptr, "        body { background-color: #282c34; color: #abb2bf; font-family: 'Consolas', 'Monaco', monospace; font-size: 14px; margin: 20px; }\n");
    fprintf(outptr, "        pre { background-color: #1e2127; padding: 15px; border-radius: 8px; overflow-x: auto; line-height: 1.5; }\n");

    fprintf(outptr, "        .keyword { color: #c678dd; font-weight: bold; }\n");
    fprintf(outptr, "        .datatype { color: #56b6c2; }\n");       
    fprintf(outptr, "        .comment { color: #7f848e; font-style: italic; }\n");   
    fprintf(outptr, "        .preprocessor { color: #98c379; }\n");
    fprintf(outptr, "        .string { color: #e5c07b; }\n");
    fprintf(outptr, "        .identifier { color: #e06c75; }\n");
    fprintf(outptr, "        .operator { color: #61afef; }\n");
    fprintf(outptr, "    </style>\n");
    fprintf(outptr, "</head>\n");
    fprintf(outptr, "<body>\n");
    fprintf(outptr, "    <h1 style=\"text-align:center;\">%s</h1>\n", title);
    fprintf(outptr, "    <p style=\"color:#ffd700; font-size:16px; text-align:center; margin-top:5px;\">Developed by <b>Adhish V</b></p>\n");
    fprintf(outptr, "    <pre><code>\n");
}