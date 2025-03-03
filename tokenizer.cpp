#include "tokenizer.hpp"

// helper to add a token to the list 
void add_token(TokenSequenceType& tokens, std::string& current_token) {
    if (!current_token.empty()) {
        tokens.push_back(current_token);
        current_token.clear();
    }
}

// Tokenizer implementation
TokenSequenceType tokenize(std::istream& seq) {
    TokenSequenceType tokens;      // list of tokens
    std::string current_token;     // current token being built
    char ch;

    // input by character
    while (seq.get(ch)) {
        if (ch == ';') {
            seq.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip comment line
            continue;
        }

        // parentheses as separate token
        if (ch == '(' || ch == ')') {
            add_token(tokens, current_token);
            tokens.push_back(std::string(1, ch));// Add parenthesis
        }
        // Add token on whitespace
        else if (std::isspace(ch)) {
            add_token(tokens, current_token);
        }
        else {
            current_token += ch;
        }
    }
    add_token(tokens, current_token);// Add any remaining token

    return tokens;
}
