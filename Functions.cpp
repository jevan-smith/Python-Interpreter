//
// Created by smith on 3/22/2019.
//

#include "Functions.hpp"

range::range() {}
range::~range() {}

int range::getStartValue() {
    return _start;
}

int range::getEndValue() {
    return _end;
}

int range::getStepValue() {
    return _step;
}

void range::setStartValue(int value) {
    _start = value;
}

void range::setEndValue(int value) {
    _end = value;
}

void range::setStepValue(int value) {
    _step = value;
}

int range::stringToInt(std::string value) {
    int result = 0;
    result = atoi(value.c_str());
    return result;
}

void range::setValues(TypeDescriptor typeDesc) {

    if(typeDesc.getTypeValue() == 2) { //Handle String values
        std::string value;
        value = typeDesc.getStringValue();

        int counter = 0;
        std::string tempString;
        std::vector<std::string> tempVector;

        for (int i = 0; i < value.size(); i++) {
            if (isdigit(value[i]) || value[i] == '-') {
                tempString += value[i];
            } else if (isspace(value[i])) {
                counter++;
                tempVector.push_back(tempString);
                tempString.clear();
            } else {
                std::cout << "Value not allowed inside range!, exiting " << value[i] << std::endl;
                exit(6543);
            }
        }

        if (counter > 2) {
            std::cout << "Cannot have more than three function arguments for range" << std::endl;
            exit(5432);
        } else {
            tempVector.push_back(tempString);
        }

        if (counter == 0)
            setEndValue(stringToInt(tempVector[0]));
        else if (counter == 1) {
            setStartValue(stringToInt(tempVector[0]));
            setEndValue(stringToInt(tempVector[1]));
        } else if (counter == 2) {
            setStartValue(stringToInt(tempVector[0]));
            setEndValue(stringToInt(tempVector[1]));
            setStepValue(stringToInt(tempVector[2]));
        }
    }
    else { // Handles int values
        int intValue = 0;
        intValue = typeDesc.getIntValue();

        setEndValue(intValue);
    }
}

void range::next() {
    _start += _step;
}