#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <vector>
#include <string>
#include <map>
#include <variant>

using namespace std;

vector<string> tokenizer(string expr);
variant<bool, string> syntax_checker(vector<string> tokens);
variant<string, double> evaluator(vector<string> tokens);
map<string, variant<string, double>(*)(vector<double>)> init_functions();
map<string, variant<string, double>(*)(double, double)> init_operator_functions();
map<string, double> init_constants();

#endif
