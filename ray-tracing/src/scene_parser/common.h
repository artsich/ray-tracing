#pragma once

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include <stdarg.h>
#include <inttypes.h>

void *xrealloc(void* buffer, size_t num_bytes) {
	void *ptr = realloc(buffer, num_bytes);
	if (!ptr) {
		perror("xrealloc failed");
		exit(1);
	}
	return ptr;
}

void *xcalloc(size_t count, size_t size) {
	void* ptr = calloc(count, size);
	if (!ptr) {
		perror("xcalloc failed");
		exit(1);
	}
	return ptr;
}

void *xmalloc(size_t num_bytes) {
	void *ptr = malloc(num_bytes);
	if (!ptr) {
		perror("xmalloc failed");
		exit(1);
	}
	return ptr;
}

void fatal(const char *format, ...) {
	va_list args; 
	va_start(args, format);
	printf("FATAL: ");
	vprintf(format, args);
	printf("\n");
	va_end(args);
	exit(1);
}

void syntax_error(const char* format, ...) {
	va_list args;
	va_start(args, format);
	printf("Syntax Error: ");
	vprintf(format, args);
	printf("\n");
	va_end(args);
}