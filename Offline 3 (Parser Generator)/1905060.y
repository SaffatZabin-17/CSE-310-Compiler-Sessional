%code requires {
	#include<bits/stdc++.h>
}


%{
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

%}

%union {
	symbolInfo *symbol;
}

%token<symbol> 	IF ELSE FOR WHILE INT FLOAT VOID RETURN ADDOP MULOP INCOP RELOP ASSIGNOP LOGICOP NOT LPAREN RPAREN
%token<symbol> 	LCURL RCURL LSQUARE RSQUARE COMMA SEMICOLON CONST_INT CONST_FLOAT ID

%type<symbol> compound_statement type_specifier parameter_list declaration_list var_declaration unit func_declaration 
%type<symbol> start program	statement statements variable expression factor arguments rel_expression term func_definition id
%type<symbol> argument_list expression_statement unary_expression simple_expression logic_expression
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE 

%define parse.error verbose

%%

start : program
	{
		logOut << "start : program" << endl;
		$$ = new symbolInfo($1->getSymbolType());
		$$->insertChild($1);
		$$->isLeaf = false;
		$$->setSymbolClass("start : program");
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		startSymbol = $$;
	}
	;

program : program unit {
		$$ = new symbolInfo($1->getSymbolType());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->isLeaf = false;
		$$->setSymbolClass("program : program unit");
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($2->getEndLine());
		logOut << "program : program unit" << endl;
	}
	| unit	{
		$$ = new symbolInfo($1->getSymbolType());
		$$->insertChild($1);
		$$->isLeaf = false;
		$$->setSymbolClass("program : unit");
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		logOut << "program : unit" << endl;
	}
	;
	
unit : var_declaration	{
		logOut << "unit: var_declaration" << endl;
		symbolInfo *temp = new symbolInfo($1->getSymbolName(), "unit");
		$$ = temp;
		$$->insertChild($1);
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->setSymbolClass("unit : var_declaration");
	}
	| func_declaration	{
		logOut << "unit : func_declaration" << endl;
		symbolInfo *temp = new symbolInfo($1->getSymbolName(), "unit");
		$$ = temp;
		$$->insertChild($1);
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->setSymbolClass("unit : func_declaration");
	}
    | func_definition	{
		logOut << "unit : func_definition" << endl;
		symbolInfo *temp = new symbolInfo($1->getSymbolName(), "unit");
		$$ = temp;
		$$->insertChild($1);
		$$->isLeaf = false;
		$$->setSymbolClass("unit : func_definition");
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
	}
    ;
func_declaration : type_specifier id embedded LPAREN parameter_list RPAREN embedded_out_dec SEMICOLON	{
		$2->setSymbolClass("ID : " + $2->getSymbolName());
		$2->isLeaf = true;

		$4->setSymbolClass("LPAREN : (");
		$4->isLeaf = true;

		$6->setSymbolClass("RPAREN : )");
		$6->isLeaf = true;

		$8->setSymbolClass("SEMICOLON : ;");
		$8->isLeaf = true;

		$$ = new symbolInfo($2->getSymbolName(), "func_declaration");
		$$->setSymbolClass("func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($8->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($4);
		$$->insertChild($5);
		$$->insertChild($6);
		$$->insertChild($8);

		if(st.lookUp($2->getSymbolName())){
			//Error stating function of the same name already exists
		}

		else{

			if($1->getSymbolType() == "int"){
				type_specified = "FUNCTION, INT";
			}

			else if($1->getSymbolType() == "float"){
				type_specified = "FUNCTION, FLOAT";
			}

			else if($1->getSymbolType() == "void"){
				type_specified = "FUNCTION, VOID";
			}

			symbolInfo* temp = new symbolInfo($2->getSymbolName(), type_specified);
			temp->parameter_list = parameter_list;
			temp->setSymbolClass("func_declaration");
			st.insert(*temp);
			logOut << "func_declaration : type_specifier ID LPAREN parameter_list RPAREN SEMICOLON" << endl;
		}

		parameter_list.clear();
	}
	| type_specifier id embedded LPAREN RPAREN embedded_out_dec SEMICOLON	{
		$2->setSymbolClass("ID : " + $2->getSymbolName());
		$2->isLeaf = true;

		$4->setSymbolClass("LPAREN : (");
		$4->isLeaf = true;

		$5->setSymbolClass("RPAREN : )");
		$5->isLeaf = true;

		$7->setSymbolClass("SEMICOLON : ;");
		$7->isLeaf = true;

		$$ = new symbolInfo($2->getSymbolName(), "func_declaration");
		$$->setSymbolClass("func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($7->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($4);
		$$->insertChild($5);
		$$->insertChild($7);

		if(st.lookUp($2->getSymbolName())){
			//Error stating function of the same name already exists
		}
		
		else{

			if($1->getSymbolType() == "int"){
				type_specified = "FUNCTION, INT";
			}

			else if($1->getSymbolType() == "float"){
				type_specified = "FUNCTION, FLOAT";
			}

			else if($1->getSymbolType() == "void"){
				type_specified = "FUNCTION, VOID";
			}

			symbolInfo* temp = new symbolInfo($2->getSymbolName(), type_specified);
			temp->setSymbolClass("func_declaration");
			temp->parameter_list = parameter_list;
			st.insert(*temp);

			logOut << "func_declaration : type_specifier ID LPAREN RPAREN SEMICOLON" << endl;		
		}

		parameter_list.clear();
	}
	;

func_definition : type_specifier id embedded LPAREN parameter_list RPAREN embedded_out_def compound_statement	{
		$2->setSymbolClass("ID : " + $2->getSymbolName());
		$2->isLeaf = true;

		$4->setSymbolClass("LPAREN : (");
		$4->isLeaf = true;

		$6->setSymbolClass("RPAREN : )");
		$6->isLeaf = true;

		if($1->getSymbolType() == "int"){
			func_type = "FUNCTION, INT";
		}
		else if($1->getSymbolType() == "float"){
			func_type = "FUNCTION, FLOAT";
		}
		else{
			func_type = "FUNCTION, VOID";
		}

		$$ = new symbolInfo($2->getSymbolName(), func_type);
		$$->setSymbolClass("func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($8->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($4);
		$$->insertChild($5);
		$$->insertChild($6);
		$$->insertChild($8);

		logOut << "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement" << endl;

	}

	| type_specifier id embedded LPAREN RPAREN embedded_out_def compound_statement	{
		$2->setSymbolClass("ID : " + $2->getSymbolName());
		$2->isLeaf = true;

		$4->setSymbolClass("LPAREN : (");
		$4->isLeaf = true;

		$5->setSymbolClass("RPAREN : )");
		$5->isLeaf = true;

		if($1->getSymbolType() == "int"){
			func_type = "FUNCTION, INT";
		}

		else if($1->getSymbolType() == "float"){
			func_type = "FUNCTION, FLOAT";
		}

		else{
			func_type = "FUNCTION, VOID";
		}
		
		$$ = new symbolInfo($2->getSymbolName(), type_specified);
		$$->setSymbolClass("func_definition : type_specifier ID LPAREN RPAREN compound_statement");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($7->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($4);
		$$->insertChild($5);
		$$->insertChild($7);
		
		logOut << "func_definition : type_specifier ID LPAREN parameter_list RPAREN compound_statement" << endl;

	}
 	;				

embedded : {
		
		final_type_specified = func_type;
		final_func_name = func_name;
	}
	;

embedded_out_def : {
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
	}
	;

embedded_out_dec : {
		symbolInfo* temp = st.lookUp(final_func_name);
		if(temp != nullptr){
			
		}
	}
	;


parameter_list  : parameter_list COMMA type_specifier id	{
		$2->setSymbolClass("COMMA : ,");
		$2->isLeaf = true;

		$4->setSymbolClass("ID : " + $4->getSymbolName());
		$4->isLeaf = true;

		string type;
		if($3->getSymbolType() == "int"){
			type = "INT";
		}
		else if($3->getSymbolType() == "float"){
			type = "FLOAT";
		}
		else{
			//Semantic error
		}
		symbolInfo* temp = new symbolInfo($4->getSymbolName(), type);

		temp->setSymbolClass("parameter_list : parameter_list COMMA type_specifier ID");

		temp->setSymbolID($4->getSymbolID());

		$$ = temp;
		$$->setSymbolClass("parameter_list : parameter_list COMMA type_specifier ID");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($4->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);
		$$->insertChild($4);

		parameter_list.push_back(temp);

		logOut << "parameter_list : parameter_list COMMA type_specifier ID" << endl;
	}


	| parameter_list COMMA type_specifier	{
		symbolInfo* x = new symbolInfo(" ", param_type);

		$2->setSymbolClass("COMMA : ,");
		$2->isLeaf = true;

		string type;
		if($3->getSymbolType() == "int"){
			type = "INT";
		}

		else if($3->getSymbolType() == "float"){
			type = "FLOAT";
		}

		else{
			//Error
		}

		symbolInfo* temp = new symbolInfo($1->getSymbolName(), $3->getSymbolType());

		temp->setSymbolClass("parameter_list : parameter_list COMMA type_specifier");

		temp->setSymbolID($1->getSymbolID());

		$$ = temp;
		$$->setSymbolClass("parameter_list : parameter_list COMMA type_specifier");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($3->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);

		if(parameter_list.size() == 0){
			parameter_list.push_back(x);
		}
		parameter_list.push_back(temp);

		logOut << "parameter_list : parameter_list COMMA type_specifier" << endl;
	}


 	| type_specifier id	{
		$2->setSymbolClass("ID : " + $2->getSymbolName());
		$2->isLeaf = true;

		string type;
		if($1->getSymbolType() == "int"){
			type = "INT";
		}
		else if($1->getSymbolType() == "float"){
			type = "FLOAT";
		}
		else{
			//Semantic error
		}
		symbolInfo* temp = new symbolInfo($2->getSymbolName(), type);
		
		temp->setSymbolClass("parameter_list : type_specifier ID");

		temp->setSymbolID($2->getSymbolID());

		$$ = temp;
		$$->setSymbolClass("parameter_list : type_specifier ID");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($2->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);

		parameter_list.push_back(temp);
		logOut << "parameter_list : type_specifier ID" << endl;
	}


	| type_specifier	{
		symbolInfo* temp = new symbolInfo($1->getSymbolName(), $1->getSymbolType());
		temp->setSymbolClass("parameter_list : type_specifier");

		$$ = temp;
		$$->setSymbolClass("parameter_list : type_specifier");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->insertChild($1);

		param_type = $1->getSymbolType();
		//parameter_list.push_back(temp);

		logOut << "parameter_list : type_specifier" << endl;
	}
 	;

 		
compound_statement : LCURL embedded_in statements RCURL	{
		$1->setSymbolClass("LCURL : {");
		$1->isLeaf = true;

		$4->setSymbolClass("RCURL : }");
		$4->isLeaf = true;

		symbolInfo* temp = new symbolInfo("compound_statement");
		temp->setSymbolClass("compound_statement : LCURL statements RCURL");

		$$ = temp;
		$$->setSymbolClass("compound_statement : LCURL statements RCURL");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($4->getEndLine());
		$$->insertChild($1);
		$$->insertChild($3);
		$$->insertChild($4);

		logOut << "compound_statement : LCURL statements RCURL" << endl;
		st.printAllScope(logOut);
		st.exitScope();
	}
 	| LCURL embedded_in RCURL	{
		$1->setSymbolClass("LCURL : {");
		$1->isLeaf = true;

		$3->setSymbolClass("RCURL : }");
		$3->isLeaf = true;

		symbolInfo* temp = new symbolInfo("compound_statement");
		temp->setSymbolClass("compound_statement : LCURL statements RCURL");
		
		$$ = temp;
		$$->setSymbolClass("compound_statement : LCURL RCURL");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($3->getEndLine());
		$$->insertChild($1);
		$$->insertChild($3);

		logOut << "compound_statement : LCURL RCURL" << endl;
		st.printAllScope(logOut);
		st.exitScope();
	}
 	;


embedded_in : {
		st.enterScope(st.getScopeNumber() + 1);

		for(int i=0;i<parameter_list.size();i++){
			symbolInfo* temp = st.lookUpCurrent(parameter_list[i]->getSymbolName());
			if(temp != nullptr){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Redefinition of parameter '" << temp->getSymbolName() << "'" << endl;
			}
			else{
				st.insert(*parameter_list[i]);
			}
		}

		parameter_list.clear();
	}
	;


var_declaration : type_specifier declaration_list SEMICOLON	{
		$3->setSymbolClass("SEMICOLON : ;");
		$3->isLeaf = true;

		$$ = new symbolInfo("var_declaration");
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($3->getEndLine());
		$$->setSymbolClass("var_declaration : type_specifier declaration_list SEMICOLON");
		if($1->getSymbolType() == "void"){
			for(int i=0;i<declaration_list.size();i++){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Variable or field '" << declaration_list[i]->getSymbolName() << "' declared void" << endl;
			}
		}
		
		else{
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
					if($1->getSymbolType() == "int"){
					
						if(x->getSymbolType() == "ARRAY"){
							type_specified = "ARRAY, INT";
							symbolID = "array";
						}

						else if(x->getSymbolType() == "ID"){
							type_specified = "INT";
							symbolID = "variable";
						}
					}

					else if($1->getSymbolType() == "float"){
					
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

					st.insert(*temp);
				}
				
			}
		}

		logOut << "var_declaration : type_specifier declaration_list SEMICOLON" << endl;
		declaration_list.clear();
	}
 	;



type_specifier	: INT	{
		$1->setSymbolClass("INT : int");
		$1->isLeaf = true;
		
		variable = "int";
		symbolInfo *temp = new symbolInfo("int");
		
		$$ = temp;
		$$ -> setSymbolName(" ");
		$$->setSymbolClass("type_specifier : INT");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());

		type_specified = "INT";
		func_type = "INT";
		logOut << "type_specifier : INT" << endl;
	}


 	| FLOAT	{
		$1->setSymbolClass("FLOAT : float");
		$1->isLeaf = true;

		variable = "float";
		symbolInfo *temp = new symbolInfo("float");
		
		$$ = temp;
		$$ -> setSymbolName(" ");
		$$->setSymbolClass("type_specifier : FLOAT");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());

		type_specified = "FLOAT";
		func_type = "FLOAT";
		logOut << "type_specifier : FLOAT" << endl;
	}


 	| VOID	{
		$1->setSymbolClass("VOID : void");
		$1->isLeaf = true;

		variable = "void";
		symbolInfo *temp = new symbolInfo("void");
		
		$$ = temp;
		$$ -> setSymbolName(" ");
		$$->setSymbolClass("type_specifier : VOID");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());

		type_specified = "VOID";
		func_type = "VOID";
		logOut << "type_specifier : VOID" << endl;
	}
 	;
 		

id : ID {
		$$ = new symbolInfo($1->getSymbolName(), $1->getSymbolType());
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		func_name = $1->getSymbolName();
		var_name = $1->getSymbolName();
	}
	;

declaration_list : declaration_list COMMA id	{
		$2->setSymbolClass("COMMA : ,");
		$2->isLeaf = true;

		$3->setSymbolClass("ID : " + $3->getSymbolName());
		$3->isLeaf = true;

		symbolInfo* temp = new symbolInfo("declaration_list");
		temp->setSymbolClass("declaration_list : declaration_list COMMA ID");
		$$ = temp;
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($3->getEndLine());
		
		symbolInfo* x = new symbolInfo($3->getSymbolName(), $3->getSymbolType());
		x->setSymbolClass("declaration_list : declaration_list COMMA ID");

		$$->input.push_back($2);
		$$->input.push_back($3);
		
		declaration_list.push_back(x);

		logOut << "declaration_list : declaration_list COMMA ID" << endl;
	}


 	| declaration_list COMMA id LSQUARE CONST_INT RSQUARE	{
		$2->setSymbolClass("COMMA : ,");
		$2->isLeaf = true;

		$3->setSymbolClass("ID : " + $3->getSymbolName());
		$3->isLeaf = true;
		
		$4->setSymbolClass("LSQUARE : [");
		$4->isLeaf = true;
		
		$5->setSymbolClass("CONST_INT : " + $5->getSymbolName());
		$5->isLeaf = true;
		
		$6->setSymbolClass("RSQUARE : ]");
		$6->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("declaration_list");
		temp->setSymbolClass("declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
		$$ = temp;
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);
		$$->insertChild($4);
		$$->insertChild($5);
		$$->insertChild($6);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($6->getEndLine());

		symbolInfo* x = new symbolInfo($3->getSymbolName(), "ARRAY");
			
		x->setSymbolClass("declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
		declaration_list.push_back(x);

		symbolInfo* y = new symbolInfo("CONST_INT");
		y->setSymbolName($5->getSymbolName());
		y->setSymbolClass("declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE");
		
		logOut << "declaration_list : declaration_list COMMA ID LSQUARE CONST_INT RSQUARE" << endl;
	}


 	| id	{
		$1->setSymbolClass("ID : " + $1->getSymbolName());
		$1->isLeaf = true;
		
		symbolInfo *temp = new symbolInfo("ID");
		temp->setSymbolName($1->getSymbolName());
		temp->setSymbolClass("declaration_list : ID");
		$$ = temp;
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		
		symbolInfo *x = new symbolInfo($1->getSymbolName(), "ID");
		declaration_list.push_back(temp);
		
		logOut << "declaration_list : ID" << endl;
	}


 	| id LSQUARE CONST_INT RSQUARE	{
		$1->setSymbolClass("ID : " + $1->getSymbolName());
		$1->isLeaf = true;

		$2->setSymbolClass("LSQUARE : [");
		$2->isLeaf = true;
		
		$3->setSymbolClass("CONST_INT : " + $3->getSymbolName());
		$3->isLeaf = true;
		
		$4->setSymbolClass("RSQUARE : ]");
		$4->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo($1->getSymbolName(), "ARRAY");
		temp->setSymbolClass("declaration_list : ID LSQUARE CONST_INT RSQUARE");
		$$ = temp;
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);
		$$->insertChild($4);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($4->getEndLine());

		declaration_list.push_back(temp);
		symbolInfo* x = new symbolInfo("CONST_INT");
		x->setSymbolClass("declaration_list : ID LSQUARE CONST_INT RSQUARE");
		x->setSymbolName($3->getSymbolName());
		
		logOut << "declaration_list : ID LSQUARE CONST_INT RSQUARE" << endl;
	}
 	;



statements : statement	{
		symbolInfo* temp = new symbolInfo($1->getSymbolName(), $1->getSymbolType());
		temp->setSymbolID("statement");
		temp->setSymbolClass("statements : statement");
		
		$$ = temp;
		$$->setSymbolClass("statements : statement");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());

		logOut << "statements : statement" << endl;
	}

	| statements statement	{
		symbolInfo* temp = new symbolInfo($1->getSymbolName() + $2->getSymbolName(), "NON_TERMINAL");
		temp->setSymbolID("statement");
		temp->setSymbolClass("statements : statements statement");
		
		$$ = temp;
		$$->setSymbolClass("statements : statements statement");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->insertChild($2);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($2->getEndLine());

		logOut << "statements : statements statement" << endl;
	}
	;
	   
statement : var_declaration	{
		symbolInfo* temp = new symbolInfo($1->getSymbolName(), "NON_TERMINAL");
		temp->setSymbolID("var_declaration");
		temp->setSymbolClass("statement : var_declaration");
		
		$$ = temp;
		$$->setSymbolClass("statement : var_declaration");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());

		logOut << "statement : var_declaration" << endl;
	}

	| expression_statement	{
		symbolInfo* temp = new symbolInfo($1->getSymbolName(), "NON_TERMINAL");
		temp->setSymbolID("expression_statement");
		temp->setSymbolClass("statement : expression_statement");
		
		$$ = temp;
		$$->setSymbolClass("statement : expression_statement");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());

		logOut << "statement : expression_statement" << endl;
	}

	| compound_statement	{
		symbolInfo* temp = new symbolInfo($1->getSymbolName(), "NON_TERMINAL");
		temp->setSymbolID($1->getSymbolID());
		temp->setSymbolClass("statement : compound_statement");
		
		$$ = temp;
		$$->setSymbolClass("statement : compound_statement");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());

		logOut << "statement : compound_statement" << endl;
	}

	| IF LPAREN expression embedded_exp RPAREN embedded_void statement %prec LOWER_THAN_ELSE	{
		$1->setSymbolClass("IF : if");
		$1->isLeaf = true;

		$2->setSymbolClass("LPAREN : (");
		$2->isLeaf = true;
		
		$5->setSymbolClass("RPAREN : )");
		$5->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("if (" + $3->getSymbolName() + " )" + $7->getSymbolName() + " ", "NON_TERMINAL");
		temp->setSymbolID("IF grammar rule 1");
		temp->setSymbolClass("statement : IF grammar rule 1");
		
		$$ = temp;
		$$->setSymbolClass("statement : IF LPAREN expression RPAREN statement");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);
		$$->insertChild($5);
		$$->insertChild($7);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($7->getEndLine());

		logOut << "statement : IF LPAREN expression RPAREN statement" << endl;
	}

	| IF LPAREN expression embedded_exp RPAREN embedded_void statement ELSE statement	{
		$1->setSymbolClass("IF : if");
		$1->isLeaf = true;
		
		$2->setSymbolClass("LPAREN : (");
		$2->isLeaf = true;
		
		$5->setSymbolClass("RPAREN : )");
		$5->isLeaf = true;
		
		$8->setSymbolClass("ELSE : else");
		$8->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("if (" + $3->getSymbolName() + " )" + $7->getSymbolName() + " else " + $9->getSymbolName() , "NON_TERMINAL");
		temp->setSymbolID("IF grammar rule 2");
		temp->setSymbolClass("statement : IF grammar rule 2");
		
		$$ = temp;
		$$->setSymbolClass("statement : IF LPAREN expression RPAREN statement ELSE statement");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);
		$$->insertChild($5);
		$$->insertChild($7);
		$$->insertChild($8);
		$$->insertChild($9);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($9->getEndLine());

		logOut << "statement : IF LPAREN expression RPAREN statement ELSE statement" << endl;
	}

	| FOR LPAREN expression_statement embedded_exp embedded_void expression_statement embedded_exp embedded_void expression embedded_exp embedded_void RPAREN statement	{
		$1->setSymbolClass("FOR : for");
		$1->isLeaf = true;
		
		$2->setSymbolClass("LPAREN : (");
		$2->isLeaf = true;
		
		$12->setSymbolClass("RPAREN : )");
		$12->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("for (" + $3->getSymbolName() + " " + $6->getSymbolName() + " " + $9->getSymbolName() + " " + $13->getSymbolName(), "NON_TERMINAL");
		temp->setSymbolID("FOR loop");
		temp->setSymbolClass("statement : FOR LPAREN expression_statement expression_statement expression RPAREN statement");
		
		$$ = temp;
		$$->setSymbolClass("statement : FOR LPAREN expression_statement expression_statement expression_statement RPAREN statement");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);
		$$->insertChild($6);
		$$->insertChild($9);
		$$->insertChild($12);
		$$->insertChild($13);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($13->getEndLine());

		logOut << "statement : FOR LPAREN expression_statement expression_statement expression_statement RPAREN statement" << endl;
	}

	| WHILE LPAREN expression embedded_exp RPAREN embedded_void statement	{
		$1->setSymbolClass("WHILE : while");
		$1->isLeaf = true;
		
		$2->setSymbolClass("LPAREN : (");
		$2->isLeaf = true;
		
		$5->setSymbolClass("RPAREN : )");
		$5->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("while (" + $3->getSymbolName() + " ) " + $7->getSymbolName(), "NON_TERMINAL");
		temp->setSymbolID("WHILE loop");
		temp->setSymbolClass("statement : WHILE LPAREN expression RPAREN statement");
		
		$$ = temp;
		$$->setSymbolClass("statement : WHILE LPAREN expression RPAREN statement");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);
		$$->insertChild($5);
		$$->insertChild($7);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($7->getEndLine());

		exp_type = $3->getSymbolType();
		logOut << "statement : WHILE LPAREN expression RPAREN statement" << endl;
	}

	| RETURN expression SEMICOLON	{
		$1->setSymbolClass("RETURN : return");
		$1->isLeaf = true;
		
		$3->setSymbolClass("SEMICOLON : ;");
		$3->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("RETURN");
		temp->setSymbolName("return");
		temp->setSymbolClass("statement : RETURN expression SEMICOLON");
		
		$$ = temp;
		$$->setSymbolClass("statement : RETURN expression SEMICOLON");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($3->getEndLine());

		logOut << "statement : RETURN expression SEMICOLON" << endl;
		type_specified = $2->getSymbolType();
	}
	;


embedded_exp :	{
		final_exp_type = exp_type;
	}
	;

embedded_void :	{
		//cout << final_exp_type << endl;
		if(exp_type == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}
	}
	;
	  
expression_statement : SEMICOLON	{
		$1->setSymbolClass("SEMICOLON : ;");
		$1->isLeaf = true;

		$$ = new symbolInfo("SEMICOLON");
		$$->setSymbolClass("expression_statement : SEMICOLON");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());

		logOut << "expression_statement : SEMICOLON" << endl;
	}		
	| expression SEMICOLON {
		$2->setSymbolClass("SEMICOLON : ;");
		$2->isLeaf = true;
		
		$$ = new symbolInfo("expression_statement");
		$$->setSymbolClass("expression_statement : expression SEMICOLON");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->insertChild($2);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($2->getEndLine());

		exp_type = $1->getSymbolType();

		logOut << "expression_statement : expression SEMICOLON" << endl;
	}
	;
	  
variable : id {
		$1->setSymbolClass("ID : " + $1->getSymbolName());
		$1->isLeaf = true;
		
		$$ = new symbolInfo("variable");
		$$->setSymbolName($1->getSymbolName());
		$$->setSymbolClass("variable : ID");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());

		symbolInfo* temp = st.lookUp($1->getSymbolName());

		if(temp == nullptr){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Undeclared variable '" << $1->getSymbolName() << "'" << endl; 
		}

		else{
			if(temp->getSymbolType() == "void"){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Variable or field '" << temp->getSymbolName() << "' declared void" << endl;
			}

			else if(temp->getSymbolType() == "INT"){
				$$->setSymbolType("INT");
				$$->setSymbolID("variable");
			}

			else if(temp->getSymbolType() == "FUNCTION, INT"){
				$$->setSymbolType("FUNCTION, INT");
				$$->setSymbolID("FUNCTION, INT");
			}

			else if(temp->getSymbolType() == "FUNCTION, FLOAT"){
				$$->setSymbolType("FUNCTION, FLOAT");
				$$->setSymbolID("FUNCTION, FLOAT");
			}

			else if(temp->getSymbolType() == "FUNCTION, VOID"){
				$$->setSymbolType("FUNCTION, VOID");
				$$->setSymbolID("FUNCTION, VOID");
			}

			else if(temp->getSymbolType() == "FLOAT"){
				$$->setSymbolType("FLOAT");
				$$->setSymbolID("variable");
			}

			else if(temp->getSymbolType() == "ARRAY, INT" || temp->getSymbolType() == "ARRAY, FLOAT" || temp->getSymbolType() == "ARRAY, VOID"){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Expression must have arithmetic type" << endl;
				$$->setSymbolID("ARRAY");
				$$->setSymbolType(temp->getSymbolType());
			}
		}

		logOut << "variable : ID" << endl;
	}

	| id LSQUARE expression RSQUARE	{
		$1->setSymbolClass("ID : " + $1->getSymbolName());
		$1->isLeaf = true;

		$2->setSymbolClass("LSQUARE : [");
		$2->isLeaf = true;
		
		$4->setSymbolClass("RSQUARE : ]");
		$4->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo($1->getSymbolName(), "ID");
		temp->setSymbolClass("variable : ID LSQUARE expression RSQUARE");
		
		$$ = temp;
		$$->setSymbolClass("variable : ID LSQUARE expression RSQUARE");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($4->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);
		$$->insertChild($4);

		temp = st.lookUp($1->getSymbolName());
		if(temp == nullptr){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Undeclared variable '" << $1->getSymbolName() << "'" << endl;
		}

		else{
			if(temp->getSymbolType() == "void"){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Variable or field '" << temp->getSymbolName() << "' declared void" << endl;
			}

			else if($3->getSymbolType() != "INT"){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Array subscript is not an integer" << endl;
			}

			else if(temp->getSymbolType() == "INT" || temp->getSymbolType() == "FLOAT"){
				errorCount++;
				errorOut << "Line# " << lineCount << ": '" << $1->getSymbolName() << "' is not an array" << endl;
			}

			else if(temp->getSymbolType() == "ARRAY, INT"){
				$$->setSymbolType("INT");
				$$->setSymbolID("array");
			}
			else{
				$$->setSymbolType("FLOAT");
				$$->setSymbolID("array");
			}
		}
		logOut << "variable : ID LSQUARE expression RSQUARE" << endl;
	}
	;

expression : logic_expression{
		exp_type = $1->getSymbolType();
		$$ = new symbolInfo($1->getSymbolName(), $1->getSymbolType());
		$$->setSymbolID($1->getSymbolID());
		$$->setSymbolClass("expression : logic_expression");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->insertChild($1);

		logOut << "expression : logic_expression" << endl;
	}

	| variable ASSIGNOP logic_expression {
		$2->setSymbolClass("ASSIGNOP : =");
		$2->isLeaf = true;
		
		$$ = new symbolInfo($1->getSymbolName(), $1->getSymbolType());
		$$->setSymbolClass("expression : variable ASSIGNOP logic_expression");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($3->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);

		//cout << "Line# " << lineCount << ": " << $1->getSymbolType() << " " << $3->getSymbolType() << endl;
		if($1->getSymbolType() == "VOID" || $3->getSymbolType() == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}
		symbolInfo* temp = st.lookUp($1->getSymbolName());
		if(temp != nullptr){
			string type1 = temp->getSymbolType();
			string type2 = $3->getSymbolType();

			if((type1 == "INT" && type2 == "FLOAT") || (type1 == "ARRAY, INT" && type2 == "FLOAT")){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Warning: possible loss of data in assignment of FLOAT to INT" << endl;
			}

			logOut << "expression : variable ASSIGNOP logic_expression" << endl;
		}
	}
	;
	 
logic_expression : rel_expression 	{
		$$ = new symbolInfo($1->getSymbolName(), $1->getSymbolType());
		$$->setSymbolID($1->getSymbolID());
		$$->setSymbolClass("logic_expression : rel_expression");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->insertChild($1);

		logOut << "logic_expression : rel_expression" << endl;
	}
	
	| rel_expression LOGICOP rel_expression 	{
		$2->setSymbolClass("LOGICOP : " + $2->getSymbolName());
		$2->isLeaf = true;

		if($1->getSymbolType() == "VOID" || $3->getSymbolType() == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}
		$$ = new symbolInfo($1->getSymbolName() + " " + $2->getSymbolName() + " " + $3->getSymbolName(), "INT");
		$$->setSymbolID($1->getSymbolID());
		$$->setSymbolClass("logic_expression : rel_expression LOGICOP rel_expression");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($3->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);

		logOut << "logic_expression : rel_expression LOGICOP rel_expression" << endl;
	}
	;
			
rel_expression	: simple_expression {
		$$ = new symbolInfo($1->getSymbolName(), $1->getSymbolType());
		$$->setSymbolID($1->getSymbolID());
		$$->setSymbolClass("rel_expression : simple_expression");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->insertChild($1);

		logOut << "rel_expression : simple_expression" << endl;
	}

	| simple_expression RELOP simple_expression	{
		$2->setSymbolClass("RELOP : " + $2->getSymbolName());
		$2->isLeaf = true;
		
		if($1->getSymbolType() == "VOID" || $3->getSymbolType() == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}
		$$ = new symbolInfo($1->getSymbolName() + " " + $2->getSymbolName() + " " + $3->getSymbolName(), "INT");
		$$->setSymbolID($1->getSymbolID());
		$$->setSymbolClass("rel_expression : simple_expression RELOP simple_expression");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($3->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);

		logOut << "rel_expression : simple_expression RELOP simple_expression" << endl;
	}
	;
				
simple_expression : term {
		$$ = new symbolInfo($1->getSymbolName(), $1->getSymbolType());
		$$->setSymbolClass("simple_expression : term");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->insertChild($1);
		$$->setSymbolID($1->getSymbolID());
		logOut << "simple_expression : term" << endl;
	}

	| simple_expression ADDOP term {
		$2->setSymbolClass("ADDOP : " + $2->getSymbolName());
		$2->isLeaf = true;
		
		if($1->getSymbolType() == "VOID" || $3->getSymbolType() == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}
		string name = $1->getSymbolName() + " " + $2->getSymbolName() + " " + $3->getSymbolName();
		
		$$ = new symbolInfo(name, "NON_TERMINAL");
		$$->setSymbolClass("simple_expression: simple_expression ADDOP term");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($3->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);

		if($1->getSymbolType() == "void"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Variable or field '" << $1->getSymbolName() << "' declared void" << endl;
		}

		if($3->getSymbolType() == "void"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Variable or field '" << $1->getSymbolName() << "' declared void" << endl;
		}

		if($1->getSymbolType() == "FLOAT" || $3->getSymbolType() == "FLOAT"){
			$$->setSymbolType("FLOAT");
			$$->setSymbolID("variable");
		}

		else if(($1->getSymbolType() == "FLOAT" && $3->getSymbolType() == "INT") || ($1->getSymbolID() == "INT" && $3->getSymbolID() == "FLOAT")){
			$$->setSymbolType("FLOAT");
			$$->setSymbolID("variable");
		}

		else{
			$$->setSymbolType($1->getSymbolType());
			$$->setSymbolID($1->getSymbolID());
		}

		logOut << "simple_expression : simple_expression ADDOP term" << endl;
	}
	;
					
term :	unary_expression	{
		$$ = new symbolInfo($1->getSymbolName(), $1->getSymbolType());
		$$->setSymbolID($1->getSymbolID());
		$$->setSymbolClass("term : unary_expression");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->insertChild($1);

		logOut << "term : unary_expression" << endl;
	}

    |  term MULOP unary_expression	{
		$2->setSymbolClass("MULOP : " + $2->getSymbolName());
		$2->isLeaf = true;
		
		$$ = new symbolInfo($1->getSymbolName() + " " + $2->getSymbolName() + " " + $3->getSymbolName(), "NON_TERMINAL");
		$$->setSymbolClass("term : term MULOP unary_expression");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($3->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);

		if($1->getSymbolType() == "VOID" || $3->getSymbolType() == "VOID"){

		}
		if($2->getSymbolName() == "%"){
			if($1->getSymbolType() != "INT" || $3->getSymbolType() != "INT"){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Operands of modulus must be integers " << endl;
			}
			else if($1->getSymbolType() == "INT" && $3->getSymbolType() == "INT"){
				$$->setSymbolType("INT");
				$$->setSymbolID("variable");
			}

			else if($1->getSymbolType() == "ARRAY, INT" || $1->getSymbolType() == "ARRAY, FLOAT" || $3->getSymbolType() == "ARRAY, INT" || $3->getSymbolType() == "ARRAY, FLOAT"){

			}

			else{
				$$->setSymbolType("VOID");
				$$->setSymbolID("variable");
			}
			
		}
		else if($2->getSymbolName() == "/"){
			if($3->getSymbolName() == "0"){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Warning: division by zero i=0f=1Const=0 " << endl;
			}
			if($1->getSymbolType() == "INT" && $3->getSymbolType() == "INT"){
				$$->setSymbolType("INT");
				$$->setSymbolID("variable");
			}

			else if($1->getSymbolType() == "ARRAY, INT" || $1->getSymbolType() == "ARRAY, FLOAT" || $3->getSymbolType() == "ARRAY, INT" || $3->getSymbolType() == "ARRAY, FLOAT"){
				
			}

			else if(($1->getSymbolType() == "INT" && $3->getSymbolType() == "FLOAT") || ($1->getSymbolType() == "FLOAT" && $3->getSymbolType() == "INT")){
				$$->setSymbolType("FLOAT");
				$$->setSymbolID("variable");
			}

			else{
				$$->setSymbolType("VOID");
				$$->setSymbolID("variable");
			}
		}
		
		else{
			if($1->getSymbolType() == "INT" && $3->getSymbolType() == "INT"){
				$$->setSymbolType("INT");
				$$->setSymbolID("variable");
			}
			else if(($1->getSymbolType() == "INT" && $3->getSymbolType() == "FLOAT") || ($1->getSymbolType() == "FLOAT" && $3->getSymbolType() == "INT")){
				$$->setSymbolType("FLOAT");
				$$->setSymbolID("variable");
			}

			else if($1->getSymbolType() == "FLOAT" && $3->getSymbolType() == "FLOAT"){
				$$->setSymbolType("FLOAT");
				$$->setSymbolID("variable");
			}

			else if($1->getSymbolType() == "ARRAY, INT" || $1->getSymbolType() == "ARRAY, FLOAT" || $3->getSymbolType() == "ARRAY, INT" || $3->getSymbolType() == "ARRAY, FLOAT"){
				
			}
			
			else{
				$$->setSymbolType("VOID");
				$$->setSymbolID("variable");
			}
		}
		logOut << "term : term MULOP unary_expression" << endl;
	}

    ;

unary_expression : ADDOP unary_expression  {
		$1->setSymbolClass("ADDOP : " + $1->getSymbolName());
		$1->isLeaf = true;
		
		if($2->getSymbolType() == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}
		$$ = new symbolInfo($2->getSymbolName(), $2->getSymbolType());
		$$->setSymbolID($2->getSymbolID());
		$$->setSymbolClass("unary_expression : ADDOP unary_expression");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($2->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);

		logOut << "unary_expression : ADDOP unary_expression" << endl;
	}

	| NOT unary_expression {
		$1->setSymbolClass("NOT : !");
		$1->isLeaf = true;
		
		if($2->getSymbolType() == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}

		$$ = new symbolInfo($2->getSymbolName(), $2->getSymbolType());
		$$->setSymbolID($2->getSymbolID());
		$$->setSymbolClass("unary_expression : NOT unary_expression");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($2->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);

		logOut << "unary_expression : NOT unary_expression" << endl;
	}

	| factor {
		$$ = new symbolInfo($1->getSymbolName(), $1->getSymbolType());
		$$->setSymbolID($1->getSymbolID());
		$$->setSymbolClass("unary_expression : factor");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->insertChild($1);

		logOut << "unary_expression : factor" << endl;
	}

	;
	
factor	: variable {
		$$ = new symbolInfo ($1->getSymbolName(), $1->getSymbolType());
		$$->setSymbolID($1->getSymbolID());
		$$->setSymbolClass("factor : variable");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->insertChild($1);

		logOut << "factor : variable" << endl;
	}
	| id LPAREN argument_list RPAREN	{
		$1->setSymbolClass("ID : " + $1->getSymbolName());
		$1->isLeaf = true;
		
		$2->setSymbolClass("LPAREN : (");
		$2->isLeaf = true;
		
		$4->setSymbolClass("RPAREN : )");
		$4->isLeaf = true;
		
		$$ = new symbolInfo($1->getSymbolName(), $1->getSymbolType());
		$$->setSymbolClass("factor : ID LPAREN argument_list RPAREN");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($4->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);
		$$->insertChild($4);

		symbolInfo* temp = st.lookUp($1->getSymbolName());

		if(temp == nullptr){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Undeclared function '" << $1->getSymbolName() << "' " << endl;
		}
		else{
			if(temp->getSymbolType() == "FUNCTION, VOID"){
				$$->setSymbolType("VOID");
			}
		
			else if(temp->getSymbolType() == "FUNCTION, INT"){
				$$->setSymbolType("INT");
			}

			else if(temp->getSymbolType() == "FUNCTION, FLOAT"){
				$$->setSymbolType("FLOAT");
			}

			else{
				errorCount++;
				errorOut << "Line# " << lineCount << ": '" << $1->getSymbolName() << "' is not a function" << endl;
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
					errorOut << "Line# " << lineCount << ": Type mismatch for argument " << i+1 << " of '" << $1->getSymbolName() << "'" << endl;
				}
			}

			if(arg_list.size() < temp->parameter_list.size()){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Too few arguments to function '" << $1->getSymbolName() << "'" << endl;
			}
			else if(arg_list.size() > temp->parameter_list.size()){
				errorCount++;
				errorOut << "Line# " << lineCount << ": Too many arguments to function '" << $1->getSymbolName() << "'" << endl;
			}
			arg_list.clear();
		}
	}

	| LPAREN expression RPAREN	{
		$1->setSymbolClass("LPAREN : (");
		$1->isLeaf = true;
		
		$3->setSymbolClass("RPAREN : )");
		$3->isLeaf = true;
		
		$$ = new symbolInfo($2->getSymbolName(), $2->getSymbolType());
		$$->setSymbolClass("factor : LPAREN expression RPAREN");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($3->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);

		symbolInfo* temp = st.lookUp($2->getSymbolName());
		
		if(temp == nullptr){
			
		}

		else{
			if(temp->getSymbolType() == "FUNCTION, VOID"){
				
			}
		
			else if(temp->getSymbolType() == "FUNCTION, INT"){
				$$->setSymbolType("INT");
			}

			else if(temp->getSymbolType() == "FUNCTION, FLOAT"){
				$$->setSymbolType("FLOAT");
			}

			else if(temp->getSymbolType() == "INT"){
				$$->setSymbolType("INT");
			}

			else if(temp->getSymbolType() == "FLOAT"){
				$$->setSymbolType("FLOAT");
			}
		}
		logOut << "factor : LPAREN expression RPAREN" << endl;
	}

	| CONST_INT {
		$1->setSymbolClass("CONST_INT : " + $1->getSymbolName());
		$1->isLeaf = true;
		
		$$ = new symbolInfo($1->getSymbolName(), "INT");
		$$->setSymbolID("constant");
		$$->setSymbolClass("factor : CONST_INT");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->insertChild($1);

		logOut << "factor : CONST_INT" << endl;
	}

	| CONST_FLOAT	{
		$1->setSymbolClass("CONST_FLOAT : " + $1->getSymbolName());
		$1->isLeaf = true;
		
		$$ = new symbolInfo($1->getSymbolName(), "FLOAT");
		$$->setSymbolID("constant");
		$$->setSymbolClass("factor : CONST_FLOAT");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->insertChild($1);

		logOut << "factor : CONST_FLOAT" << endl;
	}

	| variable INCOP {
		$2->setSymbolClass("INCOP : " + $2->getSymbolName());
		$2->isLeaf = true;
		
		string type;
		if($1->getSymbolType() == "INT"){
			type = "INT";
		}

		else{
			type = "FLOAT";
		}

		$$ = new symbolInfo($1->getSymbolName(), type);
		$$->setSymbolID("variable");
		$$->setSymbolClass("factor : variable INCOP");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($2->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);

		logOut << "factor : variable INCOP" << endl;
	}
	;
	
argument_list : arguments	{
		$$ = new symbolInfo($1->getSymbolName(), $1->getSymbolType());
		$$->setSymbolClass("argument_list : arguments");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->insertChild($1);

		logOut << "argument_list : arguments" << endl;
	}

	|	{
		$$ = new symbolInfo("NULL", "NULL");
		$$->setSymbolClass("argument_list : ");
		$$->isLeaf = false;
		
		logOut << "argument_list : " << endl;
	}
	;
	
arguments : arguments COMMA logic_expression	{
		$2->setSymbolClass("COMMA : " + $2->getSymbolName());
		$2->isLeaf = true;
		
		$$ = new symbolInfo($1->getSymbolName(), $1->getSymbolType());
		$$->setSymbolClass("arguments : arguments COMMA logic_expression");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($3->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);

		if($3->getSymbolType() == "ARRAY, INT"){
			$3->setSymbolType("INT");
		}
		else if($3->getSymbolType() == "ARRAY, FLOAT"){
			$3->setSymbolType("FLOAT");
		}
		arg_list.push_back($3);
		logOut << "arguments : arguments COMMA logic_expression" << endl;
	}

	| logic_expression	{
		arg_list.clear();
		$$ = new symbolInfo($1->getSymbolName(), $1->getSymbolType());
		$$->setSymbolClass("arguments : logic_expression");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($1->getEndLine());
		$$->insertChild($1);
		
		if($1->getSymbolType() == "ARRAY, INT"){
			$1->setSymbolType("INT");
		}
		else if($1->getSymbolType() == "ARRAY, FLOAT"){
			$1->setSymbolType("FLOAT");
		}
		arg_list.push_back($1);
		logOut << "arguments : logic_expression" << endl;
	}
	; 

%%


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
	errorOut.open("error.txt");

	lineCount = 1;
	
	yyin = fp;
	yyparse();

	logOut << "Total Lines : " << lineCount << endl;
	logOut << "Total Errors : " << errorCount << endl;

	printTree(startSymbol, 0);
 
	return 0;
}