

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

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>


#define INSTRUCTION \
IstrEnum( jeq ), \
IstrEnum( jlt ), \
IstrEnum( jgt ), \
IstrEnum( jle ), \
IstrEnum( jge ), \
IstrEnum( jne ), \
IstrEnum( jmp ), \
IstrEnum( mov ), \
IstrEnum( call ), \
IstrEnum( push ), \
IstrEnum( halt ), \
IstrEnum( add ), \
IstrEnum( div ), \
IstrEnum( mul ), \
IstrEnum( sub ), \
IstrEnum( exp ), \
IstrEnum( mod ), \
IstrEnum( ret ), \
IstrEnum( neg ), \
IstrEnum( cmp ), \
IstrEnum( var ), \
IstrEnum( funct ), \
\
IstrEnum( gt ), \
IstrEnum( lt ), \
IstrEnum( gte ), \
IstrEnum( lte ), \
IstrEnum( eq ), \
IstrEnum( neq ), \
IstrEnum( and ), \
IstrEnum( or ), \
IstrEnum( not ), \

#define IstrEnum(x) _##x

    enum Instr{ INSTRUCTION };

#undef IstrEnum

#define IstrEnum( s ) #s
const char* toString( Instr i )
{
    static const char *str[] = { INSTRUCTION };
    return str[ i ];
};



class CodeGenerator {
public:

    bool generate( Syntax* syntax );
    CodeGenerator();

private:

    void write( Instr instr, const std::string& dst, const std::string& src1, const std::string& src2 );
    void write( Instr instr, const std::string& l, const std::string& r );
    void write( Instr instr, const std::string& l );
    void write( const std::string& l );

    std::string generateExpression( Tree &node );
    std::string generateLogicalOperation( Tree &node );
    std::string generateLogicalLessEqual( Tree &node );
    std::string generateLogicalNotEqual( Tree &node );
    std::string generateLogicalGreater( Tree &node );
    std::string generateLogicalEqual( Tree &node );
    std::string generateLogicalLess( Tree &node );
    std::string generateFunctionCall(Tree &node);
    std::string generateArrayIndex( Tree &node );
    std::string generateIdentifier( Tree &node );
    std::string generateArraySize( Tree &node );
    std::string generateArray( Tree &node );
    std::string generateAddEqual( Tree &node );
    std::string generateSubEqual( Tree &node );
    std::string generateMulEqual( Tree &node );
    std::string generateDivEqual( Tree &node );
    std::string generateExpEqual( Tree &node );
    std::string generateModEqual( Tree &node );
    std::string generateAdd( Tree &node );
    std::string generateSub( Tree &node );
    std::string generateMul( Tree &node );
    std::string generateDiv( Tree &node );
    std::string generateMod( Tree &node );
    std::string generateExp( Tree &node );

    void generateFunction( const std::string& id, Tree &node );
    void generateSwitch( Tree &node );
    void generateAssign( Tree &node );
    void generateScope( Tree &node );
    void generateWhile( Tree &node );
    void generateFor( Tree &node );
    void generateIf( Tree &node );
    void traverse( Tree &node );


    std::string newLabel() {
        return std::to_string( _label_count++ ) + "_LABEL";
    }

    std::string newLoopLabel() {
        return std::to_string( _label_count++ ) + "_LOOP";
    }

    std::string newTempVar() {
        return std::to_string( _temp_var_count++ ) + "_VAR";
    }

    std::string newTempFunc() {
        return std::to_string( _temp_var_count++ ) + "_FUNCT" ;
    }

private:

    std::ostringstream output;
    uint32_t _label_count;
    uint32_t _temp_var_count;
    std::stack<std::string> _break_labels;
    std::stack<std::string> _continue_labels;
};

void CodeGenerator::write( Instr instr, const std::string& dst, const std::string& src1, const std::string& src2 )
{
    printf( "%8s %8s %8s %8s\n", toString( instr ), dst.c_str(), src1.c_str(), src2.c_str() );
}
void CodeGenerator::write( Instr instr, const std::string& l, const std::string& r )
{
    printf( "%8s %8s %8s\n", toString( instr ), l.c_str(), r.c_str() );
}
void CodeGenerator::write( Instr instr, const std::string& l )
{
    printf( "%8s %8s\n", toString( instr ), l.c_str() );
}
void CodeGenerator::write( const std::string& l )
{
    printf( "%8s\n", l.c_str() );
}

std::string CodeGenerator::generateExpression( Tree &node ) 
{
    switch (node->type) 
    {
        case Lexer::_INT:
            return node->id;
            
        case Lexer::_FLOAT:
            return node->id;
            
        case Lexer::_LONG:
            return node->id;
            
        case Lexer::_DOUBLE:
            return node->id;
            
        case Lexer::_BOOL:
            return node->id;
            
        case Lexer::_STRING:
            return ("`" + node->id + "`");
            
        case Lexer::_IDENTIFIER:
            return generateIdentifier( node );

        case Lexer::_ADD:
            return generateAdd( node );

        case Lexer::_SUB:
            return generateSub( node );

        case Lexer::_MUL:
            return generateMul( node );

        case Lexer::_DIV:
            return generateDiv( node );

        case Lexer::_MOD:
            return generateMod( node );

        case Lexer::_EXP:
            return generateExp( node );

        case Lexer::_ADD_EQUAL:
            return generateAddEqual( node );

        case Lexer::_SUB_EQUAL:
            return generateSubEqual( node );

        case Lexer::_MUL_EQUAL:
            return generateMul( node );

        case Lexer::_DIV_EQUAL:
            return generateDivEqual( node );

        case Lexer::_MOD_EQUAL:
            return generateModEqual( node );

        case Lexer::_EXP_EQUAL:
            return generateExpEqual( node );

        case Lexer::_ENCLOSE_PARENTHESIS:
            return generateExpression( node[ 0 ] );

        case Lexer::_FUNCTION_CALL:
            return generateFunctionCall( node );

        case Lexer::_ARRAY:
            return generateArray( node );

        case Lexer::_ARRAY_SIZE:
            return generateArraySize( node );

        case Lexer::_ARRAY_INDEXED:
            return generateArrayIndex( node );

        case Lexer::_TRUE:
        {
            std::string result = newTempVar();
            write( _mov, result, "1" );
            return result;
        }

        case Lexer::_FALSE:
        {
            std::string result = newTempVar();
            write( _mov, result, "0" );
            return result;
        }

        case Lexer::_INCREMENT:
        {
            write( _add, node[ 0 ]->id, "1" );
            return node[ 0 ]->id;
        }

        case Lexer::_DECREMENT:
        {
            write( _sub, node[ 0 ]->id, "1" );
            return node[ 0 ]->id;
        }
        
        case Lexer::_NOT:
        {
            write( _cmp, node->id, "0" );
        }
        break;

        case Lexer::_GREATER_EQUAL:
        case Lexer::_EQUAL_EQUAL:
        case Lexer::_LESS_EQUAL:
        case Lexer::_GREATER:
        case Lexer::_LESS:
        case Lexer::_NOT_EQUAL:
        case Lexer::_AND:
        case Lexer::_OR:
            return generateLogicalOperation( node );

        default:
            throw std::runtime_error("Unknown expression type.");
    }
    return "";
}

std::string CodeGenerator::generateLogicalOperation( Tree &node )
{
    std::string left = generateExpression( node[ 0 ] );
    std::string right = generateExpression( node[ 1 ] );
    std::string result = newTempVar();

    switch ( node->type )
    {
        case Lexer::_EQUAL_EQUAL:
        {
            write( _eq, result, left, right );
        }
        break;

        case Lexer::_LESS:
        {
            write( _lt, result, left, right );
        }
        break;

        case Lexer::_GREATER:
        {
            write( _gt, result, left, right );
        }
        break;

        case Lexer::_LESS_EQUAL:
        {
            write( _lte, result, left, right );
        }
        break;

        case Lexer::_GREATER_EQUAL:
        {
            write( _gte, result, left, right );
        }
        break;

        case Lexer::_NOT_EQUAL:
        {
            write( _neq, result, left, right );
        }
        break;

        case Lexer::_OR:
        {
            write( _or, result, left, right );
        }
        break;

        case Lexer::_AND:
        {
            write( _and, result, left, right );
        }
        break;
    
        default: // !val
            throw std::runtime_error("Unknown logical operation.");
        break;
    }
    return result;
}

std::string CodeGenerator::generateIdentifier( Tree &node )
{
    if ( node->empty() )
    {
        
    }
    
    return node->id;
}

std::string CodeGenerator::generateArrayIndex( Tree &node )
{
    // Assuming node[0] is the array identifier and node[1] is the index expression
    std::string temp = newTempVar();
    if( node[ 0 ]->type != Lexer::_IDENTIFIER )
    {
        std::string index_expression = generateExpression( node[ 0 ] );
        write( _mov, temp, node->id + "[" + index_expression + "]" );
    }
    else 
    {
        write( _mov, temp, node->id + "[" + node[ 0 ]->id + "]" );
    }
    return temp;
}

std::string CodeGenerator::generateArraySize( Tree &node )
{
    // Generate code for the expression representing the array size
    return generateExpression( node[ 0 ] );
}

std::string CodeGenerator::generateArray( Tree &node )
{
    // Get the size of the array
    std::string array_size = generateArraySize( node[ 0 ] );

    // Allocate memory for the array (simplified: using malloc or a similar function)
    std::string temp = newTempVar();
    write( _mov, "rax", array_size );
    write( _call, "malloc" );
    write( _mov, temp, "rax" );

    return temp;
}

std::string CodeGenerator::generateFunctionCall(Tree &node) 
{
    // Assume function call is structured as FUNCTION_CALL with children as arguments
    std::vector< std::string > arguments;
    
    // Generate code for each argument and collect them
    for (size_t i = 0; i < node.size(); i++) 
    {
        arguments.push_back( generateExpression( node[ i ] ));
    }
    
    // Push arguments to the stack or pass them to registers
    for (size_t i = 0; i < arguments.size(); i++) 
    {
        write( _push, arguments[ i ] );
    }

    // Call the function
    write( _call, node->id );

    // Handle the return value (assuming it's in RAX)
    std::string temp = newTempVar();
    write( _mov, temp, "rax" );

    // Return the temporary variable holding the return value
    return temp;
}

std::string CodeGenerator::generateAddEqual(Tree &node) 
{
    std::string temp_var = generateAdd( node );
    write( _mov, node[ 0 ]->id, temp_var );
    return node[ 0 ]->id;
}

std::string CodeGenerator::generateSubEqual(Tree &node) 
{
    std::string temp_var = generateSub( node );
    write( _mov, node[ 0 ]->id, temp_var );
    return node[ 0 ]->id;
}

std::string CodeGenerator::generateMulEqual(Tree &node) 
{
    std::string temp_var = generateMul( node );
    write( _mov, node[ 0 ]->id, temp_var );
    return node[ 0 ]->id;
}

std::string CodeGenerator::generateDivEqual(Tree &node) 
{
    std::string temp_var = generateDiv( node );
    write( _mov, node[ 0 ]->id, temp_var );
    return node[ 0 ]->id;
}

std::string CodeGenerator::generateModEqual(Tree &node) 
{
    std::string temp_var = generateMod( node );
    write( _mov, node[ 0 ]->id, temp_var );
    return node[ 0 ]->id;
}

std::string CodeGenerator::generateExpEqual(Tree &node) 
{
    std::string temp_var = generateExp( node );
    write( _mov, node[ 0 ]->id, temp_var );
    return node[ 0 ]->id;
}

std::string CodeGenerator::generateAdd(Tree &node) 
{
    std::string lhs = generateExpression( node[ 0 ] );
    std::string rhs = generateExpression( node[ 1 ] );
    std::string temp_var = newTempVar();
    write( _mov, temp_var, lhs );
    write( _add, temp_var, rhs );
    return temp_var;
}

std::string CodeGenerator::generateSub(Tree &node) 
{
    std::string lhs = generateExpression( node[ 0 ] );
    std::string rhs = generateExpression( node[ 1 ] );
    std::string temp_var = newTempVar();
    write( _mov, temp_var, lhs );
    write( _sub, temp_var, rhs );
    return temp_var;
}

std::string CodeGenerator::generateMul(Tree &node) 
{
    std::string lhs = generateExpression( node[ 0 ] );
    std::string rhs = generateExpression( node[ 1 ] );
    std::string temp_var = newTempVar();
    write( _mov, temp_var, lhs );
    write( _mul, temp_var, rhs );
    return temp_var;
}

std::string CodeGenerator::generateDiv(Tree &node) 
{
    std::string lhs = generateExpression( node[ 0 ] );
    std::string rhs = generateExpression( node[ 1 ] );
    std::string temp_var = newTempVar();
    write( _mov, temp_var, lhs );
    write( _div, temp_var, rhs );
    return temp_var;
}

std::string CodeGenerator::generateMod(Tree &node) 
{
    std::string lhs = generateExpression( node[ 0 ] );
    std::string rhs = generateExpression( node[ 1 ] );
    std::string temp_var = newTempVar();
    write( _mov, temp_var, lhs );
    write( _mod, temp_var, rhs );
    return temp_var;
}

std::string CodeGenerator::generateExp(Tree &node) 
{
    std::string lhs = generateExpression( node[ 0 ] );
    std::string rhs = generateExpression( node[ 1 ] );
    std::string temp_var = newTempVar();
    write( _mov, temp_var, lhs );
    write( _exp, temp_var, rhs );
    return temp_var;
}

void CodeGenerator::generateFunction(const std::string& id, Tree &node) 
{
    //std::cout << "Function " << id << ":\n";
    write( _funct, id );
    traverse( node[ 0 ] );
    generateScope( node[ 1 ] );
    write( "End Function" );
}

void CodeGenerator::generateSwitch(Tree &node) 
{
    std::unordered_map< int, std::string > case_labels;
    std::string switchExpr = generateExpression(node[0]);
    std::string default_label;

    for (size_t i = 1; i < node.size(); ++i) 
    {
        if ( node[ i ]->type == Lexer::_SWITCH_CASE ) 
        {
            int caseValue = std::stoi( node[ i ][ 0 ]->id );
            case_labels[caseValue] = newLabel();
        } 
        else if (node[ i ]->type == Lexer::_SWITCH_DEFAULT) 
        {
            default_label = newLabel();
        }
    }

    std::unordered_map< int, std::string >::iterator m = case_labels.begin();
    for (; m != case_labels.end(); m++) 
    {
        write( _cmp, switchExpr, std::to_string( m->first ) );
        write( _jeq, m->second );
    }
    write( _jmp, ( default_label.empty() ? newLabel() : default_label ) );

    for (size_t i = 1; i < node.size(); ++i) 
    {
        if (node[ i ]->type == Lexer::_SWITCH_CASE ) 
        {
            int case_value = std::stoi( node[ i ][ 0 ]->id );
            write( case_labels[ case_value ] );
            traverse(node[ i ][ 1 ]);
        } 
        else if ( node[ i ]->type == Lexer::_SWITCH_DEFAULT ) 
        {
            write( default_label );
            traverse( node[ i ][ 0 ] );
        }
    }
}

void CodeGenerator::generateAssign( Tree &node )
{
    Tree& var = node[ 0 ];
    Tree& val = node[ 1 ];
    std::string temp_var = generateExpression( val );
    write( _mov, var->id, temp_var );
}

void CodeGenerator::generateScope( Tree &node ) 
{
    for ( int i = 0; i < node.size(); i++ ) 
    {
        traverse( node[ i ] );
    }
}

void CodeGenerator::generateWhile( Tree &node ) 
{
    std::string label_start = newLoopLabel();
    std::string label_end = newLoopLabel();

    _break_labels.push( label_end );
    _continue_labels.push( label_start );

    write( label_start );

    Tree& cond = node[ 0 ];
    // Check if the condition is a constant true
    if (cond[ 0 ]->type == Lexer::_BOOL && cond[0]->id == "true") 
    {
        // Generate code for an infinite loop
        // Loop body
        generateScope( node[ 1 ] );
        write( _jmp, label_start );
    } 
    else 
    {
        // Generate code for the condition
        std::string result = generateExpression( cond[ 0 ] );
        write( _cmp, result );
        write( _jeq, label_end );

        // Loop body
        generateScope(node[1]);
        write( _jmp, label_start );
    }

    write( label_end );

    _break_labels.pop();
    _continue_labels.pop();
}

void CodeGenerator::generateFor(Tree &node) 
{
    std::string loop_start = newLoopLabel();
    std::string loop_end = newLoopLabel();
    _continue_labels.push( loop_start );
    _break_labels.push( loop_end );

    Tree& condition = node[ 0];
    Tree& body = node[ 1 ];

    traverse( condition[ 0 ][ 0 ] );

    write( loop_start );
    generateExpression( condition[ 1 ][ 0 ] );
    write( _jge, loop_end );

    traverse( body );

    generateExpression( condition[ 2 ][ 0 ] );
    write( _jmp, loop_start );
    write( loop_end );

    _continue_labels.pop();
    _break_labels.pop();
}

void CodeGenerator::generateIf( Tree &node ) 
{
    Tree& stmt = node[ 0 ];
    std::string label_else = newLabel();
    std::string label_end = newLabel();

    // Assume node[0] is the condition node
    std::string condition_result = generateExpression( stmt[ 0 ][ 0 ] );
    write( _cmp, condition_result, "0" );// Check if the condition is false (0)
    write( _jeq, label_else ); // Jump to else label if condition is false

    // Generate code for the true branch
    generateScope( stmt[ 1 ] );

    write( _jmp, label_end ); // Jump to end after true branch
    write( label_else );

    for(int i= 1; i < node.size(); i++ )
    {
        if ( node[ i ]->type == Lexer::_ELSE )
        {
            if ( node[ i ][ 0 ]->type == Lexer::_IF )
            {
                generateIf( node[ i ] );
            }
            // Generate code for the else branch
            else
            {
                generateScope( node[ i ] );
            }
        }
    }

    write( label_end );
}

void CodeGenerator::traverse( Tree &node ) 
{
    switch ( node->type ) 
    {
        case Lexer::_ASSIGN:
        {
            generateAssign( node );
        }
        break;

        case Lexer::_DECLARATION:
        {
            switch ( node[ 1 ]->type )
            {
                case Lexer::_FUNCTION:
                {
                    generateFunction( node[ 0 ]->id, node[ 1 ] );
                }
                break;

                case Lexer::_FLOAT:
                {
                    Tree& var = node[ 1 ];
                    if( var->empty() )
                    {
                        write( _var, node[ 0 ]->id, "F32" );
                    }
                    else 
                    {
                        std::string temp = generateExpression( var[ 0 ] );
                        write( _var, node[ 0 ]->id, "F32" );
                        write( _mov, node[ 0 ]->id, temp );
                    }
                }
                break;

                case Lexer::_DOUBLE:
                {
                    write( _var, node[ 0 ]->id, "F64" );
                }
                break;

                case Lexer::_INT:
                {
                    write( _var, node[ 0 ]->id, "I32" );
                }
                break;

                case Lexer::_LONG:
                {
                    write( _var, node[ 0 ]->id, "I64" );
                }
                break;

                case Lexer::_STRING:
                {
                    write( _var, node[ 0 ]->id, "STR" );
                }
                break;

                case Lexer::_BOOL:
                {
                    write( _var, node[ 0 ]->id, "BOL" );
                }
                break;
            
                default:
                {
                    generateAssign( node );
                }
                break;
            }
        }
        break;

        case Lexer::_ADD_EQUAL:
        {
            std::string res = generateAdd( node );
            write( _mov, node[ 0 ]->id, res );
        }
        break;

        case Lexer::_SUB_EQUAL:
        {
            std::string res = generateSub( node );
            write( _mov, node[ 0 ]->id, res );
        }
        break;

        case Lexer::_MUL_EQUAL:
        {
            std::string res = generateMul( node );
            write( _mov, node[ 0 ]->id, res );
        }
        break;

        case Lexer::_DIV_EQUAL:
        {
            std::string res = generateDiv( node );
            write( _mov, node[ 0 ]->id, res );
        }
        break;

        case Lexer::_MOD_EQUAL:
        {
            std::string res = generateMod( node );
            write( _mov, node[ 0 ]->id, res );
        }
        break;

        case Lexer::_EXP_EQUAL:
        {
            std::string res = generateExp( node );
            write( _mov, node[ 0 ]->id, res );
        }
        break;

        case Lexer::_INCREMENT:
        {
            write( _add, node[ 0 ]->id, "1" );
        }

        case Lexer::_DECREMENT:
        {
            write( _sub, node[ 0 ]->id, "1" );
        }

        // case Lexer::_NOT_EQUAL:
        // {
        //     std::string res = generateNotEqual( node );
        //     std::cout << "MOV " << node[ 0 ]->id << ", " << res << "\n";
        // }
        // break;

        // case Lexer::_OR_EQUAL:
        // {
        //     std::string res = generateOrEqual( node );
        //     std::cout << "MOV " << node[ 0 ]->id << ", " << res << "\n";
        // }
        // break;

        // case Lexer::_AND_EQUAL:
        // {
        //     std::string res = generateAndEqual( node );
        //     std::cout << "MOV " << node[ 0 ]->id << ", " << res << "\n";
        // }
        // break;

        case Lexer::_CONST:
        {
            traverse( node[ 0 ] );
        }
        break;

        case Lexer::_FUNCTION:
        {
            //generateFunction( node );
        }
        break;

        case Lexer::_IF_STATEMENT:
        {
            generateIf( node );
        }
        break;

        case Lexer::_FOR:
        {
            generateFor( node );
        }
        break;

        case Lexer::_WHILE:
        {
            generateWhile( node );
        }
        break;

        case Lexer::_SWITCH:
        {
            generateSwitch( node );
        }
        break;

        case Lexer::_SCOPE_BLOCK:
        {
            generateScope( node );
        }
        break;

        case Lexer::_FUNCTION_CALL:
        {
            generateFunctionCall( node );
        }
        break;

        case Lexer::_PARAMETER:
        {
            generateFunctionCall( node );
        }
        break;

        case Lexer::_IDENTIFIER:
        {
            std::cout << "var " << node->id << "\n";
            write( _var, node->id );
        }
        break;

        case Lexer::_BREAK:
        {
            if ( _break_labels.empty() )
            {
                throw std::runtime_error( "invalid-operation: 'break' requires a loops." );
            }
            write( _jmp, _break_labels.top() );
        }
        break;

        case Lexer::_CONTINUE:
        {
            if ( _break_labels.empty() )
            {
                throw std::runtime_error( "invalid-operation: 'continue' requires a loops." );
            }
            write( _jmp, _continue_labels.top() );
        }
        break;

        default:
        {
            for ( int i = 0; i < node.size(); i++ ) 
            {
                traverse( node[ i ] );
            }
        }
        break;
    }
}

CodeGenerator::CodeGenerator()
:   _temp_var_count( 0 )
,   _label_count( 0 )
{}

bool CodeGenerator::generate( Syntax* syntax )
{
    try
    {
        traverse( syntax->getTree() );
    }
    catch( const std::runtime_error& e )
    {
        std::cerr <<"[ ERROR ]: " << e.what() << '\n';
    }
    return true;
}


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

    CodeGenerator code_generator;
    if( !code_generator.generate( &syntax ) )
    {
        return 0;
    }

    return 0;
}
