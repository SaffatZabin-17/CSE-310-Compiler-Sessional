%option noyywrap
%x STRING_STATE
%x SINGLE_COMMENT_STATE
%x MULTILINE_COMMENT_STATE

%{
#include<cstdio>
#include<cstdlib>
#include<string>
#include<vector>

using namespace std;

#include "symbolTable.h"

int lineCount=1;
int errorCount = 0;

FILE *logout;
FILE *tokenout;

ofstream tokenStream;
ofstream logStream;

string constructed_string;
string token_string;
string log_string;
bool multiline_string;
int line_in_string_state;

string log_comment;
string constrcuted_comment;
int line_in_comment_state;
bool multiline_comment;

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

symbolTable st(10);

void init(){
    st.setBucketSize(10);
}

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
			    tokenStream << "<IF, " << yytext << ">" << endl;
			    logStream << "Line# " << lineCount << ": Token <IF> Lexeme " << yytext << " found" << endl;
}
		
"else"	 {
			    tokenStream << "<ELSE, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <ELSE> Lexeme " << yytext << " found" << endl;
}

"for"    {
                tokenStream << "<FOR, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <FOR> Lexeme " << yytext << " found" << endl;
}

"while"   {
                tokenStream << "<WHILE, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <WHILE> Lexeme " << yytext << " found" << endl;
}

"do"      {
                tokenStream << "<DO, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <DO> Lexeme " << yytext << " found" << endl;
}

"break"   {
                tokenStream << "<BREAK, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <BREAK> Lexeme " << yytext << " found" << endl;
}

"int"     {
                tokenStream << "<INT, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <INT> Lexeme " << yytext << " found" << endl;
}

"char"    {
                tokenStream << "<CHAR, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <CHAR> Lexeme " << yytext << " found" << endl;
}

"float"   {
                tokenStream << "<FLOAT, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <FLOAT> Lexeme " << yytext << " found" << endl;
}

"double"  {
                tokenStream << "<DOUBLE, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <DOUBLE> Lexeme " << yytext << " found" << endl;
}

"void"    {
                tokenStream << "<VOID, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <VOID> Lexeme " << yytext << " found" << endl;
}

"return"  {
                tokenStream << "<RETURN, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <RETURN> Lexeme " << yytext << " found" << endl;
}

"switch"  {
                tokenStream << "<SWITCH, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <SWITCH> Lexeme " << yytext << " found" << endl;
}

"case"    {
                tokenStream << "<CASE, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <CASE> Lexeme " << yytext << " found" << endl;
}

"default" {
                tokenStream << "<DEFAULT, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <DEFAULT> Lexeme " << yytext << " found" << endl;
}

"continue" {
                tokenStream << "<CONTINUE, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <CONTINUE> Lexeme " << yytext << " found" << endl;
}

"+"		|

"-"		{
                tokenStream << "<ADDOP, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <ADDOP> Lexeme " << yytext << " found" << endl;
}

"*"|"/"|"%"  {
                tokenStream << "<MULOP, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <MULOP> Lexeme " << yytext << " found" << endl;
}

"++"|"--"   {
               tokenStream << "<INCOP, " << yytext << ">" << endl;
               logStream << "Line# " << lineCount << ": Token <INCOP> Lexeme " << yytext << " found" << endl;
}

"<"|"<="|">"|">="|"=="|"!="  {
                tokenStream << "<RELOP, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <RELOP> Lexeme " << yytext << " found" << endl;
}

"=" {
                tokenStream << "<ASSIGNOP, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <ASSIGNOP> Lexeme " << yytext << " found" << endl;
}

"&&"|"||"  {
                tokenStream << "<LOGICOP, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <LOGICOP> Lexeme " << yytext << " found" << endl;
}

"&"|"|"|"^"|"<<"|">>"  {
                tokenStream << "<BITOP, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <BITOP> Lexeme " << yytext << " found" << endl;
}

"!" {
                tokenStream << "<NOT, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <NOT> Lexeme " << yytext << " found" << endl;
}

"(" {
                tokenStream << "<LPAREN, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <LPAREN> Lexeme " << yytext << " found" << endl;
}

")" {
                tokenStream << "<RPAREN, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <RPAREN> Lexeme " << yytext << " found" << endl;
}

"{" {
                tokenStream << "<LCURL, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <LCURL> Lexeme " << yytext << " found" << endl;
                cout << "Entering new scope" << endl;
                st.enterScope(st.getScopeNumber() + 1);
}

"}" {
                tokenStream << "<RCURL, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <RCURL> Lexeme " << yytext << " found" << endl;
                st.exitScope();
}

"[" {
                tokenStream << "<LSQUARE, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <LSQUARE> Lexeme " << yytext << " found" << endl;
}

"]" {
                tokenStream << "<RSQUARE, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <RSQUARE> Lexeme " << yytext << " found" << endl;
}

"," {
                tokenStream << "<COMMA, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <COMMA> Lexeme " << yytext << " found" << endl;
}

";" {
                tokenStream << "<SEMICOLON, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <SEMICOLON> Lexeme " << yytext << " found" << endl;
}
		
{DIGIT}+ 	{
                tokenStream << "<CONST_INT, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <CONST_INT> Lexeme " << yytext << " found" << endl;
}

{DIGIT}+(\.{DIGIT}+)?(E[+-]?{DIGIT}+)?    {
                tokenStream << "<CONST_FLOAT, " << yytext << ">" << endl;
                logStream << "Line# " << lineCount << ": Token <CONST_FLOAT> Lexeme " << yytext << " found" << endl;
}


\'{LETTER}\'|\'\\n\'|\'\\t\'|\'\\\\\'|\'\\\\'|\'\\a\'|\'\\f\'|\'\\r\'|\'\\b\'|\'\\v\'|\'\\0\'|\'\\['"]\'   {
                tokenStream << "<CONST_CHAR, " << convertToASCII(yytext) << ">" << endl;
                logStream << "line# " << lineCount << ": Token <CONST_CHAR> Lexeme " << convertToASCII(yytext) << " found" << endl;
}

[a-zA-Z_][a-zA-Z_0-9]*    {
                logStream << "line# " << lineCount << ": Token <ID> Lexeme " << yytext << " found" << endl;
                tokenStream << "<ID, " << yytext << ">" << endl;
                if(st.lookUp(yytext)){
                    logStream << "\t" << yytext << " already exists in the current ScopeTable" << endl;
                }
                else{
                    cout << "Inside else block" << endl;
                    symbolInfo* temp = new symbolInfo(yytext, "ID");
                    st.insert(*temp);
                    st.printAllScope(logStream);
                }
}

{DIGIT}+(\.{DIGIT}+)(E[+-]?\.+{DIGIT}+)+    {
                errorCount++;
                logStream << "Error at line# " << lineCount << ": TOO_MANY_DECIMAL_POINTS " << yytext << endl;
}

{DIGIT}*(\.{DIGIT}+)?(E[+-]?{DIGIT}*\.{DIGIT}+)    {
                errorCount++;
                logStream << "Error at line# " << lineCount << ": ILLFORMED_NUMBER " << yytext << endl;
}

\'{LETTER}*[;]?|\'\\'     {
                errorCount++;
                logStream << "Error at line# " << lineCount << ": UNFINISHED_CONST_CHAR " << yytext << endl;
}

\'{LETTER}{LETTER}+\'         {
                errorCount++;
                logStream << "Error at line# " << lineCount << ": MULTICHAR_CONST_CHAR " << yytext << endl;
}

{EMPTY_CHAR}             {
                errorCount++;
                logStream << "Error at line# " << lineCount << ": EMPTY_CONST_CHAR ''" << endl;
}

{DIGIT}+{LETTER}*           {
                errorCount++;
                logStream << "Error at line# " << lineCount << ": INVALID_ID_SUFFIX_NUM_PREFIX " << yytext << endl;
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
               token_string += "\n";
}

<STRING_STATE>[\r]?\n     {
               line_in_string_state++;
               errorCount++;
               logStream << "Error at line# " << lineCount << ": UNFINISHED_STRING \"" << log_string << endl;
               lineCount += line_in_string_state;
               line_in_string_state = 0;
               BEGIN INITIAL;
}

<STRING_STATE><<EOF>>         {
                errorCount++;
                logStream << "Error at line# " << lineCount << ": UNFINISHED_STRING \"" << log_string << "\n" << endl;
                lineCount += line_in_string_state;
                line_in_string_state = 0;
                BEGIN INITIAL;
}

<STRING_STATE>\"            {
                constructed_string = "\"";
                constructed_string += log_string;
                constructed_string += "\"";
                if(!multiline_string){
                    tokenStream << "<SINGLE LINE STRING, " << token_string << ">" << endl;
                    logStream << "Line# " << lineCount << ": Token <SINGLE LINE STRING> Lexeme " << constructed_string << "found" << endl;
                }
                else{
                    tokenStream << "<MULTI LINE STRING, " << token_string << ">" << endl;
                    logStream << "Line# " << lineCount << ": Token <MULTI LINE STRING> Lexeme " << constructed_string << "found" << endl;
                }
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
                logStream << "Line# "<< lineCount << ": Token <SINGLE LINE COMMENT> Lexeme " << log_comment << " found" << endl;
                lineCount += line_in_comment_state;
                line_in_comment_state = 0;
                BEGIN INITIAL;
}

<SINGLE_COMMENT_STATE><<EOF>>       {
                logStream << "Line# "<< lineCount << ": Token <SINGLE LINE COMMENT> Lexeme " << log_comment << " found" << endl;
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
                logStream << "Line# " << lineCount << ": Token <MULTI LINE COMMENT> Lexeme " << log_comment << "found" << endl;
                lineCount += line_in_comment_state;
                line_in_comment_state = 0;
                BEGIN INITIAL;
}

<MULTILINE_COMMENT_STATE><<EOF>>    {
                errorCount++;
                logStream << "Error at line# " << lineCount << ": UNFINISHED_COMMENT " << log_comment << endl;
                lineCount += line_in_comment_state;
                line_in_comment_state = 0;
                BEGIN INITIAL;
}

<MULTILINE_COMMENT_STATE>.          {
                log_comment += yytext[0];
}

@|.             {
                logStream << "Error at line# " << lineCount << ": UNRECOGNIZED_CHAR " << yytext << endl;
}

%%

int main(int argc, char** argv) {
	if(argc!=2){
		printf("Please provide input file name and try again\n");
		return 0;
	}
	
	init();
	
	FILE *fin=fopen(argv[1],"r");
	if(fin==NULL){
		printf("Cannot open specified file\n");
		return 0;
	}

	tokenStream.open("tokenOut.txt");
	logStream.open("logOut.txt");
	//logout= fopen("logout.txt","w");
	//tokenout= fopen("token.txt","w");

	yyin= fin;
	yylex();
	st.printAllScope(logStream);
	logStream << "Total lines: " << lineCount << endl;
	logStream << "Total errors: " << errorCount << endl;
	fclose(yyin);

	tokenStream.close();
	logStream.close();
	//fclose(tokenout);
	//fclose(logout);
	return 0;
}
