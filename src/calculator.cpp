#include <cstdio>
#include <cmath>
#include <string>
#include <stack>
#include <sstream>
#include <cstring>
#include <regex>

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
    regex allowedRegex("[0-9\\+\\-\\*\\/\\^\\%\\(\\)]*");
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

int Calculator::performSimpleOperation(int a, int b, char op){
    if (_DEBUG == true){
        printf("step: %d %c %d\n", a, op, b);
    }
    if (op == '/'){
        if (b == 0){
            throw runtime_error("Attempted divide by 0");
        }
    }
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        case '%': return a % b;
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

int Calculator::evaluate(const string s){
    stack<int> numbers;
    stack<char> ops;

    string s_cleaned_temp;

    for (int i = 0; i < s.size(); ++i){
        char c = s[i];

        if (isspace(c)) continue;

        s_cleaned_temp += c;

        if (isdigit(c)) {
            int num = 0;
            while (isdigit(s[i])){
                num = num * 10 + (s[i] - '0');
                i++;
            }
            i--;
            numbers.push(num);
        } else if (c == '('){
            if (i > 0 && isdigit(s_cleaned_temp[i - 1])){
                ops.push('*');
            }
            ops.push(c);
        } else if (c == ')'){
            while (ops.top() != '(' && (!ops.empty())){
                int b = numbers.top();
                numbers.pop();
                int a = numbers.top();
                numbers.pop();
                char op = ops.top();
                ops.pop();
                numbers.push(performSimpleOperation(a, b, op));
            }
            ops.pop();
        } else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(c)){
                if (c == '^' && ops.top() == '^') {
                    break;
                }
                int b = numbers.top();
                numbers.pop();
                int a = numbers.top();
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
        int b = numbers.top();
        numbers.pop();
        int a = numbers.top();
        numbers.pop();
        numbers.push(performSimpleOperation(a, b, ops.top()));
        ops.pop();
    }

    return numbers.top();
}