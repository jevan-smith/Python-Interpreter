//
// Created by smith on 3/20/2019.
//

#include <iostream>
#include "SymTab.hpp"


void SymTab::setValueFor(std::string vName, TypeDescriptor value) {
    // Define a variable by setting its initial value.

    if(value.getTypeValue() == 0) { // Int
        //std::cout << "[DEBUG] "<< vName << " <- " << value.getIntValue() << std::endl;
        symTab[vName] = value.getIntValue();
    }
    else if(value.getTypeValue() == 1) { // Double
        //std::cout << "[DEBUG] "<< vName << " <- " << value.getDoubleValue() << std::endl;
        symTab[vName] = value.getDoubleValue();
    }
    else if(value.getTypeValue() == 2) { // String
        //std::cout << "[DEBUG] "<< vName << " <- " << value.getStringValue() << std::endl;
        symTab[vName] = value.getStringValue();
    }
    else if(value.getTypeValue() == 3) { // Bool
        //std::cout << "[DEBUG] "<< vName << " <- " << value.getBoolValue() << std::endl;
        symTab[vName] = value.getBoolValue();
    }
    else if(value.getTypeValue() == 4) { // Array
        //std::cout << "[DEBUG] "<< vName << " <- " << value << std::endl;
        symTab[vName] = value;
    }

}

bool SymTab::isDefined(std::string vName) {
    return symTab.find(vName) != symTab.end();
}

TypeDescriptor SymTab::getValueFor(std::string vName) {
    if( ! isDefined(vName)) {
        std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
        exit(1);
    }
    //std::cout << "SymTab::getValueFor: " << vName << " contains " << symTab.find(vName)->second << std::endl;
    return symTab.find(vName)->second;
}
