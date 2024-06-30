#include "semantic.hpp"


enum
{
    GLOBAL_SCOPE = 1,
    SYNTAX_ERROR,
    SYNTAX_OK,
};

enum
{
    NONE = 0,
    CONST,
    ADDRESS,
};



class SymbolsTable{
public:

    struct Info 
    {
        Tree tree;
        int scope;
        int index;
    };

// public:

    bool insert( int scope, Tree& node );
    int lookup( int scope, const std::string& id );
    Tree& get( int index );
//     Symbols();

// private:

//     std::map< std::string, std::vector< Info > > table;
//     std::vector< Tree > stmts;
//     int refcount;
};

// bool Symbols::insert( int scope, const std::string& id, Syntax::Ref& node )
// {
//     int index = int( stmts.size() );
//     std::vector< Info >& arry = table[ id ];
//     for (std::vector< Info >::iterator i = arry.begin(); i != arry.end(); i++)
//     {
//         if ( i->id == id && i->scope == scope )
//         {
//             return false;
//         }
//     }
//     Info info = { id, scope, index };
//     arry.push_back( info );
//     stmts.push_back( node );
//     return true;
// }

// Syntax::Ref Symbols::find( int scope, const std::string& id )
// {
//     std::map< std::string, std::vector< Info > >::iterator m = table.find( id );
//     if ( m != table.end() )
//     {
//         std::vector< Info >& arry = m->second;
//         for (std::vector< Info >::iterator i = arry.begin(); i != arry.end(); i++)
//         {
//             if ( i->id == id && i->scope == scope )
//             {
//                 return stmts[ i->index ];
//             }
//         }
//         return stmts[ m->second[ m->second.size() - 1 ].index ];
//     }
//     return Syntax::Ref();
// }

// Syntax::Ref Symbols::find( const std::string& id )
// {
//     std::map< std::string, std::vector< Info > >::iterator m = table.find( id );
//     if ( m != table.end() )
//     {
//         std::vector< Info >& arry = m->second;
//         for (std::vector< Info >::iterator i = arry.begin(); i != arry.end(); i++)
//         {
//             if ( i->id == id )
//             {
//                 return stmts[ i->index ];
//             }
//         }
//     }
//     return Syntax::Ref();
// }

// Symbols::Symbols()
// :   refcount( 0 )
// {}




// bool analyzeNode( Semantic* semantic, std::string& errmsg, int flags, int scope, Syntax::Ref& n );


// bool isNumeric(const char* str);


// #define SEMANTIC_ERROR( msg )( std::string( __FUNCTION__ ) + "(): ln -> " + std::to_string( __LINE__ ) + " : " + msg )

// bool validateIdentifier( int scope, Symbols& symbols, Syntax::Ref& n )
// {
//     if ( symbols.find( scope, n->id ) )
//     {
//         return true;
//     }
//     return false;
// } 

// bool Semantic::validate( int scope, Symbols& symbols, Syntax::Ref& n )
// {
//     switch ( n->node_type )
//     {
//         case Lexer::_IDENTIFIER:
//         {
//             if ( !validateIdentifier(scope, symbols, n ) )
//             {
//                 errmsg = SEMANTIC_ERROR( std::string("undefined -> '"+ n->id + "'") );
//                 return false;
//             }
//             return true;
//         }
//         break;
    
//         case Lexer::_INT:
//         case Lexer::_LONG:
//         case Lexer::_FLOAT:
//         case Lexer::_DOUBLE: 
//         case Lexer::_VAR_INT:
//         case Lexer::_VAR_LONG:
//         case Lexer::_VAR_FLOAT:
//         case Lexer::_VAR_DOUBLE:
//         {
//             return true;
//         }
//         break;

//         default:
//         break;
//     }
//     errmsg = SEMANTIC_ERROR( std::string("syntax error -> '"+ n->id + "' @line: " + std::to_string( n->line ) ));
//     return false;
// } 

// // bool Semantic::analyzeContinue( int scope, Syntax::Ref& n ) 
// // {
// //     if ( !errmsg.empty() ) return false;
// //     // Check if continue statement is inside a loop
// //     if ( scope == std::string::npos ) 
// //     {
// //         errmsg = SEMANTIC_ERROR( "Continue statement must be inside a loop. @line: " + std::to_string( n->line ) );
// //         return false;
// //     }
// //     return true;
// // }

// // bool Semantic::analyzeReturn( int scope, Syntax::Ref& n ) 
// // {
// //     if ( !errmsg.empty() ) return false;
// //     // Check if return statement is inside a function
// //     if ( scope == 0 ) 
// //     {
// //         errmsg = "Return statement must be inside a function.";
// //         return false;
// //     }
// //     // Analyze return expression
// //     return analyzeNode( scope, n->nodes[ 0 ] );
// // }

// // bool Semantic::analyzeSwitch( int scope, Syntax::Ref& n )
// // {
// //     if ( !errmsg.empty() ) return false;
// //     // Analyze loop ENCLOSE_PARENTHESIS
// //     analyzeNode( scope, n->nodes[ 0 ] );

// //     bool has_default = false;
// //     for (int i = 0; i < n->nodes[ 1 ]->nodes.size(); i++ ) 
// //     {
// //         switch ( n->nodes[ i ]->node_type ) 
// //         {
// //             case Lexer::_CASE_BLOCK:
// //                 // Analyze case block
// //                 analyzeNode( scope, n->nodes[ i ] );
// //                 break;

// //             case Lexer::_DEFAULT_BLOCK:
// //                 // Analyze default block
// //                 analyzeNode( scope, n->nodes[ i ] );
// //                 has_default = true;
// //                 break;

// //             default:
// //                 // Unexpected node type within switch statement
// //                 errmsg = SEMANTIC_ERROR( "Unexpected node type within switch statement. @line: " + std::to_string( n->line ) );
// //                 return false;
// //             break;
// //         }
// //     }

// //     if ( !has_default ) 
// //     {
// //         errmsg = SEMANTIC_ERROR( "Switch statement must have a default case. @line: " + std::to_string( n->line ) );
// //         return false;
// //     }
// //     return true;
// // }

// // bool Semantic::analyzeBreak( int scope, Syntax::Ref& n ) 
// // {
// //     if ( !errmsg.empty() ) return false;
// //     // Check if break statement is inside a loop
// //     if ( scope == std::string::npos) 
// //     {
// //         errmsg = SEMANTIC_ERROR( "Break statement must be inside a loop. @line: " + std::to_string( n->line ) );
// //         return false;
// //     }
// //     return true;
// // }

// // bool Semantic::analyzeWhile( int scope, Syntax::Ref& n )
// // {
// //     if ( !errmsg.empty() ) return false;
// //     // Analyze loop ENCLOSE_PARENTHESIS
// //     if ( !analyzeNode( scope, n->nodes[ 0 ] ) )
// //     {
// //         return false;
// //     }
// //     // Analyze loop body
// //     return analyzeScope( scope, n->nodes[ 1 ] );
// // }

// // bool Semantic::analyzeFor( int scope, Syntax::Ref& n )
// // {
// //     if ( !errmsg.empty() ) return false;
// //     Syntax::Ref& params = n->nodes[ 0 ];

// //     // Analyze loop initialization
// //     if ( analyzeNode( scope, params->nodes[ 0 ] ) )
// //     {
// //         return false;
// //     }

// //     // Analyze loop ENCLOSE_PARENTHESIS
// //     if ( analyzeNode( scope, params->nodes[ 1 ] ) )
// //     {
// //         return false;
// //     }

// //     // Analyze loop increment
// //     if ( analyzeNode( scope, params->nodes[ 2 ] ) )
// //     {
// //         return false;
// //     }
// //     // Analyze loop body
// //     return analyzeScope( scope, n->nodes[ 1 ] );
// // }

// // bool Semantic::analyzeIf( int scope, Syntax::Ref& n )
// // {
// //     if ( !errmsg.empty() ) return false;
// //     // Analyze ENCLOSE_PARENTHESIS
// //     if ( !analyzeNode( scope, n->nodes[ 0 ] ) )
// //     {
// //         return false;
// //     }
// //     // Analyze body
// //     return analyzeScope( scope, n->nodes[ 1 ] );
// // }




// // bool Semantic::analyzeAddEqual( int scope, Syntax::Ref& n )
// // {    
// //     Syntax::Ref& l = n->nodes[ 0 ];
// //     Syntax::Ref& r = n->nodes[ 1 ];
// //     if ( !validate( scope, symbols, l ) )
// //     {
// //         errmsg = SEMANTIC_ERROR( std::string("error -> '"+ l->id + "'") );
// //         return false;
// //     }
// //     if ( !validate( scope, symbols, r ) )
// //     {
// //         errmsg = SEMANTIC_ERROR( std::string("error -> '"+ l->id + "'") );
// //         return false;
// //     }
// //     return true;
// // }

// // bool Semantic::analyzeSubEqual( int scope, Syntax::Ref& n )
// // {    
// //     if ( !validate( scope, symbols, n->nodes[ 0 ] ) )
// //     {
// //         errmsg = SEMANTIC_ERROR( std::string("undefined '"+ n->nodes[ 0 ]->id + "'") );
// //         return false;
// //     }
// //     if ( !validate( scope, symbols, n->nodes[ 1 ] ) )
// //     {
// //         errmsg = SEMANTIC_ERROR( std::string("undefined '"+ n->nodes[ 1 ]->id + "'") );
// //         return false;
// //     }
// //     return true;
// // }
// // bool Semantic::analyzeDivEqual( int scope, Syntax::Ref& n )
// // {    
// //     if ( !validate( scope, symbols, n->nodes[ 0 ] ) )
// //     {
// //         errmsg = SEMANTIC_ERROR( std::string("undefined '"+ n->nodes[ 0 ]->id + "'") );
// //         return false;
// //     }
// //     if ( !validate( scope, symbols, n->nodes[ 1 ] ) )
// //     {
// //         errmsg = SEMANTIC_ERROR( std::string("undefined '"+ n->nodes[ 1 ]->id + "'") );
// //         return false;
// //     }
// //     return true;
// // }
// // bool Semantic::analyzeMulEqual( int scope, Syntax::Ref& n )
// // {  
// //     if ( !validate( scope, symbols, n->nodes[ 0 ] ) )
// //     {
// //         errmsg = SEMANTIC_ERROR( std::string("undefined '"+ n->nodes[ 0 ]->id + "'") );
// //         return false;
// //     }
// //     if ( !validate( scope, symbols, n->nodes[ 1 ] ) )
// //     {
// //         errmsg = SEMANTIC_ERROR( std::string("undefined '"+ n->nodes[ 1 ]->id + "'") );
// //         return false;
// //     }
// //     return true;
// // }
// // bool Semantic::analyzeModEqual( int scope, Syntax::Ref& n )
// // {    
// //     if ( !validate( scope, symbols, n->nodes[ 0 ] ) )
// //     {
// //         errmsg = SEMANTIC_ERROR( std::string("undefined '"+ n->nodes[ 0 ]->id + "'") );
// //         return false;
// //     }
// //     if ( !validate( scope, symbols, n->nodes[ 1 ] ) )
// //     {
// //         errmsg = SEMANTIC_ERROR( std::string("undefined '"+ n->nodes[ 1 ]->id + "'") );
// //         return false;
// //     }
// //     return true;
// // }
// // bool Semantic::analyzeExpEqual( int scope, Syntax::Ref& n )
// // {    
// //     if ( !validate( scope, symbols, n->nodes[ 0 ] ) )
// //     {
// //         return false;
// //     }
// //     if ( !validate( scope, symbols, n->nodes[ 1 ] ) )
// //     {
// //         return false;
// //     }
// //     return true;
// // }

// // bool Semantic::analyzeAdd( int scope, Syntax::Ref& n )
// // {      
// //     Syntax::Ref& l = n->nodes[ 0 ];
// //     Syntax::Ref& r = n->nodes[ 1 ];
// //     if ( !validate( scope, symbols, l ) )
// //     {
// //         return false;
// //     }
// //     if ( !validate( scope, symbols, r ) )
// //     {
// //         return false;
// //     }
// //     return true;
// // }
// // bool Semantic::analyzeSub( int scope, Syntax::Ref& n )
// // {    
// //     return true;
// // }
// // bool Semantic::analyzeDiv( int scope, Syntax::Ref& n )
// // {    
// //     if ( !validate( scope, symbols, n->nodes[ 0 ] ) )
// //     {
// //         return false;
// //     }
// //     if ( !validate( scope, symbols, n->nodes[ 1 ] ) )
// //     {
// //         return false;
// //     }
// //     return true;
// // }
// // bool Semantic::analyzeMul( int scope, Syntax::Ref& n )
// // {    
// //     if ( !validate( scope, symbols, n->nodes[ 0 ] ) )
// //     {
// //         return false;
// //     }
// //     if ( !validate( scope, symbols, n->nodes[ 1 ] ) )
// //     {
// //         return false;
// //     }
// //     return true;
// // }
// // bool Semantic::analyzeMod( int scope, Syntax::Ref& n )
// // {    
// //     if ( !validate( scope, symbols, n->nodes[ 0 ] ) )
// //     {
// //         return false;
// //     }
// //     if ( !validate( scope, symbols, n->nodes[ 1 ] ) )
// //     {
// //         return false;
// //     }
// //     return true;
// // }

// // bool Semantic::analyzeExp( int scope, Syntax::Ref& n )
// // {    
// //     if ( !validate( scope, symbols, n->nodes[ 0 ] ) )
// //     {
// //         return false;
// //     }
// //     if ( !validate( scope, symbols, n->nodes[ 1 ] ) )
// //     {
// //         return false;
// //     }
// //     return true;
// // }




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

// bool analyzeDeclaration( Semantic* semantic, std::string& errmsg, int flags, int scope, Syntax::Ref& n )
// {
//     if ( !errmsg.empty() ) return false;

//     switch ( n->nodes[ 1 ]->node_type )
//     {
//         case Lexer::_HOST:
//         {
//             if ( scope != GLOBAL_SCOPE )
//             {
//                 return false;
//             }
            
//             Syntax::Ref& name = n->nodes[ 0 ];
//             Syntax::Ref& func = n->nodes[ 1 ];
//             functions.insert( scope, name->id, func );
//             return true;
//         }
//         break;

//         case Lexer::_FUNCTION:
//         {
//             return analyzeFunction( semantic, errmsg, flags, scope, n );
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
    
//         default:
//         {
//             errmsg = SEMANTIC_ERROR( std::string("unsupported '"+ n->nodes[ 1 ]->id + "' @line: " + std::to_string( n->line )) );
//             return false;
//         }
//         break;
//     }
//     return true;
// }

// bool analyzeAddress( Semantic* semantic, std::string& errmsg, int flags, int scope, Syntax::Ref& n )
// {
//     if ( !errmsg.empty() ) return false;

//     switch ( n->nodes[ 0 ]->node_type )
//     {
//         case Lexer::_DECLARATION: 
//         {
//             return analyzeDeclaration( semantic, errmsg, flags, scope, n );
//         }
//         break;

//         case Lexer::_ASSIGN:
//         {
//             return analyzeAssign( semantic, errmsg, flags, scope,n );
//         }
//         break;

//         case Lexer::_IDENTIFIER:
//         {
//         }
//         break;
    
//         default:
//         {
//             errmsg = SEMANTIC_ERROR( std::string("unsupported '"+ n->nodes[ 0 ]->id + "' @line: " + std::to_string( n->line )) );
//             return false;
//         }
//         break;
//     }
//     return true;
// }

// bool analyzeConst( Semantic* semantic, std::string& errmsg, int flags, int scope, Syntax::Ref& n )
// {
//     switch ( n->nodes[ 0 ]->node_type )
//     {
//         case Lexer::_DECLARATION:
//         {
//             return analyzeDeclaration( semantic, errmsg, CONST, scope, n->nodes[ 0 ] );
//         }

//         case Lexer::_ASSIGN:
//         {
//             return analyzeAssign( semantic, errmsg, CONST, scope, n->nodes[ 0 ] );
//         }

//         case Lexer::_ADDRESS:
//         {
//             return analyzeAddress( semantic, errmsg, CONST | ADDRESS, scope, n->nodes[ 0 ] );
//         }
    
//         default:
//         {
//             errmsg = SEMANTIC_ERROR( std::string("unsupported '"+ n->nodes[ 1 ]->id + "'") );
//         }
//         break;
//     }
//     return true;
// }

bool isCompatible( Tree& param, Tree& node )
{
    switch ( param->type )
    {
        case Lexer::_ASSIGN:
        {
            switch( param[ 1 ]->type )
            {
                case Lexer::_DOUBLE:
                case Lexer::_FLOAT:
                case Lexer::_LONG:
                case Lexer::_INT:
                {
                }
                break;

                case Lexer::_STRING:
                {
                }
                break;

                case Lexer::_BOOL:
                {
                }
                break;

                case Lexer::_FUNCTION:
                {
                }
                break;

                default:
                break;
            }
        }
        break;

        default:
        break;
    }
    return true;
}

void analyzeFunctionNode( SymbolsTable* symbols, Tree& tree, int& scope )
{

}

void analyzeAssignNode( SymbolsTable* symbols, Tree& tree, int& scope )
{
    switch ( tree[ 1 ]->type )
    {
        case Lexer::_FUNCTION:
        {
            if ( symbols->lookup( scope, tree[ 0 ]->id ) < 0 )
            {
                scope++;
                symbols->insert( scope, tree );
                analyzeFunctionNode( symbols, tree[ 1 ], scope );
            }
            else 
            {
                //THROW_SEMANTIC_ERROR( SEMANTIC_ERROR_DUPLICATE, tree[ 0 ]->line, tree->id, );
            }
        }
        break;

        case Lexer::_FUNCTION_CALL:
        {
            int index = symbols->lookup( scope, tree->id );
            if ( index >= 0 )
            {
                Tree& function = symbols->get( index );
                Tree& funccall = tree;
                
                Tree& parameters = function[ 0 ];
                if ( funccall.size() != parameters.size() )
                {
                    // for (size_t i = 0; i < parameters.size(); i++)
                    // {
                    //     Tree& param = parameters[ i ];
                    //     if ( !isCompatible( param, funccall[ i ] ) )
                    //     {
                    //         //THROW_SEMANTIC_ERROR( SEMANTIC_ERROR_NONE_COMPATIBLE_PARAM, tree->line, tree->id, );
                    //     }
                    // }
                    // THROW_SEMANTIC_ERROR( SEMANTIC_ERROR_FUNC_PARAM_SIZE, tree->line, tree->id, );
                }
            }
            else 
            {
                //THROW_SEMANTIC_ERROR( SEMANTIC_ERROR_UNDEFINE_FUNCTION, tree->line, tree->id, );
            }
        }
        break;
    }
}

bool Semantic::analyze( Syntax* syntax )
{
    Tree& tree = syntax->getTree();

    return true;
}

// bool Semantic::load( const char* filename )
// {
//     if( syntax.load( filename ) )
//     {
//         if ( syntax.parse() )
//         {
//             syntax.printTree();
            
//             Syntax::Tree* node = syntax.getTree();
//             return analyze( node );
//         }
//     }

//     syntax.error( __FILE__, __FUNCTION__, __LINE__, Lexer::_ERROR_FILE_IO, filename );
//     return false;
// }

// int Semantic::newScope()
// {
//     return ++_scope_counter;
// }

// Semantic::Semantic()
// :   _scope_counter( 0 )
// {}

