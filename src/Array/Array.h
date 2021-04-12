#include <stdlib.h>
#include <stdint.h>
#include <immintrin.h>
#include <time.h>

typedef uint64_t aligned_t;

struct Element
{
    __m256i fast_key;           //| + 32 bytes
                                //|
    char* key;                  //| + 8 bytes
    char* value;                //| + 8 byes
                                //|
    aligned_t xmm_using;        //| + 16 bytes       
};                              //|___________
                                //| = 64 bytes

typedef Element elem_t;

struct Array
{
    size_t capacity;
    size_t size;

    elem_t* data;
    bool (*comparator)(elem_t* value_a, elem_t* value_b);
};

void    initElement     (Element* element, char* key, char* value = nullptr);
void    initElement     (Element* element, __m256i fast_key, char* key, char* value);
void    initElement     (Element* element_to, Element* element_from);

void    constructArray  (Array* array, size_t start_capacity, bool (*comparator)(elem_t* value_a, elem_t* value_b));
void    reallocArray    (Array* array, size_t new_capacity);
void    pushBackArray   (Array* array, elem_t value);
void    destructArray   (Array* array);

int     findArray       (Array* array, elem_t* value);

Array*  newArray        (size_t start_capacity, bool (*comparator)(elem_t* value_a, elem_t* value_b));
Array*  deleteArray     (Array* array);