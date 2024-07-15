#ifndef syntax_hpp
#define syntax_hpp

#include <stack>
#include "tree.hpp"
#include "lexer.hpp"

/**
 * Syntax analysis, also known as parsing, is the process of analyzing the structure of a 
 * sequence of tokens according to the rules of a formal grammar. It checks whether the 
 * sequence of tokens conforms to the syntax specified by the language's grammar. The main 
 * goal of syntax analysis is to determine the hierarchical structure of the program and 
 * identify any syntax errors. Syntax analysis produces a parse tree or an abstract syntax 
 * tree (AST) representing the syntactic structure of the program.
 **/
class Syntax {
public:

    enum{ CONST = 0, ADDRESS = 1 };
    bool analyze( Lexer* lexer );
    void printTree();
    void clear();

    Tree& getTree();

    ~Syntax();
    Syntax();

private:

    Tree _tree;
};

#endif