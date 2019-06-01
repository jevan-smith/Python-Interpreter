//
// Created by smith on 3/22/2019.
//

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include "TypeDescriptor.hpp"

#ifndef EXPRINTER_FUNCTIONS_HPP
#define EXPRINTER_FUNCTIONS_HPP

class range {
public:
    int getStartValue();
    int getEndValue();
    int getStepValue();
    void setStartValue(int value);
    void setEndValue(int value);
    void setStepValue(int value);
    void next();

    void setValues(TypeDescriptor value);

    range();
    ~range();

private:
    int _start = 0;
    int _end = 0;
    int _step = 1;
    int stringToInt(std::string value);
};

#endif //EXPRINTER_FUNCTIONS_HPP
