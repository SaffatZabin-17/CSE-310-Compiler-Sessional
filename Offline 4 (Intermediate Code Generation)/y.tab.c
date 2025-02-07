/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 6 "1905060.y"

#include<iostream>
#include<cctype>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<bits/stdc++.h>
#include<typeinfo>
#include "symbolTable.hpp"

using namespace std;

int yyparse(void);
int yylex(void);
extern FILE *yyin;

extern symbolTable st;

extern ofstream logOut;
ofstream parseOut;
ofstream asmOut;
ofstream codeSegOut;
ofstream optimizedAsmOut;
extern ofstream errorOut;

string variable;

extern int lineCount;
int errorCount = 0;

vector<symbolInfo*> parameter_list;
vector<symbolInfo*> declaration_list;
vector<symbolInfo*> definition_list;
vector<symbolInfo*> symbolList;
vector<symbolInfo*> arg_list;

bool variable_defined = false;
bool wrong_specifier = false;
bool func_declared = false;

string type_specified, final_type_specified;
string func_name, final_func_name;
string var_name, final_var_name;
string func_type, final_func_type;
string exp_type, final_exp_type;
string param_name, param_type;

//Variables and functions for ICG

vector <symbolInfo*> global_var_list;

bool globalScope = true;
bool arrayIndex = false;

int labelCount = 0, boolValue, varCount = 0;

stack<string> ifTerminateStack;
stack<string> elseTerminateStack;

string mainTerminateLabel;
string ifTerminateLabel, elseTerminateLabel, shortCircuitLabel;
string forLoopEndLabel, forLoopUpdateLabel, forLoopConditionLabel, forLoopStatementLabel;
string whileLoopStartLabel, whileLoopEndLabel;

void optimizeCode(){

    ifstream codeSegmentIn("assembly.asm");

    //ofstream optimizeOut("optimized_assembly.asm", ios::out);
    vector<string> lines;
    string line;
    while (getline(codeSegmentIn, line))
    {
        lines.push_back(line);
    }

    for (int i = 0; i < lines.size(); i++)
    {
        if (i + 1 >= lines.size() || lines[i].size() < 4 || lines[i + 1].size() < 4)
        {
        }
        else if (lines[i].substr(1, 3) == "MOV" && lines[i + 1].substr(1, 3) == "MOV")
        {
            string line1 = lines[i].substr(4);
            string line2 = lines[i + 1].substr(4);

            int delIndex1 = line1.find(",");
            int delIndex2 = line2.find(",");

            if (line1.substr(1, delIndex1 - 1) == line2.substr(delIndex2 + 2))
                if (line1.substr(delIndex1 + 2) == line2.substr(1, delIndex2 - 1))
                {
                    optimizedAsmOut << "\t; Redundant MOV optimized" << endl;
                    i++;
                    continue;
                }
        }

		else if(lines[i].substr(1,4) == "PUSH" && lines[i+1].substr(1,4) == "POP"){
			string line1 = lines[i].substr(5);
			string line2 = lines[i+1].substr(4);

			if(line1 == line2){
				optimizedAsmOut << "\t; Redundant PUSH/POP optimized" << endl;
				i++;
				continue;
			}
		}

		else if(lines[i].substr(1,4) == "PUSH" && lines[i+1].substr(1,3) == "MOV"){
			string line1 = lines[i].substr(5);
			string line2 = lines[i+1].substr(4);

			int delIndex2 = line2.find(",");

			if(line1 == line2.substr(delIndex2+2)){
				optimizedAsmOut << "\t; Redundant PUSH/MOV optimized" << endl;
				i++;
				continue;
			}
		}

		else if(lines[i].substr(1,3) == "MOV" && lines[i+1].substr(1,4) == "PUSH"){
			string line1 = lines[i].substr(4);
			string line2 = lines[i+1].substr(5);

			int delIndex1 = line1.find(",");

			if(line1.substr(delIndex1+2) == line2){
				optimizedAsmOut << "\t; Redundant MOV/PUSH optimized" << endl;
				i++;
				continue;
			}
		}

		else if(lines[i].substr(1,3) == "MOV" && lines[i+1].substr(1,3) == "ADD"){
			string line1 = lines[i].substr(4);
			string line2 = lines[i+1].substr(4);

			int delIndex1 = line1.find(",");
			int delIndex2 = line2.find(",");

			if(line1.substr(1,delIndex1-1) == line2.substr(delIndex2+2)){
				if(line1.substr(delIndex1+2) == line2.substr(1,delIndex2-1)){
					optimizedAsmOut << "\t; Redundant MOV/ADD optimized" << endl;
					i++;
					continue;
				}
			}
		}

        optimizedAsmOut << lines[i] << endl;
    }

    optimizedAsmOut.close();
    codeSegmentIn.close();
}

string generateNewLabel(){
	return "L" + to_string(labelCount++);
}

string generateNewLabel(string label){
	return label + to_string(labelCount++);
}

void printNewLine(){
	string str = "\n\tPRINT_NEWLINE PROC\r"
                 "\n"
                 "        ; PRINTS A N"
                 "EW LINE WITH CARRIAG"
                 "E RETURN\r\n"
                 "        PUSH AX\r\n"
                 "        PUSH DX\r\n"
                 "        MOV AH, 2\r"
                 "\n"
                 "        MOV DL, 0Dh"
                 "\r\n"
                 "        INT 21h\r\n"
                 "        MOV DL, 0Ah"
                 "\r\n"
                 "        INT 21h\r\n"
                 "        POP DX\r\n"
                 "        POP AX\r\n"
                 "        RET\r\n"
                 "    PRINT_NEWLINE EN"
                 "DP\n";
    codeSegOut << str << endl;
}

void printFromBX(){
	char *str = "\tPRINT_NUM_FROM_BX PR"
                "OC\r\n"
                "    \tPUSH CX  \r\n"
                "    \t; push to stack "
                "to \r\n"
                "\t    ; check the end "
                "of the number  \r\n"
                "\t    MOV AX, \'X\'\r"
                "\n"
                "\t    PUSH AX\r\n"
                "    \r\n"
                "\t    CMP BX, 0  \r\n"
                "\t    JE ZERO_NUM\r\n"
                "\t    JNL NON_NEGATIVE"
                " \r\n"
                "    \r\n"
                "\t    NEG BX\r\n"
                "\t    ; print - for ne"
                "gative number\r\n"
                "\t    MOV DL, \'-\'\r"
                "\n"
                "\t    MOV AH, 2\r\n"
                "\t    INT 21H\r\n"
                "\t    JMP NON_NEGATIVE"
                "  \r\n"
                "    \r\n"
                "\t    ZERO_NUM:\r\n"
                "\t\t        MOV DX, 0\r"
                "\n"
                "\t\t        PUSH DX\r\n"
                "\t\t        JMP POP_PRIN"
                "T_LOOP\r\n"
                "    \r\n"
                "\t    NON_NEGATIVE:\r"
                "\n"
                "    \r\n"
                "\t\t    MOV CX, 10 \r\n"
                "    \r\n"
                "\t\t    MOV AX, BX\r\n"
                "\t    PRINT_LOOP:\r\n"
                "\t        ; if AX == 0"
                "\r\n"
                "\t        CMP AX, 0\r"
                "\n"
                "\t        JE END_PRINT"
                "_LOOP\r\n"
                "\t        ; else\r\n"
                "\t        MOV DX, 0 ; "
                "DX:AX = 0000:AX\r\n"
                "        \r\n"
                "\t        ; AX = AX / "
                "10 ; store reminder "
                "in DX \r\n"
                "\t        DIV CX\r\n"
                "    \r\n"
                "\t        PUSH DX\r\n"
                "        \r\n"
                "\t        JMP PRINT_LO"
                "OP\r\n\r\n"
                "\t    END_PRINT_LOOP:"
                "\r\n"
                "    \r\n"
                "    \r\n"
                "    \r\n"
                "\t    POP_PRINT_LOOP:"
                "\r\n"
                "\t        POP DX\r\n"
                "\t        ; loop endin"
                "g condition\r\n"
                "\t        ; if DX == "
                "\'X\'\r\n"
                "\t        CMP DX, \'X"
                "\'\r\n"
                "\t        JE END_POP_P"
                "RINT_LOOP\r\n"
                "        \r\n"
                "\t        ; if DX == "
                "\'-\'\r\n"
                "\t        CMP DX, \'-"
                "\'\r\n"
                "\t        JE PRINT_TO_"
                "CONSOLE\r\n"
                "        \r\n"
                "\t        ; convert to"
                "ascii\r\n"
                "\t        ADD DX, 30H "
                "      \r\n"
                "\t        ; print the "
                "digit\r\n"
                "\t        PRINT_TO_CON"
                "SOLE:\r\n"
                "\t\t        MOV AH, 2\r"
                "\n"
                "\t\t        INT 21H\r\n"
                "        \r\n"
                "\t\t        JMP POP_PRIN"
                "T_LOOP\r\n"
                "    \r\n"
                "\t    END_POP_PRINT_LO"
                "OP: \r\n"
                "\t\t\tCALL PRINT_NEWLINE\r\n"
                "\t\t    POP CX\r\n"
                "\t\t    RET\r\n"
                "\tPRINT_NUM_FROM_BX EN"
                "DP";
    codeSegOut << str << endl;
}

symbolInfo* startSymbol;

void yyerror(char *s)
{
	
}

void init(){
	st.setBucketSize(11);
}

void insertIntoTable(string symbolName, string symbolType, string symbolID){

	symbolInfo* temp = new symbolInfo(symbolName, symbolType);
	temp->setSymbolID(symbolID);

	for(int i=0;i<parameter_list.size();i++){
		temp->parameter_list.push_back(parameter_list[i]);
	}

	st.insert(*temp);
}

void printTree(symbolInfo* symbol, int depth){
	for(int i=0;i<depth;i++){
		parseOut << " ";	
	}
	if(!symbol->isLeaf){
		parseOut << symbol->getSymbolClass() << " \t<Line: " << symbol->getStartLine() << "-" << symbol->getEndLine() << ">" << endl; 
	}
	else{
		parseOut << symbol->getSymbolClass() << "\t<Line: " << symbol->getStartLine() << ">" << endl;
		return; 
	}
	vector<symbolInfo*> temp = symbol->getChildList();

	for(int i=0;i<temp.size();i++){
		printTree(temp[i], depth+1);
	}
}


#line 413 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "1905060.y"

	#include<bits/stdc++.h>

#line 452 "y.tab.c"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IF = 258,                      /* IF  */
    ELSE = 259,                    /* ELSE  */
    FOR = 260,                     /* FOR  */
    WHILE = 261,                   /* WHILE  */
    INT = 262,                     /* INT  */
    FLOAT = 263,                   /* FLOAT  */
    VOID = 264,                    /* VOID  */
    RETURN = 265,                  /* RETURN  */
    ADDOP = 266,                   /* ADDOP  */
    MULOP = 267,                   /* MULOP  */
    INCOP = 268,                   /* INCOP  */
    RELOP = 269,                   /* RELOP  */
    ASSIGNOP = 270,                /* ASSIGNOP  */
    LOGICOP = 271,                 /* LOGICOP  */
    NOT = 272,                     /* NOT  */
    LPAREN = 273,                  /* LPAREN  */
    RPAREN = 274,                  /* RPAREN  */
    LCURL = 275,                   /* LCURL  */
    RCURL = 276,                   /* RCURL  */
    LSQUARE = 277,                 /* LSQUARE  */
    RSQUARE = 278,                 /* RSQUARE  */
    COMMA = 279,                   /* COMMA  */
    SEMICOLON = 280,               /* SEMICOLON  */
    CONST_INT = 281,               /* CONST_INT  */
    CONST_FLOAT = 282,             /* CONST_FLOAT  */
    ID = 283,                      /* ID  */
    PRINTLN = 284,                 /* PRINTLN  */
    LOWER_THAN_ELSE = 285          /* LOWER_THAN_ELSE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IF 258
#define ELSE 259
#define FOR 260
#define WHILE 261
#define INT 262
#define FLOAT 263
#define VOID 264
#define RETURN 265
#define ADDOP 266
#define MULOP 267
#define INCOP 268
#define RELOP 269
#define ASSIGNOP 270
#define LOGICOP 271
#define NOT 272
#define LPAREN 273
#define RPAREN 274
#define LCURL 275
#define RCURL 276
#define LSQUARE 277
#define RSQUARE 278
#define COMMA 279
#define SEMICOLON 280
#define CONST_INT 281
#define CONST_FLOAT 282
#define ID 283
#define PRINTLN 284
#define LOWER_THAN_ELSE 285

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 348 "1905060.y"

	symbolInfo *symbol;

#line 536 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IF = 3,                         /* IF  */
  YYSYMBOL_ELSE = 4,                       /* ELSE  */
  YYSYMBOL_FOR = 5,                        /* FOR  */
  YYSYMBOL_WHILE = 6,                      /* WHILE  */
  YYSYMBOL_INT = 7,                        /* INT  */
  YYSYMBOL_FLOAT = 8,                      /* FLOAT  */
  YYSYMBOL_VOID = 9,                       /* VOID  */
  YYSYMBOL_RETURN = 10,                    /* RETURN  */
  YYSYMBOL_ADDOP = 11,                     /* ADDOP  */
  YYSYMBOL_MULOP = 12,                     /* MULOP  */
  YYSYMBOL_INCOP = 13,                     /* INCOP  */
  YYSYMBOL_RELOP = 14,                     /* RELOP  */
  YYSYMBOL_ASSIGNOP = 15,                  /* ASSIGNOP  */
  YYSYMBOL_LOGICOP = 16,                   /* LOGICOP  */
  YYSYMBOL_NOT = 17,                       /* NOT  */
  YYSYMBOL_LPAREN = 18,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 19,                    /* RPAREN  */
  YYSYMBOL_LCURL = 20,                     /* LCURL  */
  YYSYMBOL_RCURL = 21,                     /* RCURL  */
  YYSYMBOL_LSQUARE = 22,                   /* LSQUARE  */
  YYSYMBOL_RSQUARE = 23,                   /* RSQUARE  */
  YYSYMBOL_COMMA = 24,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 25,                 /* SEMICOLON  */
  YYSYMBOL_CONST_INT = 26,                 /* CONST_INT  */
  YYSYMBOL_CONST_FLOAT = 27,               /* CONST_FLOAT  */
  YYSYMBOL_ID = 28,                        /* ID  */
  YYSYMBOL_PRINTLN = 29,                   /* PRINTLN  */
  YYSYMBOL_LOWER_THAN_ELSE = 30,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_YYACCEPT = 31,                  /* $accept  */
  YYSYMBOL_start = 32,                     /* start  */
  YYSYMBOL_program = 33,                   /* program  */
  YYSYMBOL_unit = 34,                      /* unit  */
  YYSYMBOL_func_declaration = 35,          /* func_declaration  */
  YYSYMBOL_func_definition = 36,           /* func_definition  */
  YYSYMBOL_embedded = 37,                  /* embedded  */
  YYSYMBOL_embedded_out_def = 38,          /* embedded_out_def  */
  YYSYMBOL_embedded_out_dec = 39,          /* embedded_out_dec  */
  YYSYMBOL_parameter_list = 40,            /* parameter_list  */
  YYSYMBOL_compound_statement = 41,        /* compound_statement  */
  YYSYMBOL_embedded_in = 42,               /* embedded_in  */
  YYSYMBOL_var_declaration = 43,           /* var_declaration  */
  YYSYMBOL_type_specifier = 44,            /* type_specifier  */
  YYSYMBOL_id = 45,                        /* id  */
  YYSYMBOL_declaration_list = 46,          /* declaration_list  */
  YYSYMBOL_statements = 47,                /* statements  */
  YYSYMBOL_statement = 48,                 /* statement  */
  YYSYMBOL_49_1 = 49,                      /* $@1  */
  YYSYMBOL_50_2 = 50,                      /* $@2  */
  YYSYMBOL_51_3 = 51,                      /* $@3  */
  YYSYMBOL_52_4 = 52,                      /* $@4  */
  YYSYMBOL_53_5 = 53,                      /* $@5  */
  YYSYMBOL_54_6 = 54,                      /* $@6  */
  YYSYMBOL_55_7 = 55,                      /* $@7  */
  YYSYMBOL_56_8 = 56,                      /* $@8  */
  YYSYMBOL_embedded_exp = 57,              /* embedded_exp  */
  YYSYMBOL_embedded_void = 58,             /* embedded_void  */
  YYSYMBOL_embedded_stmt = 59,             /* embedded_stmt  */
  YYSYMBOL_embedded_else = 60,             /* embedded_else  */
  YYSYMBOL_expression_statement = 61,      /* expression_statement  */
  YYSYMBOL_variable = 62,                  /* variable  */
  YYSYMBOL_expression = 63,                /* expression  */
  YYSYMBOL_logic_expression = 64,          /* logic_expression  */
  YYSYMBOL_65_9 = 65,                      /* $@9  */
  YYSYMBOL_rel_expression = 66,            /* rel_expression  */
  YYSYMBOL_simple_expression = 67,         /* simple_expression  */
  YYSYMBOL_term = 68,                      /* term  */
  YYSYMBOL_unary_expression = 69,          /* unary_expression  */
  YYSYMBOL_factor = 70,                    /* factor  */
  YYSYMBOL_argument_list = 71,             /* argument_list  */
  YYSYMBOL_arguments = 72                  /* arguments  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   164

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  81
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  138

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   285


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   366,   366,   379,   389,   400,   410,   420,   431,   483,
     537,   586,   635,   643,   693,   702,   741,   784,   817,   836,
     859,   884,   904,   986,  1007,  1028,  1050,  1059,  1118,  1190,
    1241,  1307,  1325,  1345,  1360,  1378,  1393,  1424,  1460,  1469,
    1469,  1486,  1486,  1493,  1460,  1527,  1533,  1527,  1576,  1609,
    1658,  1663,  1679,  1684,  1693,  1706,  1724,  1785,  1851,  1864,
    1943,  1955,  1955,  2017,  2029,  2085,  2097,  2156,  2168,  2274,
    2302,  2336,  2350,  2386,  2472,  2518,  2534,  2549,  2597,  2608,
    2617,  2640
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IF", "ELSE", "FOR",
  "WHILE", "INT", "FLOAT", "VOID", "RETURN", "ADDOP", "MULOP", "INCOP",
  "RELOP", "ASSIGNOP", "LOGICOP", "NOT", "LPAREN", "RPAREN", "LCURL",
  "RCURL", "LSQUARE", "RSQUARE", "COMMA", "SEMICOLON", "CONST_INT",
  "CONST_FLOAT", "ID", "PRINTLN", "LOWER_THAN_ELSE", "$accept", "start",
  "program", "unit", "func_declaration", "func_definition", "embedded",
  "embedded_out_def", "embedded_out_dec", "parameter_list",
  "compound_statement", "embedded_in", "var_declaration", "type_specifier",
  "id", "declaration_list", "statements", "statement", "$@1", "$@2", "$@3",
  "$@4", "$@5", "$@6", "$@7", "$@8", "embedded_exp", "embedded_void",
  "embedded_stmt", "embedded_else", "expression_statement", "variable",
  "expression", "logic_expression", "$@9", "rel_expression",
  "simple_expression", "term", "unary_expression", "factor",
  "argument_list", "arguments", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285
};
#endif

#define YYPACT_NINF (-65)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-15)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      49,   -65,   -65,   -65,    19,    49,   -65,   -65,   -65,   -65,
       4,   -65,   -65,   -65,    -5,    44,    39,    52,     4,   -65,
      51,    -3,    56,   -65,    63,     6,     4,    64,    57,    66,
      63,    49,   -65,    69,   -65,   -65,   -65,    57,    70,     4,
     -65,    76,   -65,   -65,   -65,    71,    80,    81,    11,    11,
      11,    11,   -65,   -65,   -65,   -65,    82,   -65,   -65,     4,
      41,   103,   -65,   -65,    60,    90,   -65,    91,    53,   104,
     -65,   -65,    11,    16,   -65,    92,   105,   -65,   -65,   100,
      94,   112,    11,    11,   -65,   -65,   -65,    11,   -65,   -65,
      11,    11,    11,   -65,   -65,    11,   -65,   -65,   106,   -65,
     107,   118,   120,   -65,    11,   104,   116,   -65,   125,   -65,
     -65,   121,   -65,    11,   -65,   -65,   -65,    16,   126,   -65,
     -65,   130,   -65,   -65,   -65,   -65,   130,   145,    11,   -65,
     -65,   -65,   130,   -65,   -65,   132,   130,   -65
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    23,    24,    25,     0,     2,     4,     6,     7,     5,
       0,     1,     3,    26,    29,     0,     0,     0,     0,    22,
       0,     0,    27,    30,    13,     0,    18,     0,     0,     0,
      13,     0,    17,     0,    21,    11,     9,     0,     0,    16,
      28,     0,    10,     8,    15,     0,     0,     0,     0,     0,
       0,     0,    20,    54,    75,    76,     0,    35,    33,     0,
      56,     0,    31,    34,    72,     0,    58,    60,    63,    65,
      67,    71,     0,     0,    45,     0,    72,    69,    70,     0,
       0,    29,    79,     0,    19,    32,    77,     0,    55,    61,
       0,     0,     0,    50,    38,     0,    48,    74,     0,    81,
       0,    78,     0,    59,     0,    66,    64,    68,     0,    39,
      50,     0,    73,     0,    57,    62,    51,     0,     0,    49,
      80,     0,    40,    46,    52,    41,     0,    36,     0,    47,
      53,    42,     0,    43,    37,     0,     0,    44
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -65,   -65,   -65,   147,   -65,   -65,   -65,   123,   124,   -65,
     -14,   -65,    55,    15,    -8,   -65,   -65,   -60,   -65,   -65,
     -65,   -65,   -65,   -65,   -65,   -65,    50,   -65,   -65,   -65,
     -64,   -42,   -48,   -61,   -65,    58,    72,    74,   -38,   -65,
     -65,   -65
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     4,     5,     6,     7,     8,    17,    28,    29,    25,
      57,    41,    58,    59,    60,    15,    61,    62,   109,   117,
     125,   128,   133,   135,    95,   126,   108,   121,   127,   132,
      63,    64,    65,    66,   104,    67,    68,    69,    70,    71,
     100,   101
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      75,    85,    14,    79,     1,     2,     3,    76,    76,    94,
      22,    77,    78,   -12,    35,    10,    24,    16,    32,    11,
      10,    99,    49,    42,    93,    30,   103,    49,    50,    51,
      31,    44,    13,    50,    51,   102,    26,    54,    55,    13,
      76,    53,    54,    55,    13,    76,    39,   110,    76,    76,
      76,    81,   120,   122,   107,     9,     1,     2,     3,    82,
       9,   124,    76,    83,    90,    20,   129,    91,    18,    19,
      21,    76,   134,    86,    23,    87,   137,    34,    27,    45,
     131,    46,    47,     1,     2,     3,    48,    49,   -14,    72,
      33,    36,    40,    50,    51,    43,    34,    52,    73,    74,
      80,    53,    54,    55,    13,    56,    45,    89,    46,    47,
       1,     2,     3,    48,    49,    88,    92,    96,    86,    97,
      50,    51,    98,    34,    84,   111,   112,    90,    53,    54,
      55,    13,    56,    45,    16,    46,    47,     1,     2,     3,
      48,    49,   113,   114,   116,   123,   119,    50,    51,   130,
      34,   136,    12,    37,    38,    53,    54,    55,    13,    56,
     118,     0,   115,   106,   105
};

static const yytype_int16 yycheck[] =
{
      48,    61,    10,    51,     7,     8,     9,    49,    50,    73,
      18,    49,    50,    18,    28,     0,    19,    22,    26,     0,
       5,    82,    11,    37,    72,    19,    87,    11,    17,    18,
      24,    39,    28,    17,    18,    83,    21,    26,    27,    28,
      82,    25,    26,    27,    28,    87,    31,    95,    90,    91,
      92,    59,   113,   117,    92,     0,     7,     8,     9,    18,
       5,   121,   104,    22,    11,    26,   126,    14,    24,    25,
      18,   113,   132,    13,    23,    15,   136,    20,    22,     3,
     128,     5,     6,     7,     8,     9,    10,    11,    25,    18,
      26,    25,    23,    17,    18,    25,    20,    21,    18,    18,
      18,    25,    26,    27,    28,    29,     3,    16,     5,     6,
       7,     8,     9,    10,    11,    25,    12,    25,    13,    19,
      17,    18,    28,    20,    21,    19,    19,    11,    25,    26,
      27,    28,    29,     3,    22,     5,     6,     7,     8,     9,
      10,    11,    24,    23,    19,    19,    25,    17,    18,     4,
      20,    19,     5,    30,    30,    25,    26,    27,    28,    29,
     110,    -1,   104,    91,    90
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     7,     8,     9,    32,    33,    34,    35,    36,    43,
      44,     0,    34,    28,    45,    46,    22,    37,    24,    25,
      26,    18,    45,    23,    19,    40,    44,    22,    38,    39,
      19,    24,    45,    26,    20,    41,    25,    38,    39,    44,
      23,    42,    41,    25,    45,     3,     5,     6,    10,    11,
      17,    18,    21,    25,    26,    27,    29,    41,    43,    44,
      45,    47,    48,    61,    62,    63,    64,    66,    67,    68,
      69,    70,    18,    18,    18,    63,    62,    69,    69,    63,
      18,    45,    18,    22,    21,    48,    13,    15,    25,    16,
      11,    14,    12,    63,    61,    55,    25,    19,    28,    64,
      71,    72,    63,    64,    65,    68,    67,    69,    57,    49,
      63,    19,    19,    24,    23,    66,    19,    50,    57,    25,
      64,    58,    61,    19,    48,    51,    56,    59,    52,    48,
       4,    63,    60,    53,    48,    54,    19,    48
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    31,    32,    33,    33,    34,    34,    34,    35,    35,
      36,    36,    37,    38,    39,    40,    40,    40,    40,    41,
      41,    42,    43,    44,    44,    44,    45,    46,    46,    46,
      46,    47,    47,    48,    48,    48,    48,    48,    49,    50,
      51,    52,    53,    54,    48,    55,    56,    48,    48,    48,
      57,    58,    59,    60,    61,    61,    62,    62,    63,    63,
      64,    65,    64,    66,    66,    67,    67,    68,    68,    69,
      69,    69,    70,    70,    70,    70,    70,    70,    71,    71,
      72,    72
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     8,     7,
       8,     7,     0,     0,     0,     4,     3,     2,     1,     4,
       3,     0,     3,     1,     1,     1,     1,     3,     6,     1,
       4,     1,     2,     1,     1,     1,     8,    11,     0,     0,
       0,     0,     0,     0,    13,     0,     0,     8,     3,     5,
       0,     0,     0,     0,     1,     2,     1,     4,     1,     3,
       1,     0,     4,     1,     3,     1,     3,     1,     3,     2,
       2,     1,     1,     4,     3,     1,     1,     2,     1,     0,
       3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* start: program  */
#line 367 "1905060.y"
        {
		logOut << "start : program" << endl;
		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolType());
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setSymbolClass("start : program");
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		startSymbol = (yyval.symbol);
	}
#line 1978 "y.tab.c"
    break;

  case 3: /* program: program unit  */
#line 379 "1905060.y"
                       {
		(yyval.symbol) = new symbolInfo((yyvsp[-1].symbol)->getSymbolType());
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setSymbolClass("program : program unit");
		(yyval.symbol)->setStartLine((yyvsp[-1].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		logOut << "program : program unit" << endl;
	}
#line 1993 "y.tab.c"
    break;

  case 4: /* program: unit  */
#line 389 "1905060.y"
                {
		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolType());
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setSymbolClass("program : unit");
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		logOut << "program : unit" << endl;
	}
#line 2007 "y.tab.c"
    break;

  case 5: /* unit: var_declaration  */
#line 400 "1905060.y"
                        {
		logOut << "unit: var_declaration" << endl;
		symbolInfo *temp = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), "unit");
		(yyval.symbol) = temp;
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->setSymbolClass("unit : var_declaration");
	}
#line 2022 "y.tab.c"
    break;

  case 6: /* unit: func_declaration  */
#line 410 "1905060.y"
                                {
		logOut << "unit : func_declaration" << endl;
		symbolInfo *temp = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), "unit");
		(yyval.symbol) = temp;
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->setSymbolClass("unit : func_declaration");
	}
#line 2037 "y.tab.c"
    break;

  case 7: /* unit: func_definition  */
#line 420 "1905060.y"
                        {
		logOut << "unit : func_definition" << endl;
		symbolInfo *temp = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), "unit");
		(yyval.symbol) = temp;
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setSymbolClass("unit : func_definition");
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
	}
#line 2052 "y.tab.c"
    break;

  case 8: /* func_declaration: type_specifier id embedded LPAREN parameter_list RPAREN embedded_out_dec SEMICOLON  */
#line 431 "1905060.y"
                                                                                                        {
		(yyvsp[-6].symbol)->setSymbolClass("ID : " + (yyvsp[-6].symbol)->getSymbolName());
		(yyvsp[-6].symbol)->isLeaf = true;

		(yyvsp[-4].symbol)->setSymbolClass("LPAREN : (");
		(yyvsp[-4].symbol)->isLeaf = true;

		(yyvsp[-2].symbol)->setSymbolClass("RPAREN : )");
		(yyvsp[-2].symbol)->isLeaf = true;

		(yyvsp[0].symbol)->setSymbolClass("SEMICOLON : ;");
		(yyvsp[0].symbol)->isLeaf = true;

		(yyval.symbol) = new symbolInfo((yyvsp[-6].symbol)->getSymbolName(), "func_declaration");
		(yyval.symbol)->setSymbolClass("func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-7].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-7].symbol));
		(yyval.symbol)->insertChild((yyvsp[-6].symbol));
		(yyval.symbol)->insertChild((yyvsp[-4].symbol));
		(yyval.symbol)->insertChild((yyvsp[-3].symbol));
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		if(st.lookUp((yyvsp[-6].symbol)->getSymbolName())){
			//Error stating function of the same name already exists
		}

		else{

			if((yyvsp[-7].symbol)->getSymbolType() == "int"){
				type_specified = "FUNCTION, INT";
			}

			else if((yyvsp[-7].symbol)->getSymbolType() == "float"){
				type_specified = "FUNCTION, FLOAT";
			}

			else if((yyvsp[-7].symbol)->getSymbolType() == "void"){
				type_specified = "FUNCTION, VOID";
			}

			symbolInfo* temp = new symbolInfo((yyvsp[-6].symbol)->getSymbolName(), type_specified);
			temp->parameter_list = parameter_list;
			temp->setSymbolClass("func_declaration");
			st.insert(*temp);
			logOut << "func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON" << endl;
		}

		parameter_list.clear();
	}
#line 2109 "y.tab.c"
    break;

  case 9: /* func_declaration: type_specifier id embedded LPAREN RPAREN embedded_out_dec SEMICOLON  */
#line 483 "1905060.y"
                                                                                {
		(yyvsp[-5].symbol)->setSymbolClass("ID : " + (yyvsp[-5].symbol)->getSymbolName());
		(yyvsp[-5].symbol)->isLeaf = true;

		(yyvsp[-3].symbol)->setSymbolClass("LPAREN : (");
		(yyvsp[-3].symbol)->isLeaf = true;

		(yyvsp[-2].symbol)->setSymbolClass("RPAREN : )");
		(yyvsp[-2].symbol)->isLeaf = true;

		(yyvsp[0].symbol)->setSymbolClass("SEMICOLON : ;");
		(yyvsp[0].symbol)->isLeaf = true;

		(yyval.symbol) = new symbolInfo((yyvsp[-5].symbol)->getSymbolName(), "func_declaration");
		(yyval.symbol)->setSymbolClass("func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-6].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-6].symbol));
		(yyval.symbol)->insertChild((yyvsp[-5].symbol));
		(yyval.symbol)->insertChild((yyvsp[-3].symbol));
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		if(st.lookUp((yyvsp[-5].symbol)->getSymbolName())){
			//Error stating function of the same name already exists
		}
		
		else{

			if((yyvsp[-6].symbol)->getSymbolType() == "int"){
				type_specified = "FUNCTION, INT";
			}

			else if((yyvsp[-6].symbol)->getSymbolType() == "float"){
				type_specified = "FUNCTION, FLOAT";
			}

			else if((yyvsp[-6].symbol)->getSymbolType() == "void"){
				type_specified = "FUNCTION, VOID";
			}

			symbolInfo* temp = new symbolInfo((yyvsp[-5].symbol)->getSymbolName(), type_specified);
			temp->setSymbolClass("func_declaration");
			temp->parameter_list = parameter_list;
			st.insert(*temp);

			logOut << "func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON" << endl;		
		}

		parameter_list.clear();
	}
#line 2166 "y.tab.c"
    break;

  case 10: /* func_definition: type_specifier id embedded LPAREN parameter_list RPAREN embedded_out_def compound_statement  */
#line 537 "1905060.y"
                                                                                                                {
		(yyvsp[-6].symbol)->setSymbolClass("ID : " + (yyvsp[-6].symbol)->getSymbolName());
		(yyvsp[-6].symbol)->isLeaf = true;

		(yyvsp[-4].symbol)->setSymbolClass("LPAREN : (");
		(yyvsp[-4].symbol)->isLeaf = true;

		(yyvsp[-2].symbol)->setSymbolClass("RPAREN : )");
		(yyvsp[-2].symbol)->isLeaf = true;

		if((yyvsp[-7].symbol)->getSymbolType() == "int"){
			func_type = "FUNCTION, INT";
		}
		else if((yyvsp[-7].symbol)->getSymbolType() == "float"){
			func_type = "FUNCTION, FLOAT";
		}
		else{
			func_type = "FUNCTION, VOID";
		}

		(yyval.symbol) = new symbolInfo((yyvsp[-6].symbol)->getSymbolName(), func_type);
		(yyval.symbol)->setSymbolClass("func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-7].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-7].symbol));
		(yyval.symbol)->insertChild((yyvsp[-6].symbol));
		(yyval.symbol)->insertChild((yyvsp[-4].symbol));
		(yyval.symbol)->insertChild((yyvsp[-3].symbol));
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		logOut << "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement" << endl;

		if(final_func_name == "main"){
			codeSegOut << "\n\t" << mainTerminateLabel << ":" << endl;
			codeSegOut << "\t\tMOV AH, 4CH" << endl;
			codeSegOut << "\t\tINT 21H" << endl;
		}
		else{
			codeSegOut << "\t\t; Line: " << lineCount << " In case no return type" << endl;
			codeSegOut << "\t\tPOP BP" << endl;
			codeSegOut << "\t\tRET" << endl;
		}

		codeSegOut << "\t" << (yyvsp[-6].symbol)->getSymbolName() << " ENDP" << endl;

	}
#line 2219 "y.tab.c"
    break;

  case 11: /* func_definition: type_specifier id embedded LPAREN RPAREN embedded_out_def compound_statement  */
#line 586 "1905060.y"
                                                                                        {
		(yyvsp[-5].symbol)->setSymbolClass("ID : " + (yyvsp[-5].symbol)->getSymbolName());
		(yyvsp[-5].symbol)->isLeaf = true;

		(yyvsp[-3].symbol)->setSymbolClass("LPAREN : (");
		(yyvsp[-3].symbol)->isLeaf = true;

		(yyvsp[-2].symbol)->setSymbolClass("RPAREN : )");
		(yyvsp[-2].symbol)->isLeaf = true;

		if((yyvsp[-6].symbol)->getSymbolType() == "int"){
			func_type = "FUNCTION, INT";
		}

		else if((yyvsp[-6].symbol)->getSymbolType() == "float"){
			func_type = "FUNCTION, FLOAT";
		}

		else{
			func_type = "FUNCTION, VOID";
		}
		
		(yyval.symbol) = new symbolInfo((yyvsp[-5].symbol)->getSymbolName(), type_specified);
		(yyval.symbol)->setSymbolClass("func_definition : type_specifier ID LPAREN RPAREN compound_statement");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-6].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-6].symbol));
		(yyval.symbol)->insertChild((yyvsp[-5].symbol));
		(yyval.symbol)->insertChild((yyvsp[-3].symbol));
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		
		logOut << "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement" << endl;

		if(final_func_name == "main"){
			codeSegOut << "\n\t" << mainTerminateLabel << ":" << endl;
			codeSegOut << "\t\tMOV AH, 4CH" << endl;
			codeSegOut << "\t\tINT 21H" << endl;
		}
		else{
			codeSegOut << "\t\t; Line: " << lineCount << " In case no return type" << endl;
			codeSegOut << "\t\tPOP BP" << endl;
			codeSegOut << "\t\tRET" << endl;
		}
		codeSegOut << "\t\t" << (yyvsp[-5].symbol)->getSymbolName() << " ENDP" << endl;
	}
#line 2271 "y.tab.c"
    break;

  case 12: /* embedded: %empty  */
#line 635 "1905060.y"
           {
		final_type_specified = func_type;
		final_func_name = func_name;
		globalScope = false;
		parameter_list.clear();
	}
#line 2282 "y.tab.c"
    break;

  case 13: /* embedded_out_def: %empty  */
#line 643 "1905060.y"
                   {
		symbolInfo* temp = st.lookUp(final_func_name);

		if(final_type_specified == "INT"){
			func_type = "FUNCTION, INT";
		}
			
		else if(final_type_specified == "FLOAT"){
			func_type = "FUNCTION, FLOAT";
		}

		else{
			func_type = "FUNCTION, VOID";
		}
		if(temp == nullptr){
			
			temp = new symbolInfo(final_func_name, func_type);
			temp->parameter_list = parameter_list;
			st.insert(*temp);

		}

		else if(temp->getSymbolType() != "FUNCTION, INT" && temp->getSymbolType() != "FUNCTION, FLOAT" && temp->getSymbolType() != "FUNCTION, VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": '" << final_func_name << "' redeclared as different kind of symbol" << endl;
		}

		else if(temp->getSymbolName() == final_func_name && temp->getSymbolType() != func_type){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Conflicting types for '" << final_func_name << "'" << endl;
		}

		else if(temp->parameter_list.size() != parameter_list.size()){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Conflicting types for '" << final_func_name << "'" << endl;
		}

		codeSegOut << "\t" << final_func_name << " PROC" << endl;
		if(final_func_name == "main"){
			mainTerminateLabel = generateNewLabel();
			codeSegOut << "\t\tMOV AX, @DATA\n\t\tMOV DS, AX";
        	codeSegOut << "\t\t; data segment loaded\n";
		}
		else{
			codeSegOut << "\t\tPUSH BP" << endl;
		}
		codeSegOut << "\t\tMOV BP, SP" << endl;
	}
#line 2335 "y.tab.c"
    break;

  case 14: /* embedded_out_dec: %empty  */
#line 693 "1905060.y"
                   {
		symbolInfo* temp = st.lookUp(final_func_name);
		if(temp != nullptr){
			
		}
	}
#line 2346 "y.tab.c"
    break;

  case 15: /* parameter_list: parameter_list COMMA type_specifier id  */
#line 702 "1905060.y"
                                                                {
		(yyvsp[-2].symbol)->setSymbolClass("COMMA : ,");
		(yyvsp[-2].symbol)->isLeaf = true;

		(yyvsp[0].symbol)->setSymbolClass("ID : " + (yyvsp[0].symbol)->getSymbolName());
		(yyvsp[0].symbol)->isLeaf = true;

		string type;
		if((yyvsp[-1].symbol)->getSymbolType() == "int"){
			type = "INT";
		}
		else if((yyvsp[-1].symbol)->getSymbolType() == "float"){
			type = "FLOAT";
		}
		else{
			//Semantic error
		}
		symbolInfo* temp = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), type);

		temp->setSymbolClass("parameter_list : parameter_list COMMA type_specifier ID");

		temp->setSymbolID("parameter");

		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("parameter_list : parameter_list COMMA type_specifier ID");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-3].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-3].symbol));
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		parameter_list.push_back(temp);

		logOut << "parameter_list : parameter_list COMMA type_specifier ID" << endl;
	}
#line 2388 "y.tab.c"
    break;

  case 16: /* parameter_list: parameter_list COMMA type_specifier  */
#line 741 "1905060.y"
                                                {
		symbolInfo* x = new symbolInfo(" ", param_type);

		(yyvsp[-1].symbol)->setSymbolClass("COMMA : ,");
		(yyvsp[-1].symbol)->isLeaf = true;

		string type;
		if((yyvsp[0].symbol)->getSymbolType() == "int"){
			type = "INT";
		}

		else if((yyvsp[0].symbol)->getSymbolType() == "float"){
			type = "FLOAT";
		}

		else{
			//Error
		}

		symbolInfo* temp = new symbolInfo((yyvsp[-2].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());

		temp->setSymbolClass("parameter_list : parameter_list COMMA type_specifier");

		temp->setSymbolID("parameter");

		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("parameter_list : parameter_list COMMA type_specifier");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-2].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		if(parameter_list.size() == 0){
			parameter_list.push_back(x);
		}
		parameter_list.push_back(temp);

		logOut << "parameter_list : parameter_list COMMA type_specifier" << endl;
	}
#line 2434 "y.tab.c"
    break;

  case 17: /* parameter_list: type_specifier id  */
#line 784 "1905060.y"
                                {
		(yyvsp[0].symbol)->setSymbolClass("ID : " + (yyvsp[0].symbol)->getSymbolName());
		(yyvsp[0].symbol)->isLeaf = true;

		string type;
		if((yyvsp[-1].symbol)->getSymbolType() == "int"){
			type = "INT";
		}
		else if((yyvsp[-1].symbol)->getSymbolType() == "float"){
			type = "FLOAT";
		}
		else{
			//Semantic error
		}
		symbolInfo* temp = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), type);
		
		temp->setSymbolClass("parameter_list : type_specifier ID");

		temp->setSymbolID("parameter");

		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("parameter_list : type_specifier ID");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-1].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		parameter_list.push_back(temp);
		logOut << "parameter_list : type_specifier ID" << endl;
	}
#line 2470 "y.tab.c"
    break;

  case 18: /* parameter_list: type_specifier  */
#line 817 "1905060.y"
                                {
		symbolInfo* temp = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		temp->setSymbolClass("parameter_list : type_specifier");

		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("parameter_list : type_specifier");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		param_type = (yyvsp[0].symbol)->getSymbolType();
		//parameter_list.push_back(temp);

		logOut << "parameter_list : type_specifier" << endl;
	}
#line 2491 "y.tab.c"
    break;

  case 19: /* compound_statement: LCURL embedded_in statements RCURL  */
#line 836 "1905060.y"
                                                        {
		(yyvsp[-3].symbol)->setSymbolClass("LCURL : {");
		(yyvsp[-3].symbol)->isLeaf = true;

		(yyvsp[0].symbol)->setSymbolClass("RCURL : }");
		(yyvsp[0].symbol)->isLeaf = true;

		symbolInfo* temp = new symbolInfo("compound_statement");
		temp->setSymbolClass("compound_statement : LCURL statements RCURL");

		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("compound_statement : LCURL statements RCURL");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-3].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-3].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		logOut << "compound_statement : LCURL statements RCURL" << endl;
		st.printAllScope(logOut);
		st.exitScope();
	}
#line 2519 "y.tab.c"
    break;

  case 20: /* compound_statement: LCURL embedded_in RCURL  */
#line 859 "1905060.y"
                                        {
		(yyvsp[-2].symbol)->setSymbolClass("LCURL : {");
		(yyvsp[-2].symbol)->isLeaf = true;

		(yyvsp[0].symbol)->setSymbolClass("RCURL : }");
		(yyvsp[0].symbol)->isLeaf = true;

		symbolInfo* temp = new symbolInfo("compound_statement");
		temp->setSymbolClass("compound_statement : LCURL statements RCURL");
		
		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("compound_statement : LCURL RCURL");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-2].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		logOut << "compound_statement : LCURL RCURL" << endl;
		st.printAllScope(logOut);
		st.exitScope();
	}
#line 2546 "y.tab.c"
    break;

  case 21: /* embedded_in: %empty  */
#line 884 "1905060.y"
              {
		st.enterScope(st.getScopeNumber() + 1);

		for(int i=0;i<parameter_list.size();i++){
			symbolInfo* temp = st.lookUpCurrent(parameter_list[i]->getSymbolName());
			if(temp != nullptr){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Redefinition of parameter '" << temp->getSymbolName() << "'" << endl;
			}
			else{
				int offset = 4 + (parameter_list.size()-i)*2;
				parameter_list[i]->setStackOffset(offset);
				parameter_list[i]->setSymbolClass("parameter_list");
				st.insert(*parameter_list[i]);
			}
		}
	}
#line 2568 "y.tab.c"
    break;

  case 22: /* var_declaration: type_specifier declaration_list SEMICOLON  */
#line 904 "1905060.y"
                                                                {
		(yyvsp[0].symbol)->setSymbolClass("SEMICOLON : ;");
		(yyvsp[0].symbol)->isLeaf = true;

		(yyval.symbol) = new symbolInfo("var_declaration");
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-2].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->setSymbolClass("var_declaration : type_specifier declaration_list SEMICOLON");
		if((yyvsp[-2].symbol)->getSymbolType() == "void"){
			for(int i=0;i<declaration_list.size();i++){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Variable or field '" << declaration_list[i]->getSymbolName() << "' declared void" << endl;
			}
		}
		
		else{
			if(!globalScope && final_func_name != "main"){
				varCount = declaration_list.size();
			}
			for(int i=0;i<declaration_list.size();i++){
				symbolInfo* x = declaration_list[i];

				string type_specified;
				string symbolID;

				symbolInfo* temp = st.lookUpCurrent(x->getSymbolName());

				if(temp != nullptr){
	
					if(temp->getSymbolType() != x->getSymbolType()){
						errorCount++;
						errorOut << "Line# " << lineCount << ": Conflicting types for '" << x->getSymbolName() << "'" << endl;
					}
				}
				else{
					if((yyvsp[-2].symbol)->getSymbolType() == "int"){
					
						if(x->getSymbolType() == "ARRAY"){
							type_specified = "ARRAY, INT";
							symbolID = "array";
						}

						else if(x->getSymbolType() == "ID"){
							type_specified = "INT";
							symbolID = "variable";
						}
					}

					else if((yyvsp[-2].symbol)->getSymbolType() == "float"){
					
						if(x->getSymbolType() == "ARRAY"){
							type_specified = "ARRAY, FLOAT";
							symbolID = "array";
						}

						else if(x->getSymbolType() == "ID"){
							type_specified = "FLOAT";
							symbolID = "variable";
						}
					}

					symbolInfo* temp = new symbolInfo(x->getSymbolName(), type_specified);

					temp-> setSymbolID(symbolID);

					st.insert(*x);
				}
				
			}
		}

		logOut << "var_declaration : type_specifier declaration_list SEMICOLON" << endl;
		declaration_list.clear();
	}
#line 2651 "y.tab.c"
    break;

  case 23: /* type_specifier: INT  */
#line 986 "1905060.y"
                        {
		(yyvsp[0].symbol)->setSymbolClass("INT : int");
		(yyvsp[0].symbol)->isLeaf = true;
		
		variable = "int";
		symbolInfo *temp = new symbolInfo("int");
		
		(yyval.symbol) = temp;
		(yyval.symbol) -> setSymbolName(" ");
		(yyval.symbol)->setSymbolClass("type_specifier : INT");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		type_specified = "INT";
		func_type = "INT";
		logOut << "type_specifier : INT" << endl;
	}
#line 2675 "y.tab.c"
    break;

  case 24: /* type_specifier: FLOAT  */
#line 1007 "1905060.y"
                {
		(yyvsp[0].symbol)->setSymbolClass("FLOAT : float");
		(yyvsp[0].symbol)->isLeaf = true;

		variable = "float";
		symbolInfo *temp = new symbolInfo("float");
		
		(yyval.symbol) = temp;
		(yyval.symbol) -> setSymbolName(" ");
		(yyval.symbol)->setSymbolClass("type_specifier : FLOAT");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		type_specified = "FLOAT";
		func_type = "FLOAT";
		logOut << "type_specifier : FLOAT" << endl;
	}
#line 2699 "y.tab.c"
    break;

  case 25: /* type_specifier: VOID  */
#line 1028 "1905060.y"
                {
		(yyvsp[0].symbol)->setSymbolClass("VOID : void");
		(yyvsp[0].symbol)->isLeaf = true;

		variable = "void";
		symbolInfo *temp = new symbolInfo("void");
		
		(yyval.symbol) = temp;
		(yyval.symbol) -> setSymbolName(" ");
		(yyval.symbol)->setSymbolClass("type_specifier : VOID");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		type_specified = "VOID";
		func_type = "VOID";
		logOut << "type_specifier : VOID" << endl;
	}
#line 2723 "y.tab.c"
    break;

  case 26: /* id: ID  */
#line 1050 "1905060.y"
        {
		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		func_name = (yyvsp[0].symbol)->getSymbolName();
		var_name = (yyvsp[0].symbol)->getSymbolName();
	}
#line 2735 "y.tab.c"
    break;

  case 27: /* declaration_list: declaration_list COMMA id  */
#line 1059 "1905060.y"
                                                {
		(yyvsp[-1].symbol)->setSymbolClass("COMMA : ,");
		(yyvsp[-1].symbol)->isLeaf = true;

		(yyvsp[0].symbol)->setSymbolClass("ID : " + (yyvsp[0].symbol)->getSymbolName());
		(yyvsp[0].symbol)->isLeaf = true;

		symbolInfo* temp = new symbolInfo("declaration_list");
		temp->setSymbolClass("declaration_list : declaration_list COMMA ID");
		(yyval.symbol) = temp;
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[-2].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
	
		symbolInfo* x = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		x->setSymbolSize(0);
		x->setSymbolID("variable");
		x->setGlobal(globalScope);
		//printVariable(x, globalScope);
		if(globalScope){
			asmOut << "\t" << x->getSymbolName() << " DW " << x->getSymbolSize();
			if(x->getSymbolID() == "ARRAY"){
				asmOut << " DUP(" << 0 << ")" << "\t\t; array " << x->getSymbolName() << " declared" << endl;
			}
			else{
				asmOut << "   \t\t\t; variable " << x->getSymbolName() << " declared" << endl;
			}
		}
		else{
			if(x->getSymbolID() == "array" || x->getSymbolID() == "ARRAY"){
				int arrayStart = (st.getVariableCount()+1)*2;
				int arrayEnd = arrayStart + (x->getSymbolSize()-1)*2;
				x->setStackOffset(arrayStart);
				st.addVariableCount(x->getSymbolSize());
				codeSegOut << "\t\tSUB SP, " << x->getSymbolSize()*2 << "\t\t;Line " << lineCount << " : array " << x->getSymbolName() << " of size " << x->getSymbolSize() << " declared" << endl;
				codeSegOut << "\t\t; Location : [BP -" << arrayStart << "] to [BP -" << arrayEnd << "]" << endl;
			}
			else{
				st.addVariableCount(1);
				int stackOffset = st.getVariableCount() * 2;
				x->setStackOffset(stackOffset);
				codeSegOut << "\t\tSUB SP, 2\t\t;Line" << lineCount << " : variable " << x->getSymbolName() << " declared" << endl;
			}
		}
		
		x->setSymbolClass("declaration_list : declaration_list COMMA ID");

		(yyval.symbol)->input.push_back((yyvsp[-1].symbol));
		(yyval.symbol)->input.push_back((yyvsp[0].symbol));
		
		declaration_list.push_back(x);

		logOut << "declaration_list : declaration_list COMMA ID" << endl;
	}
#line 2797 "y.tab.c"
    break;

  case 28: /* declaration_list: declaration_list COMMA id LSQUARE CONST_INT RSQUARE  */
#line 1118 "1905060.y"
                                                                {
		(yyvsp[-4].symbol)->setSymbolClass("COMMA : ,");
		(yyvsp[-4].symbol)->isLeaf = true;

		(yyvsp[-3].symbol)->setSymbolClass("ID : " + (yyvsp[-3].symbol)->getSymbolName());
		(yyvsp[-3].symbol)->isLeaf = true;
		
		(yyvsp[-2].symbol)->setSymbolClass("LSQUARE : [");
		(yyvsp[-2].symbol)->isLeaf = true;
		
		(yyvsp[-1].symbol)->setSymbolClass("CONST_INT : " + (yyvsp[-1].symbol)->getSymbolName());
		(yyvsp[-1].symbol)->isLeaf = true;
		
		(yyvsp[0].symbol)->setSymbolClass("RSQUARE : ]");
		(yyvsp[0].symbol)->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("declaration_list");
		temp->setSymbolClass("declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
		(yyval.symbol) = temp;
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[-5].symbol));
		(yyval.symbol)->insertChild((yyvsp[-4].symbol));
		(yyval.symbol)->insertChild((yyvsp[-3].symbol));
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[-5].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		symbolInfo* x = new symbolInfo((yyvsp[-3].symbol)->getSymbolName(), "ARRAY");
		x->setSymbolSize(atoi((yyvsp[-1].symbol)->getSymbolName().c_str()));
		x->setSymbolID("ARRAY");
		x->setGlobal(globalScope);
		//printVariable(x, globalScope);

		if(globalScope){
			asmOut << "\t" << x->getSymbolName() << " DW " << x->getSymbolSize();
			if(x->getSymbolID() == "ARRAY"){
				asmOut << " DUP(" << 0 << ")" << "\t\t; array " << x->getSymbolName() << " declared" << endl;
			}
			else{
				asmOut << "   \t\t\t; variable " << x->getSymbolName() << " declared" << endl;
			}
		}
		else{
			if(x->getSymbolID() == "array" || x->getSymbolID() == "ARRAY"){
				int arrayStart = (st.getVariableCount()+1)*2;
				int arrayEnd = arrayStart + (x->getSymbolSize()-1)*2;
				x->setStackOffset(arrayStart);
				st.addVariableCount(x->getSymbolSize());
				codeSegOut << "\t\tSUB SP, " << x->getSymbolSize()*2 << "\t\t;Line " << lineCount << " : array " << x->getSymbolName() << " of size " << x->getSymbolSize() << " declared" << endl;
				codeSegOut << "\t\t; Location : [BP -" << arrayStart << "] to [BP -" << arrayEnd << "]" << endl;
			}
			else{
				st.addVariableCount(1);
				int stackOffset = st.getVariableCount() * 2;
				x->setStackOffset(stackOffset);
				codeSegOut << "\t\tSUB SP, 2\t\t;Line" << lineCount << " : variable " << x->getSymbolName() << " declared" << endl;
			}
		}
			
		x->setSymbolClass("declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
		declaration_list.push_back(x);

		symbolInfo* y = new symbolInfo("CONST_INT");
		y->setSymbolName((yyvsp[-1].symbol)->getSymbolName());
		y->setSymbolClass("declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
		
		logOut << "declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE" << endl;
	}
#line 2872 "y.tab.c"
    break;

  case 29: /* declaration_list: id  */
#line 1190 "1905060.y"
                {
		(yyvsp[0].symbol)->setSymbolClass("ID : " + (yyvsp[0].symbol)->getSymbolName());
		(yyvsp[0].symbol)->isLeaf = true;
		
		symbolInfo *temp = new symbolInfo("ID");
		temp->setSymbolName((yyvsp[0].symbol)->getSymbolName());
		temp->setSymbolClass("declaration_list : ID");
		(yyval.symbol) = temp;
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		
		symbolInfo *x = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), "ID");
		x->setSymbolSize(0);
		x->setSymbolID("variable");
		x->setGlobal(globalScope);
		//printVariable(x, globalScope);

		if(globalScope){
			asmOut << "\t" << x->getSymbolName() << " DW " << x->getSymbolSize();
			if(x->getSymbolID() == "ARRAY"){
				asmOut << " DUP(" << 0 << ")" << "\t\t; array " << x->getSymbolName() << " declared" << endl;
			}
			else{
				asmOut << "   \t\t\t; variable " << x->getSymbolName() << " declared" << endl;
			}
		}
		else{
			if(x->getSymbolID() == "array" || x->getSymbolID() == "ARRAY"){
				int arrayStart = (st.getVariableCount()+1)*2;
				int arrayEnd = arrayStart + (x->getSymbolSize()-1)*2;
				x->setStackOffset(arrayStart);
				st.addVariableCount(x->getSymbolSize());
				codeSegOut << "\t\tSUB SP, " << x->getSymbolSize()*2 << "\t\t;Line " << lineCount << " : array " << x->getSymbolName() << " of size " << x->getSymbolSize() << " declared" << endl;
				codeSegOut << "\t\t; Location : [BP -" << arrayStart << "] to [BP -" << arrayEnd << "]" << endl;
			}
			else{
				st.addVariableCount(1);
				int stackOffset = st.getVariableCount() * 2;
				x->setStackOffset(stackOffset);
				codeSegOut << "\t\tSUB SP, 2\t\t;Line" << lineCount << " : variable " << x->getSymbolName() << " declared" << endl;
			}
		}

		declaration_list.push_back(x);
		
		logOut << "declaration_list : ID" << endl;
	}
#line 2926 "y.tab.c"
    break;

  case 30: /* declaration_list: id LSQUARE CONST_INT RSQUARE  */
#line 1241 "1905060.y"
                                        {
		(yyvsp[-3].symbol)->setSymbolClass("ID : " + (yyvsp[-3].symbol)->getSymbolName());
		(yyvsp[-3].symbol)->isLeaf = true;

		(yyvsp[-2].symbol)->setSymbolClass("LSQUARE : [");
		(yyvsp[-2].symbol)->isLeaf = true;
		
		(yyvsp[-1].symbol)->setSymbolClass("CONST_INT : " + (yyvsp[-1].symbol)->getSymbolName());
		(yyvsp[-1].symbol)->isLeaf = true;
		
		(yyvsp[0].symbol)->setSymbolClass("RSQUARE : ]");
		(yyvsp[0].symbol)->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo((yyvsp[-3].symbol)->getSymbolName(), "ARRAY");
		temp->setSymbolClass("declaration_list : ID LSQUARE CONST_INT RSQUARE");
		(yyval.symbol) = temp;
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[-3].symbol));
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[-3].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		symbolInfo* x = new symbolInfo("CONST_INT");
		x->setSymbolClass("declaration_list : ID LSQUARE CONST_INT RSQUARE");
		x->setSymbolName((yyvsp[-3].symbol)->getSymbolName());
		x->setSymbolSize(atoi((yyvsp[-1].symbol)->getSymbolName().c_str()));
		x->setSymbolID("ARRAY");
		x->setGlobal(globalScope);
		//printVariable(x, globalScope);

		if(globalScope){
			asmOut << "\t" << x->getSymbolName() << " DW " << x->getSymbolSize();
			if(x->getSymbolID() == "ARRAY"){
				asmOut << " DUP(" << 0 << ")" << "\t\t; array " << x->getSymbolName() << " declared" << endl;
			}
			else{
				asmOut << "   \t\t\t; variable " << x->getSymbolName() << " declared" << endl;
			}
		}
		else{
			if(x->getSymbolID() == "array" || x->getSymbolID() == "ARRAY"){
				int arrayStart = (st.getVariableCount()+1)*2;
				int arrayEnd = arrayStart + (x->getSymbolSize()-1)*2;
				x->setStackOffset(arrayStart);
				st.addVariableCount(x->getSymbolSize());
				codeSegOut << "\t\tSUB SP, " << x->getSymbolSize()*2 << "\t\t;Line " << lineCount << " : array " << x->getSymbolName() << " of size " << x->getSymbolSize() << " declared" << endl;
				codeSegOut << "\t\t; Location : [BP -" << arrayStart << "] to [BP -" << arrayEnd << "]" << endl;
			}
			else{
				st.addVariableCount(1);
				int stackOffset = st.getVariableCount() * 2;
				x->setStackOffset(stackOffset);
				codeSegOut << "\t\tSUB SP, 2\t\t;Line" << lineCount << " : variable " << x->getSymbolName() << " declared" << endl;
			}
		}

		declaration_list.push_back(x);
		
		logOut << "declaration_list : ID LSQUARE CONST_INT RSQUARE" << endl;
	}
#line 2993 "y.tab.c"
    break;

  case 31: /* statements: statement  */
#line 1307 "1905060.y"
                        {
		symbolInfo* temp = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		temp->setSymbolID("statement");
		temp->setSymbolClass("statements : statement");
		
		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("statements : statement");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		string KMSLabel = generateNewLabel();
		codeSegOut << "\t" << KMSLabel << ": " << endl;

		logOut << "statements : statement" << endl;
	}
#line 3015 "y.tab.c"
    break;

  case 32: /* statements: statements statement  */
#line 1325 "1905060.y"
                                {
		symbolInfo* temp = new symbolInfo((yyvsp[-1].symbol)->getSymbolName() + (yyvsp[0].symbol)->getSymbolName(), "NON_TERMINAL");
		temp->setSymbolID("statement");
		temp->setSymbolClass("statements : statements statement");
		
		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("statements : statements statement");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[-1].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		string KMSLabel = generateNewLabel();
		codeSegOut << "\t" << KMSLabel << ": " << endl;

		logOut << "statements : statements statement" << endl;
	}
#line 3038 "y.tab.c"
    break;

  case 33: /* statement: var_declaration  */
#line 1345 "1905060.y"
                                {
		symbolInfo* temp = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), "NON_TERMINAL");
		temp->setSymbolID("var_declaration");
		temp->setSymbolClass("statement : var_declaration");
		
		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("statement : var_declaration");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		logOut << "statement : var_declaration" << endl;
	}
#line 3057 "y.tab.c"
    break;

  case 34: /* statement: expression_statement  */
#line 1360 "1905060.y"
                                {
		symbolInfo* temp = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), "NON_TERMINAL");
		temp->setSymbolID("expression_statement");
		temp->setSymbolClass("statement : expression_statement");
		
		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("statement : expression_statement");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Expression evaluated" << endl;
		//codeSegOut << endl;

		logOut << "statement : expression_statement" << endl;
	}
#line 3079 "y.tab.c"
    break;

  case 35: /* statement: compound_statement  */
#line 1378 "1905060.y"
                                {
		symbolInfo* temp = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), "NON_TERMINAL");
		temp->setSymbolID((yyvsp[0].symbol)->getSymbolID());
		temp->setSymbolClass("statement : compound_statement");
		
		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("statement : compound_statement");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		logOut << "statement : compound_statement" << endl;
	}
#line 3098 "y.tab.c"
    break;

  case 36: /* statement: IF LPAREN expression embedded_exp RPAREN embedded_void statement embedded_stmt  */
#line 1393 "1905060.y"
                                                                                                                {
		(yyvsp[-7].symbol)->setSymbolClass("IF : if");
		(yyvsp[-7].symbol)->isLeaf = true;

		(yyvsp[-6].symbol)->setSymbolClass("LPAREN : (");
		(yyvsp[-6].symbol)->isLeaf = true;
		
		(yyvsp[-3].symbol)->setSymbolClass("RPAREN : )");
		(yyvsp[-3].symbol)->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("if (" + (yyvsp[-5].symbol)->getSymbolName() + " )" + (yyvsp[-1].symbol)->getSymbolName() + " ", "NON_TERMINAL");
		temp->setSymbolID("IF grammar rule 1");
		temp->setSymbolClass("statement : IF grammar rule 1");
		
		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("statement : IF LPAREN expression RPAREN statement");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[-7].symbol));
		(yyval.symbol)->insertChild((yyvsp[-6].symbol));
		(yyval.symbol)->insertChild((yyvsp[-5].symbol));
		(yyval.symbol)->insertChild((yyvsp[-3].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->setStartLine((yyvsp[-7].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[-1].symbol)->getEndLine());

		codeSegOut << "\t" << ifTerminateStack.top() << ": " << endl;
		ifTerminateStack.pop();

		logOut << "statement : IF LPAREN expression RPAREN statement" << endl;
	}
#line 3133 "y.tab.c"
    break;

  case 37: /* statement: IF LPAREN expression embedded_exp RPAREN embedded_void statement embedded_stmt ELSE embedded_else statement  */
#line 1424 "1905060.y"
                                                                                                                        {
		(yyvsp[-10].symbol)->setSymbolClass("IF : if");
		(yyvsp[-10].symbol)->isLeaf = true;
		
		(yyvsp[-9].symbol)->setSymbolClass("LPAREN : (");
		(yyvsp[-9].symbol)->isLeaf = true;
		
		(yyvsp[-6].symbol)->setSymbolClass("RPAREN : )");
		(yyvsp[-6].symbol)->isLeaf = true;
		
		(yyvsp[-2].symbol)->setSymbolClass("ELSE : else");
		(yyvsp[-2].symbol)->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("if (" + (yyvsp[-8].symbol)->getSymbolName() + " )" + (yyvsp[-4].symbol)->getSymbolName() + " else " + (yyvsp[0].symbol)->getSymbolName() , "NON_TERMINAL");
		temp->setSymbolID("IF grammar rule 2");
		temp->setSymbolClass("statement : IF grammar rule 2");
		
		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("statement : IF LPAREN expression RPAREN statement ELSE statement");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[-10].symbol));
		(yyval.symbol)->insertChild((yyvsp[-9].symbol));
		(yyval.symbol)->insertChild((yyvsp[-8].symbol));
		(yyval.symbol)->insertChild((yyvsp[-6].symbol));
		(yyval.symbol)->insertChild((yyvsp[-4].symbol));
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[-10].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		codeSegOut << "\t\t;Ending else block" << endl;
		codeSegOut << "\t" << elseTerminateStack.top() << ": " << endl;
		elseTerminateStack.pop();
		logOut << "statement : IF LPAREN expression RPAREN statement ELSE statement" << endl;
	}
#line 3173 "y.tab.c"
    break;

  case 38: /* $@1: %empty  */
#line 1460 "1905060.y"
                                          {

		codeSegOut << "\t\t;Line " << lineCount << ": For loop start" << endl;
		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << " : Evaluated for loop start expression" << endl;
		string label = generateNewLabel();
		forLoopConditionLabel = label;
		codeSegOut << "\t\t; For loop condition label" << endl;
		codeSegOut << "\t" << label << ": " << endl;

	}
#line 3188 "y.tab.c"
    break;

  case 39: /* $@2: %empty  */
#line 1469 "1905060.y"
          {	}
#line 3194 "y.tab.c"
    break;

  case 40: /* $@3: %empty  */
#line 1469 "1905060.y"
                                       {

		string label = generateNewLabel();
		string forLoopLabel = generateNewLabel();
		string forLoopUpdate = generateNewLabel();
		
		forLoopEndLabel = label;
		forLoopUpdateLabel = forLoopUpdate;
		forLoopStatementLabel = forLoopLabel;

		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Load " << (yyvsp[0].symbol)->getSymbolName() << endl;
		codeSegOut << "\t\tCMP AX, 0" << endl;
		codeSegOut << "\t\tJE " << label << "\t\t; For loop condition unsatisfied. Break for loop" << endl;
		codeSegOut << "\t\tJMP " << forLoopLabel << endl;
		codeSegOut << "\t\t; For loop update start" << endl;
		codeSegOut << "\t" << forLoopUpdate << ": " << endl;

	}
#line 3217 "y.tab.c"
    break;

  case 41: /* $@4: %empty  */
#line 1486 "1905060.y"
          {	}
#line 3223 "y.tab.c"
    break;

  case 42: /* $@5: %empty  */
#line 1486 "1905060.y"
                             {

		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Evaluated for loop update expression" << endl;
		codeSegOut << "\t\tJMP " << forLoopConditionLabel << endl;
		codeSegOut << "\t\t; Line " << lineCount << ": Evaluate for loop statement " << endl;
		codeSegOut << "\t" << forLoopStatementLabel << ": " << endl;

	}
#line 3236 "y.tab.c"
    break;

  case 43: /* $@6: %empty  */
#line 1493 "1905060.y"
          {	}
#line 3242 "y.tab.c"
    break;

  case 44: /* statement: FOR LPAREN expression_statement $@1 $@2 expression_statement $@3 $@4 expression $@5 $@6 RPAREN statement  */
#line 1493 "1905060.y"
                                        {
		(yyvsp[-12].symbol)->setSymbolClass("FOR : for");
		(yyvsp[-12].symbol)->isLeaf = true;
		
		(yyvsp[-11].symbol)->setSymbolClass("LPAREN : (");
		(yyvsp[-11].symbol)->isLeaf = true;
		
		(yyvsp[-1].symbol)->setSymbolClass("RPAREN : )");
		(yyvsp[-1].symbol)->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("for (" + (yyvsp[-10].symbol)->getSymbolName() + " " + (yyvsp[-7].symbol)->getSymbolName() + " " + (yyvsp[-4].symbol)->getSymbolName() + " " + (yyvsp[0].symbol)->getSymbolName(), "NON_TERMINAL");
		temp->setSymbolID("FOR loop");
		temp->setSymbolClass("statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
		
		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("statement : FOR LPAREN expression_statement expression_statement expression_statement RPAREN statement");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[-12].symbol));
		(yyval.symbol)->insertChild((yyvsp[-11].symbol));
		(yyval.symbol)->insertChild((yyvsp[-10].symbol));
		(yyval.symbol)->insertChild((yyvsp[-7].symbol));
		(yyval.symbol)->insertChild((yyvsp[-4].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[-12].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		codeSegOut << "\t\tJMP " << forLoopUpdateLabel << endl;
		codeSegOut << "\t\t; For loop ending" << endl;
		codeSegOut << "\t" << forLoopEndLabel << ": " << endl;

		logOut << "statement : FOR LPAREN expression_statement expression_statement expression_statement RPAREN statement" << endl;
	}
#line 3280 "y.tab.c"
    break;

  case 45: /* $@7: %empty  */
#line 1527 "1905060.y"
                       {

		string label = generateNewLabel();
		whileLoopStartLabel = label;
		codeSegOut << "\t" << label << ": \t\t; Line " << lineCount << ": While Loop start" << endl;

	}
#line 3292 "y.tab.c"
    break;

  case 46: /* $@8: %empty  */
#line 1533 "1905060.y"
                                         {

		string label = generateNewLabel();
		whileLoopEndLabel = label;

		codeSegOut << "\t\t; While loop block start" << endl;
		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Load" << (yyvsp[-2].symbol)->getSymbolName() << endl;
		codeSegOut << "\t\tCMP AX, 0" << endl;
		codeSegOut << "\t\tJE " << label << endl;

	}
#line 3308 "y.tab.c"
    break;

  case 47: /* statement: WHILE LPAREN $@7 expression embedded_exp RPAREN $@8 statement  */
#line 1543 "1905060.y"
                        {
		(yyvsp[-7].symbol)->setSymbolClass("WHILE : while");
		(yyvsp[-7].symbol)->isLeaf = true;
		
		(yyvsp[-6].symbol)->setSymbolClass("LPAREN : (");
		(yyvsp[-6].symbol)->isLeaf = true;
		
		(yyvsp[-2].symbol)->setSymbolClass("RPAREN : )");
		(yyvsp[-2].symbol)->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("while (" + (yyvsp[-4].symbol)->getSymbolName() + " ) " + (yyvsp[0].symbol)->getSymbolName(), "NON_TERMINAL");
		temp->setSymbolID("WHILE loop");
		temp->setSymbolClass("statement : WHILE LPAREN expression RPAREN statement");
		
		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("statement : WHILE LPAREN expression RPAREN statement");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[-7].symbol));
		(yyval.symbol)->insertChild((yyvsp[-6].symbol));
		(yyval.symbol)->insertChild((yyvsp[-4].symbol));
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[-7].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		exp_type = (yyvsp[-4].symbol)->getSymbolType();

		codeSegOut << "\t\tJMP " << whileLoopStartLabel << endl;
		codeSegOut << "\t" << whileLoopEndLabel << ":\t\t; Line " << lineCount << ": While loop ending label" << endl;

		logOut << "statement : WHILE LPAREN expression RPAREN statement" << endl;
	}
#line 3345 "y.tab.c"
    break;

  case 48: /* statement: RETURN expression SEMICOLON  */
#line 1576 "1905060.y"
                                        {
		(yyvsp[-2].symbol)->setSymbolClass("RETURN : return");
		(yyvsp[-2].symbol)->isLeaf = true;
		
		(yyvsp[0].symbol)->setSymbolClass("SEMICOLON : ;");
		(yyvsp[0].symbol)->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("RETURN");
		temp->setSymbolName("return");
		temp->setSymbolClass("statement : RETURN expression SEMICOLON");
		
		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("statement : RETURN expression SEMICOLON");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[-2].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		logOut << "statement : RETURN expression SEMICOLON" << endl;
		type_specified = (yyvsp[-1].symbol)->getSymbolType();

		codeSegOut << "\t\t; Line " << lineCount << ": return " << (yyvsp[-1].symbol)->getSymbolName() << endl;
		codeSegOut << "\t\tPOP AX" << endl;
		codeSegOut << "\t\tMOV [BP+4], AX" << endl;
		codeSegOut << "\t\tADD SP, " << varCount*2 << "\t\t;Line " << lineCount << ": Removing all the local variables from the stack" << endl;
		codeSegOut << "\t\tPOP BP" << endl;
		if(final_func_name != "main"){
			codeSegOut << "\t\tRET" << endl;
		}
	}
#line 3382 "y.tab.c"
    break;

  case 49: /* statement: PRINTLN LPAREN ID RPAREN SEMICOLON  */
#line 1609 "1905060.y"
                                                {
		(yyvsp[-4].symbol)->setSymbolClass("PRINTLN : println");
		(yyvsp[-4].symbol)->isLeaf = true;
		
		(yyvsp[-3].symbol)->setSymbolClass("LPAREN : (");
		(yyvsp[-3].symbol)->isLeaf = true;
		
		(yyvsp[-1].symbol)->setSymbolClass("RPAREN : )");
		(yyvsp[-1].symbol)->isLeaf = true;
		
		(yyvsp[0].symbol)->setSymbolClass("SEMICOLON : ;");
		(yyvsp[0].symbol)->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("println (" + (yyvsp[-2].symbol)->getSymbolName() + " )", "NON_TERMINAL");
		temp->setSymbolID("PRINTLN");
		temp->setSymbolClass("statement : PRINTLN LPAREN ID RPAREN SEMICOLON");
		
		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("statement : PRINTLN LPAREN ID RPAREN SEMICOLON");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[-4].symbol));
		(yyval.symbol)->insertChild((yyvsp[-3].symbol));
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[-4].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		temp = st.lookUpRoot((yyvsp[-2].symbol)->getSymbolName());
		if(temp != nullptr){
			codeSegOut << "\t\tMOV BX, " << (yyvsp[-2].symbol)->getSymbolName() << "\t\t; Line " << lineCount << ": println(" << (yyvsp[-2].symbol)->getSymbolName() << ")" << endl;
		}
		else{
			temp = st.lookUp((yyvsp[-2].symbol)->getSymbolName());
			if(temp->getSymbolID() == "parameter"){
				codeSegOut << "\t\tMOV BX, [BP+" << temp->getStackOffset() << "]\t\t; Line " << lineCount << ": println(" << (yyvsp[-2].symbol)->getSymbolName() << ")" << endl;
			}
			else{
				codeSegOut << "\t\tMOV BX, [BP-" << temp->getStackOffset() << "]\t\t; Line " << lineCount << ": println(" << (yyvsp[-2].symbol)->getSymbolName() << ")" << endl;
			}
			//codeSegOut << "\t\tMOV BX, [BP-" << temp->getStackOffset() << "]\t\t; Line " << lineCount << ": println(" << $3->getSymbolName() << ")" << endl;
		}
		codeSegOut << "\t\tCALL PRINT_NUM_FROM_BX" << endl;

		logOut << "statement : PRINTLN LPAREN ID RPAREN SEMICOLON" << endl;
	}
#line 3433 "y.tab.c"
    break;

  case 50: /* embedded_exp: %empty  */
#line 1658 "1905060.y"
                {
		final_exp_type = exp_type;
	}
#line 3441 "y.tab.c"
    break;

  case 51: /* embedded_void: %empty  */
#line 1663 "1905060.y"
                {
		if(exp_type == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}
		string endLabel = generateNewLabel();
		codeSegOut << "\t\t; Line " << lineCount << ": Evaluating if condition" << endl;
		codeSegOut << "\t\tPOP AX\t\t; Load if condition expression" << endl;
		codeSegOut << "\t\tCMP AX, 0" << endl;
		codeSegOut << "\t\tJE " << endLabel << endl;
		codeSegOut << "\t\t;Starting of if block" << endl;
		ifTerminateLabel = endLabel;
		ifTerminateStack.push(endLabel);
	}
#line 3460 "y.tab.c"
    break;

  case 52: /* embedded_stmt: %empty  */
#line 1679 "1905060.y"
                {
		codeSegOut << "\t\t; Line " << lineCount << ": End of if block" << endl;
		//codeSegOut << "\t\t" << endLabel << ":" << endl;
	}
#line 3469 "y.tab.c"
    break;

  case 53: /* embedded_else: %empty  */
#line 1684 "1905060.y"
                {
		string elseLabel = generateNewLabel();
		codeSegOut << "\t\t" << "JMP " << elseLabel << endl;
		codeSegOut << "\t" << ifTerminateLabel << ": " << endl;
		codeSegOut << "\t\t;ELSE BLOCK START" << endl;
		elseTerminateStack.push(elseLabel);
		elseTerminateLabel = elseLabel;	
	}
#line 3482 "y.tab.c"
    break;

  case 54: /* expression_statement: SEMICOLON  */
#line 1693 "1905060.y"
                                        {
		(yyvsp[0].symbol)->setSymbolClass("SEMICOLON : ;");
		(yyvsp[0].symbol)->isLeaf = true;

		(yyval.symbol) = new symbolInfo("SEMICOLON");
		(yyval.symbol)->setSymbolClass("expression_statement : SEMICOLON");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		logOut << "expression_statement : SEMICOLON" << endl;
	}
#line 3500 "y.tab.c"
    break;

  case 55: /* expression_statement: expression SEMICOLON  */
#line 1706 "1905060.y"
                               {
		(yyvsp[0].symbol)->setSymbolClass("SEMICOLON : ;");
		(yyvsp[0].symbol)->isLeaf = true;
		
		(yyval.symbol) = new symbolInfo("expression_statement");
		(yyval.symbol)->setSymbolClass("expression_statement : expression SEMICOLON");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[-1].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		exp_type = (yyvsp[-1].symbol)->getSymbolType();

		logOut << "expression_statement : expression SEMICOLON" << endl;
	}
#line 3521 "y.tab.c"
    break;

  case 56: /* variable: id  */
#line 1724 "1905060.y"
              {
		(yyvsp[0].symbol)->setSymbolClass("ID : " + (yyvsp[0].symbol)->getSymbolName());
		(yyvsp[0].symbol)->isLeaf = true;
		
		(yyval.symbol) = new symbolInfo("variable");
		(yyval.symbol)->setSymbolName((yyvsp[0].symbol)->getSymbolName());
		(yyval.symbol)->setSymbolClass("variable : ID");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());

		symbolInfo* temp = st.lookUp((yyvsp[0].symbol)->getSymbolName());

		if(temp == nullptr){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Undeclared variable '" << (yyvsp[0].symbol)->getSymbolName() << "'" << endl; 
		}

		else{
			if(temp->getSymbolType() == "void"){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Variable or field '" << temp->getSymbolName() << "' declared void" << endl;
			}

			else if(temp->getSymbolType() == "INT"){
				(yyval.symbol)->setSymbolType("INT");
				(yyval.symbol)->setSymbolID("variable");
			}

			else if(temp->getSymbolType() == "FUNCTION, INT"){
				(yyval.symbol)->setSymbolType("FUNCTION, INT");
				(yyval.symbol)->setSymbolID("FUNCTION, INT");
			}

			else if(temp->getSymbolType() == "FUNCTION, FLOAT"){
				(yyval.symbol)->setSymbolType("FUNCTION, FLOAT");
				(yyval.symbol)->setSymbolID("FUNCTION, FLOAT");
			}

			else if(temp->getSymbolType() == "FUNCTION, VOID"){
				(yyval.symbol)->setSymbolType("FUNCTION, VOID");
				(yyval.symbol)->setSymbolID("FUNCTION, VOID");
			}

			else if(temp->getSymbolType() == "FLOAT"){
				(yyval.symbol)->setSymbolType("FLOAT");
				(yyval.symbol)->setSymbolID("variable");
			}

			else if(temp->getSymbolType() == "ARRAY, INT" || temp->getSymbolType() == "ARRAY, FLOAT" || temp->getSymbolType() == "ARRAY, VOID"){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Expression must have arithmetic type" << endl;
				(yyval.symbol)->setSymbolID("ARRAY");
				(yyval.symbol)->setSymbolType(temp->getSymbolType());
			}
		}

		logOut << "variable : ID" << endl;
	}
#line 3586 "y.tab.c"
    break;

  case 57: /* variable: id LSQUARE expression RSQUARE  */
#line 1785 "1905060.y"
                                        {
		(yyvsp[-3].symbol)->setSymbolClass("ID : " + (yyvsp[-3].symbol)->getSymbolName());
		(yyvsp[-3].symbol)->isLeaf = true;

		(yyvsp[-2].symbol)->setSymbolClass("LSQUARE : [");
		(yyvsp[-2].symbol)->isLeaf = true;
		
		(yyvsp[0].symbol)->setSymbolClass("RSQUARE : ]");
		(yyvsp[0].symbol)->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo((yyvsp[-3].symbol)->getSymbolName(), "ID");
		temp->setSymbolClass("variable : ID LSQUARE expression RSQUARE");
		
		(yyval.symbol) = temp;
		(yyval.symbol)->setSymbolClass("variable : ID LSQUARE expression RSQUARE");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-3].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-3].symbol));
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		temp = st.lookUp((yyvsp[-3].symbol)->getSymbolName());
		if(temp == nullptr){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Undeclared variable '" << (yyvsp[-3].symbol)->getSymbolName() << "'" << endl;
		}

		else{
			if(temp->getSymbolType() == "void"){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Variable or field '" << temp->getSymbolName() << "' declared void" << endl;
			}

			else if((yyvsp[-1].symbol)->getSymbolType() != "INT"){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Array subscript is not an integer" << endl;
			}

			else if(temp->getSymbolType() == "INT" || temp->getSymbolType() == "FLOAT"){
				errorCount++;
				errorOut << "Line# " << lineCount << ": '" << (yyvsp[-3].symbol)->getSymbolName() << "' is not an array" << endl;
			}

			else if(temp->getSymbolType() == "ARRAY, INT"){
				(yyval.symbol)->setSymbolType("INT");
				(yyval.symbol)->setSymbolID("array");
			}
			else{
				(yyval.symbol)->setSymbolType("FLOAT");
				(yyval.symbol)->setSymbolID("array");
			}

			codeSegOut << "\t\t; " << (yyvsp[-3].symbol)->getSymbolName() << "[" << (yyvsp[-1].symbol)->getSymbolName() << "]" << endl;
			codeSegOut << "\t\tPOP AX\t\t; Pop index " << (yyvsp[-1].symbol)->getSymbolName() << " from stack" << endl;
			codeSegOut << "\t\tSHL AX, 1\t\t; Multiply index by 2" << endl;
			codeSegOut << "\t\tLEA BX, [BP-" << temp->getStackOffset() << "]\t\t; Load address of " << (yyvsp[-3].symbol)->getSymbolName() << " into BX" << endl;
			codeSegOut << "\t\tSUB BX, AX\t\t; Subtract index from address" << endl;
			codeSegOut << "\t\tPUSH BX\t\t; Push address of " << (yyvsp[-3].symbol)->getSymbolName() << "[" << (yyvsp[-1].symbol)->getSymbolName() << "] onto stack" << endl;

		}
		logOut << "variable : ID LSQUARE expression RSQUARE" << endl;
	}
#line 3655 "y.tab.c"
    break;

  case 58: /* expression: logic_expression  */
#line 1851 "1905060.y"
                             {
		exp_type = (yyvsp[0].symbol)->getSymbolType();
		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolID((yyvsp[0].symbol)->getSymbolID());
		(yyval.symbol)->setSymbolClass("expression : logic_expression");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		logOut << "expression : logic_expression" << endl;
	}
#line 3672 "y.tab.c"
    break;

  case 59: /* expression: variable ASSIGNOP logic_expression  */
#line 1864 "1905060.y"
                                             {
		(yyvsp[-1].symbol)->setSymbolClass("ASSIGNOP : =");
		(yyvsp[-1].symbol)->isLeaf = true;
		
		(yyval.symbol) = new symbolInfo((yyvsp[-2].symbol)->getSymbolName(), (yyvsp[-2].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolClass("expression : variable ASSIGNOP logic_expression");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-2].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		if((yyvsp[-2].symbol)->getSymbolType() == "VOID" || (yyvsp[0].symbol)->getSymbolType() == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}
		symbolInfo* temp = st.lookUp((yyvsp[-2].symbol)->getSymbolName());
		if(temp != nullptr){
			string type1 = temp->getSymbolType();
			string type2 = (yyvsp[0].symbol)->getSymbolType();

			if((type1 == "INT" && type2 == "FLOAT") || (type1 == "ARRAY, INT" && type2 == "FLOAT")){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Warning: possible loss of data in assignment of FLOAT to INT" << endl;
			}

			logOut << "expression : variable ASSIGNOP logic_expression" << endl;
		}
		
		codeSegOut << "\t\tPOP AX\t\t; Load " << (yyvsp[0].symbol)->getSymbolName() << endl;
		if((yyvsp[-2].symbol)->getSymbolID() == "ARRAY" || (yyvsp[-2].symbol)->getSymbolID() == "array"){
			codeSegOut << "\t\tPOP BX\t\t; Load address of " << (yyvsp[-2].symbol)->getSymbolName() << endl;
		}
		temp = st.lookUp((yyvsp[-2].symbol)->getSymbolName());
		//symbolInfo* tempLocal = st.lookUp($1->getSymbolName());
		if(temp != nullptr){
			//temp = st.lookUp($1->getSymbolName());
			if(temp->getGlobal()){
				codeSegOut << "\t\tMOV " << (yyvsp[-2].symbol)->getSymbolName() << ", AX\t\t; Line " << lineCount << ": " << (yyvsp[-2].symbol)->getSymbolName() << " " << (yyvsp[-1].symbol)->getSymbolName() << " " << (yyvsp[0].symbol)->getSymbolName() << endl;
			}

			else{
				if(temp->getSymbolID() == "ARRAY" || temp->getSymbolID() == "array"){
					codeSegOut << "\t\tMOV [BX], AX\t\t; Line " << lineCount << ": " << (yyvsp[-2].symbol)->getSymbolName() << " " << (yyvsp[-1].symbol)->getSymbolName() << " " << (yyvsp[0].symbol)->getSymbolName() << endl;
				}
				else{
					if(temp->getSymbolID() == "parameter"){
						codeSegOut << "\t\tMOV [BP+" << temp->getStackOffset() << "], AX\t\t; Line " << lineCount << ": " << (yyvsp[-2].symbol)->getSymbolName() << " " << (yyvsp[-1].symbol)->getSymbolName() << " " << (yyvsp[0].symbol)->getSymbolName() << endl;
					}
					else{
						codeSegOut << "\t\tMOV [BP-" << temp->getStackOffset() << "], AX\t\t; Line " << lineCount << ": " << (yyvsp[-2].symbol)->getSymbolName() << " " << (yyvsp[-1].symbol)->getSymbolName() << " " << (yyvsp[0].symbol)->getSymbolName() << endl;
					}
				}
			}
		}
		/*else{
			codeSegOut << "\t\tMOV " << $1->getSymbolName() << ", AX\t\t; Line " << lineCount << ": " << $1->getSymbolName() << " " << $2->getSymbolName() << " " << $3->getSymbolName() << endl;
		}*/

		
		/*temp = st.lookUp($1->getSymbolName());
		if(temp != nullptr){
			if(temp->getGlobal() == true){
				codeSegOut << "\t\tMOV " << $1->getSymbolName() << ", AX\t\t; Line " << lineCount << ": " << $1->getSymbolName() << " " << $2->getSymbolName() << " " << $3->getSymbolName() << endl;
			}
			else{
				if(temp->getSymbolID() == "ARRAY" || temp->getSymbolID() == "array"){
					codeSegOut << "\t\tMOV [BX], AX\t\t; Line " << lineCount << ": " << $1->getSymbolName() << " " << $2->getSymbolName() << " " << $3->getSymbolName() << endl;
				}
				else{
					codeSegOut << "\t\tMOV [BP-" << temp->getStackOffset() << "], AX\t\t; Line " << lineCount << ": " << $1->getSymbolName() << " " << $2->getSymbolName() << " " << $3->getSymbolName() << endl;
				}
			}
		}*/
		codeSegOut << "\t\tPUSH AX\t\t; Save declared value of " << (yyvsp[-2].symbol)->getSymbolName() << endl; 
	}
#line 3754 "y.tab.c"
    break;

  case 60: /* logic_expression: rel_expression  */
#line 1943 "1905060.y"
                                        {
		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolID((yyvsp[0].symbol)->getSymbolID());
		(yyval.symbol)->setSymbolClass("logic_expression : rel_expression");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		logOut << "logic_expression : rel_expression" << endl;
	}
#line 3770 "y.tab.c"
    break;

  case 61: /* $@9: %empty  */
#line 1955 "1905060.y"
                                 {

		codeSegOut << "\t\t; Line " << lineCount << ": Short Circuit jump code evaluation" << endl;
		codeSegOut << "\t\tPOP AX\t\t; Load " << (yyvsp[-1].symbol)->getSymbolName() << endl;
		if((yyvsp[0].symbol)->getSymbolName() == "&&"){
			boolValue = 1;
		}
		else{
			boolValue = 0;
		}
		codeSegOut << "\t\tCMP AX , " << boolValue <<  "\t\t; Check if " << (yyvsp[-1].symbol)->getSymbolName() << " is true" << endl;
		string tempLabel = generateNewLabel();
		codeSegOut << "\t\tJNE " << tempLabel << "\t\t; Jump to " << tempLabel << " if " << (yyvsp[-1].symbol)->getSymbolName() << " is false" << endl;
		shortCircuitLabel = tempLabel;
		(yyvsp[-1].symbol)->setLabel(tempLabel);

	}
#line 3792 "y.tab.c"
    break;

  case 62: /* logic_expression: rel_expression LOGICOP $@9 rel_expression  */
#line 1971 "1905060.y"
                                {
		(yyvsp[-2].symbol)->setSymbolClass("LOGICOP : " + (yyvsp[-2].symbol)->getSymbolName());
		(yyvsp[-2].symbol)->isLeaf = true;

		if((yyvsp[-3].symbol)->getSymbolType() == "VOID" || (yyvsp[0].symbol)->getSymbolType() == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}
		(yyval.symbol) = new symbolInfo((yyvsp[-3].symbol)->getSymbolName() + " " + (yyvsp[-2].symbol)->getSymbolName() + " " + (yyvsp[0].symbol)->getSymbolName(), "INT");
		(yyval.symbol)->setSymbolID((yyvsp[-3].symbol)->getSymbolID());
		(yyval.symbol)->setSymbolClass("logic_expression : rel_expression LOGICOP rel_expression");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-3].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-3].symbol));
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		codeSegOut << "\t\tPOP AX\t\t; Load " << (yyvsp[0].symbol)->getSymbolName() << endl;

		if((yyvsp[-2].symbol)->getSymbolName() == "&&"){
			boolValue = 1;
		}
		else{
			boolValue = 0;
		}

		codeSegOut << "\t\tCMP AX , " << boolValue <<  "\t\t; Check if " << (yyvsp[0].symbol)->getSymbolName() << " is true" << endl;
		codeSegOut << "\t\tJNE " << (yyvsp[-3].symbol)->getLabel() << "\t\t; Jump to " << (yyvsp[-3].symbol)->getLabel() << " if " << (yyvsp[0].symbol)->getSymbolName() << " is false" << endl;
		codeSegOut << "\t\tPUSH " << boolValue << "\t\t; Push " << boolValue << " to stack" << endl;
		string logicEnd = generateNewLabel();
		codeSegOut << "\t\tJMP " << logicEnd << "\t\t; Jump to " << logicEnd << endl;
		codeSegOut << "\t" << (yyvsp[-3].symbol)->getLabel() << ":" << endl;
		if((yyvsp[-2].symbol)->getSymbolName() == "&&"){
			boolValue = 0;
		}
		else{
			boolValue = 1;
		}
		codeSegOut << "\t\tPUSH " << boolValue << "\t\t; Push " << boolValue << " to stack" << endl;
		codeSegOut << "\t" << logicEnd << ":" << endl;

		logOut << "logic_expression : rel_expression LOGICOP rel_expression" << endl;
	}
#line 3841 "y.tab.c"
    break;

  case 63: /* rel_expression: simple_expression  */
#line 2017 "1905060.y"
                                    {
		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolID((yyvsp[0].symbol)->getSymbolID());
		(yyval.symbol)->setSymbolClass("rel_expression : simple_expression");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		logOut << "rel_expression : simple_expression" << endl;
	}
#line 3857 "y.tab.c"
    break;

  case 64: /* rel_expression: simple_expression RELOP simple_expression  */
#line 2029 "1905060.y"
                                                        {
		(yyvsp[-1].symbol)->setSymbolClass("RELOP : " + (yyvsp[-1].symbol)->getSymbolName());
		(yyvsp[-1].symbol)->isLeaf = true;
		
		if((yyvsp[-2].symbol)->getSymbolType() == "VOID" || (yyvsp[0].symbol)->getSymbolType() == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}
		(yyval.symbol) = new symbolInfo((yyvsp[-2].symbol)->getSymbolName() + " " + (yyvsp[-1].symbol)->getSymbolName() + " " + (yyvsp[0].symbol)->getSymbolName(), "INT");
		(yyval.symbol)->setSymbolID((yyvsp[-2].symbol)->getSymbolID());
		(yyval.symbol)->setSymbolClass("rel_expression : simple_expression RELOP simple_expression");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-2].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		string trueLabel = generateNewLabel();
		string falseLabel = generateNewLabel();
		string KMSLabel = generateNewLabel();
		
		string jumpCode;
		if((yyvsp[-1].symbol)->getSymbolName() == "<"){
			jumpCode = "JL";
		}
		else if((yyvsp[-1].symbol)->getSymbolName() == "<="){
			jumpCode = "JLE";
		}
		else if((yyvsp[-1].symbol)->getSymbolName() == ">"){
			jumpCode = "JG";
		}
		else if((yyvsp[-1].symbol)->getSymbolName() == ">="){
			jumpCode = "JGE";
		}
		else if((yyvsp[-1].symbol)->getSymbolName() == "=="){
			jumpCode = "JE";
		}
		else if((yyvsp[-1].symbol)->getSymbolName() == "!="){
			jumpCode = "JNE";
		}

		codeSegOut << "\t\tPOP BX\t\t; Line " << lineCount << ": Load " << (yyvsp[0].symbol)->getSymbolName() << endl;
		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Load " << (yyvsp[-2].symbol)->getSymbolName() << endl;
		codeSegOut << "\t\tCMP AX, BX" << endl;
		codeSegOut << "\t\t" << jumpCode << " " << trueLabel << "\t\t; Line " << lineCount << ": " << (yyvsp[-2].symbol)->getSymbolName() << (yyvsp[-1].symbol)->getSymbolName() << (yyvsp[0].symbol)->getSymbolName() << endl;
		codeSegOut << "\t\tJMP " << KMSLabel << endl;
		codeSegOut << "\t" << trueLabel << ":\n\t\tPUSH 1" << endl;
		codeSegOut << "\t\tJMP " << falseLabel << endl;
		codeSegOut << "\t" << KMSLabel << ": " << endl;
		codeSegOut << "\t\tPUSH 0" << endl;
		codeSegOut << "\t" << falseLabel << ": " << endl;
		logOut << "rel_expression : simple_expression RELOP simple_expression" << endl;
	}
#line 3916 "y.tab.c"
    break;

  case 65: /* simple_expression: term  */
#line 2085 "1905060.y"
                         {
		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolClass("simple_expression : term");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		(yyval.symbol)->setSymbolID((yyvsp[0].symbol)->getSymbolID());
		
		logOut << "simple_expression : term" << endl;
	}
#line 3932 "y.tab.c"
    break;

  case 66: /* simple_expression: simple_expression ADDOP term  */
#line 2097 "1905060.y"
                                       {
		(yyvsp[-1].symbol)->setSymbolClass("ADDOP : " + (yyvsp[-1].symbol)->getSymbolName());
		(yyvsp[-1].symbol)->isLeaf = true;
		
		if((yyvsp[-2].symbol)->getSymbolType() == "VOID" || (yyvsp[0].symbol)->getSymbolType() == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}
		string name = (yyvsp[-2].symbol)->getSymbolName() + " " + (yyvsp[-1].symbol)->getSymbolName() + " " + (yyvsp[0].symbol)->getSymbolName();
		
		(yyval.symbol) = new symbolInfo(name, "NON_TERMINAL");
		(yyval.symbol)->setSymbolClass("simple_expression: simple_expression ADDOP term");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-2].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		if((yyvsp[-2].symbol)->getSymbolType() == "void"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Variable or field '" << (yyvsp[-2].symbol)->getSymbolName() << "' declared void" << endl;
		}

		if((yyvsp[0].symbol)->getSymbolType() == "void"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Variable or field '" << (yyvsp[-2].symbol)->getSymbolName() << "' declared void" << endl;
		}

		if((yyvsp[-2].symbol)->getSymbolType() == "FLOAT" || (yyvsp[0].symbol)->getSymbolType() == "FLOAT"){
			(yyval.symbol)->setSymbolType("FLOAT");
			(yyval.symbol)->setSymbolID("variable");
		}

		else if(((yyvsp[-2].symbol)->getSymbolType() == "FLOAT" && (yyvsp[0].symbol)->getSymbolType() == "INT") || ((yyvsp[-2].symbol)->getSymbolID() == "INT" && (yyvsp[0].symbol)->getSymbolID() == "FLOAT")){
			(yyval.symbol)->setSymbolType("FLOAT");
			(yyval.symbol)->setSymbolID("variable");
		}

		else{
			(yyval.symbol)->setSymbolType((yyvsp[-2].symbol)->getSymbolType());
			(yyval.symbol)->setSymbolID((yyvsp[-2].symbol)->getSymbolID());
		}

		codeSegOut << "\t\tPOP BX\t\t; Line " << lineCount << ": Load " << (yyvsp[-2].symbol)->getSymbolName() << endl;
		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Load " << (yyvsp[0].symbol)->getSymbolName() << endl;

		if((yyvsp[-1].symbol)->getSymbolName() == "+"){
			codeSegOut << "\t\tADD AX, BX\t\t; Line " << lineCount << ": Add " << (yyvsp[-2].symbol)->getSymbolName() << " and " << (yyvsp[0].symbol)->getSymbolName() << endl;
		}
		else{
			codeSegOut << "\t\tSUB AX, BX\t\t; Line " << lineCount << ": Subtract " << (yyvsp[-2].symbol)->getSymbolName() << " and " << (yyvsp[0].symbol)->getSymbolName() << endl;
		}
		codeSegOut << "\t\tPUSH AX\t\t;Save result" << endl;

		logOut << "simple_expression : simple_expression ADDOP term" << endl;
	}
#line 3994 "y.tab.c"
    break;

  case 67: /* term: unary_expression  */
#line 2156 "1905060.y"
                                {
		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolID((yyvsp[0].symbol)->getSymbolID());
		(yyval.symbol)->setSymbolClass("term : unary_expression");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		logOut << "term : unary_expression" << endl;
	}
#line 4010 "y.tab.c"
    break;

  case 68: /* term: term MULOP unary_expression  */
#line 2168 "1905060.y"
                                        {
		(yyvsp[-1].symbol)->setSymbolClass("MULOP : " + (yyvsp[-1].symbol)->getSymbolName());
		(yyvsp[-1].symbol)->isLeaf = true;
		
		(yyval.symbol) = new symbolInfo((yyvsp[-2].symbol)->getSymbolName() + " " + (yyvsp[-1].symbol)->getSymbolName() + " " + (yyvsp[0].symbol)->getSymbolName(), "NON_TERMINAL");
		(yyval.symbol)->setSymbolClass("term : term MULOP unary_expression");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-2].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		if((yyvsp[-2].symbol)->getSymbolType() == "VOID" || (yyvsp[0].symbol)->getSymbolType() == "VOID"){

		}
		if((yyvsp[-1].symbol)->getSymbolName() == "%"){
			if((yyvsp[-2].symbol)->getSymbolType() != "INT" || (yyvsp[0].symbol)->getSymbolType() != "INT"){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Operands of modulus must be integers " << endl;
			}
			else if((yyvsp[-2].symbol)->getSymbolType() == "INT" && (yyvsp[0].symbol)->getSymbolType() == "INT"){
				(yyval.symbol)->setSymbolType("INT");
				(yyval.symbol)->setSymbolID("variable");
			}

			else if((yyvsp[-2].symbol)->getSymbolType() == "ARRAY, INT" || (yyvsp[-2].symbol)->getSymbolType() == "ARRAY, FLOAT" || (yyvsp[0].symbol)->getSymbolType() == "ARRAY, INT" || (yyvsp[0].symbol)->getSymbolType() == "ARRAY, FLOAT"){

			}

			else{
				(yyval.symbol)->setSymbolType("VOID");
				(yyval.symbol)->setSymbolID("variable");
			}
			
		}
		else if((yyvsp[-1].symbol)->getSymbolName() == "/"){
			if((yyvsp[0].symbol)->getSymbolName() == "0"){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Warning: division by zero i=0f=1Const=0 " << endl;
			}
			if((yyvsp[-2].symbol)->getSymbolType() == "INT" && (yyvsp[0].symbol)->getSymbolType() == "INT"){
				(yyval.symbol)->setSymbolType("INT");
				(yyval.symbol)->setSymbolID("variable");
			}

			else if((yyvsp[-2].symbol)->getSymbolType() == "ARRAY, INT" || (yyvsp[-2].symbol)->getSymbolType() == "ARRAY, FLOAT" || (yyvsp[0].symbol)->getSymbolType() == "ARRAY, INT" || (yyvsp[0].symbol)->getSymbolType() == "ARRAY, FLOAT"){
				
			}

			else if(((yyvsp[-2].symbol)->getSymbolType() == "INT" && (yyvsp[0].symbol)->getSymbolType() == "FLOAT") || ((yyvsp[-2].symbol)->getSymbolType() == "FLOAT" && (yyvsp[0].symbol)->getSymbolType() == "INT")){
				(yyval.symbol)->setSymbolType("FLOAT");
				(yyval.symbol)->setSymbolID("variable");
			}

			else{
				(yyval.symbol)->setSymbolType("VOID");
				(yyval.symbol)->setSymbolID("variable");
			}
		}
		
		else{
			if((yyvsp[-2].symbol)->getSymbolType() == "INT" && (yyvsp[0].symbol)->getSymbolType() == "INT"){
				(yyval.symbol)->setSymbolType("INT");
				(yyval.symbol)->setSymbolID("variable");
			}
			else if(((yyvsp[-2].symbol)->getSymbolType() == "INT" && (yyvsp[0].symbol)->getSymbolType() == "FLOAT") || ((yyvsp[-2].symbol)->getSymbolType() == "FLOAT" && (yyvsp[0].symbol)->getSymbolType() == "INT")){
				(yyval.symbol)->setSymbolType("FLOAT");
				(yyval.symbol)->setSymbolID("variable");
			}

			else if((yyvsp[-2].symbol)->getSymbolType() == "FLOAT" && (yyvsp[0].symbol)->getSymbolType() == "FLOAT"){
				(yyval.symbol)->setSymbolType("FLOAT");
				(yyval.symbol)->setSymbolID("variable");
			}

			else if((yyvsp[-2].symbol)->getSymbolType() == "ARRAY, INT" || (yyvsp[-2].symbol)->getSymbolType() == "ARRAY, FLOAT" || (yyvsp[0].symbol)->getSymbolType() == "ARRAY, INT" || (yyvsp[0].symbol)->getSymbolType() == "ARRAY, FLOAT"){
				
			}
			
			else{
				(yyval.symbol)->setSymbolType("VOID");
				(yyval.symbol)->setSymbolID("variable");
			}
		}
		logOut << "term : term MULOP unary_expression" << endl;

		codeSegOut << "\t\tPOP BX\t\t; Line " << lineCount << ": Pop " << (yyvsp[-2].symbol)->getSymbolName() << endl;
		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Pop " << (yyvsp[0].symbol)->getSymbolName() << endl;
		codeSegOut << "\t\tXOR DX, DX\t\t; Line " << lineCount << ": Clear DX" << endl;
		if((yyvsp[-1].symbol)->getSymbolName() == "*"){
			codeSegOut << "\t\tIMUL BX\t\t; Line " << lineCount << ": Multiply " << (yyvsp[-2].symbol)->getSymbolName() << " and " << (yyvsp[0].symbol)->getSymbolName() << endl;
		}
		else{
			codeSegOut << "\t\tIDIV BX\t\t; Line " << lineCount << ": Divide " << (yyvsp[-2].symbol)->getSymbolName() << " and " << (yyvsp[0].symbol)->getSymbolName() << endl;
		}
		if((yyvsp[-1].symbol)->getSymbolName() == "%"){
			codeSegOut << "\t\tPUSH DX" << endl;
		}
		else{
			codeSegOut << "\t\tPUSH AX" << endl;
		}
	}
#line 4118 "y.tab.c"
    break;

  case 69: /* unary_expression: ADDOP unary_expression  */
#line 2274 "1905060.y"
                                           {
		(yyvsp[-1].symbol)->setSymbolClass("ADDOP : " + (yyvsp[-1].symbol)->getSymbolName());
		(yyvsp[-1].symbol)->isLeaf = true;
		
		if((yyvsp[0].symbol)->getSymbolType() == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}
		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolID((yyvsp[0].symbol)->getSymbolID());
		(yyval.symbol)->setSymbolClass("unary_expression : ADDOP unary_expression");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-1].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		codeSegOut << "\t\t; Line " << lineCount << ": " << (yyvsp[-1].symbol)->getSymbolName() << (yyvsp[0].symbol)->getSymbolName() << endl; 

		if((yyvsp[-1].symbol)->getSymbolName() == "-"){
			codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Pop " << (yyvsp[0].symbol)->getSymbolName() << " from stack" << endl;
			codeSegOut << "\t\tNEG AX\t\t; Negating the value of " << (yyvsp[0].symbol)->getSymbolName() << endl;
			codeSegOut << "\t\tPUSH AX\t\t; Saving the negative value" << endl;	
		}

		logOut << "unary_expression : ADDOP unary_expression" << endl;
	}
#line 4150 "y.tab.c"
    break;

  case 70: /* unary_expression: NOT unary_expression  */
#line 2302 "1905060.y"
                               {
		(yyvsp[-1].symbol)->setSymbolClass("NOT : !");
		(yyvsp[-1].symbol)->isLeaf = true;
		
		if((yyvsp[0].symbol)->getSymbolType() == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}

		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolID((yyvsp[0].symbol)->getSymbolID());
		(yyval.symbol)->setSymbolClass("unary_expression : NOT unary_expression");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-1].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		string label1 = generateNewLabel();
		string label2 = generateNewLabel();
		
		codeSegOut << "\t\tLine " << lineCount << ": ! " << (yyvsp[0].symbol)->getSymbolName() << endl;
		codeSegOut << "\t\tPOP AX\t\t; Load the value of " << (yyvsp[0].symbol)->getSymbolName() << endl;
		codeSegOut << "\t\tCMP AX, 0" << endl;
		codeSegOut << "\t\tJE " << label1 << endl;
		codeSegOut << "\t\tPUSH 0" << endl;
		codeSegOut << "\t\tJMP " << label2 << endl;
		codeSegOut << "\t" << label1 << ":" << endl;
		codeSegOut << "\t\t" << "PUSH 1" << endl;
		codeSegOut << "\t\t" << label2 << ": " << endl;

		logOut << "unary_expression : NOT unary_expression" << endl;
	}
#line 4188 "y.tab.c"
    break;

  case 71: /* unary_expression: factor  */
#line 2336 "1905060.y"
                 {
		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolID((yyvsp[0].symbol)->getSymbolID());
		(yyval.symbol)->setSymbolClass("unary_expression : factor");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		logOut << "unary_expression : factor" << endl;
	}
#line 4204 "y.tab.c"
    break;

  case 72: /* factor: variable  */
#line 2350 "1905060.y"
                   {
		(yyval.symbol) = new symbolInfo ((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolID((yyvsp[0].symbol)->getSymbolID());
		(yyval.symbol)->setSymbolClass("factor : variable");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		
		if((yyvsp[0].symbol)->getSymbolID() == "array" && globalScope == false){
			codeSegOut << "\t\tPOP BX" << endl;
		}

		symbolInfo *temp = st.lookUp((yyvsp[0].symbol)->getSymbolName());
		symbolInfo *tempGlobal = st.lookUpRoot((yyvsp[0].symbol)->getSymbolName());
		if(tempGlobal != nullptr && temp->getSymbolID() != "parameter"){
			codeSegOut << "\t\tPUSH " << (yyvsp[0].symbol)->getSymbolName() << "\t\t; Save " << (yyvsp[0].symbol)->getSymbolName() << endl;
		}
		else{
			if(temp != nullptr){
				if(temp->getSymbolID() == "parameter"){
					codeSegOut << "\t\tPUSH [BP + " << to_string(temp->getStackOffset()) << "]\t\t;Line " << lineCount << ": Save " << (yyvsp[0].symbol)->getSymbolName() << endl;
				}
				else{
					if(temp->getSymbolID() == "array" || temp->getSymbolID() == "ARRAY"){
						codeSegOut << "\t\tPUSH [BX]\t\t; Line " << lineCount << ": Save " << (yyvsp[0].symbol)->getSymbolName() << endl;
					}
					else{
						codeSegOut << "\t\tPUSH [BP-" << temp->getStackOffset() << "]\t\t; Line " << lineCount << ": Save " << (yyvsp[0].symbol)->getSymbolName() << endl;
					}
				}
			}
		}

		logOut << "factor : variable" << endl;
	}
#line 4245 "y.tab.c"
    break;

  case 73: /* factor: id LPAREN argument_list RPAREN  */
#line 2386 "1905060.y"
                                                {
		(yyvsp[-3].symbol)->setSymbolClass("ID : " + (yyvsp[-3].symbol)->getSymbolName());
		(yyvsp[-3].symbol)->isLeaf = true;
		
		(yyvsp[-2].symbol)->setSymbolClass("LPAREN : (");
		(yyvsp[-2].symbol)->isLeaf = true;
		
		(yyvsp[0].symbol)->setSymbolClass("RPAREN : )");
		(yyvsp[0].symbol)->isLeaf = true;
		
		(yyval.symbol) = new symbolInfo((yyvsp[-3].symbol)->getSymbolName(), (yyvsp[-3].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolClass("factor : ID LPAREN argument_list RPAREN");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-3].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-3].symbol));
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		symbolInfo* temp = st.lookUp((yyvsp[-3].symbol)->getSymbolName());

		if(temp == nullptr){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Undeclared function '" << (yyvsp[-3].symbol)->getSymbolName() << "' " << endl;
		}
		else{
			if(temp->getSymbolType() == "FUNCTION, VOID"){
				(yyval.symbol)->setSymbolType("VOID");
			}
		
			else if(temp->getSymbolType() == "FUNCTION, INT"){
				(yyval.symbol)->setSymbolType("INT");
			}

			else if(temp->getSymbolType() == "FUNCTION, FLOAT"){
				(yyval.symbol)->setSymbolType("FLOAT");
			}

			else{
				errorCount++;
				errorOut << "Line# " << lineCount << ": '" << (yyvsp[-3].symbol)->getSymbolName() << "' is not a function" << endl;
			}

			logOut << "factor : ID LPAREN argument_list RPAREN" << endl;
			
			int smaller = (arg_list.size() < temp->parameter_list.size()? arg_list.size() : temp->parameter_list.size());
			for(int i=0;i<smaller;i++){
				string type;
				if(temp->parameter_list[i]->getSymbolType() == "int"){
					type = "INT";
				}
				else if(temp->parameter_list[i]->getSymbolType() == "float"){
					type = "FLOAT";
				}
				if(type != arg_list[i]->getSymbolType()){
					errorCount++;
					errorOut << "Line# " << lineCount << ": Type mismatch for argument " << i+1 << " of '" << (yyvsp[-3].symbol)->getSymbolName() << "'" << endl;
				}
			}

			if(arg_list.size() < temp->parameter_list.size()){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Too few arguments to function '" << (yyvsp[-3].symbol)->getSymbolName() << "'" << endl;
			}
			else if(arg_list.size() > temp->parameter_list.size()){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Too many arguments to function '" << (yyvsp[-3].symbol)->getSymbolName() << "'" << endl;
			}

			codeSegOut << "\t\t; Line " << lineCount << ": " << "Call " << (yyvsp[-3].symbol)->getSymbolName() << endl;
			codeSegOut << "\t\tPUSH 0" << endl;
			codeSegOut << "\t\tCALL " << (yyvsp[-3].symbol)->getSymbolName() << "\t\t; Line " << lineCount << ": Call " << (yyvsp[-3].symbol)->getSymbolName() << endl;
			codeSegOut << "\t\tPOP AX\t\t; Pop the return value" << endl;
			codeSegOut << "\t\tADD SP, " << to_string(temp->parameter_list.size()*2) << "\t\t; Pop the arguments" << endl;
			if((yyvsp[-2].symbol)->getSymbolType() != "VOID"){
				codeSegOut << "\t\tPUSH AX\t\t; Push the return value" << endl;
			}
			else{
				codeSegOut << "\t\tPUSH 0\t\t; Push dummy value for other function returns" << endl; 
			}

			arg_list.clear();
		}
	}
#line 4335 "y.tab.c"
    break;

  case 74: /* factor: LPAREN expression RPAREN  */
#line 2472 "1905060.y"
                                        {
		(yyvsp[-2].symbol)->setSymbolClass("LPAREN : (");
		(yyvsp[-2].symbol)->isLeaf = true;
		
		(yyvsp[0].symbol)->setSymbolClass("RPAREN : )");
		(yyvsp[0].symbol)->isLeaf = true;
		
		(yyval.symbol) = new symbolInfo((yyvsp[-1].symbol)->getSymbolName(), (yyvsp[-1].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolClass("factor : LPAREN expression RPAREN");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-2].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		symbolInfo* temp = st.lookUp((yyvsp[-1].symbol)->getSymbolName());
		
		if(temp == nullptr){
			
		}

		else{
			if(temp->getSymbolType() == "FUNCTION, VOID"){
				
			}
		
			else if(temp->getSymbolType() == "FUNCTION, INT"){
				(yyval.symbol)->setSymbolType("INT");
			}

			else if(temp->getSymbolType() == "FUNCTION, FLOAT"){
				(yyval.symbol)->setSymbolType("FLOAT");
			}

			else if(temp->getSymbolType() == "INT"){
				(yyval.symbol)->setSymbolType("INT");
			}

			else if(temp->getSymbolType() == "FLOAT"){
				(yyval.symbol)->setSymbolType("FLOAT");
			}
		}
		logOut << "factor : LPAREN expression RPAREN" << endl;
	}
#line 4385 "y.tab.c"
    break;

  case 75: /* factor: CONST_INT  */
#line 2518 "1905060.y"
                    {
		(yyvsp[0].symbol)->setSymbolClass("CONST_INT : " + (yyvsp[0].symbol)->getSymbolName());
		(yyvsp[0].symbol)->isLeaf = true;
		
		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), "INT");
		(yyval.symbol)->setSymbolID("constant");
		(yyval.symbol)->setSymbolClass("factor : CONST_INT");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		codeSegOut << "\t\tPUSH " << (yyvsp[0].symbol)->getSymbolName() << "\t\t;Line " << lineCount << ": Save " << (yyvsp[0].symbol)->getSymbolName() << endl;
		logOut << "factor : CONST_INT" << endl;
	}
#line 4405 "y.tab.c"
    break;

  case 76: /* factor: CONST_FLOAT  */
#line 2534 "1905060.y"
                        {
		(yyvsp[0].symbol)->setSymbolClass("CONST_FLOAT : " + (yyvsp[0].symbol)->getSymbolName());
		(yyvsp[0].symbol)->isLeaf = true;
		
		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), "FLOAT");
		(yyval.symbol)->setSymbolID("constant");
		(yyval.symbol)->setSymbolClass("factor : CONST_FLOAT");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		logOut << "factor : CONST_FLOAT" << endl;
	}
#line 4424 "y.tab.c"
    break;

  case 77: /* factor: variable INCOP  */
#line 2549 "1905060.y"
                         {
		(yyvsp[0].symbol)->setSymbolClass("INCOP : " + (yyvsp[0].symbol)->getSymbolName());
		(yyvsp[0].symbol)->isLeaf = true;
		
		string type;
		if((yyvsp[-1].symbol)->getSymbolType() == "INT"){
			type = "INT";
		}

		else{
			type = "FLOAT";
		}

		(yyval.symbol) = new symbolInfo((yyvsp[-1].symbol)->getSymbolName(), type);
		(yyval.symbol)->setSymbolID("variable");
		(yyval.symbol)->setSymbolClass("factor : variable INCOP");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-1].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		string op;

		if((yyvsp[0].symbol)->getSymbolName() == "++"){
			op = "INC";
		}

		else{
			op = "DEC";
		}

		symbolInfo* temp = st.lookUp((yyvsp[-1].symbol)->getSymbolName());

		if(temp->getSymbolID() == "parameter"){
			codeSegOut << "\t\tPUSH [BP+" << temp->getStackOffset() << "]\t\t;Line " << lineCount << ": Save " << (yyvsp[-1].symbol)->getSymbolName() << endl;
			codeSegOut << "\t\t" << op << " W. [BP+" << temp->getStackOffset() << "]\t\t;Line " << lineCount << ": " << op << " " << (yyvsp[-1].symbol)->getSymbolName() << endl;
		}

		else{
			codeSegOut << "\t\tPUSH [BP-" << temp->getStackOffset() << "]\t\t;Line " << lineCount << ": Save " << (yyvsp[-1].symbol)->getSymbolName() << endl;
			codeSegOut << "\t\t" << op << " W. [BP-" << temp->getStackOffset() << "]\t\t;Line " << lineCount << ": " << op << " " << (yyvsp[-1].symbol)->getSymbolName() << endl;
		}
		
		logOut << "factor : variable INCOP" << endl;
	}
#line 4475 "y.tab.c"
    break;

  case 78: /* argument_list: arguments  */
#line 2597 "1905060.y"
                                {
		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolClass("argument_list : arguments");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		logOut << "argument_list : arguments" << endl;
	}
#line 4490 "y.tab.c"
    break;

  case 79: /* argument_list: %empty  */
#line 2608 "1905060.y"
                {
		(yyval.symbol) = new symbolInfo("NULL", "NULL");
		(yyval.symbol)->setSymbolClass("argument_list : ");
		(yyval.symbol)->isLeaf = false;
		
		logOut << "argument_list : " << endl;
	}
#line 4502 "y.tab.c"
    break;

  case 80: /* arguments: arguments COMMA logic_expression  */
#line 2617 "1905060.y"
                                                {
		(yyvsp[-1].symbol)->setSymbolClass("COMMA : " + (yyvsp[-1].symbol)->getSymbolName());
		(yyvsp[-1].symbol)->isLeaf = true;
		
		(yyval.symbol) = new symbolInfo((yyvsp[-2].symbol)->getSymbolName(), (yyvsp[-2].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolClass("arguments : arguments COMMA logic_expression");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[-2].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[-2].symbol));
		(yyval.symbol)->insertChild((yyvsp[-1].symbol));
		(yyval.symbol)->insertChild((yyvsp[0].symbol));

		if((yyvsp[0].symbol)->getSymbolType() == "ARRAY, INT"){
			(yyvsp[0].symbol)->setSymbolType("INT");
		}
		else if((yyvsp[0].symbol)->getSymbolType() == "ARRAY, FLOAT"){
			(yyvsp[0].symbol)->setSymbolType("FLOAT");
		}
		arg_list.push_back((yyvsp[0].symbol));
		logOut << "arguments : arguments COMMA logic_expression" << endl;
	}
#line 4529 "y.tab.c"
    break;

  case 81: /* arguments: logic_expression  */
#line 2640 "1905060.y"
                                {
		arg_list.clear();
		(yyval.symbol) = new symbolInfo((yyvsp[0].symbol)->getSymbolName(), (yyvsp[0].symbol)->getSymbolType());
		(yyval.symbol)->setSymbolClass("arguments : logic_expression");
		(yyval.symbol)->isLeaf = false;
		(yyval.symbol)->setStartLine((yyvsp[0].symbol)->getStartLine());
		(yyval.symbol)->setEndLine((yyvsp[0].symbol)->getEndLine());
		(yyval.symbol)->insertChild((yyvsp[0].symbol));
		
		if((yyvsp[0].symbol)->getSymbolType() == "ARRAY, INT"){
			(yyvsp[0].symbol)->setSymbolType("INT");
		}
		else if((yyvsp[0].symbol)->getSymbolType() == "ARRAY, FLOAT"){
			(yyvsp[0].symbol)->setSymbolType("FLOAT");
		}
		arg_list.push_back((yyvsp[0].symbol));
		logOut << "arguments : logic_expression" << endl;
	}
#line 4552 "y.tab.c"
    break;


#line 4556 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          goto yyexhaustedlab;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if 1
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 2660 "1905060.y"



int main(int argc,char *argv[])
{

	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}

	FILE *fp = fopen(argv[1], "r");

	if(fp == NULL){
		cout << "Cannot open input file" << endl;
		exit(1);
	}

	init();

	logOut.open("log.txt");
	parseOut.open("parseTree.txt");
	//errorOut.open("error.txt");
	asmOut.open("assembly.asm");
	codeSegOut.open("codeSegment.asm");
	optimizedAsmOut.open("optimized_assembly.asm");

	asmOut << ".MODEL SMALL\n.STACK 100H\n.DATA" << endl;

	codeSegOut << ".CODE" << endl;

	lineCount = 1;
	
	yyin = fp;
	yyparse();

	printNewLine();
	printFromBX();

	codeSegOut << "END MAIN" << endl;

	asmOut.close();
	codeSegOut.close();

	asmOut.open("assembly.asm", ios::app);

	ifstream codeSegIn("codeSegment.asm");

	string line;
	while(getline(codeSegIn, line)){
		asmOut << line << endl;
	}

	//asmOut << endl;

	optimizeCode();

	logOut << "Total Lines : " << lineCount << endl;
	logOut << "Total Errors : " << errorCount << endl;

	printTree(startSymbol, 0);
 
	return 0;
}
