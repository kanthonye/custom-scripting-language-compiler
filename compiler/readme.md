# KESL (Kenneth Esdaile Scripting Language)

## Basic Data Type

```
int      : 32 bit integer
long     : 64 bit integer
float    : 32 bit float
double   : 64 bit float
string   : string data type
function : function data type
```

## Defining Variables:

In KESL (Kenneth Esdaile Scripting Language), there are static variables and dynamic variables. Both serve different functions in the language. Static type variables are type safe. The value can change but the type is static. It can not change. The type will remain the same through the life spand of the variable. This is useful for cases when you want to accept a specific data type. Dynamic type variables are not type safe, and can be assign any value. All though, their are way to make them type safe by apply a const before the data type as you will see.

## Declaring a static type variable.

Declaring an integer type variable.
```
num : int;
```

Declaring an integer type variable, and assigning it a default value.
```
num : int(9 * m);
```

Once a static type variable is define, if you try to change its type, this will lead to an syntax error. For e.g.,

```
num = "hellow world";
```

The variable num is a static integer, thus its type is not changable. Now it is being set to hold a string. This is not a valid operation. This is different for dynamic types tho. A static type must alway be declared with a type at creation. An identifier defined with out a type is an dynamic variable. Thus any type can be assign at any point in the program. To declear an dynamic variable. Examine the following code below, it show two ways of defining an anonimous variable. one with out a default value, and one with a default value.

```
num1;
num2 = int(14);
```

As stated, you can assign a different data-type value to a dynamic variable at any point throughout the program life spand. Examine the following code below.

```
num1 = "hellow world";
num2 = "hellow world";
```



## Defining Constant Variables:

Decaring a constant static variable type. Its type and value can not be altered.

```
const num : double(n / 678);
```

Becuse static containers data-type can not change on the fly like dynamic containers, only the container needs to be marks as constant. Lets take a look at decaring a dynamic container constant data-type.

```
num = const double(n / 678);
```

The value of the data-type with in the dynamic container can not be altered, because the data-type is marked as const, but the container can alway contain a different data-type. This is because its container is not mark as constant. To make a dynamic container fully constant, both the container and the data-type must also be marked as const. For e.g., examine the following code

```
const pi = const double(3.14);
```

Now you maybe be wondering, well what if I make the dynamic container const, and the data-type not const? 

```
const pi = double(3.14);
```

What this means is that the container data-type value of 3.14 can not be altered. For instance trying to change the floating point value from 3.14 to 0.707 like so

```
pi = 0.707;
```

This can not be done and is an error because the container is const. However, the container pi on the other hand can contain a different data-type, for instance, a string type, or an int type. Examine the following code

```
pi = string( "Hi" );
pi = int( 5 );
```


## Conversion between data types:

```
num : int( 256 );
num = 167.78;
num = "10";
```



## Defining Functions:

In the KESL functions are created with the function keyword, and store to a value which will be the identifier use to invoke the function. They can be assign to variables and passed as parameters to other functions. The declaration of a function looks simplare to the declaration of variables. For e.g., below is the declaration of a function called sum. Take note that it is a statically defined function. Functions define in the global scope should only be static. Statically defined function should not be defined in a local scope.

```
sum : function( a, b )
{
    return a + b;
}
```




## Support for both block and inline comments.
```
/*
  declearing a non static type.
 */
x: float;

// dynamic function declaration. storing function as a variable
add : function () 
{
}
```





Tree parseConst( lexer ) {
  const_node = Node( CONST );
  const_node->push( parserIdentifier() )
  return const_node;
}

Tree parserIdentifier() {
  identifier_node = Node( IDENTIFIER, id );
  switch()
  {
    case ASSIGN: assign_node->add({ identifier_node, parserRightSide() });
    case COLON: declaration_node->add({ identifier_node, parserRightSide() })
    case +=: assign_node->add({ identifier_node, parserRightSide() });
    case -=:
    case *=:
    case /=:
    case ^=:
    case %=:
    case ~=:
  }
}

Tree parserIdentifier() {
  identifier_node = Node( IDENTIFIER, id );
  switch()
  {
    case ASSIGN:
    case COLON:
    case +=:
    case -=:
    case *=:
    case /=:
    case ^=:
    case %=:
    case ~=:
  }
}
