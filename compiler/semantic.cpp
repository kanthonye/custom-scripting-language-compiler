#include "semantic.hpp"

void Semantic::analyzeAssign( Tree& node ) 
{
    if ( node.size() != 2 ) 
    {
        throw std::runtime_error( "Invalid assignment node." );
    }

    switch ( node[ 0 ]->type )
    {
        case Lexer::_IDENTIFIER:
        case Lexer::_CONST_IDENTIFIER:
        case Lexer::_STATIC_IDENTIFIER:
        case Lexer::_CONST_STATIC_IDENTIFIER:
        { 
            Tree* symbol = _symbol_table.lookup( node[ 0 ]->id );
            if ( !symbol )
            {
                _symbol_table.declare( node[ 0 ]->id, node[ 1 ] );
            }
            else 
            {
                _symbol_table.set( node[ 0 ]->id, node[ 1 ] );
            }
        }
        break;
    
        default:
        {
            throw std::runtime_error( "Left-hand side of assignment must be an identifier." );
        }
        break;
    }
}

void Semantic::analyzeDeclaration( Tree& node ) 
{
    if ( node.size() != 2 ) 
    {
        throw std::runtime_error( "Invalid declaration node." );
    }

    switch ( node[ 0 ]->type )
    {
        case Lexer::_IDENTIFIER:
        case Lexer::_CONST_IDENTIFIER:
        case Lexer::_STATIC_IDENTIFIER:
        case Lexer::_CONST_STATIC_IDENTIFIER:
        { 
            _symbol_table.declare( node[ 0 ]->id, node[ 0 ][ 0 ] );
        }
        break;
    
        default:
        {
            throw std::runtime_error( "Left-hand side of assignment must be an identifier." );
        }
        break;
    }
}

void Semantic::analyzeFunction( Tree& node ) 
{
    _symbol_table.enterScope();
    for ( int i = 0; i < node.size(); i++ ) 
    {
        analyzeNode( node[ i ] );
    }
    _symbol_table.exitScope();
}

void Semantic::analyzeIf( Tree& node ) 
{
    if ( node.size() < 2 ) 
    {
        throw std::runtime_error("Invalid if node.");
    }
    
    analyzeNode( node[ 0 ] ); // Condition

    _symbol_table.enterScope();
    analyzeNode( node[ 1 ] ); // Then block
    _symbol_table.exitScope();

    if ( node.size() > 2 ) 
    {
        _symbol_table.enterScope();
        analyzeNode( node[ 2 ] ); // Else block
        _symbol_table.exitScope();
    }
}

void Semantic::analyzeFor( Tree& node ) 
{
    if ( node.size() != 2 ) 
    {
        throw std::runtime_error( "Invalid for loop node." );
    }

    if ( node[ 0 ].size() != 3 ) 
    {
        throw std::runtime_error( "Invalid for loop node condition." );
    }

    _symbol_table.enterScope();
    for ( int i = 0; i < node.size(); i++ ) 
    {
        analyzeNode( node[ i ] );
    }
    _symbol_table.exitScope();
}

void Semantic::analyzeWhile( Tree& node ) 
{
    if ( node.size() != 2 ) 
    {
        throw std::runtime_error( "Invalid while loop node." );
    }

    analyzeNode( node[ 0 ] ); // Condition

    _symbol_table.enterScope();
    analyzeNode( node[ 1 ] ); // Body
    _symbol_table.exitScope();
}

void Semantic::analyzeFunctionCall( Tree& node ) 
{
    Tree* symbol = _symbol_table.lookup( node->id );
    if( !symbol )
    {
        throw std::runtime_error( "Function '" + node->id + "' not declared." );
    }

    Tree& funct = *symbol;
    if( funct->nodes[ 0 ].size() != node.size() )
    {
        throw std::runtime_error( "Function call parameters count must be equal to the function it's calling." );
    }
    /*
    Tree param[ 2 ];
    for ( int i = 0; i < node.size(); i++ ) 
    {
        param[ 0 ] = node[ i ];
        param[ 1 ] = funct[ 0 ][ i ];

        if ( isCompatable( param[ 0 ], param[ 1 ] ) )
        {
            throw std::runtime_error( "Function " + node->id + "() parameter " + std::to_string( i ) + " requires a string." );
            switch ( param[ 1 ]->type )
            {
                case Lexer::_INT:
                {}
                break;

                case Lexer::_LONG:
                {}
                break;

                case Lexer::_FLOAT:
                {}
                break;

                case Lexer::_DOUBLE:
                {}
                break;

                case Lexer::_STRING:
                {
                    if ( Lexer::_STRING != evaluate( param[ 0 ] ) )
                    {
                        throw std::runtime_error( "Function " + node->id + "() parameter " + std::to_string( i ) + " requires a string." );
                    }
                }
                break;

                case Lexer::_ARRAY:
                {
                    if ( param[ 0 ]->type != Lexer::_ARRAY )
                    {
                        throw std::runtime_error( "Function " + node->id + "() parameter " + std::to_string(i) + " requires a array." );
                    }
                }
                break;

                case Lexer::_FUNCTION:
                {
                    if ( param[ 0 ]->type != Lexer::_FUNCTION )
                    {
                        throw std::runtime_error( "Function " + node->id + "() parameter " + std::to_string(i) + " requires a function." );
                    }
                }
                break;
            
                default:
                break;
            }
        }
        
    }
    */
}

void Semantic::analyzeNode( Tree& node )
{
    switch ( node->type ) 
    {
        case Lexer::_IDENTIFIER:
        {
            Tree null_node;
            _symbol_table.declare( node->id, null_node );
        }
        break;

        case Lexer::_ASSIGN:
        {
            analyzeAssign( node );
        }
        break;

        case Lexer::_DECLARATION:
        {
            analyzeDeclaration( node );
        }
        break;

        case Lexer::_FUNCTION:
        {
            analyzeFunction( node );
        }
        break;

        case Lexer::_IF:
        {
            analyzeIf( node );
        }
        break;

        case Lexer::_FOR:
        {
            analyzeFor( node );
        }
        break;

        case Lexer::_WHILE:
        {
            analyzeWhile( node );
        }
        break;

        case Lexer::_FUNCTION_CALL:
        {
            analyzeFunctionCall( node );
        }
        break;

        default:
        {
            for ( int i = 0; i < node.size(); i++ ) 
            {
                analyzeNode( node[ i ] );
            }
        }
        break;
    }
}

bool Semantic::analyze( Syntax* syntax )
{
    try
    {
        Tree& node = syntax->getTree();
        analyzeNode( node );
    }
    catch( const std::runtime_error& e )
    {
        std::cerr <<"[ SEMANTIC_ERROR ]:"<< e.what() << '\n';
        return false;
    }
    return true;
}

Semantic::Semantic()
{
    _symbol_table.enterScope();
}
