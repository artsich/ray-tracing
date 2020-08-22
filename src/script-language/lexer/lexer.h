enum TokenKind {
    TOKEN_NONE = 127,

    TOKEN_NAME,
    TOKEN_STRING,
    TOKEN_NUMBER_INT,
    TOKEN_NUMBER_DOUBLE,
};

struct Token { 
    TokenKind kind;
    
    union { 
        int int_value;
        double double_value;
        const char* name;
    };

    bool is_token(TokenKind token_kind) { 
        return kind == token_kind;
    }

    bool match_token(TokenKind token_kind, const char* value) { 
        if (kind == token_kind) {
            return match_token_name(value);
        }
        return false;
    }

    bool match_token_name(const char* value) { 
        return strcmp(name, value);
    }
};

class Lexer {
public:
    Lexer(const char* stream) 
        : stream(stream) 
    {
    }

    bool is_end() { return *stream == EOF; }

    Token next_token()
    {
        
        return token;
    }

private:
    Token token;
    const char* stream;
};