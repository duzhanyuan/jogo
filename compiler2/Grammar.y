%{
#include <Parser.h>

#define YYSTYPE ParseNode
#define YYLTYPE Location
#define YY_EXTRA_TYPE Parser*
#define YY_NO_INPUT
#define YYERROR_VERBOSE
int yylex(ParseNode *node, Location *loc, void *scanner);
void yyerror(Location *loc, Parser *parser, void *scanner, const char *message);

%}

%union { Expression *expression; }
%union { Statement *statement; }
%union { Name *identifier; }
%union { Unit *unit; }
%union { Feature *feature; }
%union { Formal *formal; }
%union { int null; }
%union { int flag; }

%pure_parser
%locations
%parse-param { Parser *parser }
%parse-param { void *scanner }
%lex-param { yyscan_t *scanner }

%destructor { delete $$; $$ = 0; } <expression>
%destructor { delete $$; $$ = 0; } <identifier>
%destructor { delete $$; $$ = 0; } <statement>
%destructor { delete $$; $$ = 0; } <unit>
%destructor { delete $$; $$ = 0; } <feature>
%destructor { delete $$; $$ = 0; } <formal>


%left '.' '['
%left INCREMENT DECREMENT
%left '!' '~'
%left '*' '/' '%'
%left '+' '-'
%left LEFT_SHIFT RIGHT_SHIFT
%left '<' '>' LESS_OR_EQUAL GREATER_OR_EQUAL
%left EQUAL NOT_EQUAL
%left '&'
%left '|' '^'
%left AND
%left OR
%left '=' MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN 
%left ADD_ASSIGN SUB_ASSIGN
%left BITAND_ASSIGN BITOR_ASSIGN
%left '?'


/* BISON declarations */
%token <name> IDENTIFIER
%token <expression> STRING NUMBER 
%token <flag> PUBLIC PRIVATE STATIC NATIVE
%token CLASS INTERFACE STRUCT MODULE
%token IMPORT DEF VAR INIT DESTROY
%token SEPARATOR
%token WHEN CASE WHILE ELSE UNTIL IF DO FOR RETURN
%token RIGHT_ARROW LEFT_ARROW
%token EQUAL NOT_EQUAL GREATER_OR_EQUAL LESS_OR_EQUAL
%token OR AND
%token LEFT_SHIFT RIGHT_SHIFT
%token MUL_ASSIGN DIV_ASSIGN SUB_ASSIGN ADD_ASSIGN
%token MOD_ASSIGN BITOR_ASSIGN BITAND_ASSIGN
%token INCREMENT DECREMENT

%type <feature> feature feature_list attribute import define
%type <feature> constructor destructor function prototype native
%type <flag> modifiers
%type <formal> formal_signature formal_list
%type <statement> block when_list when statement_list statement
%type <expression> expression expression_list storage assignment


/* The Standard Apollo Grammar, version 2010 */
%%
unit
    : CLASS IDENTIFIER '{' class_feature_list '}' {
        $$ = new Class(@$, $2, $4);    
    }
    | INTERFACE IDENTIFIER '{' interface_feature_list '{' {
        $$ = new Interface(@$, $2, $4);    
    }
    | STRUCT IDENTIFIER '{' struct_feature_list '}' {
        $$ = new Structure(@$, $2, $4);    
    }
    | MODULE IDENTIFIER '{' module_feature_list '}' {
        $$ = new Module(@$, $2, $4);    
    }
	| /* empty is an error */ { 
		yyerror(&@$, parser, scanner, "Input file is empty"); 
		YYERROR;
	}
	| error { }
    ;

feature_list
    : feature feature_list { 
        $$ = $1;
        $$->next($2);
    }
    | feature {
        $$ = $1;
    }
    ;

feature
    : import { $$ = $1; }
    | define { $$ = $1; }
    | attribute { $$ = $1; }
    | constructor { $$ = $1; }
    | destructor { $$ = $1; }
    | function { $$ = $1; }
	| native { $$ = $1; }
	| prototype { $$ = $1; }
    ;

import
    : IMPORT IDENTIFIER SEPARATOR { 
		$$ = new Import(@$, $2);
	}
    ;

define
    : DEF type IDENTIFIER SEPARATOR { 
		$$ = new Define(@$, $2, $1);
	}
    ;

attribute
    : VAR IDENTIFIER ':' type '=' expression SEPARATOR {
		// TODO: Set symbol table for class-level
		$$ = new Attribute(@$, $2, $4, $6);
    }
    | VAR IDENTIFIER ':' type SEPARATOR {
		$$ = new Attribute(@$, $2, $4, 0);
    }
	;

constructor
    : DEF INIT formal_signature block { 
        Name* name = parser->environment()->name("@init");
        $$ = new Function(@$, name, $3, 0, $4);
	}
    ;

destructor
    : DEF DESTROY '(' ')' block { 
        Name* name = parser->environment()->name("@destroy");
        $$ = new Function(@$, name, 0, 0, $5);
	}
    ;

function
    : DEF IDENTIFIER formal_signature ':' type modifiers block {
        $$ = new Function(@$, $2, $3, $5, $7);
    }
	| DEF IDENTIFIER formal_signature modifiers block {
        $$ = new Function(@$, $2, $3, $4, $5);
	}
	;

prototype
	: DEF IDENTIFIER formal_signature ':' type modifiers SEPARATOR {
        $$ = new Function(@$, $2, $3, $5, 0);
	}
	| DEF IDENTIFIER formal_signature modifiers SEPARATOR {
        $$ = new Function(@$, $2, $3, 0, 0);
	}
    ;

native
	: DEF IDENTIFIER formal_signature ':' type modifiers NATIVE SEPARATOR {
        $$ = new Function(@$, $2, $3, $5, 0);
	}
	| DEF IDENTIFIER formal_signature modifiers NATIVE SEPARATOR {
        $$ = new Function(@$, $2, $3, 0, 0);
	}
    ;
	
formal_signature
	: '(' formal_list ')' { $$ = $2; }
	| '(' ')' { $$ = 0; }
	;

formal_list
	: formal ',' formal_list { 
        $$ = $1;
        $$->next($3);
	}
	| formal { 
        $$ = $1;
	}
    ;

formal
    : IDENTIFIER ':' type {
        $$ = new Formal(@$, $1, $3);
    }

modifiers
	: PUBLIC { $$ = 0; }
	| PRIVATE { $$ = 0; }
	| STATIC { $$ = 0; }
	| PUBLIC STATIC { $$ = 0; }
	| PRIVATE STATIC { $$ = 0; }
	| /* empty */ { $$ = 0; }
	;

type
    : IDENTIFIER { $$ = $1; }
    ;

block
    : '{' statement_list '}' { 
		$$ = new Block(@$, $2); 
	}
	| RIGHT_ARROW RETURN expression SEPARATOR { 
        $$ = new Return(@$, $3); 
    }
	| RIGHT_ARROW RETURN SEPARATOR { 
        $$ = new Return(@$, 0); 
    }
	| RIGHT_ARROW expression SEPARATOR { 
		$$ = new Simple(@$, $2); 
    }
    ;

statement_list
    : statement statement_list { 
		$$ = $1;
        $$->next($1);
	}
    | statement {
        $$ = $1;
    }
	| error statement_list { 
        $$ = $2; 
    }
    ;


statement
	: FOR IDENTIFIER ':' type LEFT_ARROW expression block {
		$$ = new For(@$, $2, $4, $5, $6);
	}
	| UNTIL expression block {
        // TODO: FIX UNTIL
        Name* op = parser->environment()->name("!");
		$$ = new While(@$, new Unary(@$, name, $2), $3);
        
	}
	| WHILE expression block {
		$$ = new While(@$, $2, $3);
	}
    | CASE expression '{' when_list '}' {
        $$ = new Case(@$, $2, $4);
    }
    | VAR IDENTIFIER ':' type { 
		$$ = new Variable(@$, $2, $4, 0); 
	}
    | VAR IDENTIFIER ':' type '=' expression SEPARATOR { 
        $$ = new Variable(@$, $2, $4, $6); 
	}  
	| RETURN expression SEPARATOR { 
        $$ = new Return(@$, $2);
    }
	| RETURN SEPARATOR { 
        $$ = new Return(@$, 0);
    }
    | expression SEPARATOR { 
		$$ = new Simple(@$, $1); 
	}
    | assignment SEPARATOR { 
		$$ = new Simple(@$, $1); 
	}
	| IF expression block {
		$$ = new Conditional(@$, $2, $3, 0);
	}
    | IF expression block ELSE block {
        $$ = new Conditional(@$, $2, $3, $5);
    }
	;

storage
    : expression '.' IDENTIFIER { 
		/* Member variable access */
		$$ = new Member(@$, $1, $3); 
	}
    | expression '[' expression ']' { 
		$$ = new Index(@$, $1, $3);
	}

assignment
    : storage '=' expression { 
		$$ = new Binary(@$, parser->environment()->name("="), $1, $3); 
        $$ = new Binary(@$, parser->environment()->name("="), $1, $3);
	}
    | storage MUL_ASSIGN expression { 
		$$ = new Binary(@$, parser->environment()->name("*="), $1, $3); 
	}
    | storage DIV_ASSIGN expression { 
		$$ = new Binary(@$, parser->environment()->name("/="), $1, $3); 
	}
    | storage MOD_ASSIGN expression { 
		$$ = new Binary(@$, parser->environment()->name("%="), $1, $3); 
	}
    | storage SUB_ASSIGN expression { 
		$$ = new Binary(@$, parser->environment()->name("-="), $1, $3); 
	}
    | storage ADD_ASSIGN expression { 
		$$ = new Binary(@$, parser->environment()->name("+="), $1, $3); 
	}
    | storage BITAND_ASSIGN expression { 
		$$ = new Binary(@$, parser->environment()->name("&="), $1, $3); 
	}
    | storage BITOR_ASSIGN expression { 
		$$ = new Binary(@$, parser->environment()->name("|="), $1, $3); 
	}
    ;


expression
	: expression '?' expression { 
		$$ = new Binary(@$, parser->environment()->name("?"), $1, $3);
	}
    | expression OR expression { 
		$$ = new Binary(@$, parser->environment()->name("||"), $1, $3); 
	}
    | expression AND expression { 
		$$ = new Binary(@$, parser->environment()->name("&&"), $1, $3); 
	}
    | expression '|' expression { 
		$$ = new Binary(@$, parser->environment()->name("|"), $1, $3); 
	}
    | expression '^' expression { 
		$$ = new Binary(@$, parser->environment()->name("^"), $1, $3); 
	}
    | expression '&' expression { 
		$$ = new Binary(@$, parser->environment()->name("&"), $1, $3); 
	}
    | expression EQUAL expression { 
		$$ = new Binary(@$, parser->environment()->name("=="), $1, $3); 
	}
    | expression NOT_EQUAL expression { 
		$$ = new Binary(@$, parser->environment()->name("!="), $1, $3); 
	}
    | expression '>' expression { 
		$$ = new Binary(@$, parser->environment()->name(">"), $1, $3); 
	}
    | expression '<' expression { 
		$$ = new Binary(@$, parser->environment()->name("<"), $1, $3); 
	}
    | expression GREATER_OR_EQUAL expression { 
		$$ = new Binary(@$, parser->environment()->name(">="), $1, $3); 
	}
    | expression LESS_OR_EQUAL expression { 
		$$ = new Binary(@$, parser->environment()->name("<="), $1, $3); 
	}
    | expression LEFT_SHIFT expression { 
		$$ = new Binary(@$, parser->environment()->name("<<"), $1, $3); 
	}
    | expression RIGHT_SHIFT expression { 
		$$ = new Binary(@$, parser->environment()->name(">>"), $1, $3); 
	}
    | expression '+' expression { 
		$$ = new Binary(@$, parser->environment()->name("+"), $1, $3); 
	}
    | expression '-' expression { 
		$$ = new Binary(@$, parser->environment()->name("-"), $1, $3); 
	}
    | expression '*' expression { 
		$$ = new Binary(@$, parser->environment()->name("*"), $1, $3); 
	}
    | expression '/' expression { 
		$$ = new Binary(@$, parser->environment()->name("/"), $1, $3); 
	}
    | expression '%' expression { 
		$$ = new Binary(@$, parser->environment()->name("%"), $1, $3); 
	}
    | '!' expression { 
        $$ = new Unary(@$, parser->environment()->name("!"), $2); 
    }
    | '~' expression { 
        $$ = new Unary(@$, parser->environment()->name("~"), $2); 
    }
    | expression INCREMENT { 
		$$ = new Unary(@$, parser->environment()->name("++"), $1); 
	}
    | expression DECREMENT { 
		$$ = new Unary(@$, parser->environment()->name("--"), $1); 
	}
    | IDENTIFIER '(' expression_list ')' {
		$$ = new Call(@$, $1, $3);
    }
	| IDENTIFIER '(' ')' {
		/* Call on an object expression */
		$$ = new Call(@$, $1, 0);
	}
    | expression '.' IDENTIFIER '(' expression_list ')' {
        $1->next($5);
        $$ = new Dispatch(@$, $3, $1);
    }
    | expression '.' IDENTIFIER '(' ')' {
        $$ = new Dispatch(@$, $3, $1);
    }
    | storage { $$ = $1; }
	| '(' expression ')' { $$ = $2; } 
    | STRING { $$ = $1; }
    | NUMBER { $$ = $1; }
	| IDENTIFIER { $$ = new Identifier(@$, $1); }
    ;

expression_list
	: expression ',' expression_list { 
		$$ = $1;
        $$->next($3);
	}
	| expression { $$ = $1; } 
	;


when_list
    : when when_list {
        $$ = $1
        $$->next($2);
    }
    | when {
        $$ = $1;
    }
    ;

when
    : WHEN IDENTIFIER ':' type block {
        $$ = new When(@$, $1, $4, $5);
    }
    ;


