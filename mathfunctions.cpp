#define _USE_MATH_DEFINES

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <variant>
#include "consolecalculator.h"

using namespace std;

// Operator functions (add, subtract, multiply, divide, modulo, exponent)

variant<string, double> add_(double a, double b) {

   if (isnan(a) || isnan(b)) return "undefined";
   if (isinf(a) && a > 0 && isinf(b) && b < 0) return "undefined";
   if (isinf(a) && a < 0 && isinf(b) && b > 0) return "undefined";

   return a + b;
}

variant<string, double> sub_(double a, double b) {

   if (isnan(a) || isnan(b)) return "undefined";
   if (isinf(a) && a > 0 && isinf(b) && b > 0) return "undefined";
   if (isinf(a) && a < 0 && isinf(b) && b < 0) return "undefined";

   return a - b;
}

variant<string, double> mul_(double a, double b) {

   if (isnan(a) || isnan(b)) return "undefined";
   if (a == 0 && isinf(b)) return "undefined";
   if (isinf(a) && b == 0) return "undefined";
   if (a == 0 && isinf(b)) return "undefined";

   return a * b;
}

variant<string, double> div_(double a, double b) {

   if (isnan(a) || isnan(b)) return "undefined";
   if (b == 0) return "undefined";
   if (isinf(a) && isinf(b)) return "undefined";

   return a / b;
}

variant<string, double> exp_(double a, double b) {

   if (isnan(a) || isnan(b)) return "undefined";
   if (a == 0 && b == 0) return "undefined";
   if ((a == 0 && isinf(b)) || (isinf(a) && b == 0) || (a == 1 && isinf(b))) return "undefined";
   if (a < 0 && isinf(b)) return "undefined";

   double r = pow(a, b);

   if (isnan(r)) return "undefined";

   return r;
}

variant<string, double> mdo_(double a, double b) {

   if (isnan(a) || isnan(b)) return "undefined";
   if (b == 0) return "undefined";
   if (isinf(a)) return "undefined";
   if (isinf(b) && b < 0) return "undefined";

   return fmod(a, b);
}

// Custom and math Functions

variant<string, double> sin(vector<double> args) {

   if (args.size() == 2) {
      
      double r = std::sin(args[1]);

      return exp_(r, args[0]);
   }

   double r2 = std::sin(args[0]);

   if (isnan(r2)) return "undefined";

   return r2;
}

variant<string, double> cos(vector<double> args) {

   if (args.size() == 2) {
      
      double r = std::cos(args[1]);

      return exp_(r, args[0]);
   }

   double r2 = std::cos(args[0]);

   if (isnan(r2)) return "undefined";

   return r2;
}

variant<string, double> tan(vector<double> args) {

   if (args.size() == 2) {
      
      double r = std::tan(args[1]);

      return exp_(r, args[0]);
   }

   double r2 = std::tan(args[0]);

   if (isnan(r2)) return "undefined";

   return r2;
}

variant<string, double> log(vector<double> args) {

   if (args.size() == 2) {

      if (args[1] <= 0) return "undefined";

      variant<string, double> r = div_(std::log(args[1]), std::log(args[0]));

      return r;
   }

   if (args[0] <= 0) return "undefined";

   double r2 = std::log10(args[0]);

   if (isnan(r2)) return "undefined";

   return r2;
}

variant<string, double> ln(vector<double> args) {

   if (args.size() == 2) {

      if (args[1] <= 0) return "undefined";

      double r = std::log(args[1]);

      return exp_(r, args[0]);
   }

   if (args[0] <= 0) return "undefined";

   double r2 = std::log(args[0]);

   if (isnan(r2)) return "undefined";

   return r2;
}

// Map all functions to later be initiated for use

map<string, variant<string, double>(*)(vector<double>)> init_functions() {

   map<string, variant<string, double>(*)(vector<double>)> funcs;

   funcs["sin"] = sin;
   funcs["cos"] = cos;
   funcs["tan"] = tan;
   funcs["log"] = log;
   funcs["ln"] = ln;

   return funcs;
}

// Map all constants to later be initiated for use

map<string, double> init_constants() {

   map<string, double> consts;

   consts["pi"] = M_PI;
   consts["inf"] = INFINITY;
   consts ["e"] = M_E;

   return consts;
}

// Map all operator functions to later be initiated for use

map<string, variant<string, double>(*)(double, double)> init_operator_functions() {

   map<string, variant<string, double>(*)(double, double)> ofuncs;

   ofuncs["add"] = add_;
   ofuncs["sub"] = sub_;
   ofuncs["mul"] = mul_;
   ofuncs["div"] = div_;
   ofuncs["exp"] = exp_;
   ofuncs["mod"] = mdo_;

   return ofuncs;
}