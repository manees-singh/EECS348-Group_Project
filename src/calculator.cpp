#include <cstdio>
#include <cmath>
#include <string>
#include <stack>
#include <sstream>
#include <cstring>
#include <regex>
#include <climits>

#include "calculator.h"

bool _DEBUG = true;

using namespace std;

/*
    Constructor and Destructor
*/
Calculator::Calculator(bool _debug){
    _DEBUG = _debug;
}

Calculator::~Calculator(){}

bool Calculator::isValidEquation(const string equation){
    regex allowedRegex("[0-9\\+\\-\\*\\/\\^\\%\\(\\) ]*");
    bool onlyAllowedCharacters = regex_match(equation, allowedRegex);

    bool matchingOpenAndClosedParens = false;
    stack<char> parenStack;
    for (char c : equation) {
        if (c == '(') {
            parenStack.push(c);
        } else if (c == ')'){
            if (parenStack.empty()) {
                matchingOpenAndClosedParens = false;
                break;
            }
            parenStack.pop();
        }
    }
    matchingOpenAndClosedParens = parenStack.empty();

    return matchingOpenAndClosedParens && onlyAllowedCharacters;
}

/*
    Should take in a simple equation like "a + b", "a - b", "a * b", etc. and return the solution
*/

float Calculator::performSimpleOperation(float a, float b, char op){
    if (_DEBUG == true){
        printf("step: %f %c %f\n", a, op, b);
    }
    switch (op) {
        case '+': 
            if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b)){
                throw runtime_error("Integer overflow or underflow");
            }
            return a + b;
        case '-':  
            if ((b < 0 && a > INT_MAX + b) || (b > 0 && a < INT_MIN + b)){
                throw runtime_error("Integer overflow or underflow");
            }
            return a - b;
        case '*': 
            if (a > 0){
                if (b > 0) {
                    if (a > (INT_MAX / b)) {
                        throw runtime_error("Integer overflow or underflow");
                    }
                } else {
                    if (b < (INT_MIN / a)) {
                        throw runtime_error("Integer overflow or underflow");
                    }
                }
               
            } else {
                if (b > 0) {
                    if (a < (INT_MIN / b)){
                        throw runtime_error("Integer overflow or underflow");
                    }
                } else {
                    if ( (a != 0) && (b < (INT_MAX / a))) {
                        throw runtime_error("Integer overflow or underflow");
                    }
                }
            }
            return a * b;
        case '/':
             if (b == 0){
                throw runtime_error("Attempted divide by 0");
            } else if (a == INT_MIN && b == -1) {
                throw runtime_error("Integer overflow or underflow");
            }
            return a / b;
        case '^': return pow(a, b);
        case '%': 
            if (b == 0){
                throw runtime_error("Attempt module by 0");
            }
            float result = fmod(a,b);
            if ((result < 0 && b > 0) || (result > 0 && b < 0)){
                result += b;
            }
            return result;
    }

    printf("Unknown operator: %c", op);

    return 0;
}

/*
    Simple lookup for operator precedence. This helps us follow pemdas.
*/

int Calculator::precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '^') return 3;
    return 0;
}

/*
    Main evaluation function. Dual stack based implementation of a calculator: one stack for numbers, one for operators.
*/

float Calculator::evaluate(const string s){
    stack<float> numbers;
    stack<char> ops;

    string s_cleaned_temp;

    bool closeParenForNegativeCompensationAfterNextFullNumber = false;

    for (int i = 0; i < s.size(); ++i){
        char c = s[i];
        if (isspace(c)) continue;
        s_cleaned_temp += c;
    }

    int eq_size = s_cleaned_temp.size();

    for (int i = 0; i < eq_size; ++i){
        char c = s_cleaned_temp[i];
        if (isdigit(c)) {
            float num = 0;
            while (isdigit(s_cleaned_temp[i])){
                float nextDigit = s_cleaned_temp[i] - '0';
                if (num > (INT_MAX - nextDigit) / 10) {
                    throw runtime_error("Number in equation results in overflow (Int32 limit)");
                } else if (num < (INT_MIN + nextDigit) / 10){
                    throw runtime_error("Number in equation results in underflow (Int32 limit)");
                }
                num = num * 10 + nextDigit;
                i++;
            }
            i--;
            numbers.push(num);
            if (closeParenForNegativeCompensationAfterNextFullNumber){
                s_cleaned_temp.insert(i+1, 1, ')');
                closeParenForNegativeCompensationAfterNextFullNumber = false;
                eq_size = s_cleaned_temp.size();
            }
        } else if (c == '('){
            if (i > 0 && isdigit(s_cleaned_temp[i - 1])){
                ops.push('*');
            }
            ops.push(c);
        } else if (c == ')'){
            while ((!ops.empty()) && ops.top() != '('){
                if (numbers.size() < 2){
                    throw runtime_error("Invalid Equation: One or more operators attempted to operate on nothing.");
                }
                float b = numbers.top();
                numbers.pop();
                float a = numbers.top();
                numbers.pop();
                char op = ops.top();
                ops.pop();
                numbers.push(performSimpleOperation(a, b, op));
            }
            if ((!ops.empty())){
                ops.pop();
            }
        } else {
            if (c == '-' && !isdigit(s_cleaned_temp[i-1]) && (s_cleaned_temp[i-1] != ')')){
               // if (s_cleaned_temp[i-1]){
                    if (s_cleaned_temp[i+1] == '('){
                        s_cleaned_temp.insert(i+1, 1, ')');
                        eq_size = s_cleaned_temp.size();
                    } else if (!isdigit(s_cleaned_temp[i+1])){
                        throw runtime_error("Invalid Equation: Negative Inference on symbol");
                    } else {
                        closeParenForNegativeCompensationAfterNextFullNumber = true;
                    }
                    
               // }
                numbers.push(0);
                ops.push('-');
                continue;
            }
            while (!ops.empty() && precedence(ops.top()) >= precedence(c)){
                if (numbers.size() < 2){
                    throw runtime_error("Invalid Equation: One or more operators attempted to operate on nothing.");
                }
                if (c == '^' && ops.top() == '^') {
                    break;
                }
                float b = numbers.top();
                numbers.pop();
                float a = numbers.top();
                numbers.pop();
                char op = ops.top();
                ops.pop();
                numbers.push(performSimpleOperation(a, b, op));
            }
            ops.push(c);
        }
    }

    while (!ops.empty()){
        if (numbers.size() < 2){
            throw runtime_error("Invalid Equation: One or more operators attempted to operate on nothing.");
        }
        float b = numbers.top();
        numbers.pop();
        float a = numbers.top();
        numbers.pop();
        numbers.push(performSimpleOperation(a, b, ops.top()));
        ops.pop();
    }

    return numbers.top();
}