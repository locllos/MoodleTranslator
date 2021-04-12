#include "Array.h"
#include <string.h>

const size_t SYSTEM_CAPACYTY = 64;

bool compare(elem_t* value_a, elem_t* value_b)
{
    return (_mm256_movemask_epi8(_mm256_cmpeq_epi8(value_a->fast_key, value_b->fast_key)) != 0);
}

void initElement(Element* element, char* key, char* value)
{
    element->key = key;
    element->value = value;
}

void initElement(Element* element, __m256i fast_key, char* key, char* value)
{
    element->key = key;
    element->value = value;
    element->fast_key = fast_key;
    element->xmm_using = 1;
}

void initElement(Element* element_to, Element* element_from)
{
    element_to->fast_key = element_from->fast_key;
    element_to->value = element_from->value;
    element_to->key = element_from->key;
    element_to->xmm_using = element_from->xmm_using;
}

void constructArray(Array* array, size_t start_capacity, 
                    bool (*comparator)(elem_t* value_a, elem_t* value_b))
{
    reallocArray(array, start_capacity);

    array->comparator = comparator;
    array->capacity = start_capacity;
    array->size = 0;
}

void reallocArray(Array* array, size_t new_capacity)
{
    Element* new_data = (Element*)aligned_alloc(SYSTEM_CAPACYTY, new_capacity * sizeof(Element));

    if (array->data != nullptr)
    {
        memcpy(new_data, array->data, sizeof(Element) * array->capacity);
        free(array->data);
    }
    array->data = new_data;
    array->capacity = new_capacity;
}

void pushBackArray(Array* array, elem_t value)
{
    if (array->size + 1 > array->capacity)
    {   
        reallocArray(array, (array->capacity + 1) * 2);
    }
    initElement(&array->data[array->size++], &value);
}

void destructArray(Array* array)
{
    array->capacity = 0;
    array->size = 0;

    free(array->data);
}

int findArray(Array* array, elem_t* value)
{   
    int size = array->size;

    for (int i = 0; i < size; ++i)
    {
        if ((_mm256_movemask_epi8(_mm256_cmpeq_epi8(array->data[i].fast_key, value->fast_key)) != 0))
        {
            return i;
        }
    }
    return -1;
}

Array* newArray(size_t start_capacity, 
                bool (*comparator)(elem_t* value_a, elem_t* value_b))
{
    Array* array = (Array*)calloc(start_capacity, sizeof(elem_t));

    constructArray(array, start_capacity, comparator);

    return array;
}

Array* deleteArray(Array* array)
{
    destructArray(array);
    free(array);

    return nullptr;
}