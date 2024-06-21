

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

/*
#define INSTR_MOV               0
#define INSTR_ADD               1
#define INSTR_SUB               2
#define INSTR_MUL               3
#define INSTR_DIV               4
#define INSTR_MOD               5
#define INSTR_EXP               6
#define INSTR_NEG               7
#define INSTR_INC               8
#define INSTR_DEC               9
#define INSTR_AND               10
#define INSTR_OR                11
#define INSTR_XOR               12
#define INSTR_NOT               13
#define INSTR_SHL               14
#define INSTR_SHR               15
#define INSTR_CONCAT            16
#define INSTR_GETCHAR           17
#define INSTR_SETCHAR           18
#define INSTR_JMP               19
#define INSTR_JE                20
#define INSTR_JNE               21
#define INSTR_JG                22
#define INSTR_JL                23
#define INSTR_JGE               24
#define INSTR_JLE               25
#define INSTR_PUSH              26
#define INSTR_POP               27
#define INSTR_CALL              28
#define INSTR_RET               29
#define INSTR_CALLHOST          30
#define INSTR_PAUSE             31
#define INSTR_EXIT              32
#define OP_TYPE_INT                 0
#define OP_TYPE_FLOAT               1
#define OP_TYPE_STRING_INDEX        2
#define OP_TYPE_VAR                 3
#define OP_TYPE_ARRAY_INDEX_ABS     4
#define OP_TYPE_ARRAY_INDEX_VAR     5
#define OP_TYPE_JUMP_TARGET_INDEX   6
#define OP_TYPE_FUNC_INDEX          7
#define OP_TYPE_REG                 9

typedef struct _Op
{
    int iType;
    union
    {
        int iIntLiteral;
        float fFloatLiteral;
        int iStringIndex;
        int iSymbolIndex;
        int iJumpTargetIndex;
        int iFuncIndex;
        int iRegCode;
    };
    int iOffset;
    int iOffsetSymbolIndex;
}
Op;

typedef struct _ICodeNode           // An I-code node
{
    int iType;                      // The node type
    union
    {
        ICodeInstr Instr;
        int iJumpTargetIndex;
    }; 
}
ICodeNode;

#define ICODE_NODE_INSTR        0
#define ICODE_NODE_JUMP_TARGET  1







typedef struct _ICodeInstr
{
    int iOpcode;
    LinkedList OpList;
}
ICodeInstr;


#define SCOPE_GLOBAL        0
#define SYMBOL_TYPE_VAR     0
#define SYMBOL_TYPE_PARAM   1
#define MAX_IDENT_SIZE      64

typedef struct _SymbolNode          // A symbol table node
{
    int iIndex;                     // Index
    char pstrIdent[ MAX_IDENT_SIZE ];      // Identifier
    int iSize;
    int iScope;
    int iType;
}
SymbolNode;


typedef struct _ScriptHeader
{
    int iStackSize;
    int iIsMainFuncPresent;
    int iMainFuncIndex;
    int iPriorityType;
    int iUserPriority;
}
ScriptHeader;

int AddSymbol ( char * pstrIdent, int iSize, int iScope, int iType )
{
    // If a label already exists
    if ( GetSymbolByIdent ( pstrIdent, iScope ) )
return -1;
    // Create a new symbol node
    SymbolNode * pNewSymbol = ( SymbolNode * )
        malloc ( sizeof ( SymbolNode ) );
    // Initialize the new label
    strcpy ( pNewSymbol->pstrIdent, pstrIdent );
    pNewSymbol->iSize = iSize;
    pNewSymbol->iScope = iScope;
    pNewSymbol->iType = iType;
    // Add the symbol to the list and get its index
    int iIndex = AddNode ( & g_SymbolTable, pNewSymbol );
    // Set the symbol node's index
    pNewSymbol->iIndex = iIndex;
    // Return the new symbol's index
    return iIndex;
}
SymbolNode * GetSymbolByIdent ( char * pstrIdent, int iScope )
{
    // Local symbol node pointer
    SymbolNode * pCurrSymbol;
    // Loop through each symbol in the table to find the match
    for ( int iCurrSymbolIndex = 0;
          iCurrSymbolIndex < g_SymbolTable.iNodeCount;
          ++ iCurrSymbolIndex )
    {
    // Get the current symbol structure
        pCurrSymbol = GetSymbolByIndex ( iCurrSymbolIndex );
    // Return the symbol if the identifier and scope matches
        if ( pCurrSymbol && stricmp ( pCurrSymbol->pstrIdent, pstrIdent )
            == 0 && pCurrSymbol->iScope == iScope )
            return pCurrSymbol;
}
    // The symbol was not found, so return a NULL pointer
    return NULL;
}

SymbolNode * GetSymbolByIndex ( int iIndex )
{
    // If the table is empty, return a NULL pointer
    if ( ! g_SymbolTable.iNodeCount )
return NULL;
    // Create a pointer to traverse the list
    LinkedListNode * pCurrNode = g_SymbolTable.pHead;
    // Traverse the list until the matching structure is found
    for ( int iCurrNode = 0; iCurrNode < g_SymbolTable.iNodeCount; ++ iCurrNode )
    {
        // Create a pointer to the current symbol structure
        SymbolNode * pCurrSymbol = ( SymbolNode * ) pCurrNode->pData;
        // If the indexes match, return the symbol
        if ( iIndex == pCurrSymbol->iIndex )
            return pCurrSymbol;
        // Otherwise move to the next node
        pCurrNode = pCurrNode->pNext;
    }
    // The symbol was not found, so return a NULL pointer
    return NULL;
}


int GetSizeByIdent ( char * pstrIdent, int iScope )
{
    // Get the symbol's information
    SymbolNode * pSymbol = GetSymbolByIdent ( pstrIdent, iScope );
    // Return its size
    return pSymbol->iSize;
}

typedef struct _FuncNode            // A function table node
{
    char pstrName[ MAX_IDENT_SIZE ];   // Name
    int iIndex;                     // Index
    int iIsHostAPI; // Is this a host API function?
    int iParamCount; // The number of accepted parameters
    LinkedList ICodeStream; // Local I-code stream
}
FuncNode;

int AddFunc ( char * pstrName, int iIsHostAPI )
{
    // If a function already exists with the specified name,
    // exit and return an invalid index
    if ( GetFuncByName ( pstrName ) )
        return -1;
    // Create a new function node
    FuncNode * pNewFunc = ( FuncNode * ) malloc ( sizeof ( FuncNode ) );
    // Set the function's name
    strcpy ( pNewFunc->pstrName, pstrName );
    // Add the function to the list and get its index, but add
    // one since the zero index is reserved for the global scope
    int iIndex = AddNode ( & g_FuncTable, pNewFunc ) + 1;
    // Set the function node's index
    pNewFunc->iIndex = iIndex;
    // Set the host API flag
    pNewFunc->iIsHostAPI = iIsHostAPI;
    // Set the parameter count to zero
    pNewFunc->iParamCount = 0;
    // Clear the function's I-code block
    pNewFunc->ICodeStream.iNodeCount = 0;
    // If the function was _Main (), set its flag and index in the header
    if ( stricmp ( pstrName, MAIN_FUNC_NAME ) == 0 )
    {
        g_ScriptHeader.iIsMainFuncPresent = TRUE;
        g_ScriptHeader.iMainFuncIndex = iIndex;
    }
    // Return the new function's index
    return iIndex;
}


FuncNode * GetFuncByName ( char * pstrName )
{
    // Local function node pointer
    FuncNode * pCurrFunc;
    // Loop through each function in the table to find the match
    for ( int iCurrFuncIndex = 1; iCurrFuncIndex <= g_FuncTable.iNodeCount;
        ++ iCurrFuncIndex )
    {
    // Get the current function structure
        pCurrFunc = GetFuncByIndex ( iCurrFuncIndex );
        // Return the function if the name matches
        if ( pCurrFunc && stricmp ( pCurrFunc->pstrName, pstrName ) == 0 )
           return pCurrFunc;
    }
    // The function was not found, so return a NULL pointer
    return NULL;
}

FuncNode * GetFuncByIndex ( int iIndex )
{
    // If the table is empty, return a NULL pointer
    if ( ! g_FuncTable.iNodeCount )
return NULL;
    // Create a pointer to traverse the list
    LinkedListNode * pCurrNode = g_FuncTable.pHead;
    // Traverse the list until the matching structure is found
    for ( int iCurrNode = 1; iCurrNode <= g_FuncTable.iNodeCount;
        ++ iCurrNode )
    {
    // Create a pointer to the current function structure
        FuncNode * pCurrFunc = ( FuncNode * ) pCurrNode->pData;
    // If the indexes match, return the current pointer
        if ( iIndex == pCurrFunc->iIndex )
            return pCurrFunc;
    // Otherwise move to the next node
        pCurrNode = pCurrNode->pNext;
}
    // The function was not found, so return a NULL pointer
    return NULL;
}
void SetFuncParamCount ( int iIndex, int iParamCount )
{
    // Get the function
    FuncNode * pFunc = GetFuncByIndex ( iIndex );
    // Set the parameter count
    pFunc->iParamCount = iParamCount;
}


ScriptHeader g_ScriptHeader;
void Init () {
    // ---- Initialize the script header
    g_ScriptHeader.iIsMainFuncPresent = false;
    g_ScriptHeader.iStackSize = 0;
    g_ScriptHeader.iPriorityType = PRIORITY_NONE;
    // ---- Initialize the main settings
    // Mark the assembly file for deletion
      g_iPreserveOutputFile = false;
      // Generate an .XSE executable
      g_iGenerateXSE = true;
      // Initialize the source code list
      InitLinkedList ( & g_SourceCode );
      // Initialize the tables
      InitLinkedList ( & g_FuncTable );
      InitLinkedList ( & g_SymbolTable );
      InitLinkedList ( & g_StringTable );
}

void ShutDown ()
{
    // Free the source code
    FreeLinkedList ( & g_SourceCode );
    // Free the tables
    FreeLinkedList ( & g_FuncTable );
    FreeLinkedList ( & g_SymbolTable );
    FreeLinkedList ( & g_StringTable );
}
*/


// Syntax Analysis
// syntax errors
// abstract syntax tree
// Semantic Analysis:





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

    std::cout << "\n\n\n";

    Syntax syntax;
    if( !syntax.analyze( &parser ) )
    {
        return 0;
    }   
    syntax.printTree();

    return 0;
}
