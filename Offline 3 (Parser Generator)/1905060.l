%option noyywrap
%x STRING_STATE
%x SINGLE_COMMENT_STATE
%x MULTILINE_COMMENT_STATE

%{
#include<cstdio>
#include<cstdlib>
#include<string>
#include<vector>
#include <bits/stdc++.h>
#include "symbolTable.hpp"
#include "y.tab.h"


using namespace std;

FILE *logout;
FILE *tokenout;

ofstream tokenStream;
ofstream logOut;
ofstream errorOut;

int lineCount = 1;

string constructed_string;
string token_string;
string log_string;
bool multiline_string;
int line_in_string_state;

string log_comment;
string constrcuted_comment;
int line_in_comment_state;
bool multiline_comment;

symbolTable st(10);

//extern YYSTYPE yylval;
//extern symbolTable* symbol_table;
void yyerror(char *);


char convertToASCII(string s){
    if(s.length()==3 && s[1] != '\\'){
        return (char) s[1];
    }
    else if(s.length()==4 && s[1] == '\\'){
        if(s[2] == '\''){
            return (char) 39;
        }
        else if(s[2] == '\"'){
            return (char) 34;
        }
        else if(s[2] == 'n'){
            return (char) 10;
        }
        else if(s[2] == 't'){
            return (char) 9;
        }
        else if(s[2] = '\\'){
            return (char) 92;
        }
        else if(s[2] == 'a'){
            return (char) 7;
        }
        else if(s[2] == 'f'){
            return (char) 12;
        }
        else if(s[2] == 'r'){
            return (char) 13;
        }
        else if(s[2] == 'b'){
            return (char) 8;
        }
        else if(s[2] == 'v'){
            return (char) 11;
        }
        else if(s[2] == '0'){
            return (char) 0;
        }
        else{
            return (char) 127;
        }
    }
    else{
        return (char) 127;
    }
}

//symbolTable st(10);

%}

WHITESPACE [ \t\f\r\v]+
LETTER [a-zA-Z_]
DIGIT [0-9]
IDENTIFIER [a-zA-z0-9_]
NEWLINE [\r]?\n
ALL_EXCEPT_BACKSLASH [^\\]
EMPTY_CHAR (\'\')
SPECIAL_CHAR [\n\r]
CONST_CHAR [\'{LETTER}\'|\'\\n\'|\'\\t\'|\'\\\\\'|\'\\\\'|\'\\a\'|\'\\f\'|\'\\r\'|\'\\b\'|\'\\v\'|\'\\0\'|\'\\['"]\']


%%

{WHITESPACE}    {

}

{NEWLINE} {
               lineCount++; 
}

"if"	 {
                symbolInfo* temp = new symbolInfo(yytext, "IF");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": TOken <IF> Lexeme if found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return IF;
}
		
"else"	 {      
                symbolInfo* temp = new symbolInfo(yytext, "ELSE");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <ELSE> Lexeme else found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return ELSE;
}

"for"    {
                symbolInfo* temp = new symbolInfo(yytext, "FOR");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <FOR> Lexeme for found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return FOR;
}

"while"   {     
                symbolInfo* temp = new symbolInfo(yytext, "WHILE");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <WHILE> Lexeme while found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return WHILE;
}

"do"      {
                symbolInfo* temp = new symbolInfo(yytext, "DO");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <DO> Lexeme do found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return DO;
}

"break"   {
                symbolInfo* temp = new symbolInfo(yytext, "BREAK");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <BREAK> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return BREAK;
}

"int"     {
                symbolInfo* temp = new symbolInfo(yytext, "INT");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <INT> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return INT;
}

"char"    {
                //tokenStream << "<CHAR, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "CHAR");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <CHAR> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return CHAR;
}

"float"   {
                //tokenStream << "<FLOAT, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "FLOAT");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <FLOAT> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return FLOAT;
}

"double"  {
                //tokenStream << "<DOUBLE, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "DOUBLE");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <DOUBLE> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return DOUBLE;
}

"void"    {
                //tokenStream << "<VOID, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "VOID");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <VOID> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return VOID;
}

"return"  {
                //tokenStream << "<RETURN, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "RETURN");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <RETURN> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return RETURN;
}

"switch"  {
                //tokenStream << "<SWITCH, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "SWITCH");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <SWITCH> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return SWITCH;
}

"case"    {
                //tokenStream << "<CASE, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "CASE");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <CASE> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return CASE;
}

"default" {
                //tokenStream << "<DEFAULT, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "DEFAULT");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <DEFAULT> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return DEFAULT;
}

"continue" {
                //tokenStream << "<CONTINUE, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "CONTINUE");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <CONTINUE> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return CONTINUE;
}

"+"		|

"-"		{
                //tokenStream << "<ADDOP, " << yytext << ">" << endl;
                logOut << "Line# " << lineCount << ": Token <ADDOP> Lexeme " << yytext << " found" << endl;
                symbolInfo *temp = new symbolInfo(yytext, "ADDOP");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return ADDOP;

}

"*"|"/"|"%"  {
                //tokenStream << "<MULOP, " << yytext << ">" << endl;
                logOut << "Line# " << lineCount << ": Token <MULOP> Lexeme " << yytext << " found" << endl;
                symbolInfo *temp = new symbolInfo (yytext, "MULOP");
                yylval.symbol = temp;
                //yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return MULOP;

}

"++"|"--"   {
               //tokenStream << "<INCOP, " << yytext << ">" << endl;
                logOut << "Line# " << lineCount << ": Token <INCOP> Lexeme " << yytext << " found" << endl;
                symbolInfo *temp = new symbolInfo (yytext, "INCOP");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return INCOP;
}

"<"|"<="|">"|">="|"=="|"!="  {
                //tokenStream << "<RELOP, " << yytext << ">" << endl;
                logOut << "Line# " << lineCount << ": Token <RELOP> Lexeme " << yytext << " found" << endl;
                symbolInfo *temp = new symbolInfo(yytext, "RELOP");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return RELOP;
}

"=" {
                //tokenStream << "<ASSIGNOP, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "ASSIGNOP");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <ASSIGNOP> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return ASSIGNOP;
}

"&&"|"||"  {
                //tokenStream << "<LOGICOP, " << yytext << ">" << endl;
                logOut << "Line# " << lineCount << ": Token <LOGICOP> Lexeme " << yytext << " found" << endl;
                symbolInfo *temp = new symbolInfo(yytext, "LOGICOP");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return LOGICOP;
}

"&"|"|"|"^"|"<<"|">>"  {
                //tokenStream << "<BITOP, " << yytext << ">" << endl;
                logOut << "Line# " << lineCount << ": Token <BITOP> Lexeme " << yytext << " found" << endl;
                symbolInfo *temp = new symbolInfo(yytext, "BITOP");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return BITOP;
}

"!" {
                //tokenStream << "<NOT, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "NOT");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <NOT> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return NOT;
}

"(" {
                //tokenStream << "<LPAREN, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "LPAREN");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <LPAREN> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return LPAREN;
}

")" {
                //tokenStream << "<RPAREN, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "RPAREN");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <RPAREN> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return RPAREN;
}

"{" {
                //tokenStream << "<LCURL, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "LCURL");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <LCURL> Lexeme " << yytext << " found" << endl;
                //st.enterScope(st.getScopeNumber() + 1);
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return LCURL;
}

"}" {
                //tokenStream << "<RCURL, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "RCURL");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <RCURL> Lexeme " << yytext << " found" << endl;
                //st.printAllScope(logOut);
                //st.exitScope();
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return RCURL;
}

"[" {
                //tokenStream << "<LSQUARE, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "LSQUARE");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <LSQUARE> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return LSQUARE;
}

"]" {
                //tokenStream << "<RSQUARE, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "RSQUARE");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <RSQUARE> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return RSQUARE;
}

"," {
                //tokenStream << "<COMMA, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "COMMA");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <COMMA> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return COMMA;
}

";" {
                //tokenStream << "<SEMICOLON, " << yytext << ">" << endl;
                symbolInfo* temp = new symbolInfo(yytext, "SEMICOLON");
                yylval.symbol = temp;
                logOut << "Line# " << lineCount << ": Token <SEMICOLON> Lexeme " << yytext << " found" << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return SEMICOLON;
}

		
{DIGIT}+ 	{
                //tokenStream << "<CONST_INT, " << yytext << ">" << endl;
                logOut << "Line# " << lineCount << ": Token <CONST_INT> Lexeme " << yytext << " found" << endl;
                symbolInfo *temp = new symbolInfo(yytext, "CONST_INT");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return CONST_INT;
}

{DIGIT}+(\.{DIGIT}+)?(E[+-]?{DIGIT}+)?    {
                //tokenStream << "<CONST_FLOAT, " << yytext << ">" << endl;
                logOut << "Line# " << lineCount << ": Token <CONST_FLOAT> Lexeme " << yytext << " found" << endl;
                symbolInfo *temp = new symbolInfo(yytext, "CONST_FLOAT");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return CONST_FLOAT;
}


\'{LETTER}\'|\'\\n\'|\'\\t\'|\'\\\\\'|\'\\\\'|\'\\a\'|\'\\f\'|\'\\r\'|\'\\b\'|\'\\v\'|\'\\0\'|\'\\['"]\'   {
                //tokenStream << "<CONST_CHAR, " << convertToASCII(yytext) << ">" << endl;
                logOut << "line# " << lineCount << ": Token <CONST_CHAR> Lexeme " << convertToASCII(yytext) << " found" << endl;
                symbolInfo *temp = new symbolInfo(yytext, "CONST_CHAR");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return CONST_CHAR;
}

[a-zA-Z_][a-zA-Z_0-9]*    {
                logOut << "Line# " << lineCount << ": Token <ID> Lexeme " << yytext << " found" << endl;
                //tokenStream << "<ID, " << yytext << ">" << endl;
                symbolInfo *temp = new symbolInfo(yytext, "ID");
                yylval.symbol = temp;
                //cout << lineCount << endl;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                return ID;
}

{DIGIT}+(\.{DIGIT}+)(E[+-]?\.+{DIGIT}+)+    {
                //errorCount++;
                logOut << "Error at line# " << lineCount << ": TOO_MANY_DECIMAL_POINTS " << yytext << endl;
                symbolInfo *temp = new symbolInfo(yytext, "TOO_MANY_DECIMAL_POINTS");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return TOO_MANY_DECIMAL_POINTS;
}

{DIGIT}*(\.{DIGIT}+)?(E[+-]?{DIGIT}*\.{DIGIT}+)    {
                //errorCount++;
                //logStream << "Error at line# " << lineCount << ": ILLFORMED_NUMBER " << yytext << endl;
                symbolInfo *temp = new symbolInfo(yytext, "ILLFORMED_NUMBER");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return ILLFORMED_NUMBER;
}

\'{LETTER}*[;]?|\'\\'     {
                //errorCount++;
                //logStream << "Error at line# " << lineCount << ": UNFINISHED_CONST_CHAR " << yytext << endl;
                symbolInfo *temp = new symbolInfo(yytext, "UNFINISHED_CONST_CHAR");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return UNFINISHED_CONST_CHAR;
}

\'{LETTER}{LETTER}+\'         {
                //errorCount++;
                //logStream << "Error at line# " << lineCount << ": MULTICHAR_CONST_CHAR " << yytext << endl;
                symbolInfo *temp = new symbolInfo(yytext, "MULTICHAR_CONST_CHAR");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return MULTICHAR_CONST_CHAR;
}

{EMPTY_CHAR}             {
                //errorCount++;
                //logStream << "Error at line# " << lineCount << ": EMPTY_CONST_CHAR ''" << endl;
                symbolInfo *temp = new symbolInfo(yytext, "EMPTY_CONST_CHAR");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return EMPTY_CONST_CHAR;
}

{DIGIT}+{LETTER}*           {
                //errorCount++;
                //logStream << "Error at line# " << lineCount << ": INVALID_ID_SUFFIX_NUM_PREFIX " << yytext << endl;
                symbolInfo *temp = new symbolInfo(yytext, "INVALID_ID_SUFFIX_NUM_PREFIX");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return INVALID_ID_SUFFIX_NUM_PREFIX;
}

\"      {
                constructed_string.clear();
                log_string.clear();
                token_string.clear();
                line_in_string_state = 0;
                BEGIN STRING_STATE;
}

<STRING_STATE>\\n        {
                log_string += "\\n";
                token_string += "\n";
}

<STRING_STATE>\\t        {
                log_string += "\\t";
                token_string += "\t";
}

<STRING_STATE>\\\\        {
                log_string += "\\\\";
                token_string += "\\";
}

<STRING_STATE>\\a        {
                log_string += "\\a";
                token_string += "\a";
}

<STRING_STATE>\\f        {
                log_string += "\\f";
                token_string += "\f";
}

<STRING_STATE>\\r        {
                log_string += "\\r";
                token_string += "\r";
}

<STRING_STATE>\\b        {
                log_string += "\\b";
                token_string += "\b";
}

<STRING_STATE>\\v        {
                log_string += "\\v";
                token_string += "\v";
}

<STRING_STATE>\\0        {
                log_string += "\\0";
                token_string += "\0";
}

<STRING_STATE>\\\r\n      {
               line_in_string_state++;
               log_string += "\\\r\n";
               token_string += "\r\n";
}

<STRING_STATE>\\\n        {
               multiline_string = true;
               line_in_string_state++;
               log_string += "\\\n";
               token_string += "\t";
}

<STRING_STATE>[\r]?\n     {
               line_in_string_state++;
               //errorCount++;
               //logStream << "Error at line# " << lineCount << ": UNFINISHED_STRING \"" << log_string << endl;
               lineCount += line_in_string_state;
               line_in_string_state = 0;
               BEGIN INITIAL;
}

<STRING_STATE><<EOF>>         {
                //errorCount++;
                //logStream << "Error at line# " << lineCount << ": UNFINISHED_STRING \"" << log_string << "\n" << endl;
                lineCount += line_in_string_state;
                line_in_string_state = 0;
                BEGIN INITIAL;
}

<STRING_STATE>\"            {
                constructed_string = "\"";
                constructed_string += log_string;
                constructed_string += "\"";
                /*if(!multiline_string){
                    tokenStream << "<SINGLE LINE STRING, " << token_string << ">" << endl;
                    logStream << "Line# " << lineCount << ": Token <SINGLE LINE STRING> Lexeme " << constructed_string << "found" << endl;
                }
                else{
                    tokenStream << "<MULTI LINE STRING, " << token_string << ">" << endl;
                    logStream << "Line# " << lineCount << ": Token <MULTI LINE STRING> Lexeme " << constructed_string << "found" << endl;
                }*/
                lineCount += line_in_string_state;
                line_in_string_state = 0;
                BEGIN INITIAL;
}

<STRING_STATE>.         {
                token_string += yytext[0];
                log_string += yytext[0];
}

\/\/                    {
                constrcuted_comment.clear();
                log_comment.clear();
                log_comment += "\\";
                line_in_comment_state = 0;
                BEGIN SINGLE_COMMENT_STATE;
}

<SINGLE_COMMENT_STATE>\\\r\n        {
                line_in_comment_state+=2;
                log_comment += "\\\r\n";
}

<SINGLE_COMMENT_STATE>\\\n          {
                line_in_comment_state++;
                log_comment += "\\\n";
}

<SINGLE_COMMENT_STATE>[\r]?\n       {
                line_in_comment_state++;
                //logStream << "Line# "<< lineCount << ": Token <SINGLE LINE COMMENT> Lexeme " << log_comment << " found" << endl;
                lineCount += line_in_comment_state;
                line_in_comment_state = 0;
                BEGIN INITIAL;
}

<SINGLE_COMMENT_STATE><<EOF>>       {
                //logStream << "Line# "<< lineCount << ": Token <SINGLE LINE COMMENT> Lexeme " << log_comment << " found" << endl;
                lineCount += line_in_comment_state;
                line_in_comment_state = 0;
                BEGIN INITIAL;
}

<SINGLE_COMMENT_STATE>\\            {
                log_comment += "\\";
}

<SINGLE_COMMENT_STATE>.             {
                log_comment += yytext[0];
}

\/\*                {
                constrcuted_comment.clear();
                log_comment.clear();
                log_comment += "/*";
                line_in_comment_state = 0;
                BEGIN MULTILINE_COMMENT_STATE;
}

<MULTILINE_COMMENT_STATE>\\\r\n      {
                line_in_comment_state++;
                log_comment += "\\\r\n";
}

<MULTILINE_COMMENT_STATE>\\\n        {
                line_in_comment_state++;
                log_comment += "\\\n";
}

<MULTILINE_COMMENT_STATE>\*\/       {
                log_comment += "*/";
                //logStream << "Line# " << lineCount << ": Token <MULTI LINE COMMENT> Lexeme " << log_comment << "found" << endl;
                lineCount += line_in_comment_state;
                line_in_comment_state = 0;
                BEGIN INITIAL;
}

<MULTILINE_COMMENT_STATE><<EOF>>    {
                //errorCount++;
                //logStream << "Error at line# " << lineCount << ": UNFINISHED_COMMENT " << log_comment << endl;
                lineCount += line_in_comment_state;
                line_in_comment_state = 0;
                BEGIN INITIAL;
}

<MULTILINE_COMMENT_STATE>.          {
                log_comment += yytext[0];
}

@|.             {
                //errorCount++;
                //logStream << "Error at line# " << lineCount << ": UNRECOGNIZED_CHAR " << yytext << endl;
                symbolInfo *temp = new symbolInfo(yytext, "UNRECOGNIZED_CHAR");
                yylval.symbol = temp;
                yylval.symbol->setStartLine(lineCount);
                yylval.symbol->setEndLine(lineCount);
                //return UNRECOGNIZED_CHAR;
}

%%