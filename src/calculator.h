#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>

using namespace std;

class Calculator {
    public:
        Calculator(bool _debug);
        ~Calculator();
        /*
            evaluate
            Takes a string representing a mathematical expression and evaluates it.
            The function supports basic arithmetic operations including +, -, *, /, ^, and %,
            and handles parentheses for operation precedence.
        */
        int evaluate(const string s);

        /*
            isValidEquation
            Takes a string that is supposed to represent a matematical expression and checks
            to make sure that it:
                A.) Contains only valid characters ( 0-9, +, -, *, /, %, ^, (, ) )
            and
                B.) Has matching number of open parentheses "(" and closing ")".
        */
        bool isValidEquation(const string s);

    private:
        /*
        performSimpleOperation
        Takes two integers and a char representing an operator,
        and performs a simple arithmetic operation based on the operator.
        Supported operations are +, -, *, /, ^, and %.
        */
        int performSimpleOperation(int a, int b, char op);

        /*
            precedence
            Takes a char representing an operator and returns an integer
            representing the precedence for that operator -- higher precedence
            operators are handled with higher priority.
        */
        int precedence(char op);
};

#endif
