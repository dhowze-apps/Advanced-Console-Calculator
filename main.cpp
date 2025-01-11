#include <iostream>
#include <string>
#include <vector>
#include "consolecalculator.h"

using namespace std;

int main() {

    while (true) {

        string input;

        getline(cin, input); // Get console input

        vector<string> tokens = tokenizer(input); // Tokenize the input into a series of tokens
        variant<bool, string> a = syntax_checker(tokens); // Check for token syntax

        auto* r = get_if<string>(&a); // Get the error message if there is a syntax error
        // If there is not an error, preceed to evaluate
        if (r == nullptr) {

            variant<string, double> b = evaluator(tokens); //Evaluate the tokens

            auto* r2 = get_if<double>(&b); // Get the result
            // Output the result as a string if undefined and a double if a double
            if (r2 == nullptr) {

                cout << *get_if<string>(&b) << endl;
            }
            
            else cout << *r2 << endl;
            
        } else cout << *r << endl;
    }

    return 0;
}