#ifndef lexer_hpp
#define lexer_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "hash-table.hpp"
#include "lexer-tokens.hpp"
#include "lexer-preprocessor.hpp"


struct Token
{
    uint16_t category;
    uint16_t type;
};


class Lexer
{
public:

#define DEFINE_LEXER_TOKEN(x) _##x

    enum Token{ LEXER_TOKENS };

#undef DEFINE_LEXER_TOKEN
  
public:

    void error( const char* file, const char* func, int line, Token token, const char* msg = "" );
    static const char* toString( Lexer::Token token );
    bool load( const char* filename );
    const char* getLexeme(); 
    Token getNextToken();
    int getCurrLine();
    void revertToken();
    virtual void clear();
    virtual void print();

    virtual ~Lexer();
    Lexer();

protected:

    LexerPreprocessor preprocessor;
    
    std::vector< char > file_bufr;
    std::vector< char > lexeme;
    HashTable< Token > token_table; 
    std::string _error;
    int line_count;
    int err_code;
};

#endif