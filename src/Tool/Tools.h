#define NEXT_CHR(string, is_end) \
    (is_end == true || *string == '\0' && (is_end = true)) ? '\0' : (*string++)

#define UNPACK_KEY(string, is_end) \
    NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), \
    NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), \
    NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), \
    NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end), NEXT_CHR(string, is_end)

