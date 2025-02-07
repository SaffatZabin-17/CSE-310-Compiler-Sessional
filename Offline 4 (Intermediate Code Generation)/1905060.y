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

%}

%union {
	symbolInfo *symbol;
}

%token<symbol> 	IF ELSE FOR WHILE INT FLOAT VOID RETURN ADDOP MULOP INCOP RELOP ASSIGNOP LOGICOP NOT LPAREN RPAREN
%token<symbol> 	LCURL RCURL LSQUARE RSQUARE COMMA SEMICOLON CONST_INT CONST_FLOAT ID PRINTLN

%type<symbol> compound_statement type_specifier parameter_list declaration_list var_declaration unit func_declaration 
%type<symbol> start program	statement statements variable expression factor arguments rel_expression term func_definition id
%type<symbol> argument_list expression_statement unary_expression simple_expression logic_expression embedded_stmt embedded_else

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

		codeSegOut << "\t" << $2->getSymbolName() << " ENDP" << endl;

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
		codeSegOut << "\t\t" << $2->getSymbolName() << " ENDP" << endl;
	}
 	;				

embedded : {
		final_type_specified = func_type;
		final_func_name = func_name;
		globalScope = false;
		parameter_list.clear();
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

		temp->setSymbolID("parameter");

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

		temp->setSymbolID("parameter");

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

		temp->setSymbolID("parameter");

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
				int offset = 4 + (parameter_list.size()-i)*2;
				parameter_list[i]->setStackOffset(offset);
				parameter_list[i]->setSymbolClass("parameter_list");
				st.insert(*parameter_list[i]);
			}
		}
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

					st.insert(*x);
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
		x->setSymbolSize(atoi($5->getSymbolName().c_str()));
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

		symbolInfo* x = new symbolInfo("CONST_INT");
		x->setSymbolClass("declaration_list : ID LSQUARE CONST_INT RSQUARE");
		x->setSymbolName($1->getSymbolName());
		x->setSymbolSize(atoi($3->getSymbolName().c_str()));
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

		string KMSLabel = generateNewLabel();
		codeSegOut << "\t" << KMSLabel << ": " << endl;

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

		string KMSLabel = generateNewLabel();
		codeSegOut << "\t" << KMSLabel << ": " << endl;

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

		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Expression evaluated" << endl;
		//codeSegOut << endl;

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

	| IF LPAREN expression embedded_exp RPAREN embedded_void statement embedded_stmt %prec LOWER_THAN_ELSE	{
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

		codeSegOut << "\t" << ifTerminateStack.top() << ": " << endl;
		ifTerminateStack.pop();

		logOut << "statement : IF LPAREN expression RPAREN statement" << endl;
	}

	| IF LPAREN expression embedded_exp RPAREN embedded_void statement embedded_stmt ELSE embedded_else statement	{
		$1->setSymbolClass("IF : if");
		$1->isLeaf = true;
		
		$2->setSymbolClass("LPAREN : (");
		$2->isLeaf = true;
		
		$5->setSymbolClass("RPAREN : )");
		$5->isLeaf = true;
		
		$9->setSymbolClass("ELSE : else");
		$9->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("if (" + $3->getSymbolName() + " )" + $7->getSymbolName() + " else " + $11->getSymbolName() , "NON_TERMINAL");
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
		$$->insertChild($9);
		$$->insertChild($11);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($11->getEndLine());

		codeSegOut << "\t\t;Ending else block" << endl;
		codeSegOut << "\t" << elseTerminateStack.top() << ": " << endl;
		elseTerminateStack.pop();
		logOut << "statement : IF LPAREN expression RPAREN statement ELSE statement" << endl;
	}

	| FOR LPAREN expression_statement {

		codeSegOut << "\t\t;Line " << lineCount << ": For loop start" << endl;
		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << " : Evaluated for loop start expression" << endl;
		string label = generateNewLabel();
		forLoopConditionLabel = label;
		codeSegOut << "\t\t; For loop condition label" << endl;
		codeSegOut << "\t" << label << ": " << endl;

	} {	} expression_statement {

		string label = generateNewLabel();
		string forLoopLabel = generateNewLabel();
		string forLoopUpdate = generateNewLabel();
		
		forLoopEndLabel = label;
		forLoopUpdateLabel = forLoopUpdate;
		forLoopStatementLabel = forLoopLabel;

		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Load " << $6->getSymbolName() << endl;
		codeSegOut << "\t\tCMP AX, 0" << endl;
		codeSegOut << "\t\tJE " << label << "\t\t; For loop condition unsatisfied. Break for loop" << endl;
		codeSegOut << "\t\tJMP " << forLoopLabel << endl;
		codeSegOut << "\t\t; For loop update start" << endl;
		codeSegOut << "\t" << forLoopUpdate << ": " << endl;

	} {	} expression {

		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Evaluated for loop update expression" << endl;
		codeSegOut << "\t\tJMP " << forLoopConditionLabel << endl;
		codeSegOut << "\t\t; Line " << lineCount << ": Evaluate for loop statement " << endl;
		codeSegOut << "\t" << forLoopStatementLabel << ": " << endl;

	} {	} RPAREN statement	{
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

		codeSegOut << "\t\tJMP " << forLoopUpdateLabel << endl;
		codeSegOut << "\t\t; For loop ending" << endl;
		codeSegOut << "\t" << forLoopEndLabel << ": " << endl;

		logOut << "statement : FOR LPAREN expression_statement expression_statement expression_statement RPAREN statement" << endl;
	}

	| WHILE LPAREN {

		string label = generateNewLabel();
		whileLoopStartLabel = label;
		codeSegOut << "\t" << label << ": \t\t; Line " << lineCount << ": While Loop start" << endl;

	} expression embedded_exp RPAREN {

		string label = generateNewLabel();
		whileLoopEndLabel = label;

		codeSegOut << "\t\t; While loop block start" << endl;
		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Load" << $4->getSymbolName() << endl;
		codeSegOut << "\t\tCMP AX, 0" << endl;
		codeSegOut << "\t\tJE " << label << endl;

	} statement	{
		$1->setSymbolClass("WHILE : while");
		$1->isLeaf = true;
		
		$2->setSymbolClass("LPAREN : (");
		$2->isLeaf = true;
		
		$6->setSymbolClass("RPAREN : )");
		$6->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("while (" + $4->getSymbolName() + " ) " + $8->getSymbolName(), "NON_TERMINAL");
		temp->setSymbolID("WHILE loop");
		temp->setSymbolClass("statement : WHILE LPAREN expression RPAREN statement");
		
		$$ = temp;
		$$->setSymbolClass("statement : WHILE LPAREN expression RPAREN statement");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($4);
		$$->insertChild($6);
		$$->insertChild($8);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($8->getEndLine());

		exp_type = $4->getSymbolType();

		codeSegOut << "\t\tJMP " << whileLoopStartLabel << endl;
		codeSegOut << "\t" << whileLoopEndLabel << ":\t\t; Line " << lineCount << ": While loop ending label" << endl;

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

		codeSegOut << "\t\t; Line " << lineCount << ": return " << $2->getSymbolName() << endl;
		codeSegOut << "\t\tPOP AX" << endl;
		codeSegOut << "\t\tMOV [BP+4], AX" << endl;
		codeSegOut << "\t\tADD SP, " << varCount*2 << "\t\t;Line " << lineCount << ": Removing all the local variables from the stack" << endl;
		codeSegOut << "\t\tPOP BP" << endl;
		if(final_func_name != "main"){
			codeSegOut << "\t\tRET" << endl;
		}
	}

	| PRINTLN LPAREN ID RPAREN SEMICOLON	{
		$1->setSymbolClass("PRINTLN : println");
		$1->isLeaf = true;
		
		$2->setSymbolClass("LPAREN : (");
		$2->isLeaf = true;
		
		$4->setSymbolClass("RPAREN : )");
		$4->isLeaf = true;
		
		$5->setSymbolClass("SEMICOLON : ;");
		$5->isLeaf = true;
		
		symbolInfo* temp = new symbolInfo("println (" + $3->getSymbolName() + " )", "NON_TERMINAL");
		temp->setSymbolID("PRINTLN");
		temp->setSymbolClass("statement : PRINTLN LPAREN ID RPAREN SEMICOLON");
		
		$$ = temp;
		$$->setSymbolClass("statement : PRINTLN LPAREN ID RPAREN SEMICOLON");
		$$->isLeaf = false;
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($3);
		$$->insertChild($4);
		$$->insertChild($5);
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($5->getEndLine());

		temp = st.lookUpRoot($3->getSymbolName());
		if(temp != nullptr){
			codeSegOut << "\t\tMOV BX, " << $3->getSymbolName() << "\t\t; Line " << lineCount << ": println(" << $3->getSymbolName() << ")" << endl;
		}
		else{
			temp = st.lookUp($3->getSymbolName());
			if(temp->getSymbolID() == "parameter"){
				codeSegOut << "\t\tMOV BX, [BP+" << temp->getStackOffset() << "]\t\t; Line " << lineCount << ": println(" << $3->getSymbolName() << ")" << endl;
			}
			else{
				codeSegOut << "\t\tMOV BX, [BP-" << temp->getStackOffset() << "]\t\t; Line " << lineCount << ": println(" << $3->getSymbolName() << ")" << endl;
			}
			//codeSegOut << "\t\tMOV BX, [BP-" << temp->getStackOffset() << "]\t\t; Line " << lineCount << ": println(" << $3->getSymbolName() << ")" << endl;
		}
		codeSegOut << "\t\tCALL PRINT_NUM_FROM_BX" << endl;

		logOut << "statement : PRINTLN LPAREN ID RPAREN SEMICOLON" << endl;
	}
	;


embedded_exp :	{
		final_exp_type = exp_type;
	}
	;

embedded_void :	{
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
	;

embedded_stmt : {
		codeSegOut << "\t\t; Line " << lineCount << ": End of if block" << endl;
		//codeSegOut << "\t\t" << endLabel << ":" << endl;
	};	

embedded_else : {
		string elseLabel = generateNewLabel();
		codeSegOut << "\t\t" << "JMP " << elseLabel << endl;
		codeSegOut << "\t" << ifTerminateLabel << ": " << endl;
		codeSegOut << "\t\t;ELSE BLOCK START" << endl;
		elseTerminateStack.push(elseLabel);
		elseTerminateLabel = elseLabel;	
	};
  
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

			codeSegOut << "\t\t; " << $1->getSymbolName() << "[" << $3->getSymbolName() << "]" << endl;
			codeSegOut << "\t\tPOP AX\t\t; Pop index " << $3->getSymbolName() << " from stack" << endl;
			codeSegOut << "\t\tSHL AX, 1\t\t; Multiply index by 2" << endl;
			codeSegOut << "\t\tLEA BX, [BP-" << temp->getStackOffset() << "]\t\t; Load address of " << $1->getSymbolName() << " into BX" << endl;
			codeSegOut << "\t\tSUB BX, AX\t\t; Subtract index from address" << endl;
			codeSegOut << "\t\tPUSH BX\t\t; Push address of " << $1->getSymbolName() << "[" << $3->getSymbolName() << "] onto stack" << endl;

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
		
		codeSegOut << "\t\tPOP AX\t\t; Load " << $3->getSymbolName() << endl;
		if($1->getSymbolID() == "ARRAY" || $1->getSymbolID() == "array"){
			codeSegOut << "\t\tPOP BX\t\t; Load address of " << $1->getSymbolName() << endl;
		}
		temp = st.lookUp($1->getSymbolName());
		//symbolInfo* tempLocal = st.lookUp($1->getSymbolName());
		if(temp != nullptr){
			//temp = st.lookUp($1->getSymbolName());
			if(temp->getGlobal()){
				codeSegOut << "\t\tMOV " << $1->getSymbolName() << ", AX\t\t; Line " << lineCount << ": " << $1->getSymbolName() << " " << $2->getSymbolName() << " " << $3->getSymbolName() << endl;
			}

			else{
				if(temp->getSymbolID() == "ARRAY" || temp->getSymbolID() == "array"){
					codeSegOut << "\t\tMOV [BX], AX\t\t; Line " << lineCount << ": " << $1->getSymbolName() << " " << $2->getSymbolName() << " " << $3->getSymbolName() << endl;
				}
				else{
					if(temp->getSymbolID() == "parameter"){
						codeSegOut << "\t\tMOV [BP+" << temp->getStackOffset() << "], AX\t\t; Line " << lineCount << ": " << $1->getSymbolName() << " " << $2->getSymbolName() << " " << $3->getSymbolName() << endl;
					}
					else{
						codeSegOut << "\t\tMOV [BP-" << temp->getStackOffset() << "], AX\t\t; Line " << lineCount << ": " << $1->getSymbolName() << " " << $2->getSymbolName() << " " << $3->getSymbolName() << endl;
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
		codeSegOut << "\t\tPUSH AX\t\t; Save declared value of " << $1->getSymbolName() << endl; 
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
	
	| rel_expression LOGICOP {

		codeSegOut << "\t\t; Line " << lineCount << ": Short Circuit jump code evaluation" << endl;
		codeSegOut << "\t\tPOP AX\t\t; Load " << $1->getSymbolName() << endl;
		if($2->getSymbolName() == "&&"){
			boolValue = 1;
		}
		else{
			boolValue = 0;
		}
		codeSegOut << "\t\tCMP AX , " << boolValue <<  "\t\t; Check if " << $1->getSymbolName() << " is true" << endl;
		string tempLabel = generateNewLabel();
		codeSegOut << "\t\tJNE " << tempLabel << "\t\t; Jump to " << tempLabel << " if " << $1->getSymbolName() << " is false" << endl;
		shortCircuitLabel = tempLabel;
		$1->setLabel(tempLabel);

	} rel_expression 	{
		$2->setSymbolClass("LOGICOP : " + $2->getSymbolName());
		$2->isLeaf = true;

		if($1->getSymbolType() == "VOID" || $4->getSymbolType() == "VOID"){
			errorCount++;
			errorOut << "Line# " << lineCount << ": Void cannot be used in expression" << endl;
		}
		$$ = new symbolInfo($1->getSymbolName() + " " + $2->getSymbolName() + " " + $4->getSymbolName(), "INT");
		$$->setSymbolID($1->getSymbolID());
		$$->setSymbolClass("logic_expression : rel_expression LOGICOP rel_expression");
		$$->isLeaf = false;
		$$->setStartLine($1->getStartLine());
		$$->setEndLine($4->getEndLine());
		$$->insertChild($1);
		$$->insertChild($2);
		$$->insertChild($4);

		codeSegOut << "\t\tPOP AX\t\t; Load " << $4->getSymbolName() << endl;

		if($2->getSymbolName() == "&&"){
			boolValue = 1;
		}
		else{
			boolValue = 0;
		}

		codeSegOut << "\t\tCMP AX , " << boolValue <<  "\t\t; Check if " << $4->getSymbolName() << " is true" << endl;
		codeSegOut << "\t\tJNE " << $1->getLabel() << "\t\t; Jump to " << $1->getLabel() << " if " << $4->getSymbolName() << " is false" << endl;
		codeSegOut << "\t\tPUSH " << boolValue << "\t\t; Push " << boolValue << " to stack" << endl;
		string logicEnd = generateNewLabel();
		codeSegOut << "\t\tJMP " << logicEnd << "\t\t; Jump to " << logicEnd << endl;
		codeSegOut << "\t" << $1->getLabel() << ":" << endl;
		if($2->getSymbolName() == "&&"){
			boolValue = 0;
		}
		else{
			boolValue = 1;
		}
		codeSegOut << "\t\tPUSH " << boolValue << "\t\t; Push " << boolValue << " to stack" << endl;
		codeSegOut << "\t" << logicEnd << ":" << endl;

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

		string trueLabel = generateNewLabel();
		string falseLabel = generateNewLabel();
		string KMSLabel = generateNewLabel();
		
		string jumpCode;
		if($2->getSymbolName() == "<"){
			jumpCode = "JL";
		}
		else if($2->getSymbolName() == "<="){
			jumpCode = "JLE";
		}
		else if($2->getSymbolName() == ">"){
			jumpCode = "JG";
		}
		else if($2->getSymbolName() == ">="){
			jumpCode = "JGE";
		}
		else if($2->getSymbolName() == "=="){
			jumpCode = "JE";
		}
		else if($2->getSymbolName() == "!="){
			jumpCode = "JNE";
		}

		codeSegOut << "\t\tPOP BX\t\t; Line " << lineCount << ": Load " << $3->getSymbolName() << endl;
		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Load " << $1->getSymbolName() << endl;
		codeSegOut << "\t\tCMP AX, BX" << endl;
		codeSegOut << "\t\t" << jumpCode << " " << trueLabel << "\t\t; Line " << lineCount << ": " << $1->getSymbolName() << $2->getSymbolName() << $3->getSymbolName() << endl;
		codeSegOut << "\t\tJMP " << KMSLabel << endl;
		codeSegOut << "\t" << trueLabel << ":\n\t\tPUSH 1" << endl;
		codeSegOut << "\t\tJMP " << falseLabel << endl;
		codeSegOut << "\t" << KMSLabel << ": " << endl;
		codeSegOut << "\t\tPUSH 0" << endl;
		codeSegOut << "\t" << falseLabel << ": " << endl;
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

		codeSegOut << "\t\tPOP BX\t\t; Line " << lineCount << ": Load " << $1->getSymbolName() << endl;
		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Load " << $3->getSymbolName() << endl;

		if($2->getSymbolName() == "+"){
			codeSegOut << "\t\tADD AX, BX\t\t; Line " << lineCount << ": Add " << $1->getSymbolName() << " and " << $3->getSymbolName() << endl;
		}
		else{
			codeSegOut << "\t\tSUB AX, BX\t\t; Line " << lineCount << ": Subtract " << $1->getSymbolName() << " and " << $3->getSymbolName() << endl;
		}
		codeSegOut << "\t\tPUSH AX\t\t;Save result" << endl;

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

		codeSegOut << "\t\tPOP BX\t\t; Line " << lineCount << ": Pop " << $1->getSymbolName() << endl;
		codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Pop " << $3->getSymbolName() << endl;
		codeSegOut << "\t\tXOR DX, DX\t\t; Line " << lineCount << ": Clear DX" << endl;
		if($2->getSymbolName() == "*"){
			codeSegOut << "\t\tIMUL BX\t\t; Line " << lineCount << ": Multiply " << $1->getSymbolName() << " and " << $3->getSymbolName() << endl;
		}
		else{
			codeSegOut << "\t\tIDIV BX\t\t; Line " << lineCount << ": Divide " << $1->getSymbolName() << " and " << $3->getSymbolName() << endl;
		}
		if($2->getSymbolName() == "%"){
			codeSegOut << "\t\tPUSH DX" << endl;
		}
		else{
			codeSegOut << "\t\tPUSH AX" << endl;
		}
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

		codeSegOut << "\t\t; Line " << lineCount << ": " << $1->getSymbolName() << $2->getSymbolName() << endl; 

		if($1->getSymbolName() == "-"){
			codeSegOut << "\t\tPOP AX\t\t; Line " << lineCount << ": Pop " << $2->getSymbolName() << " from stack" << endl;
			codeSegOut << "\t\tNEG AX\t\t; Negating the value of " << $2->getSymbolName() << endl;
			codeSegOut << "\t\tPUSH AX\t\t; Saving the negative value" << endl;	
		}

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

		string label1 = generateNewLabel();
		string label2 = generateNewLabel();
		
		codeSegOut << "\t\tLine " << lineCount << ": ! " << $2->getSymbolName() << endl;
		codeSegOut << "\t\tPOP AX\t\t; Load the value of " << $2->getSymbolName() << endl;
		codeSegOut << "\t\tCMP AX, 0" << endl;
		codeSegOut << "\t\tJE " << label1 << endl;
		codeSegOut << "\t\tPUSH 0" << endl;
		codeSegOut << "\t\tJMP " << label2 << endl;
		codeSegOut << "\t" << label1 << ":" << endl;
		codeSegOut << "\t\t" << "PUSH 1" << endl;
		codeSegOut << "\t\t" << label2 << ": " << endl;

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
		
		if($1->getSymbolID() == "array" && globalScope == false){
			codeSegOut << "\t\tPOP BX" << endl;
		}

		symbolInfo *temp = st.lookUp($1->getSymbolName());
		symbolInfo *tempGlobal = st.lookUpRoot($1->getSymbolName());
		if(tempGlobal != nullptr && temp->getSymbolID() != "parameter"){
			codeSegOut << "\t\tPUSH " << $1->getSymbolName() << "\t\t; Save " << $1->getSymbolName() << endl;
		}
		else{
			if(temp != nullptr){
				if(temp->getSymbolID() == "parameter"){
					codeSegOut << "\t\tPUSH [BP + " << to_string(temp->getStackOffset()) << "]\t\t;Line " << lineCount << ": Save " << $1->getSymbolName() << endl;
				}
				else{
					if(temp->getSymbolID() == "array" || temp->getSymbolID() == "ARRAY"){
						codeSegOut << "\t\tPUSH [BX]\t\t; Line " << lineCount << ": Save " << $1->getSymbolName() << endl;
					}
					else{
						codeSegOut << "\t\tPUSH [BP-" << temp->getStackOffset() << "]\t\t; Line " << lineCount << ": Save " << $1->getSymbolName() << endl;
					}
				}
			}
		}

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

			codeSegOut << "\t\t; Line " << lineCount << ": " << "Call " << $1->getSymbolName() << endl;
			codeSegOut << "\t\tPUSH 0" << endl;
			codeSegOut << "\t\tCALL " << $1->getSymbolName() << "\t\t; Line " << lineCount << ": Call " << $1->getSymbolName() << endl;
			codeSegOut << "\t\tPOP AX\t\t; Pop the return value" << endl;
			codeSegOut << "\t\tADD SP, " << to_string(temp->parameter_list.size()*2) << "\t\t; Pop the arguments" << endl;
			if($2->getSymbolType() != "VOID"){
				codeSegOut << "\t\tPUSH AX\t\t; Push the return value" << endl;
			}
			else{
				codeSegOut << "\t\tPUSH 0\t\t; Push dummy value for other function returns" << endl; 
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

		codeSegOut << "\t\tPUSH " << $1->getSymbolName() << "\t\t;Line " << lineCount << ": Save " << $1->getSymbolName() << endl;
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

		string op;

		if($2->getSymbolName() == "++"){
			op = "INC";
		}

		else{
			op = "DEC";
		}

		symbolInfo* temp = st.lookUp($1->getSymbolName());

		if(temp->getSymbolID() == "parameter"){
			codeSegOut << "\t\tPUSH [BP+" << temp->getStackOffset() << "]\t\t;Line " << lineCount << ": Save " << $1->getSymbolName() << endl;
			codeSegOut << "\t\t" << op << " W. [BP+" << temp->getStackOffset() << "]\t\t;Line " << lineCount << ": " << op << " " << $1->getSymbolName() << endl;
		}

		else{
			codeSegOut << "\t\tPUSH [BP-" << temp->getStackOffset() << "]\t\t;Line " << lineCount << ": Save " << $1->getSymbolName() << endl;
			codeSegOut << "\t\t" << op << " W. [BP-" << temp->getStackOffset() << "]\t\t;Line " << lineCount << ": " << op << " " << $1->getSymbolName() << endl;
		}
		
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