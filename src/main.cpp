#include "calculator.h"
#include <iostream>
#include <string>

using namespace std;


int main(int argc, char *argv[]){

    bool _debug = false;

    if (argc > 1) {
        _debug = argv[1]; // Override with command line argument if provided
    }

    Calculator c = Calculator(_debug);

    bool quit = false;
    printf("===================================================================================\n");
    printf("                              Epic Calculator Program                              \n");
    printf("===================================================================================\n\n");

    while(!quit){
        string BaseEquation;

        printf("Enter an Equation or 'q' to quit: ");
        getline(cin, BaseEquation);

        if (BaseEquation.empty()){
            printf("\nYou need to enter an equation.\n\n");
            continue;
        } else if(BaseEquation == "q"){
            quit = true;
            break;
        }

        if (!c.isValidEquation(BaseEquation)){
            printf("The equation you entered is not valid. Please make sure that your equation has balanced parentheses and only contains valid characters:\n0-9, +, -, *, /, %%, ^, (, ), and [space]\n\n");
            continue;
        }

        try {
            int result = c.evaluate(BaseEquation);
            printf("Result: %d\n\n", result);
        }
        catch (const exception& e){
            cerr << "Exception caught: " << e.what() << endl << endl;
        }
       
    }
    

    return 0;
}
