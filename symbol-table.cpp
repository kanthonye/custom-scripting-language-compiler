
#include "symbol-table.hpp"

void SymbolTable::enterScope() 
{
    tables.emplace_back();
}

void SymbolTable::exitScope() 
{
    if (!tables.empty()) {
        tables.pop_back();
    }
}

void SymbolTable::declare( const std::string& name, const std::string& type ) 
{
    if (tables.back().find(name) != tables.back().end()) {
        throw std::runtime_error("Variable " + name + " is already declared in this scope.");
    }
    tables.back()[name] = type;
}

std::string SymbolTable::lookup(const std::string& name) 
{
    for (auto it = tables.rbegin(); it != tables.rend(); ++it) 
    {
        if (it->find(name) != it->end()) 
        {
            return it->at(name);
        }
    }
    throw std::runtime_error("Variable " + name + " not declared.");
}