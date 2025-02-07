#include<bits/stdc++.h>
#include "symbolTable.h"
using namespace std;

int main() {
    int count = 0, bucket_size;
    string line, name, type;
    ifstream fin;
    fin.open("sample_input.txt");
    getline(fin, line);
    bucket_size = stoi(line);
    symbolTable symbolTable(bucket_size);
    symbolTable.setBucketSize(bucket_size);
    ofstream fout;
    fout.open("output.txt");
    fout << "\tScopeTable# " << symbolTable.getCurrentID()+1 << " created" << endl;
    while (getline(fin, line)) {
        count++;
        string array[3];
        int i = 0;
        stringstream stringStream(line);
        while (stringStream.good() && i < 3) {
            stringStream >> array[i];
            i++;
        }
        if (array[0] == "I") {
            name = array[1];
            type = array[2];
            if(array[2].empty()){
                fout << "Cmd " << count << ": " << array[0] << " " << array[1] << " " << array[2] << endl;
                fout << "\tNumber of parameters mismatch for the command I" << endl;
            }
            else{
                if(symbolTable.getCurrentScope()->lookUp(array[1])){
                    fout << "Cmd " << count << ": " << array[0] << " " << array[1] << " " << array[2] << endl;
                    fout << "\t'" << array[1] << "' already exists in the current ScopeTable" << endl;
                }
                else{
                    symbolInfo* temp = new symbolInfo(name, type);
                    symbolTable.insert(*temp);
                    fout << "Cmd " << count << ": " << array[0] << " " << array[1] << " " << array[2] << endl;
                    fout << "\tInserted in ScopeTable# "
                         << symbolTable.getCurrentID()+1
                         << " at position "
                         << (symbolTable.getCurrentScope()->hash(array[1]))%symbolTable.getBucketSize() + 1
                         << ", "
                         << symbolTable.getCurrentScope()->getCount()+1
                         << endl;
                }
            }
        }
        if (array[0] == "L") {
            name = array[1];
            fout << "Cmd " << count << ": " << array[0] << " " << array[1] << " " << array[2] << endl;
            if(!array[2].empty()){
                fout << "\tNumber of parameters mismatch for the command L" << endl;
            }
            else{
                if(symbolTable.lookUp(array[1])){
                    scopeTable* temp = symbolTable.getCurrentScope();
                    int tempID = symbolTable.getCurrentID() + 1;
                    for(int k=0; k < symbolTable.getTableCount(); k++){
                        if(symbolTable.getCurrentScope()->getParentScope()!=nullptr){
                            temp = symbolTable.getCurrentScope()->getParentScope();
                            tempID--;
                        }
                        else{
                            tempID--;
                        }
                    }
                    fout << "\t'"
                         << name
                         << "' found in ScopeTable# "
                         << tempID + 1
                         << " at position "
                         << (symbolTable.getCurrentScope()->hash(array[1])) % symbolTable.getBucketSize() +1
                         << ", "
                         << temp->getCount()
                         << endl;
                }
                else{
                    fout << "\t'" << array[1] << "' not found in any of the ScopeTables" << endl;
                }
            }
        }
        if (array[0] == "D") {
            if(!array[2].empty() || array[1].empty()){
                fout << "Cmd " << count << ": " << array[0] << " " << array[1] << " " << array[2] << endl;
                fout << "\tNumber of parameters mismatch for the command D" << endl;
            }
            else{
                fout << "Cmd " << count << ": " << array[0] << " " << array[1] << " " << array[2] << endl;
                if(symbolTable.remove(array[1])){
                    fout<< "\tDeleted "
                        << "'"
                        << array[1]
                        << "'"
                        << " from ScopeTable# "
                        << symbolTable.getCurrentID()+1
                        << " at position "
                        << (symbolTable.getCurrentScope()->hash(array[1]))%symbolTable.getBucketSize()+1
                        << ", "
                        << symbolTable.getCurrentScope() -> getCount() + 1
                        << endl;
                }
                else{
                    fout << "\tNot found in the current ScopeTable" << endl;
                }
            }
        }
        if (array[0] == "P") {
            fout << "Cmd " << count << ": " << array[0] << " " << array[1] << " " << endl;
            if (array[1] == "A") {
                symbolTable.printAllScope(fout);
            } else {
                fout << "\tScopeTable# " << symbolTable.getCurrentID() + 1 << endl;
                symbolTable.printCurrentScope(fout);
            }
        }
        if (array[0] == "S") {
            cout << symbolTable.getScopeNumber() + 1 << endl;
            symbolTable.enterScope(symbolTable.getScopeNumber() + 1);
            fout << "Cmd " << count << ": " << array[0] << " " << array[1] << " " << endl;
            fout << "\tScopeTable# " << symbolTable.getScopeNumber() + 1 << " created" << endl;
        }
        if (array[0] == "E") {
            int removedScope = symbolTable.exitScope();
            fout << "Cmd " << count << ": " << array[0] << " " << array[1] << " " << endl;
            if(removedScope == -1){
                fout << "\tScopeTable# 1 cannot be removed" << endl;
            }
            else{
                fout << "\tScopeTable# " << removedScope + 1 << " removed" << endl;
            }
        }
        if (array[0] == "Q") {
            fout << "Cmd " << count << ": " << array[0] << " " << endl;
            scopeTable* temp = symbolTable.getCurrentScope();
            while(temp != nullptr){
                int removedScope = symbolTable.exitScope();
                if(removedScope == -1){
                    fout << "\tScopeTable# " << removedScope + 2 << " removed" << endl;
                }
                else{
                    fout << "\tScopeTable# " << removedScope + 1 << " removed" << endl;
                }
                temp = temp->getParentScope();
            }
        }
    }
    fin.close();
    fout.close();
    return 0;
}
