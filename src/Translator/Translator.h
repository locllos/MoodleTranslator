#include "../hashTable/hashTable.h"
#include <stdio.h>

enum INPUT
{
    GET  = '?',
    PUSH = '+',
    EXIT = '-'
};

struct Dictionary
{
    HashTable* table;
    char* buffer;
};


int         approxLength        (const char* filename);
char*       readFile            (const char* filename, size_t* buffer_size);

void        Test                (const char* filename);
void        interfaceDictionary ();

void        constructDictionary (Dictionary* dict, char* buffer);
void        constructDictionary (Dictionary* dict, const char* filename);
void        constructDictionary (Dictionary* dict, size_t (* getHash)(char* value, size_t module));
void        constructDictionary (Dictionary* dict, const char* filename, size_t (* getHash)(char* value, size_t module));
void        constructDictionary (Dictionary* dict, char* buffer, size_t (* getHash)(char* value, size_t module));
void        pushDictionary      (Dictionary* dict, char* key, char* word);
void        translateSentence   (Dictionary* dict, char* sentence, FILE* output);
void        translateFile       (const char* filename, const char* translated_file);
void        destructDictionary  (Dictionary* dict);

Dictionary* newDictionary       (char* buffer);

char*       getTranslation      (Dictionary* dict,  char* english_word);