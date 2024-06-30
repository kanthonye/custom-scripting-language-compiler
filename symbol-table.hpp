#ifndef symbol_table_hpp
#define symbol_table_hpp

#include <stack>
#include "tree.hpp"
#include "lexer.hpp"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>

class SymbolTable {
public:

    void declare( const std::string& name, Tree& type );
    Tree lookup( const std::string& name );
    void enterScope();
    void exitScope();

private:

    std::vector< std::unordered_map< std::string, Tree > > tables;
};

#endif // symbol_table_hpp