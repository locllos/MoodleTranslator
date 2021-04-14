#include "Translator.h"
#include "../Parser/Parser.h"
#include <math.h>

size_t DEFAULT_SIZE = (size_t)64000;
size_t AMOUNT_PASSES = 128;
const char* punct_marks = ".\t ,:\n?!;";
double tolower_sum_time = 0;
double hash_sum_time = 0;

extern "C" size_t HolyPoly_asm(char* value, size_t module);

size_t OH_YEAH_MA_SENPAI_GO_ROR_ME(size_t rorable_number, size_t amount_rors)
{
    return (rorable_number >> amount_rors) | (rorable_number << (sizeof(size_t) * 8 - amount_rors));
}

bool compare_(elem_t* value_a, elem_t* value_b)
{
    return (~(_mm256_movemask_epi8(_mm256_cmpeq_epi8(value_a->fast_key, value_b->fast_key))) == 0);
}

size_t HolyPoly(char* value, size_t module)
{
    size_t prime_number = 7988587;
    size_t hash = *value;
    size_t accumulator = 1;

    for (value; *value != '\0'; ++value)
    {   
        hash = hash + (*value) * accumulator;

        accumulator *= prime_number;
    }
    return hash % module;
}

void skipUselessSymbols(char** string)
{   
    while (isspace(*(*string)))
    {   
        ++*string; 
    }
}

void constructDictionary(Dictionary* dict, size_t (* getHash)(char* value, size_t module))
{
    dict->buffer = nullptr;

    dict->table = (HashTable*)calloc(1, sizeof(HashTable));

    constructHashTable(dict->table, DEFAULT_SIZE, getHash);
}

void constructDictionary(Dictionary* dict, char* buffer, 
                         size_t (* getHash)(char* value, size_t module))
{
    dict->buffer = buffer;
    dict->table = (HashTable*)calloc(1, sizeof(HashTable));

    constructHashTable(dict->table, DEFAULT_SIZE, getHash);

    // FILE* hash_dump = fopen("res/hash_dump.txt", "w");
    Element element = {};
    char* current_word = strtok(buffer, "=");
    while (current_word != nullptr)
    {   
        element.key = current_word + 1;
        element.fast_key = getFastKey(element.key);

        current_word = strtok(nullptr, "\r\n");  
        
        element.value = current_word;
        current_word = strtok(nullptr, "=");

        // fprintf(hash_dump,"(%s | %s) = (%zu)\n", element.key, element.value, dict->table->getHash(element.key, dict->table->size));
        pushHashTable(dict->table, element);
    }
    // fclose(hash_dump);
}

void constructDictionary(Dictionary* dict, char* buffer)
{   
   constructDictionary(dict, buffer, HolyPoly_asm);
}

void constructDictionary(Dictionary* dict, const char* filename)
{
    char* buffer = readFile(filename);

    constructDictionary(dict, buffer);
}

void constructDictionary(Dictionary* dict, const char* filename, 
                         size_t (* getHash)(char* value, size_t module))
{
    char* buffer = readFile(filename);

    constructDictionary(dict, buffer, getHash);
}

void pushDictionary(Dictionary* dict, char* key, char* word)
{
    Element element = {};

    initElement(&element, key, word);
    element.fast_key = getFastKey(key);

    pushHashTable(dict->table, element);
}

/**
 * @brief Translating the sentence and put translations in output.
 *        The sentence changes while function is running
 * 
 * @param dict 
 * @param sentence 
 * @param output 
 */
void translateSentence(Dictionary* dict, char* sentence, FILE* output)
{   
    char* translation = nullptr;
    char* current_word = strtok(sentence, " .,!?;:\n\t");
    
    while (current_word != nullptr)
    {   
        fprintf(output, "%s: ", current_word);
        toLower_asm(current_word);
        translation = getTranslation(dict, current_word);

        fprintf(output, "%s\n", translation != nullptr ? 
                                translation : "(not found)");

        current_word = strtok(nullptr, " .,!?;:\n\t");
    }
}

void writeHTMLPrologue(FILE* html_file, const char* filename)
{
    fprintf(html_file, "<head>\n");
    fprintf(html_file, "<meta charset=\"utf-8\">");
    fprintf(html_file, "<title> %s file translation </title>\n", filename);
    fprintf(html_file, "</head>\n");
}

void writeTranslation(Dictionary* dict, TokenArray* tokens, FILE* translation)
{   
    char copied_word[128] = {};
    char* current_word = nullptr;
    char* translated_word = nullptr;

    fprintf(translation, "<p>");
    double start = 0;
    for (size_t i = 0; i < tokens->size; ++i)
    {   
        switch (tokens->data[i].type)
        {
            case WORD:
                current_word = tokens->data[i].lexeme;

                strcpy(copied_word, current_word); //| без этого не обойтись, так как 
                toLower(copied_word);          //| подсчёт хеш-суммы регистрозависимый

                translated_word = getTranslation(dict, copied_word);

                fprintf(translation, "<text title=\"%s\">", translated_word != nullptr ? 
                                                            translated_word : "(not found)");
                fprintf(translation, "%s\n</text>\n", current_word);

                break;
    
            case NEW_LINE:
                fprintf(translation, "</p>");
                fprintf(translation, "<p>");
                break;
    
            default:
                fprintf(translation, "%c", punct_marks[tokens->data[i].type]);
        }
    }
    fprintf(translation, "</p>");
    printf("tolower time: %lg\n", tolower_sum_time);
}

Array* getExample(TokenArray* tokens)
{
    Element value = {};
    Array* array = (Array*)calloc(1, sizeof(Array));
    constructArray(array, 4, compare_);

    for (size_t i = 0; i < tokens->size; ++i)
    {
        if (tokens->data[i].type == WORD)
        {   
            Token token = tokens->data[i];
            initElement(&value, getFastKey(token.lexeme), token.lexeme, nullptr);
            pushBackArray(array, value);
        }
    }
    return array;
}

void Test(const char* filename)
{   
    Dictionary dict = {};
    TokenArray tokens = {};
    constructDictionary(&dict, "res/dict.txt");
    constructTokenArray(&tokens, filename);
    Array* test_data = getExample(&tokens);

    char copied_word[128] = {};
    char* current_word = nullptr;
    char* translated_word = nullptr;

    double sum_time = 0;
    printf("test data size: %zu\n", test_data->size);
    for (size_t j = 0; j < AMOUNT_PASSES; ++j)
    {
        for (size_t i = 0; i < test_data->size; ++i)
        {   
            // size_t idx = rand() % test_data->size;
            
            // double start = clock();

            elem_t* tmp = findHashTable(dict.table, &test_data->data[i]);

            // sum_time += difftime(clock(), start) / CLOCKS_PER_SEC;
        }
    }
    // printf("Test time: %lg\n", sum_time);

    test_data = deleteArray(test_data);
    destructDictionary(&dict);
    destructTokenArray(&tokens);
}

void interfaceDictionary()
{
    char original_word[128];
    char translated_word[128];
    char* translation = nullptr;

    Dictionary dict = {};
    INPUT choice = GET;

    constructDictionary(&dict, HolyPoly_asm);

    printf("Get: ?\nPush: +\nExit: -\n");
    while (choice != EXIT)
    {   
        scanf("\n%c", &choice);
        printf("input: %c\n", choice);
        switch (choice)
        {
            case GET:
                printf("Original: ");
                scanf("%s", original_word);
                translation = getTranslation(&dict, original_word);
                printf("\t%s\n", translation == nullptr ? "(not found)" : translation);
                break;

            case PUSH:
                printf("Original: ");
                scanf("%s", original_word);

                printf("Translation: ");
                scanf("%s", translated_word);

                pushDictionary(&dict, original_word, translated_word);
                break;

            default:
                printf("(undefined input)\n");
                break;
        }
        fflush(stdin);
    }
}

void translateFile(const char* filename, const char* translated_file)
{
    Dictionary dict = {};
    TokenArray tokens = {};
    constructDictionary(&dict, "res/dict.txt");
    constructTokenArray(&tokens, filename);

    FILE* translation = fopen(translated_file, "w");

    fprintf(translation, "<html5>\n");

    writeHTMLPrologue(translation, filename);

    fprintf(translation, "<body>\n");
    fprintf(translation, "<p>\n");
    writeTranslation(&dict, &tokens, translation);

    fprintf(translation, "</p>\n");
    fprintf(translation, "</body>\n");
    fprintf(translation, "</html5>\n");

    destructDictionary(&dict);
    destructTokenArray(&tokens);
    fclose(translation);
}

void destructDictionary(Dictionary* dict)
{
    destructHashTable(dict->table);

    free(dict->table);
    free(dict->buffer);
}

char* getTranslation(Dictionary* dict, char* word)
{
    Element target_translation = {};
    initElement(&target_translation, word);

    Element* result_translation = findHashTable(dict->table, 
                                                &target_translation);
    
    if (result_translation == nullptr) return nullptr;
    
    return result_translation->value;
}
