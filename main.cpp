#include <cstdio>
#include <cmath>
#include <string>
#include <cstring>
#include <vector>
//#include <iostream>

using namespace std;

/*
    Should take in a simple equation like "a + b", "a - b", "a * b", etc. and return the solution
*/

int performSimpleOperation(char* subEquationChars){
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

int main(int argc, char *argv[]){
    if (argc != 2){
        printf("Incorrect number of params (%d) for test code. Please provide a problem as a string", argc-1);
        return 0;
    }

    char* BaseEquation = argv[1];
    
    performSimpleOperation(BaseEquation);

    return 0;
}

