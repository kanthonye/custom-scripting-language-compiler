

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
#include "syntax.hpp"
#include "semantic.hpp"


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

    // std::cout << "\n------------------------------------------------------\n";

    Syntax syntax;
    if( !syntax.analyze( &lexer ) )
    {
        return 0;
    }   
    syntax.printTree();

    // Semantic semantic;
    // if( !semantic.analyze( &syntax ) )
    // {
    //     return 0;
    // }   
    //parser.printTree();

    return 0;
}
