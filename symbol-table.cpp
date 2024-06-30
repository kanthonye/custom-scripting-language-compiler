
#include "symbol-table.hpp"

void SymbolTable::enterScope() 
{
    tables.emplace_back();
}

void SymbolTable::exitScope() 
{
    if (!tables.empty()) 
    {
        tables.pop_back();
    }
}

void SymbolTable::declare( const std::string& name, Tree& type ) 
{
    if ( tables.back().find( name ) != tables.back().end() ) 
    {
        throw std::runtime_error("Variable " + name + " is already declared in this scope.");
    }
    tables.back()[ name ] = type;
}

Tree SymbolTable::lookup(const std::string& name) 
{
    std::vector< std::unordered_map< std::string, Tree > >::reverse_iterator it;
    for (it = tables.rbegin(); it != tables.rend(); ++it) 
    {
        if (it->find( name ) != it->end()) 
        {
            return it->at( name );
        }
    }
    throw std::runtime_error( "Variable " + name + " not declared." );
}