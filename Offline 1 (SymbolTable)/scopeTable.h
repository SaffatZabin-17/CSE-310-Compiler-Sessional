#include<bits/stdc++.h>
#include "symbolInfo.h"
using namespace std;

class scopeTable{
    symbolInfo** symbolArray;
    int bucket_size;
    scopeTable* parentTable;
    int number;
    int chainCount = 0;  //Keeps track of the chain traversal length to find the string
public:

    scopeTable(){
        this->symbolArray = nullptr;
        this->bucket_size = 0;
        this->parentTable = nullptr;
        this->number = 0;
    }

    scopeTable(int bucket_size){
        this->symbolArray = new symbolInfo*[bucket_size];
        for(int i=0;i<bucket_size;i++){
            symbolArray[i] = nullptr;
        }
        this->bucket_size = bucket_size;
        this->parentTable = nullptr;
        this->number = 0;
    }

    scopeTable(int bucket_size, int number){
        this->symbolArray = new symbolInfo*[bucket_size];
        for(int i=0;i<bucket_size;i++){
            symbolArray[i] = nullptr;
        }
        this->bucket_size = bucket_size;
        this->parentTable = nullptr;
        this->number = number;
    }

    scopeTable(int bucket_size, int number, scopeTable* parentTable){
        this->bucket_size = bucket_size;
        this->symbolArray = new symbolInfo*[bucket_size];
        for(int i=0;i<bucket_size;i++){
            symbolArray[i] = nullptr;
        }
        this->parentTable = parentTable;
        this->number = number;
    }

    ~scopeTable(){
        delete[] symbolArray;
        delete parentTable;
        this->bucket_size = 0;
        this->number = 0;
    }

    unsigned int SDBMHash(string str) {
        unsigned int hash = 0;
        unsigned int i = 0;
        unsigned int len = str.length();

        for (i = 0; i < len; i++)
        {
            hash = (str[i]) + (hash << 6) + (hash << 16) - hash;
            hash = hash % bucket_size;
        }
        return hash;
    }

    void setNumber(int number){
        this->number = number;
    }

    int getNumber(){
        return this->number;
    }

    int hash(string key){
        return SDBMHash(key)%bucket_size;
    }

    int getCount(){
        return this->chainCount;
    }

    void setCount(int count){
        this->chainCount = count;
    }

    void setParentScope(scopeTable* parentTable){
        this->parentTable = parentTable;
    }

    scopeTable* getParentScope(){
        return this->parentTable;
    }

    int getBucketSize(){
        return this->bucket_size;
    }

    bool insert(symbolInfo& symbol){
        if(lookUp(symbol.getSymbolName())){
            return false;
        }
        string name = symbol.getSymbolName();
        int hash = SDBMHash(name) % bucket_size;
        symbolInfo* symbolPointer = &symbol;
        chainCount = 0;
        if(symbolArray[hash] == nullptr){
            symbolArray[hash] = symbolPointer;
            return true;
        }
        symbolInfo* curr = symbolArray[hash] -> getNextSymbol();
        symbolInfo* prev = symbolArray[hash];
        while(prev != nullptr){
            chainCount++;
            if(curr == nullptr){
                break;
            }
            curr = curr->getNextSymbol();
            prev = prev->getNextSymbol();
        }
        prev->setNextSymbol(symbolPointer);
        return true;
    }

    symbolInfo* lookUp(string symbol){
        chainCount = 0;
        int position = hash(symbol);
        symbolInfo* SymbolPointer = symbolArray[position];
        while(SymbolPointer != nullptr){
            chainCount++;
            if(SymbolPointer->getSymbolName() == symbol){
                return SymbolPointer;
            }
            else{
                SymbolPointer = SymbolPointer-> getNextSymbol();
            }
        }
        return nullptr;
    }

    bool Delete(string symbol){
        chainCount = 0;
        int position = hash(symbol);
        symbolInfo* currSymbolPointer = symbolArray[position];
        if(currSymbolPointer == nullptr){
            return false;
        }
        symbolInfo* nextSymbolPointer = symbolArray[position]->getNextSymbol();
        symbolInfo* prevSymbolPointer = symbolArray[position];
        if(currSymbolPointer-> getSymbolName() == symbol){
            if(nextSymbolPointer == nullptr){
                symbolArray[position] = nullptr;
                return true;
            }
            else{
                delete currSymbolPointer;
                symbolArray[position] = nextSymbolPointer;
                return true;
            }
        }
        else{
            currSymbolPointer = currSymbolPointer -> getNextSymbol();
            nextSymbolPointer = nextSymbolPointer -> getNextSymbol();
            chainCount++;
            while(currSymbolPointer -> getSymbolName() != symbol){
                prevSymbolPointer = prevSymbolPointer -> getNextSymbol();
                currSymbolPointer = currSymbolPointer -> getNextSymbol();
                nextSymbolPointer = nextSymbolPointer -> getNextSymbol();
                chainCount++;
            }
            if(currSymbolPointer -> getSymbolName() == symbol){
                if(nextSymbolPointer != nullptr){
                    delete currSymbolPointer;
                    prevSymbolPointer ->setNextSymbol(nextSymbolPointer);
                    return true;
                }
                else{
                    delete currSymbolPointer;
                    prevSymbolPointer ->setNextSymbol(nullptr);
                    return true;
                }
            }
            return false;
        }
    }

    void print(ofstream& obj){
        for(int i=0;i<bucket_size;i++){
            symbolInfo* symbolPointer = symbolArray[i];
            if(symbolPointer != nullptr){
                obj << "\t" << i+1 << "--> ";
                while(symbolPointer!= nullptr){
                    symbolPointer->print(obj);
                    symbolPointer = symbolPointer->getNextSymbol();
                }
                obj << endl;
            }
        }
    }
};
