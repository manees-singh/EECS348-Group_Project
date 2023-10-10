#include <cstdio>
#include <cmath>
#include <string>
#include <stack>
#include <sstream>
#include <cstring>
#include <vector>
//#include <iostream>

using namespace std;

/*
    Should take in a simple equation like "a + b", "a - b", "a * b", etc. and return the solution
*/

int old_performSimpleOperation(char* subEquationChars){
    string subEquation(subEquationChars);
    vector<char> operands;
    for (char c : subEquation) {
        if (c == '+' || c == '-') {
            operands.push_back(c);
        }
    }


    char delimiters[] = "+-";
    char* token = strtok(subEquationChars, delimiters);

    int result = 0;
    int itter = -1;

    while (token != NULL){

        if (itter < operands.size() && itter >= 0) {
            if (operands[itter] == '+'){
                result += atoi(token);
            } else if (operands[itter] == '-'){
                result -= atoi(token);
            }
        } else if (itter == -1) {
            result = atoi(token);
        }

        token = strtok(NULL, delimiters);
        itter++;
    }
    
    printf("The result was %d\n", result);

    return result;
}

int performSimpleOperation(int a, int b, char op){
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
    }

    return 0;
}

/* WARNING: CURRENT VERSION DOES NOT NECESSARILY FOLLOW PEMDAS */

int evaluate(const char* s){
    stack<int> numbers;
    stack<char> ops;

    for (int i = 0; i < s.length(); ++i){
        char c = s[i];

        if (isdigit(c)) {
            int num = 0
            while (isdigit(s[i])){
                num = num * 10 + (s[i] - '0']);
                i++;
            }
            i--;
            numbers.push(num);
        } else if (c == '('){
            ops.push(c);
        } else if (c == ')'){
            while (!ops.empty() && ops.top() != '('){
                int b = numbers.top();
                numbers.pop();
                int a = numbers.top();
                numbers.pop();
                numbers.push(operate(a, b, ops.top()));
                ops.pop();
            }
            ops.push(c);
        }
    }

    while (!ops.empty()){
        int b = numbers.top();
        numbers.pop();
        int a = numbers.top();
        numbers.pop();
        numbers.push(operate(a, b, ops.top()));
        ops.pop();
    }

    return numbers.top();
}

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Incorrect number of params (%d) for test code. Please provide a problem as a string", argc-1);
        return 0;
    }

    char* BaseEquation = argv[1];
    
    evaluate(BaseEquation);

    return 0;
}

