#include<bits/stdc++.h>
using namespace std;

class symbolInfo{
    string symbolType, symbolName;
    symbolInfo* nextSymbol;
public:

    symbolInfo(){
        this->symbolName = "";
        this->symbolType = "";
        this->nextSymbol = nullptr;
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

    void setSymbolType(string symbolType){
        this->symbolType = symbolType;
    }

    void setSymbolName(string symbolName){
        this->symbolName = symbolName;
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

    void print(ofstream& obj){
        obj << "<" << this->symbolName << "," << this->symbolType << "> ";
    }
};
