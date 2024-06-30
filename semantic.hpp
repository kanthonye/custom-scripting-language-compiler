#ifndef semantic_hpp
#define semantic_hpp

#include "syntax.hpp"
#include "symbol-table.hpp"

class Semantic{
public:

    bool analyze( Syntax* syntax );
    Semantic();

private:

    void analyzeNode( Tree& node );
    void analyzeAssign( Tree& node );
    void analyzeFunction( Tree& node );
    void analyzeIf( Tree& node );
    void analyzeFor( Tree& node );
    void analyzeWhile( Tree& node );
    void analyzeFunctionCall( Tree& node );
    std::string inferType( Tree& node );

private:

    SymbolTable _symbol_table;
};

#endif