#include <bits/stdc++.h>
using namespace std;

class symbolInfo{
    string symbolType, symbolName;
    symbolInfo* nextSymbol;
    string symbolClass;         //SymbolClass indicates which class of rule $x came from
    string symbolID;            //If the symbol is a variable/Array/function
    string return_type;
    vector<symbolInfo*> childList;
    int depth;
    int startLine, endLine;
    bool global;
    int stackOffset;
    int symbolSize;
    string label;

public:

    bool isLeaf;
    vector<symbolInfo*> input;                  //Stores the entire input line
    vector<symbolInfo*> parameter_list;         //Stores the parameters
    symbolInfo(){
        this->symbolName = "";
        this->symbolType = "";
        this->nextSymbol = nullptr;
    }

    symbolInfo(string symbolType){
        this->symbolType = symbolType;
    }

    symbolInfo(string symbolName, string symbolType){
        this->symbolType = symbolType;
        this->symbolName = symbolName;
        this->nextSymbol = nullptr;
    }

    symbolInfo(string symbolType, string symbolName, symbolInfo* nextSymbol){
        this->symbolType = symbolType;
        this->symbolName = symbolName;
        this->nextSymbol = nextSymbol;
    }

    ~symbolInfo(){
        this->input.clear();
    }

    void setSymbolClass(string symbolClass){
        this->symbolClass = symbolClass;
    }

    string getSymbolClass(){
        return this->symbolClass;
    }

    void setSymbolType(string symbolType){
        this->symbolType = symbolType;
    }

    void setSymbolName(string symbolName){
        this->symbolName = symbolName;
    }

    void setReturnType(string return_type){
        this->return_type = return_type;
    }

    string getReturnType(){
        return this->return_type;
    }

    string getSymbolType(){
        return this->symbolType;
    }

    string getSymbolName(){
        return this->symbolName;
    }

    symbolInfo* getNextSymbol(){
        return this->nextSymbol;
    }

    void setNextSymbol(symbolInfo* nextSymbol){
        this->nextSymbol = nextSymbol;
    }

    vector<symbolInfo*> getInput(){
        return this->input;
    }

    void setSymbolID(string symbolID){
        this->symbolID = symbolID;
    }

    string getSymbolID(){
        return this->symbolID;
    }

    void print(ofstream& obj){
        obj << "<" << this->symbolName << ", " << this->symbolType << "> ";
    }

    void insertChild(symbolInfo* symbol){
        childList.push_back(symbol);
    }

    void setDepth(int depth){
        this->depth = depth;
    }

    void setStartLine(int startLine){
        this->startLine = startLine;
    }

    int getStartLine(){
        return this->startLine;
    }

    void setEndLine(int endLine){
        this->endLine = endLine;
    }

    int getEndLine(){
        return this->endLine;
    }

    vector<symbolInfo*> getChildList(){
        return this->childList;
    }

    void setGlobal(bool global){
        this->global = global;
    }

    bool getGlobal(){
        return this->global;
    }

    void setStackOffset(int stackOffset){
        this->stackOffset = stackOffset;
    }

    int getStackOffset(){
        return this->stackOffset;
    }

    void setSymbolSize(int symbolSize){
        this->symbolSize = symbolSize;
    }

    int getSymbolSize(){
        return this->symbolSize;
    }

    void setLabel(string label){
        this->label = label;
    }

    string getLabel(){
        return this->label;
    }
};
