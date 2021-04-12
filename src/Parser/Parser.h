#include <stdio.h>
#include <cctype>
#include <sys/stat.h>

enum TYPE
{
    DOT,
    TAB,
    SPACE,
    COMMA,
    COLON,
    NEW_LINE,
    QST_MARK,
    EXCL_MARK,
    SEMICOLON,

    WORD,
};

struct Token
{
    TYPE type;
    char* lexeme;
};

struct TokenArray
{
    Token* data;

    size_t size;
    size_t capacity;

    char* buffer;
};

void        toLower             (char* string);
void        toLower_asm         (char* string);
void        constructTokenArray (TokenArray* tokens);
void        constructTokenArray (TokenArray* tokens, const char* filename);
void        destructTokenArray  (TokenArray* tokens);
void        pushBackTokenArray  (TokenArray* tokens, Token token);

int         approxLength        (const char* filename);

char*       readFile            (const char* filename, size_t* buffer_size = nullptr);
