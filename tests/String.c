
#include "runtime/String.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

String* String__init(const Char* literal) {
    String* ret = malloc(sizeof(String) + strlen(literal) + 1);
    ret->_vtable = 0;
    ret->_refcount = 1;
    ret->length = strlen(literal);
    strcpy(ret->data, literal);
    return ret;
}

int main() {
    String* str1 = String__init("hello "); 
    assert(String_length__g(str1) == 6);
    String* str2 = String__init("world!");
    assert(String_length__g(str2) == 6);

    // Concatenation
    String* str3 = String__add(str1, str2);
    assert(String_length__g(str3) == 12);
    assert(strncmp(str3->data, "hello world!", 12) == 0);
    
    // Equality check
    String* str4 = String__init("hello world!");
    String* str5 = String__init("hello world?");
    assert(String__equal(str3, str4));
    assert(!String__equal(str3, str5));

    // Slice (substring)
    String* str6 = String_slice(str3, -1, 20);
    assert(String__equal(str6, str3));
    assert(String_length__g(str6) == 12);
    String* str7 = String_slice(str3, 5, 1);
    assert(String_length__g(str7) == 0);
    String* str8 = String_slice(str3, 2, 6);    
    assert(String_length__g(str8) == 4);
    assert(strncmp(str8->data, "llo ", 4) == 0);

    // Index
    assert('\0' == String__index(str3, -1));
    assert('h' == String__index(str3, 0));
    assert('!' == String__index(str3, 11));
    assert('\0' == String__index(str3, 12));
    assert('\0' == String__index(str3, 209));

    return 0;
}