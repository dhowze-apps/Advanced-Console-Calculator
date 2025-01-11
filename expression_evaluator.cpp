#include <iostream>
#include <sstream>
#include <vector>
#include <variant>
#include <string>
#include <cstdint>
#include "consolecalculator.h"
#include <algorithm>

using namespace std;
// Initialize constants, functions, and operator functions
map<string, variant<string, double>(*)(vector<double>)> functions = init_functions();
map<string, variant<string, double>(*)(double, double)> ofunctions = init_operator_functions();
map<string, double> constants = init_constants();
// Check if the token is a string implying a constant name or a function name
bool is_string(string s) {

    string letters = "abcdefghijklmnopqrstuvwxyz";

    for (char i : s) {

        string c(1, i);

        if (letters.find(c) == string::npos)  return false;
    }

    return true;
}
// Check if the token is a number based on the characters
bool is_number(string s) {

    string numbers = "0123456789.";

    for (char i : s) {

        string c(1, i);

        if (numbers.find(c) == string::npos) return false;
    }

    return true;
}
// Check for operators
bool is_operator(string s) {

    string operators = "+-*/^%()";

    if (s.length() > 1) return false;
    if (operators.find(s) == string::npos) return false;

    return true;
}
// Tokenizes the expression
vector<string> tokenizer(string expr) {

    vector<string> tokens;
    string string_buffer = ""; // Used to collect a series of characters
    string number_buffer = ""; // Used to collect a series of number characters including decimals

    bool string_buffing = false; // Buffering flag for strings
    bool number_buffing = false; // Buffering flag for numbers

    int i = 0;

    while (true) {
        // If beyond the end of the expression, push the remaining buffer to the tokens
        if (i == expr.length()) {

            if (number_buffing) tokens.push_back(number_buffer);
            else if (string_buffing) tokens.push_back(string_buffer);

            return tokens;
        }

        string c(1, expr[i]); // Convert characters to strings for easy comparing
        // If string buffering is still enabled, keep adding to the buffer until a non-string character is encountered
        if (string_buffing) {

            if (is_string(c)) {

                string_buffer += c;
                i++;

                continue;
            }

            string_buffing = false;
            tokens.push_back(string_buffer);
            string_buffer = "";
        }
        // If number buffering is still enabled, keep adding to the buffer until a non-number character is encountered
        if (number_buffing) {

            if (is_number(c)) {

                number_buffer += c;
                i++;

                continue;
            }

            number_buffing = false;
            tokens.push_back(number_buffer);
            number_buffer = "";
            // Apply implicit multiplication if the number is not after a function and is before a open parenthesis
            if (c == "(") {

                int last_index = tokens.size() - 2; // The last token of the token vector

                if (last_index >= 0) {

                    string last_token = tokens[last_index];
                    // If the last token is not a string (implying a function or constant) then apply implicit multiplication
                    if (is_string(last_token) == false) tokens.push_back("*");
                }
                // Apply implicit multiplication if there is only the number in the vector
                else tokens.push_back("*");
            }

            else if (is_string(c)) tokens.push_back("*");
            
        }
        // check if the character is an operator to later detect negative numbers
        if (is_operator(c)) {

            if (c == "-") {

                int prev_index = i - 1;
                /* If there is a character before the '-" sign and it's a closed parenthesis, replace the "-" sign
                with "-1 *" to make evaluation easier, otherwise just add "-" to the token vector */
                if (prev_index >= 0) {

                    string prev_char(1, expr[prev_index]);

                    if (prev_char == "(") {

                        tokens.push_back("-1");
                        tokens.push_back("*");
                    }

                    else {

                        tokens.push_back(c);
                    }
                }
            
                else {

                    tokens.push_back("-1");
                    tokens.push_back("*");
                }

                i++;
                
                continue;
            }
            // Apply implicit multiplication for ")("
            else if (c == ")") {

                tokens.push_back(c);

                int next_index = i + 1;

                if (next_index < expr.length()) {

                    string next_char(1, expr[next_index]);

                    if (next_char == "(") tokens.push_back("*");
                    else if (is_number(next_char)) tokens.push_back("*");
                    else if (is_string(next_char)) tokens.push_back("*");
                }

                i++;

                continue;
            }

            tokens.push_back(c);

            i++;

            continue;
        }
        /* If the character is a string character (implying a name to a function or constant),
        enable the string buffer flag add the character to the string buffer */
        if (is_string(c)) {

            string_buffing = true;
            string_buffer += c;

            i++;

            continue;
        }
        /* If the character is a number character (including decimals), enable the number buffer flag
        and add the character to the number buffer */
        if (is_number(c)) {

            number_buffing = true;
            number_buffer += c;

            i++;

            continue;
        }
        // Ignore spaces
        if (c == " ") {

            i++;

            continue;
        }
        /* If an invalid character is detected, the the function returns an "!" which the syntax checker checks
        to give an invalid character error */
        return vector<string>({"!"});
    }
}
// Check token syntax
variant<bool, string> syntax_checker(vector<string> tokens) {

    if (tokens.size() == 0) return "";

    int i = 0;
    int open = 0; // Count open parenthesis
    int closed = 0; // Count closed parethesis

    while (true) {
        // If by the end of the tokens the number open and closed parenthesis don't match, issue out an error
        if (i == tokens.size()) {

            if (open != closed) return "Parenthesis syntax error!";

            return true;
        }

        string s = tokens[i];

        // "!" is issued out by the tokenizer when it detects an invalid character
        if (s == "!") return "Invalid characters!";
        // Checks for uneven open and closed parenthesis
        if (s == ")") {

            if (closed == open) return "Operator syntax error!";
            
            closed++;
            i++;

            continue;
        }
        /* If the next operator after an open parenthesis is not an open parenthesis 
        or the open parenthesis is the last token, issue out an error
        */
        if (s == "(") {

            int next_index = i + 1;

            if (next_index < tokens.size()) {

                string next_token = tokens[next_index];

                if (next_token != "(" && is_operator(next_token)) return "Operator syntax error!";
            }

            else return "Operator syntax error!";
            
            open++;
            i++;

            continue;
        }
        /* If the operator is anything but parenthesis and followed by another operator that is not an open
        parethesis, or an operator is the last token, issue out an error
        */
        if (is_operator(s)) {

            int next_index = i + 1;

            if (next_index < tokens.size()) {

                string next_token = tokens[next_index];

                if (next_token != "(" && is_operator(next_token)) return "Operator syntax error!";

            }

            else return "Operator syntax error!";
        }
        /* If a function or constant is detected, check if it is in the form of "functionB(N)" or 
        "function(N)" or a constant not following a open parenthesis or a number, otherwise issue
        out an error */
        if (is_string(s)) {

            if (functions.count(s) == 1) {

                if (tokens.size() == 1) return "Function syntax error!";

                int next_index = i + 1;

                if (next_index < tokens.size()) {

                    string next_token = tokens[next_index];

                    if (is_number(next_token)) {

                        if (next_token.find(".") != string::npos) return "Function syntax error!";
                        if (next_token.length() > 1 && next_token[0] == '0') return "Function syntax error!";
                    }

                    if (is_operator(next_token)) {

                        if (next_token != "(") return "Function syntax error!";

                        i++;

                        continue;
                    }
                    
                    int nexter_index = i + 2;

                    if (nexter_index < tokens.size()) {

                        string nexter_token = tokens[nexter_index];

                        if (nexter_token != "(") return "Function syntax error (5)!";
                    }

                    else return "Function syntax error!";
                }

                else return "Function syntax error!";
            }

            else if (constants.count(s) == 1) {

                int next_index = i + 1;

                if (next_index < tokens.size()) {

                    string next_token = tokens[next_index];

                    if (next_token == "(") return "Constant syntax error!";
                    if (is_number(next_token)) return "Constant syntax error!";
                }
            }

            else return "Unknown function/constant error!";
        }
        /* Check if the token with number characters and/or decimal is in the form of "D.D" or a
        multi-digit integer not starting with 0 */
        if (is_number(s)) {

            if (s[0] == '.' || s.back() == '.') return "Number syntax error!";

            if (s.length() > 1) {

                if (s[0] == '0' && s[1] != '.') return "Number syntax error!";
            }

            bool d = false;

            for (char c : s) {

                if (c == '.') {

                    if (d == true) return "Number syntax error!";

                    d = true;
                }
            }
        }

        i++;

        continue;
    }
}
// Used to evaluate expressions inside parenthesis to later evaluate the simplified expression in PEMDAS order
variant<string, double> evaluate(vector<string> subtokens) {

    int i = 0;
    // Convert constant into their value
    while (true) {

        if (i == subtokens.size()) break;

        string subtoken = subtokens[i];

        if (is_string(subtoken)) {

            if (constants.count(subtoken) == 1) {

                subtokens[i] = to_string(constants[subtoken]);

                i++;

                continue;
            }
        }

        i++;
    }

    i = 0;
    // Evaluate functions
    while (true) {

        if (i == subtokens.size()) break;

        string subtoken = subtokens[i];

        if (is_string(subtoken) && subtoken != "inf") {

            vector<double> args;

            args.push_back(stod(subtokens[i + 1]));

            if (i + 2 < subtokens.size()) {

                string potential_arg2 = subtokens[i + 2];
                
                if (is_number(potential_arg2)) args.push_back(stod(potential_arg2));
            }

            variant<string, double> result = functions[subtoken](args);
            auto* return_value = get_if<double>(&result);

            if (return_value == nullptr) return *get_if<string>(&result);

            subtokens.erase(subtokens.begin() + i, subtokens.begin() + i + args.size() + 1);
            subtokens.insert(subtokens.begin() + i, to_string(*return_value));

            i++;

            continue;
        }

        i++;
    }

    i = subtokens.size() - 2;
    // Evaluate exponents
    while (true) {

        if (i == -1) break;

        string subtoken = subtokens[i];

        if (subtoken == "^") {

            string operand1 = subtokens[i - 1];
            string operand2 = subtokens[i + 1];

            double a = stod(subtokens[i - 1]);
            double b = stod(subtokens[i + 1]);

            variant<string, double> result = ofunctions["exp"](a, b);

            auto* return_value = get_if<double>(&result);

            if (return_value == nullptr) return *get_if<string>(&result);

            subtokens.erase(subtokens.begin() + i - 1, subtokens.begin() + i + 2);
            subtokens.insert(subtokens.begin() + i - 1, to_string(*return_value));

            i -= 2;

            continue;
        }

        i -= 2;
    }

    i = 1;
    // Evaluate multiplication, division, and modulo
    while (true) {

        if (i == subtokens.size()) break;
        
        string subtoken = subtokens[i];

        if (subtoken == "*" || subtoken == "/" || subtoken == "%") {

            string operand1 = subtokens[i - 1];
            string operand2 = subtokens[i + 1];

            double a = stod(operand1);
            double b = stod(operand2);

            string o = (subtoken == "*") ? "mul" : (subtoken == "/") ? "div" : "mod";

            variant<string, double> result = ofunctions[o](a, b);

            auto* return_value = get_if<double>(&result);

            if (return_value == nullptr) return *get_if<string>(&result);

            subtokens.erase(subtokens.begin() + i - 1, subtokens.begin() + i + 2);
            subtokens.insert(subtokens.begin() + i - 1, to_string(*return_value));

            continue;
        }

        i+= 2;
    }

    i = 1;
    // Evaluate addition and subtraction
    while (true) {

        if (i == subtokens.size()) break;

        string subtoken = subtokens[i];

        if (subtoken == "+" || subtoken == "-") {

            string operand1 = subtokens[i - 1];
            string operand2 = subtokens[i + 1];

            double a = stod(operand1);
            double b = stod(operand2);

            string o = (subtoken == "+") ? "add" : "sub";
            variant<string, double> result = ofunctions[o](a, b);

            auto* return_value = get_if<double>(&result);

            if (return_value == nullptr) return *get_if<string>(&result);

            subtokens.erase(subtokens.begin() + i - 1, subtokens.begin() + i + 2);
            subtokens.insert(subtokens.begin() + i - 1, to_string(*return_value));

            continue;
        }

        i+= 2;
    }

    i = 0;

    double result = stod(subtokens[0]);
    // Remove the useless negative sign if the result is -0
    if (result == 0) return double(0);
    
    return result;
}
// The expression evaluator
variant<string, double> evaluator(vector<string> tokens) {

    int i = 0;

    vector<int> open; // Store the indexes of the open parenthesis
    // Repetitively expressions in the most inner parenthesis and evaluate to remove parenthesis
    while (true) {

        if (i == tokens.size()) break;

        string token = tokens[i];

        if (token == "(") {

            open.push_back(i);

            i++;

            continue;
        }
        // Find a closed parenthesis after an open parenthesis, evaluate what's inside, and remove the parenthesis
        if (token == ")") {

            variant<string, double> result = evaluate(vector(tokens.begin() + open.back() + 1, tokens.begin() + i));
            auto* return_value = get_if<double>(&result);

            if (return_value == nullptr) return *get_if<string>(&result);

            tokens.erase(tokens.begin() + open.back(), tokens.begin() + i + 1);
            tokens.insert(tokens.begin() + open.back(), to_string(*return_value));

            i = open.back() + 1;

            open.pop_back();

            continue;
        }

        i++;
    }
    
    // Evaluate what is left
    return evaluate(tokens);
}
