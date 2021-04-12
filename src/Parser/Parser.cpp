#include "Parser.h"
#include <string.h>
#include "../Array/Array.h"

const char* punctuaction_marks = ".\t ,:\n?!;";

TYPE getPunctuactionMark(char* symbol)
{   
    const char* found_symbol = strchr(punctuaction_marks, *symbol);
    TYPE result = (found_symbol == nullptr) ?  WORD : (TYPE)(found_symbol - punctuaction_marks);

    return result;
}

void toLower(char* string)
{   
    while (*string != '\0')
    {
        *string = tolower(*string);
        ++string;
    }
}

void toLower_asm(char* string)
{
    __asm__
    (
        "mov    %rdi,%rbx\n"
        "movzbl (%rbx),%edi\n"
        "test   %dil,%dil\n"
        "je     .End\n"
    ".Continue:\n"
        "call   tolower\n"
        "movb   %al,(%rbx)\n"
        "inc    %rbx\n"
        "movzbl (%rbx),%edi\n"
        "test   %dil,%dil\n"
        "jne    .Continue\n"
    ".End:\n"
    );
}

int approxLength(const char* filename)
{	
	struct stat* buff = (struct stat*)calloc(1, sizeof(struct stat));

	stat(filename, buff);

    int size = buff->st_size;

    free(buff);

	return size;
}

char* readFile(const char* filename, size_t* buffer_size)
{
    FILE* file = fopen(filename, "r");

    if (file == nullptr) return nullptr;
    
    size_t apr_size = approxLength(filename);
    char* buffer = (char*)calloc(apr_size, sizeof(char));
    
    apr_size =  fread(buffer, sizeof(char), apr_size, file);

    if (buffer_size != nullptr) *buffer_size = apr_size;

    if (buffer != nullptr) buffer[apr_size - 1] = '\0';

    fclose(file);

    return buffer;
}

void constructTokenArray(TokenArray* tokens)
{
    tokens->size = 0;
    tokens->capacity = 0;
    tokens->data = nullptr;
    tokens->buffer = nullptr;
}

void constructTokenArray(TokenArray* tokens, const char* filename)
{
    constructTokenArray(tokens);

    tokens->buffer = readFile(filename);
    
    char* current_symbol = tokens->buffer;
    char* punct_mark = nullptr;
    while (current_symbol != nullptr && *current_symbol != '\0') 
    {   
        if (strchr(punctuaction_marks, *current_symbol) == nullptr)
        {
            pushBackTokenArray(tokens, {WORD, current_symbol});
        }
        current_symbol = strpbrk(current_symbol, punctuaction_marks);
        
        if (current_symbol != nullptr)
        {
            pushBackTokenArray(tokens, {getPunctuactionMark(current_symbol), nullptr});
            *current_symbol = '\0';
            ++current_symbol;
        }
    }
}

void pushBackTokenArray(TokenArray* tokens, Token token)
{
    if (tokens->size + 1 > tokens->capacity)
    {
        tokens->capacity = (tokens->capacity + 1) * 2;
        tokens->data = (Token*)realloc(tokens->data, tokens->capacity * sizeof(Token));
    }
    tokens->data[tokens->size++] = token;
}

void destructTokenArray(TokenArray* tokens)
{
    free(tokens->buffer);
    free(tokens->data);
}
