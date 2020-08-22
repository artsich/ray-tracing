#include "../lexer.h"

#include <assert.h>

#define assert_token(token, token_type) assert(token.is_token(token_type))
#define assert_token_string(token, str_value) assert(token.match_token(TokenKind::TOKEN_STRING, str_value))

void lexer_test() {

    Lexer lexer(
R"(
    {
        "name": "value"
    }
)"
);

    Token token = lexer.next_token();
    assert_token(token, '{');
    
    token = lexer.next_token();
    assert_token_string(token, "name");
    
    token = lexer.next_token();
    assert_token(token, ':');

    token = lexer.next_token();
    assert_token_string(token, "value");

    token = lexer.next_token();
    assert_token(token, '}');
}