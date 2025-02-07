#include <bits/stdc++.h>
#include "scopeTable.h"

class symbolTable{
    scopeTable* currentScope;
    int bucket_size;
    int tableCount = 0;         //Keeps track of how many parent scopes we have visited
    int scopeNumber = 0;
public:

    symbolTable(int bucket_size){
        this-> currentScope = new scopeTable(bucket_size, 0);
        tableCount++;
    }

    void setCurrentScope(scopeTable* curr){
        this->currentScope = curr;
    }

    scopeTable* getCurrentScope(){
        return this->currentScope;
    }

    void setBucketSize(int bucket_size){
        this->bucket_size = bucket_size;
    }

    int getBucketSize() const{
        return this->bucket_size;
    }

    int getCurrentID(){
        return this->currentScope->getNumber();
    }

    int getTableCount() const{
        return this->tableCount;
    }

    int getScopeNumber() const{
        return this->scopeNumber;
    }

    void setTableCount(int tableCount){
        this->tableCount = tableCount;
    }

    void enterScope(int scopeID){
        scopeNumber++;
        scopeTable* scope;
        scope = new scopeTable(this->bucket_size, scopeID, currentScope);
        currentScope = scope;
    }

    int exitScope(){
        scopeTable* temp = currentScope->getParentScope();
        if(temp != nullptr){
            int x = currentScope->getNumber();
            currentScope = currentScope -> getParentScope();
            return x;
        }
        return -1;
    }

    bool insert(symbolInfo& symbol){
        return currentScope->insert(symbol);
    }

    bool remove(string symbol){
        return currentScope ->Delete(symbol);
    }

    symbolInfo* lookUp(string symbol){
        scopeTable* temp = currentScope;
        tableCount = 0;
        while(temp != nullptr){
            tableCount++;
            if(temp->lookUp(symbol)){
                return temp->lookUp(symbol);
            }
            else{
                temp = temp->getParentScope();
            }
        }
        return nullptr;
    }

    void printCurrentScope(ofstream& obj){
        currentScope -> print(obj);
    }

    void printAllScope(ofstream& obj){
        ofstream& fout = obj;
        scopeTable* curr = currentScope;
        if(curr-> getParentScope() == nullptr){
            fout << "\tScopeTable# " << curr->getNumber() + 1<< endl;
            curr->print(obj);
        }
        else{
            while(curr != nullptr){
                fout << "\tScopeTable# " << curr->getNumber() + 1<< endl;
                curr -> print(obj);
                curr = curr->getParentScope();
            }
        }
    }
};