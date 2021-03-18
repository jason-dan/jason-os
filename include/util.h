#ifndef UTIL_H
#define UTIL_H
// Utility method to duplicate strings like POSIX strdup, minus the GCC errors.
// Source: https://cboard.cprogramming.com/c-programming/95462-compiler-error-warning-implicit-declaration-function-strdup.html
char *strdup(const char *str);

#endif