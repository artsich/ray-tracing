#pragma once

#include "scene_parser/common.h"
#include "common.h"

typedef enum TokenKind {
	TOKEN_EOF = 0,
	TOKEN_LAST_CHAR = 127,
	TOKEN_MINUS,
	TOKEN_COLON,
	TOKEN_NAME,
	TOKEN_INT,
	TOKEN_FLOAT,
} TokenKind;

struct Token { 
	TokenKind kind;
	const char* start;
	const char* end;

	union {
		uint64_t int_val;
		double float_val;		
		struct { 
			const char* name;
			size_t name_len;
		};
	};
};

Token token;
const char* stream;

size_t copy_token_kind_str(char* dest, size_t dest_size, TokenKind kind) {
	size_t n = 0;
	switch (kind) {
	case 0:
		n = snprintf(dest, dest_size, "end of file");
		break;
	case TOKEN_INT:
		n = snprintf(dest, dest_size, "integer");
		break;
	case TOKEN_FLOAT:
		n = snprintf(dest, dest_size, "float");
		break;
	case TOKEN_NAME:
		n = snprintf(dest, dest_size, "name");
		break;
	default:
		if (kind < 128 && isprint(kind)) {
			n = snprintf(dest, dest_size, "%c", kind);
		}
		else {
			n = snprintf(dest, dest_size, "<ASCII %d>", kind);
		}
		break;
	}
	return n;
}

const char *token_kind_str(TokenKind kind) {
	static char buf[256];
	size_t n = copy_token_kind_str(buf, sizeof(buf), kind);
	assert(n + 1 <= sizeof(buf));
	return buf;
}

const uint8_t char_to_digit[] = {
	['0'] = 0,
	['1'] = 1,
	['2'] = 2,
	['3'] = 3,
	['4'] = 4,
	['5'] = 5,
	['6'] = 6,
	['7'] = 7,
	['8'] = 8,
	['9'] = 9,
	['a'] = 10, ['A'] = 10,
	['b'] = 11, ['B'] = 11,
	['c'] = 12, ['C'] = 12,
	['d'] = 13, ['D'] = 13,
	['e'] = 14, ['E'] = 14,
	['f'] = 15, ['F'] = 15,
};

void scan_float() { 
	const char* start = stream;
	while (isdigit(*stream)) {
		stream++;
	}

	if (*stream == '.') {
		stream++;
	}

	while (isdigit(*stream)) {
		stream++;
	}

	if (tolower(*stream) == 'e') {
		stream++;
		if (*stream == '+' || *stream == '-') {
			stream++;
		}
		if (!isdigit(*stream)) {
			syntax_error("Epected digit after exponent in float literal, found %c.", *stream);
		}
		else {
			while (isdigit(*stream)) {
				stream++;
			}
		}
	}

	double val = strtod(start, NULL);

	if (val == HUGE_VAL || val == -HUGE_VAL) {
		syntax_error("Float literal overflow.");
	}
	token.kind = TOKEN_FLOAT;
	token.float_val = val;

}

void scan_int() {
	uint64_t base = 10;
	uint64_t val = 0;
	for (;;) {
		uint64_t digit = char_to_digit[*stream];
		if (digit == 0 && *stream != '0') {
			break;
		}

		if (digit > base) {
			syntax_error("Digit '%c' out of range for base %"PRIu64, *stream, base);
			digit = 0;
		}

		if (val > (UINT64_MAX - digit) / base) {
			syntax_error("integer literal overflow");
			while (isdigit(*stream)) {
				stream++;
			}
		}

		val = val * base + digit;
		stream++;
	}
	token.kind = TOKEN_INT;
	token.int_val = val;
}

void next_token() { 
top:
	token.start = stream;

	switch(*stream) {
		case ' ': case '\n': case '\r': case '\t': case '\v':
			while (isspace(*stream)) {
				stream++;
			}
			goto top;
		case '.':
			scan_float();
			break;
		case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9': {
			const char* bookmark = stream;
			while (isdigit(*bookmark)) {
				bookmark++;
			}

			if (*bookmark == '.' || tolower(*bookmark) == 'e') {
				scan_float();
			}
			else {
				scan_int();
			}
			break;
		}
		case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
		case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
		case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
		case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
		case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
		case 'U': case 'V': case 'W': case 'X': case 'Y':
		case '_': {
			while (isalnum(*stream) || *stream == '_') {
				stream++;
			}

			token.kind = TokenKind::TOKEN_NAME;
			token.name = token.start;
			token.name_len = stream - token.start;
			break;
		}
		default:
			token.kind = *stream++;
			break;
		}

	token.end = stream;
}

inline bool is_token(TokenKind kind) { 
	return token.kind == kind;
}

inline bool is_token_name(const char* name) {
	assert(false);
	// token.name == name doesn't work.
	return is_token(TOKEN_NAME) && token.name == name;
}

inline bool match_token(TokenKind kind) {
	if(is_token(kind)) { 
		next_token();
		return true;
	}

	return false;
}

inline bool expec_token(TokenKind kind) { 
	if(is_token(kind)) { 
		next_token();
		return true;
	}
	else {
		char buf[256];
		copy_token_kind_str(buf, sizeof(256), kind);
		fatal("expected token %s, got %s", buf, token_kind_str(token.kind));
		return false;
	}
}


#define assert_token(x) assert(match_token(x));
#define assert_token_name(x) assert(token.name == str_intern(x) && match_token(TOKEN_NAME))
#define assert_token_int(x) assert(token.int_val == (x) && match_token(TOKEN_INT))
#define assert_token_float(x) assert(token.float_val == (x) && match_token(TOKEN_FLOAT))
#define assert_token_eof() assert(is_token(TOKEN_EOF))

void lex_test() { 
	
}