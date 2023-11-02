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

int performSimpleOperation(int a, int b, char op){
    printf("step: %d %c %d\n", a, op, b);
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
    }

    printf("Unknown operator: %c", op);

    return 0;
}

int evaluate(const string s){
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
            while (!ops.empty() && ((c=='+') || (c=='-') || (c=='*') || (c=='/')) && (ops.top() == '*' || ops.top() == '/')){
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
        int b = numbers.top();
        numbers.pop();
        int a = numbers.top();
        numbers.pop();
        numbers.push(performSimpleOperation(a, b, ops.top()));
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
    
    int result = evaluate(BaseEquation);

    printf("\n\nResult: %d", result);

    return 0;
}

