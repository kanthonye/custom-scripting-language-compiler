#include "semantic.hpp"



// // bool Semantic::analyzeFunctCall( int scope, Syntax::Ref& n )
// // {
// //     if ( !errmsg.empty() ) return false;

// //     Syntax::Ref function = functions.find( GLOBAL_SCOPE, n->id );
// //     if ( !function )
// //     {
// //         function = symbols.find( scope, n->id );
// //         if ( !function )
// //         {
// //             errmsg = SEMANTIC_ERROR( std::string("undefined '"+ n->id + "' @line: " + std::to_string( n->line )) );
// //             return false;
// //         }
// //         if ( function->node_type != Lexer::_FUNCTION )
// //         {
// //             return true;
// //         }
        
// //     }

// //     Syntax::Ref& params = function->nodes[ 0 ];
// //     std::vector< Syntax::Ref >& args = n->nodes;

// //     if ( params->nodes.size() != args.size() ) 
// //     {
// //         errmsg = "function '" + n->id + "' expects " + std::to_string( params->nodes.size() ) + 
// //         " arguments, but " + std::to_string( args.size() ) + " were provided.";
// //         return false;
// //     }

// //     for (size_t i = 0; i < args.size(); ++i) 
// //     {
// //         Syntax::Ref& arg = args[ i ];
// //         Syntax::Ref& param = params->nodes[ i ];

// //         if( param->node_type == Lexer::_DECLARATION )
// //         {
// //             switch ( param->nodes[ 1 ]->node_type )
// //             {
// //                 case Lexer::_VAR_INT:
// //                 case Lexer::_VAR_LONG:
// //                 case Lexer::_VAR_FLOAT:
// //                 case Lexer::_VAR_DOUBLE:
// //                 {
// //                     if 
// //                     ( 
// //                         arg->node_type != Lexer::_INT   &&
// //                         arg->node_type != Lexer::_LONG  &&
// //                         arg->node_type != Lexer::_FLOAT &&
// //                         arg->node_type != Lexer::_DOUBLE &&
// //                         arg->node_type != Lexer::_VAR_INT   && 
// //                         arg->node_type != Lexer::_VAR_LONG  &&
// //                         arg->node_type != Lexer::_VAR_FLOAT &&
// //                         arg->node_type != Lexer::_VAR_DOUBLE
// //                     )
// //                     {
// //                         errmsg = "The function '" + n->id + "()' require parameter -> '" + param->nodes[ 0 ]->id + 
// //                         "' to be numeric. But the argument has value -> '" + arg->id + "'.";
// //                         return false;
// //                     }
// //                 }
// //                 break;

// //                 case Lexer::_VAR_STRING:
// //                 {
// //                     if ( arg->node_type != Lexer::_VAR_STRING && arg->node_type != Lexer::_STRING )
// //                     {
// //                         errmsg = "The function '" + n->id + "()' require parameter '" + param->nodes[ 0 ]->id + 
// //                         "' to be string. But the argument has value -> '" + arg->id + "'.";
// //                         return false;
// //                     }
// //                 }
// //                 break;

// //                 default:
// //                 break;
// //             }
// //         }
// //     }
// //     return true;
// // }

// bool analyzeScope( Semantic* semantic, std::string& errmsg, int flags, int scope, Syntax::Ref& n )
// {
//     if ( !errmsg.empty() ) return false;

//     // Analyze loop body
//     for (int i = 0; i < n->nodes.size(); i++ ) 
//     {
//         if ( !analyzeNode( errmsg, flags, scope, n->nodes[ i ] ) )
//         {
//             return false;
//         }
//     }

//     return true;
// }

// bool analyzeFuncParams( Semantic* semantic, std::string& errmsg, int flags, int scope, Syntax::Ref& n )
// {
//     Syntax::Ref p;
//     for (size_t i = 0; i < n->nodes.size(); ++i) 
//     {
//         p = n->nodes[ i ];
//         if ( p->node_type == Lexer::_IDENTIFIER ) 
//         {
//             symbols.insert( scope, p->id, p );
//         }
//         else if ( p->node_type == Lexer::_DECLARATION ) 
//         {
//             symbols.insert( scope, p->nodes[ 0 ]->id, p->nodes[ 1 ] );
//         }
//         else if ( p->node_type == Lexer::_ASSIGN ) 
//         {
//             symbols.insert( scope, p->nodes[ 0 ]->id, p->nodes[ 1 ] );
//         }
//         else if ( p->node_type == Lexer::_ADDRESS ) 
//         {
//             if( !analyzeFuncParams( scope, p ) )
//             {
//                 return false;
//             }
//         }
//         else if ( p->node_type == Lexer::_CONST ) 
//         {
//             if( !analyzeFuncParams( scope, p ) )
//             {
//                 return false;
//             }
//         }
//         else
//         {
//             switch ( p->node_type ) 
//             {
//                 case Lexer::_DECLARATION:
//                 case Lexer::_ASSIGN:
//                 case Lexer::_ADDRESS:
//                 {
//                     std::string s = Lexer::toString( p->node_type );
//                     errmsg = SEMANTIC_ERROR( std::string("invalid function parameter: '"+ s + " -> "+ p->nodes[ 0 ]->id + "' @line: " + std::to_string( p->line ) ) );
//                 }
//                 break;

//                 case Lexer::_CONST:
//                 {
//                     if ( p->nodes[ 0 ]->node_type == Lexer::_IDENTIFIER )
//                     {
//                         errmsg = SEMANTIC_ERROR( std::string("invalid function parameter -> '"+ p->nodes[ 0 ]->id + "' @line: " + std::to_string( p->line ) ) );
//                     }
//                     else
//                     {
//                         p = p->nodes[ 0 ];
//                         errmsg = SEMANTIC_ERROR( std::string("invalid function parameter -> '"+ p->nodes[ 0 ]->id + "' @line: " + std::to_string( p->line ) ) );
//                     }
//                 }
//                 break;

//                 default:
//                 {
//                     errmsg = SEMANTIC_ERROR( std::string("invalid function parameter -> '"+ p->id + "' @line: " + std::to_string( p->line ) ) );
//                 }
//                 break;
//             }
//             return false;
//         }
//     }
//     return true;
// }

// bool analyzeFunction( Semantic* semantic, std::string& errmsg, int flags, int scope, Syntax::Ref& n )
// {
//     if ( !errmsg.empty() ) return false;

//     Syntax::Ref& name   = n->nodes[ 0 ];
//     Syntax::Ref& func   = n->nodes[ 1 ];
//     Syntax::Ref& params = func->nodes[ 0 ];
//     Syntax::Ref& body   = func->nodes[ 1 ];

//     if ( !analyzeFuncParams( semantic, errmsg, flags, scope, params ) ) 
//     {
//         return false;
//     }

//     if ( !analyzeScope( semantic, errmsg, flags, scope, body ) ) 
//     {
//         return false;
//     }

//     semantic->insert( scope, name->id, func );
//     return true;
// }

// bool analyzeAssign( Semantic* semantic, std::string& errmsg, int flags, int scope, Syntax::Ref& n )
// {
//     if ( !errmsg.empty() ) return false;
//     switch ( n->nodes[ 1 ]->node_type )
//     {
//         case Lexer::_FUNCTION:
//         {
//             return analyzeFunction( semantic, scope, n );
//         }
//         break;

//         case Lexer::_FUNCTION_CALL:
//         {
//             return analyzeFunctionCall( semantic, scope, n->nodes[ 1 ] );
//         }
//         break;

//         case Lexer::_FALSE:
//         case Lexer::_TRUE:
//         case Lexer::_INT:
//         case Lexer::_LONG:
//         case Lexer::_FLOAT:
//         case Lexer::_DOUBLE:
//         case Lexer::_STRING:
//         case Lexer::_VAR_INT:
//         case Lexer::_VAR_LONG:
//         case Lexer::_VAR_FLOAT:
//         case Lexer::_VAR_DOUBLE:
//         case Lexer::_VAR_STRING:
//         {
//             symbols.insert( scope, n->nodes[ 0 ]->id, n->nodes[ 1 ] );
//         }
//         break;

//         case Lexer::_ADD:
//         case Lexer::_SUB:
//         case Lexer::_DIV:
//         case Lexer::_MUL:
//         case Lexer::_MOD:
//         case Lexer::_EXP:
//         {
//             return analyzeAdd( semantic, scope, n->nodes[ 1 ] );
//         }
//         break;

//         case Lexer::_INCREMENT:
//         case Lexer::_DECREMENT:
//         {
//             if ( !validateIdentifier( semantic, scope, symbols, n->nodes[ 0 ] ) )
//             {
//                 errmsg = SEMANTIC_ERROR( std::string("syntax '"+ n->nodes[ 1 ]->id + "' @line: " + std::to_string( n->line )) );
//                 return false;
//             }
//         }
//         break;

//         case Lexer::_IDENTIFIER:
//         {
//             if ( !validate( semantic, scope, symbols, n->nodes[ 1 ] ) )
//             {
//                 return false;
//             }
//         }
//         break;
    
    
//         default:
//         {
//             errmsg = SEMANTIC_ERROR( std::string("unsupported '"+ n->nodes[ 1 ]->id + "'") );
//             return false;
//         }
//         break;
//     }

//     return true;
// }

// void analyzeAssignNode( SymbolsTable* symbols, Tree& tree, int& scope )
// {
//     switch ( tree[ 1 ]->type )
//     {
//         case Lexer::_FUNCTION:
//         {
//             if ( symbols->lookup( scope, tree[ 0 ]->id ) < 0 )
//             {
//                 scope++;
//                 symbols->insert( scope, tree );
//                 analyzeFunctionNode( symbols, tree[ 1 ], scope );
//             }
//             else 
//             {
//                 //THROW_SEMANTIC_ERROR( SEMANTIC_ERROR_DUPLICATE, tree[ 0 ]->line, tree->id, );
//             }
//         }
//         break;

//         case Lexer::_FUNCTION_CALL:
//         {
//             int index = symbols->lookup( scope, tree->id );
//             if ( index >= 0 )
//             {
//                 Tree& function = symbols->get( index );
//                 Tree& funccall = tree;
                
//                 Tree& parameters = function[ 0 ];
//                 if ( funccall.size() != parameters.size() )
//                 {
//                     // for (size_t i = 0; i < parameters.size(); i++)
//                     // {
//                     //     Tree& param = parameters[ i ];
//                     //     if ( !isCompatible( param, funccall[ i ] ) )
//                     //     {
//                     //         //THROW_SEMANTIC_ERROR( SEMANTIC_ERROR_NONE_COMPATIBLE_PARAM, tree->line, tree->id, );
//                     //     }
//                     // }
//                     // THROW_SEMANTIC_ERROR( SEMANTIC_ERROR_FUNC_PARAM_SIZE, tree->line, tree->id, );
//                 }
//             }
//             else 
//             {
//                 //THROW_SEMANTIC_ERROR( SEMANTIC_ERROR_UNDEFINE_FUNCTION, tree->line, tree->id, );
//             }
//         }
//         break;
//     }
// }



void Semantic::analyzeAssign( Tree& node ) 
{
    if ( node.size() != 2 ) 
    {
        throw std::runtime_error( "Invalid assignment node." );
    }

    Tree& var_node = node[ 0 ];
    Tree& val_node = node[ 1 ];
    switch ( var_node->type )
    {
        case Lexer::_IDENTIFIER:
        case Lexer::_CONST_IDENTIFIER:
        case Lexer::_STATIC_IDENTIFIER:
        case Lexer::_CONST_STATIC_IDENTIFIER:
        { 
            _symbol_table.declare( var_node->id, val_node );
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
    Tree funct = _symbol_table.lookup( node->id );

    if( funct->nodes[ 0 ].size() != node.size() )
    {
        throw std::runtime_error( "Function call parameters count must be equal to the function it's calling." );
    }
    
    for ( int i = 0; i < node.size(); i++ ) 
    {
        analyzeNode( node[ i ] );
    }
}

void Semantic::analyzeNode( Tree& node )
{
    switch ( node->type ) 
    {
        case Lexer::_ASSIGN:
        {
            analyzeAssign( node );
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
        std::cerr << e.what() << '\n';
    }
    return true;
}

Semantic::Semantic()
{
    _symbol_table.enterScope();
}
