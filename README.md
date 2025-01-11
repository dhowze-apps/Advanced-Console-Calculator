# Advanced-Console-Calculator
Console Calculator with Advanced Features
# Features
## Operations
Addition  
Subtraction  
Multiplication  
Division  
Modulo  
## Functions
sin  
cos  
tan  
log  
ln  
sqrt  
## Constants
inf  
pi  
e  
# Information
## Function capabilities
All functions have base capabilities. Functions that traditionally use bases such as log
can be typed as logb(n), where b is a positive integer base. If you include a base for a
function that traditionally does not use a base such as sine, the base will be
interpreted as the function raised to the b power. For instance, sin2(5) would be
interpreted as sin(5)^2.
## Syntax
Syntax is traditional, except with constants you cannot do implicit multiplication. For instance,
pi(4) will raise a syntax error because pi would be interpreted as a function instead of
a constant.
## Indeterminate Forms
There is support for indeterminate forms in which the calculator returns undefined.
### Addition:
-inf + inf  
inf + -inf
### Subtraction
inf - inf  
(-inf) - (-inf)
### Multiplication
0 \* inf  
inf \* 0
### Division
x / 0  
inf / inf
### Exponential
0 ^ inf  
inf ^ 0  
0 ^ 0  
1 ^ inf  
(-x) ^ inf
### Modulo
x % 0  
inf % x
## Customizable
This calculator is customizable, so you can add custom functions and constants for your liking.
