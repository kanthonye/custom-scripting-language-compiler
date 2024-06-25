#include "syntax.hpp"

enum
{
    SYNTAX_ERROR_UNDEFINED_IDENTIFIER,
    SYNTAX_ERROR_UNKNOWN_NODE_TYPE,
    SYNTAX_ERROR_UNKNOWN_DATA_TYPE,
};
void throwSyntaxError( int error_code, int line, const char* msg, const char* funct )
{
    std::string errmsg;
    errmsg += std::string( funct ) + "()";
    switch ( error_code )
    {
        case SYNTAX_ERROR_UNDEFINED_IDENTIFIER:
        {
            errmsg += std::string( ": UNDEFINED_IDENTIFIER -> " ) + msg;
            errmsg += std::string( ": at line# " ) + std::to_string( line );
        }
        break;

        case SYNTAX_ERROR_UNKNOWN_DATA_TYPE:
        {
            errmsg += std::string( ": UNKNOWN_DATA_TYPE -> " ) + msg;
            errmsg += std::string( ": at line# " ) + std::to_string( line );
        }
        break;

        case SYNTAX_ERROR_UNKNOWN_NODE_TYPE:
        {
            errmsg += std::string( " does not yet support NODE_TYPE -> " ) + msg;
            errmsg += std::string( ": line# " ) + std::to_string( line ) + " in script file";
        }
        break;

        default:
        break;
    }
    throw std::runtime_error( errmsg );
}
#define THROW_SYNTAX_ERROR( code, line, msg ) throwSyntaxError( code, line, msg, __FUNCTION__ )


Tree analyzeScopeBlock( Tree& node );
Tree analyzeOperation( Tree& operand );
Tree getIdentifier( Tree& declaration );
Tree analyzeFunction( Tree& function );
Tree analyzeHost( Tree& function );
Tree analyzeNode( Tree& node );
Tree getValue( Tree& node );

Tree analyzeIf( Tree& node )
{
    Tree loop = new Tree::Node( Lexer::_IF );
    loop->push( analyzeOperation( node[ 0 ][ 0 ] ) );
    loop->push( analyzeNode( node[ 1 ] ) );
    return loop;
}

Tree analyzeElse( Tree& node )
{
    Tree stmt;
    if ( node[ 0 ]->node_type == Lexer::_IF )
    {
        stmt = analyzeIf( node[ 0 ] );
        stmt->node_type = Lexer::_ELSE_IF;
    }
    else
    {
        stmt = new Tree::Node( Lexer::_ELSE );
        stmt->push( analyzeNode( node[ 0 ] ) );
    }
    return stmt;
}

Tree analyzeForLoopCondition( Tree& node )
{
    Tree conditions = new Tree::Node( Lexer::_CONDITIONS );

    Tree params = new Tree::Node(0, Lexer::_PARAMETERS);
    for(int k = 0; k < node[ 0 ].size(); k++)
    {
        params->push( analyzeNode( node[ 0 ][ k ] ) );
    }
    conditions->push( params );

    for(int i = 1; i < node.size(); i++)
    {
        params = new Tree::Node(0, Lexer::_PARAMETERS);
        for(int k = 0; k < node[ i ].size(); k++)
        {
            params->push( analyzeOperation( node[ i ][ k ] ) );
        }
        conditions->push( params );
    }
    return conditions;
}

Tree analyzeForLoop( Tree& node )
{
    Tree loop = new Tree::Node( Lexer::_FOR );
    loop->push( analyzeForLoopCondition( node[ 0 ] ) );
    loop->push( analyzeNode( node[ 1 ] ) );
    return loop;
}

Tree analyzeWhileLoop( Tree& node )
{
    Tree loop = new Tree::Node( Lexer::_WHILE );
    loop->push( analyzeOperation( node[ 0 ] ) );
    loop->push( analyzeNode( node[ 1 ] ) );
    return loop;
}

Tree analyzeScopeBlock( Tree& node )
{
    Tree block = new Tree::Node(0, Lexer::_SCOPE_BLOCK);
    for( uint32_t i = 0; i < node.size(); i++ )
    {
        block->push( analyzeNode( node[ i ] ) );
    }
    return block;
}

Tree analyzeParameter( Tree& node )
{
    Tree var;
    switch ( node->node_type )
    {
        case Lexer::_IDENTIFIER:
        {
            var = getIdentifier( node );
        }
        break;

        case Lexer::_DECLARATION:
        {
            var = new Tree::Node( Lexer::_VAR_STATIC, node->nodes[ 0 ]->id );
            var->push( analyzeOperation( node->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_ASSIGN:
        {
            var = new Tree::Node( Lexer::_VAR, node->nodes[ 0 ]->id );
            var->push( analyzeOperation( node->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_ADDRESS:
        {
            var = new Tree::Node( Lexer::_ADDRESS );
            var = analyzeParameter( node->nodes[ 0 ] );
        }
        break;

        case Lexer::_CONST:
        {
            var = analyzeParameter( node->nodes[ 0 ] );
        }
        break;

        default:
        {
            THROW_SYNTAX_ERROR
            ( 
                SYNTAX_ERROR_UNKNOWN_NODE_TYPE, 
                node->line, Lexer::toString( node->node_type ) 
            );
        }
        break;
    }
    return var;
}

Tree analyzeParameters( Tree& node )
{
    Tree parameters = new Tree::Node( node->line, Lexer::_ENCLOSE_PARENTHESIS );
    for( int i = 0; i < node->nodes.size(); i++ )
    {
        parameters->push( analyzeParameter( node->nodes[ i ] ) );
    }
    return parameters;
}

Tree analyzeFunction( Tree& function )
{
    Tree function_definition = new Tree::Node();
    function_definition->node_type = Lexer::_FUNCTION; 
    function_definition->push( analyzeParameters( function->nodes[ 0 ] ) );
    function_definition->push( analyzeScopeBlock( function->nodes[ 1 ] ) );
    return function_definition;
}

Tree analyzeHost( Tree& function )
{
    Tree function_definition = new Tree::Node();
    function_definition->node_type = Lexer::_HOST; 
    function_definition->push( analyzeParameters( function->nodes[ 0 ] ) );
    return function_definition;
}

Tree getIdentifier( Tree& node )
{
    Tree var = new Tree::Node();
    if ( node->nodes.empty() )
    {
        var->node_type = Lexer::_VAR;
        var->id = node->id;
    }
    else
    {
        var->node_type = node->nodes[ 0 ]->node_type;
        var->id = node->id;
    }
    return var;
}


Tree newValue( Lexer::Token node_type, Tree node )
{
    Tree var = new Tree::Node();
    var->node_type = node_type;
    var->id = node->id;
    if( !node->nodes.empty() )
    {
        var->push( analyzeOperation( node->nodes[ 0 ] ) );
    }
    return var;
}

Tree getValue( Tree& node )
{
    Tree var;
    switch( node->node_type )
    {
        case Lexer::_VAR_BOOL:
        case Lexer::_BOOL:
        { 
            var = newValue( Lexer::_BOOL, node );
        }
        break;

        case Lexer::_VAR_INT:
        case Lexer::_INT:
        { 
            var = newValue( Lexer::_INT, node );
        }
        break;

        case Lexer::_VAR_LONG:
        case Lexer::_LONG:
        { 
            var = newValue( Lexer::_LONG, node );
        }
        break;

        case Lexer::_VAR_FLOAT:
        case Lexer::_FLOAT:
        { 
            var = newValue( Lexer::_FLOAT, node );
        }
        break;

        case Lexer::_VAR_DOUBLE:
        case Lexer::_DOUBLE:
        { 
            var = newValue( Lexer::_DOUBLE, node );
        }
        break;

        case Lexer::_VAR_STRING:
        case Lexer::_STRING:
        { 
            var = newValue( Lexer::_STRING, node );
        }
        break;

        case Lexer::_FUNCTION:
        {   
            var = analyzeFunction( node );
        }
        break;

        case Lexer::_HOST:
        {   
            var = analyzeHost( node );
        }
        break;

        case Lexer::_ARRAY:
        { 
            var = node;
        }
        break;

        case Lexer::_CONST:
        { 
            var = getValue( node->nodes[ 0 ] );
            switch ( var->node_type )
            {
                case Lexer::_INT:
                {
                    var->node_type = Lexer::_CONST_INT;
                }
                break;

                case Lexer::_LONG:
                {   
                    var->node_type = Lexer::_CONST_LONG;
                }
                break;

                case Lexer::_FLOAT:
                {   
                    var->node_type = Lexer::_CONST_FLOAT;
                }
                break;

                case Lexer::_DOUBLE:
                {   
                    var->node_type = Lexer::_CONST_DOUBLE;
                }
                break;

                case Lexer::_STRING:
                {   
                    var->node_type = Lexer::_CONST_STRING;
                }
                break;

                case Lexer::_FUNCTION:
                {   
                    var->node_type = Lexer::_CONST_FUNCTION;
                }
                break;

                case Lexer::_ARRAY:
                {   
                    var->node_type = Lexer::_CONST_STRING;
                }
                break;

                default:
                {
                    THROW_SYNTAX_ERROR
                    ( 
                        SYNTAX_ERROR_UNKNOWN_DATA_TYPE, 
                        node->line, Lexer::toString( var->node_type ) 
                    );
                }
                break;
            }
        }
        break;

        default:
        {
            THROW_SYNTAX_ERROR
            ( 
                SYNTAX_ERROR_UNKNOWN_NODE_TYPE, 
                node->line, Lexer::toString( node->node_type ) 
            );
        }
        break;
    }
    return var;
}

Tree analyzeOperation( Tree& operand )
{
    Tree op;
    switch( operand->node_type )
    {
        default:
        {   
            THROW_SYNTAX_ERROR
            ( 
                SYNTAX_ERROR_UNKNOWN_NODE_TYPE, 
                operand->line, Lexer::toString( operand->node_type ) 
            );
        }
        break;

        case Lexer::_IDENTIFIER:
        {   
            op = getIdentifier( operand );
        }
        break;

        case Lexer::_ARRAY_INDEXED:
        {   
            op = new Tree::Node( Lexer::_VAR, operand->id );
            op->push( new Tree::Node( Lexer::_ARRAY_INDEXED, operand->nodes[ 0 ]->id ) );
        }
        break;

        case Lexer::_ADD:
        case Lexer::_SUB:
        case Lexer::_DIV:
        case Lexer::_MUL:
        case Lexer::_MOD:
        case Lexer::_OR:
        case Lexer::_OR_EQUAL:
        case Lexer::_AND:
        case Lexer::_AND_EQUAL:
        case Lexer::_LESS:
        case Lexer::_LESS_EQUAL:
        case Lexer::_GREATER:
        case Lexer::_GREATER_EQUAL:
        case Lexer::_EQUAL_EQUAL:
        {   
            op = new Tree::Node( operand->node_type );
            op->push( analyzeOperation( operand->nodes[ 0 ] ) );
            op->push( analyzeOperation( operand->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_INCREMENT:
        case Lexer::_DECREMENT:
        case Lexer::_NOT:
        case Lexer::_INVERT:
        {   
            op = new Tree::Node( operand->node_type );
            op->id = operand->nodes[ 0 ]->id;
        }
        break;

        case Lexer::_ADD_EQUAL:
        case Lexer::_SUB_EQUAL:
        case Lexer::_MUL_EQUAL:
        case Lexer::_DIV_EQUAL:
        case Lexer::_MOD_EQUAL:
        case Lexer::_EXP_EQUAL:
        case Lexer::_INVERT_EQUAL:
        {   
            Tree n = new Tree::Node( operand->node_type );
            n->push( getIdentifier( operand->nodes[ 0 ] ) );
            n->push( analyzeOperation( operand->nodes[ 1 ] ) );

            op = new Tree::Node( Lexer::_ASSIGN );
            op->push( n->nodes[ 0 ] );
            op->push( n );
        }
        break;

        case Lexer::_ENCLOSE_PARENTHESIS:
        {   
            op = new Tree::Node( Lexer::_ENCLOSE_PARENTHESIS );
            op->push( analyzeOperation( operand->nodes[ 0 ] ) );
        }
        break;

        case Lexer::_FUNCTION_CALL:
        {   
            op = new Tree::Node( Lexer::_FUNCTION_CALL );
            for (size_t i = 0; i < operand.size(); i++)
            {
                op->push( analyzeOperation( operand[ i ] ) );
            }
        }
        break;

        case Lexer::_INT:
        case Lexer::_BOOL:
        case Lexer::_LONG:
        case Lexer::_CONST:
        case Lexer::_FLOAT:
        case Lexer::_DOUBLE:
        case Lexer::_STRING:
        case Lexer::_VAR_INT:
        case Lexer::_VAR_BOOL:
        case Lexer::_VAR_LONG:
        case Lexer::_VAR_FLOAT:
        case Lexer::_VAR_DOUBLE:
        case Lexer::_VAR_STRING:
        case Lexer::_FUNCTION:
        case Lexer::_ARRAY:
        {   
            op = getValue( operand );
        }
        break;
    }
    return op;
}

Tree analyzeNode( Tree& node )
{
    Tree var;
    switch( node->node_type )
    {
        case Lexer::_IDENTIFIER:
        {   
            var = getIdentifier( node );
        }
        break;

        case Lexer::_DECLARATION:
        {   
            var = new Tree::Node( Lexer::_VAR_STATIC, node->nodes[ 0 ]->id );
            var->push( analyzeOperation( node->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_ASSIGN:
        {   
            var = new Tree::Node( Lexer::_ASSIGN );
            var->push( new Tree::Node( Lexer::_VAR, node->nodes[ 0 ]->id ) );
            var->push( analyzeOperation( node->nodes[ 1 ] ) );
        }
        break;

        case Lexer::_CONST:
        {   
            var = analyzeNode( node->nodes[ 0 ] );
            switch( var->node_type )
            {
                case Lexer::_IDENTIFIER:
                {   
                    var->node_type = Lexer::_CONST_VAR;
                }
                break;

                case Lexer::_ASSIGN:
                {   
                    var->nodes[ 0 ]->node_type = Lexer::_CONST_VAR;
                }
                break;

                case Lexer::_VAR_STATIC:
                {   
                    var->node_type = Lexer::_CONST_VAR_STATIC;
                }
                break;

                default:
                {
                    THROW_SYNTAX_ERROR
                    ( 
                        SYNTAX_ERROR_UNKNOWN_NODE_TYPE, 
                        var->line, Lexer::toString( var->node_type ) 
                    );
                }
                break;
            }
        }
        break;

        case Lexer::_FUNCTION_CALL:
        {   
            var = analyzeOperation( node );
        }
        break;

        case Lexer::_SCOPE_BLOCK:
        {
            var = analyzeScopeBlock( node );
        }
        break;

        case Lexer::_WHILE:
        {
            var = analyzeWhileLoop( node );
        }
        break;

        case Lexer::_FOR:
        {
            var = analyzeForLoop( node );
        }
        break;

        case Lexer::_IF:
        {
            var = analyzeIf( node );
        }
        break;

        case Lexer::_ELSE:
        {
            var = analyzeElse( node );
        }
        break;

        case Lexer::_CONTINUE:
        case Lexer::_BREAK:
        {
            var = node;
        }
        break;

        case Lexer::_ADD_EQUAL:
        case Lexer::_SUB_EQUAL:
        case Lexer::_MUL_EQUAL:
        case Lexer::_DIV_EQUAL:
        case Lexer::_MOD_EQUAL:
        case Lexer::_EXP_EQUAL:
        case Lexer::_INVERT_EQUAL:
        {   
            var = analyzeOperation( node );
        }
        break;

        default:
        { 
            THROW_SYNTAX_ERROR
            ( 
                SYNTAX_ERROR_UNKNOWN_NODE_TYPE, 
                node->line, Lexer::toString( node->node_type ) 
            );
        }
        break;
    }
    return var;
}

bool Syntax::analyze( Parser* parser )
{
    try
    {
        uint32_t scope = 0;
        _syntax_tree = new Tree::Node();
        Tree parse_tree = parser->getTree();
        for( uint32_t i = 0; i < parse_tree->nodes.size(); i++ )
        { 
            Tree& node = parse_tree->nodes[ i ];
            _syntax_tree->push( analyzeNode( node ) );
        }
    }
    catch(const std::runtime_error& e)
    {
        std::cerr <<"[ ERROR ] : "<< e.what() << '\n';
        return false;
    }
    
    return true;
}

void printSyntaxTree(int tabs, Tree& n)
{
    for(int i=0; i<tabs; i++) std::cout <<"- ";

    std::cout << Lexer::toString( n->node_type ) <<": " << n->id <<"\n";
    
    for(int i=0; i<n->nodes.size(); i++)
    {
        printSyntaxTree( tabs + 1, n->nodes[ i ] );
    }
}

void Syntax::printTree()
{
    if( !_syntax_tree ) return;
    std::cout <<"\n\n--- --- Syntax Analyzer --- ---\n";
    for(int i=0; i<_syntax_tree->nodes.size(); i++)
    {
        printSyntaxTree( 0, _syntax_tree->nodes[ i ] );
        std::cout <<"\n";
    }
}

Tree& Syntax::getTree()
{
    return _syntax_tree;
}

void Syntax::clear()
{
    _syntax_tree.clear();
}

Syntax::~Syntax()
{
    clear();
}

Syntax::Syntax()
{}
    