
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_TYPE = 258,
     TOK_PRIMITIVE = 259,
     TOK_IDENT = 260,
     TOK_CONST = 261,
     TOK_STRING = 262,
     TOK_NUMBER = 263,
     TOK_CLASS = 264,
     TOK_INTERFACE = 265,
     TOK_STRUCT = 266,
     TOK_MODULE = 267,
     TOK_IMPORT = 268,
     TOK_DEF = 269,
     TOK_INIT = 270,
     TOK_DESTROY = 271,
     TOK_PUBLIC = 272,
     TOK_PRIVATE = 273,
     TOK_PROTECTED = 274,
     TOK_STATIC = 275,
     TOK_NATIVE = 276,
     TOK_WHILE = 277,
     TOK_ELSE = 278,
     TOK_UNTIL = 279,
     TOK_IF = 280,
     TOK_FOREACH = 281,
     TOK_DO = 282,
     TOK_FOR = 283,
     TOK_RETURN = 284,
     TOK_EQUAL = 285,
     TOK_NOTEQUAL = 286,
     TOK_SCOPE = 287,
     TOK_OR = 288,
     TOK_AND = 289,
     TOK_GE = 290,
     TOK_LE = 291,
     TOK_LSHIFT = 292,
     TOK_RSHIFT = 293,
     TOK_MUL_ASSIGN = 294,
     TOK_DIV_ASSIGN = 295,
     TOK_SUB_ASSIGN = 296,
     TOK_ADD_ASSIGN = 297,
     TOK_MOD_ASSIGN = 298,
     TOK_BITOR_ASSIGN = 299,
     TOK_BITAND_ASSIGN = 300,
     TOK_INC = 301,
     TOK_DEC = 302
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 12 "grammar.y"
 expr_t *expr; 

/* Line 1676 of yacc.c  */
#line 13 "grammar.y"
 stmt_t *stmt; 

/* Line 1676 of yacc.c  */
#line 14 "grammar.y"
 type_t *type; 

/* Line 1676 of yacc.c  */
#line 15 "grammar.y"
 unit_t *unit; 

/* Line 1676 of yacc.c  */
#line 16 "grammar.y"
 var_t *var; 

/* Line 1676 of yacc.c  */
#line 17 "grammar.y"
 func_t *func; 

/* Line 1676 of yacc.c  */
#line 18 "grammar.y"
 char *string; 

/* Line 1676 of yacc.c  */
#line 19 "grammar.y"
 int null; 

/* Line 1676 of yacc.c  */
#line 20 "grammar.y"
 int flag; 


/* Line 1676 of yacc.c  */
#line 135 "grammar.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif




