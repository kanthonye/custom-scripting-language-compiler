

#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <string>
#include <unordered_map>
#include <map>
#include <string>
#include <stack>
#include <list>
#include "tree.hpp"
#include "parser.hpp"
#include "syntax.hpp"


int main( int input_count, char* input_values[] )
{
    printf( "Lecical Analyzer\n" );
    if ( input_count < 2 )
    {
        printf( "Usage: LEXER Source.txt\n" );
        return 0;
    }

    Lexer lexer;
    if( !lexer.load( input_values[ 1 ] ) )
    {
        return 0;
    }    

    Parser parser;
    if( !parser.parse( &lexer ) )
    {
        return 0;
    }   
    parser.printTree();

    std::cout << "\n------------------------------------------------------\n";

    Syntax syntax;
    if( !syntax.analyze( &parser ) )
    {
        return 0;
    }   
    syntax.printTree();

    return 0;
}
