/*
 * Copyright (c) 2010 Matt Fichman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, APEXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "Primitives.h"
#include "String.h"
#include "Object.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef WINDOWS
#include <windows.h>
#else
#include <errno.h>
extern char *strerror(int errnum);
#endif

extern int main();

void Boot_Main__call(Object self) {
    main();
}

void Boot_print_ptr(Object object) {
    // Writes a pointer value to stdout, and then flushes it.
    fprintf(stdout, "%p, vtable: %p", object, object->_vtable);
    fflush(stdout);
}

void Boot_print_str(String string) {
    // Write string to stdout.  This is function is here for convenience's 
    // sake.  Once a full-fledged IO framework has been written, this function
    // will really only be useful for simple output.

    fwrite(string->data, 1, string->length, stdout);
    fflush(stdout);
}

void Boot_print_int(Int integer) {
    // Print an integer to stdout.  This function is here only to run initial
    // tests on the compiler, and isn't part of the public API.

#ifdef DARWIN
    fprintf(stdout, "%lld", integer);
#else
    fprintf(stdout, "%ld", integer);    
#endif
    fflush(stdout);
}

void Boot_print_char(Char character) {
    // Print a character to stdout.  This function is not part of the public 
    // API for the Apollo library.

    fputc(character, stdout);
    fflush(stdout);
}

void Boot_dummy(Int a, Int b, Int c) {
    Boot_print_int(a);
    Boot_print_int(b);
    Boot_print_int(c);
}

Ptr Boot_malloc(Int size) {
    // Allocates 'size' bytes of memory, and aborts if the memory couldn't be
    // allocated.
    Ptr ret = malloc(size);
    if (!ret) {
        fprintf(stderr, "Out of memory\n");
        fflush(stderr);
        abort();
    }
    return ret;
}

Ptr Boot_calloc(Int size) {
    // Allocates 'size' byte of zeroed memory, and aborts if the memory
    // couldn't be allocated.
    Ptr ret = calloc(size, 1);
    if (!ret) {
        fprintf(stderr, "Out of memory\n");
        fflush(stderr);
        abort();
    }
    return ret;
}

void Boot_free(Ptr memory) {
    // Frees the memory at address 'memory'
    free(memory);
}

void Boot_abort() {
    // Aborts the process after printing the last system error message.
#ifdef WINDOWS
    DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM;
    DWORD id = GetLastError(); 
    LPTSTR buffer = 0;
    
    FormatMessage(flags, 0, id, 0, (LPTSTR)&buffer, 1, 0);
    fprintf(stderr, "%s\n", buffer);
#else
    fprintf(stderr, "%s\n", strerror(errno));
#endif
    fflush(stderr);
    abort();
}

