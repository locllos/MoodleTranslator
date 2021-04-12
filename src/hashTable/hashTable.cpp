#include "hashTable.h"
#include "../Tool/Tools.h"

#define dbg 

#ifndef dbg
#define dbg printf("(func: %s | line: %d)\n", __func__, __LINE__)
#endif

const size_t MAX_BUCKET_SIZE =  15;
const size_t START_CAPACITY = 8;


inline bool compareElements(elem_t* value_a, elem_t* value_b)
{
    return (~(_mm256_movemask_epi8(_mm256_cmpeq_epi8(value_a->fast_key, value_b->fast_key))) == 0);
}

__m256i getFastKey(char* key)
{   
    bool is_end = false;

    return _mm256_set_epi8(UNPACK_KEY(key, is_end));
}

void constructHashTable(HashTable* table, size_t size, 
                        size_t (* getHash)(char* value, size_t module))
{
    table->buckets = (Array*)calloc(size, sizeof(Array));
    table->getHash = getHash;
    table->size = size;
    table->amount_elements = 0;
    for (size_t i = 0; i < size; ++i)
    {
        constructArray(&table->buckets[i], START_CAPACITY, compareElements);
    }
}

void destructHashTable(HashTable* table)
{   
    for (size_t i = 0; i < table->size; ++i)
    {
        destructArray(&table->buckets[i]);
    }
    free(table->buckets);
}


void pushHashTable(HashTable* table, elem_t value)
{   
    size_t hash = table->getHash(value.key, table->size);
    
    pushBackArray(&table->buckets[hash], value);
}

elem_t* findHashTable(HashTable* table, elem_t* element)
{   
    size_t hash = table->getHash(element->key, table->size);

    if (element->xmm_using == 0)
    {
        element->fast_key = getFastKey(element->key);
        element->xmm_using = 1;
    }
    int idx = findArray(&table->buckets[hash], element);

    if (idx != -1) return &table->buckets[hash].data[idx];

    return nullptr;
}

HashTable* newHashTable(size_t size, 
                        size_t (* getHash)(char* value, size_t module))
{
    HashTable* new_table = (HashTable*)calloc(1, sizeof(HashTable));

    constructHashTable(new_table, size, getHash);

    return new_table;
}

HashTable* deleteHashTable(HashTable* table)
{
    destructHashTable(table);

    free(table);

    return nullptr;
}