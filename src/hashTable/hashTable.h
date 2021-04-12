#include <string.h>
#include "../Array/Array.h"

struct HashTable
{
    Array* buckets;
    size_t size;
    size_t amount_elements;

    size_t (* getHash)(char* key, size_t module);
};

__m256i     getFastKey          (char* key);

void        constructHashTable  (HashTable* table, size_t size, size_t (* getHash)(char* value, size_t module));
void        destructHashTable   (HashTable* table);
void        reallocHashTable    (HashTable* table);
void        pushHashTable       (HashTable* table, elem_t value);

elem_t*     findHashTable       (HashTable* table, elem_t* value);

HashTable*  newHashTable        (size_t size, size_t (* getHash)(char* value, size_t module));
HashTable*  deleteHashTable     (HashTable* table);


































