#ifndef parser_hpp
#define parser_hpp

#include <stack>
#include "tree.hpp"

class Parser{
public:

    bool parse( Lexer* lexer );
    void printTree();
    void clear();

    Tree& getTree();

    ~Parser();
    Parser();

private:

    Tree _parse_tree;
};


#endif // parser_hpp