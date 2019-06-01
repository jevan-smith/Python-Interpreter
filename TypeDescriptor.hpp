//
// Created by smith on 3/18/2019.
//

#ifndef EXPRINTER_TYPEDESCRIPTOR_HPP
#define EXPRINTER_TYPEDESCRIPTOR_HPP

#include <vector>
#include <iostream>

class TypeDescriptor {
public:
    enum types { INTEGER, DOUBLE, STRING, BOOL, ARRAY, null };

    int getIntValue();
    std::string getStringValue();
    double getDoubleValue();
    bool getBoolValue();
    types getTypeValue();
    bool isEqual( int b);

    TypeDescriptor createArray();
    TypeDescriptor push(TypeDescriptor a);
    TypeDescriptor pop();
    int length();
    int typeLength(TypeDescriptor a);

    TypeDescriptor &operator [] (int);

    TypeDescriptor turnIntoArray();
    TypeDescriptor removeComma(TypeDescriptor a);
    TypeDescriptor keepTogether(TypeDescriptor a, TypeDescriptor b);
    TypeDescriptor floorDivision(TypeDescriptor a, TypeDescriptor b);

    TypeDescriptor & operator = (const int &d);
    TypeDescriptor & operator = (const double &d);
    TypeDescriptor & operator = (const std::string &d);
    TypeDescriptor & operator = (const bool &d);

    //Operations
    friend std::ostream & operator << (std::ostream &out, const TypeDescriptor &c);
    TypeDescriptor operator - ();
    TypeDescriptor operator + ( TypeDescriptor &d);
    TypeDescriptor operator == ( TypeDescriptor &d);
    TypeDescriptor operator - ( TypeDescriptor &d);
    TypeDescriptor operator * ( TypeDescriptor &d);
    TypeDescriptor operator / ( TypeDescriptor &d);
    TypeDescriptor operator % ( TypeDescriptor &d);
    TypeDescriptor operator > ( TypeDescriptor &d);
    TypeDescriptor operator >= ( TypeDescriptor &d);
    TypeDescriptor operator < ( TypeDescriptor &d);
    TypeDescriptor operator <= ( TypeDescriptor &d);
    TypeDescriptor operator != ( TypeDescriptor &d);
    TypeDescriptor operator || ( TypeDescriptor &d);
    TypeDescriptor operator && ( TypeDescriptor &d);
    TypeDescriptor operator ! ( void );

    TypeDescriptor();
    ~TypeDescriptor();

private:
    int _intValue = 0;
    std::string _stringValue = "";
    double _doubleValue = 0.0;
    bool _boolValue;
    std::vector<TypeDescriptor> _arrayValue;
    int _arraySize = 0;
    types _type = null;

};

#endif //EXPRINTER_TYPEDESCRIPTOR_HPP
