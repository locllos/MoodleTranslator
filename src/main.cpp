#include "Translator/Translator.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

size_t getHash(char* value, size_t module)
{
    return value[0] % module;
}

size_t OH_YEAH_MA_SENPAI_GO_ROR_ME_(size_t rorable_number, size_t amount_rors)
{
    return (rorable_number >> amount_rors) | (rorable_number << (sizeof(size_t) * 8 - amount_rors));
}

size_t getConstantHash(char* value, size_t module)
{
    return 21;
}

size_t getLengthDependenceHash(char* value, size_t module)
{
    return strlen(value) % module;
}

size_t getFirstSymbolDependenceHash(char* value, size_t module)
{   
    return value[0] % module;
}

size_t getSumASCIICodeDependenceHash(char* value, size_t module)
{   
    if (*value == '\0') return 0;

    return ((*value) + getSumASCIICodeDependenceHash(value + 1, module)) % module;
}

size_t getNormalButNotTheBestHash(char* value, size_t module)
{
    size_t hash = 0;
    for (size_t pos = 0; value[pos] != '\0'; ++pos)
    {
        hash = OH_YEAH_MA_SENPAI_GO_ROR_ME_(hash, 1) ^ value[pos];
    }
    return hash % module;
}

size_t fnv64(char *p, size_t module) {
    size_t hash = 0;
    const size_t fnv_prime = 1099511628211;
    for (size_t i = 0; p[i] != 0; i++) {
        hash *= fnv_prime;
        hash ^= (p[i]);
    }
    return hash % module;
}

size_t HolyPoly_(char* value, size_t module)
{
    size_t prime_number = 7988587;
    size_t hash = *value;
    size_t accumulator = 1;

    for (value; *value != '\0'; ++value)
    {   
        hash += (*value) * accumulator;

        accumulator *= prime_number;
    }
    return hash % module;
}

void writeBucketSizes(Dictionary* dict, FILE* file)
{
    for (size_t i = 0; i < dict->table->size; ++i)
    {
        fprintf(file, "%zu,", dict->table->buckets[i].size);
    }
    fprintf(file, "\n");
}

int main()
{   
    Test("res/text.txt");

    // translateFile("res/text.txt", "res/translated_file.html");

    // interfaceDictionary();

    return 0;
}

























