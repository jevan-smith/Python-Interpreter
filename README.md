## Authors
Made by: Jevan

## Project Goals
1. The goal of this project was to create a simple sudo-python interpreter, with the intent to
learn how to create an interpreter from the groundup.
2. This included learning concepts such as lexical analysis/tokanization, building symbol tables, 
and the understanding of formal grammar rules.

## Requirements
- type "make" to build file (on linux)
- "./statement.x inputExpressions.txt" to run file (on linux)
- Compiled with MinGW (Version: w64 6.0)

## Features
- [x] function support
- [x] return value
- [x] array support
- [x] array len, array pop, array append
- [x] Assignment Statements
- [x] Relation Operators
- [x] Print statements
- [x] For Loops
- [x] if, elif, and else statements
- [x] Empty/Comments lines allowed
- [x] Strings, Doubles, and Bool Types supported
- [x] python style negative syntax supported "-- ----1"
- [x] 'not', 'and', 'or' supported
- [x] '<>' and '//' operators added and supported

### Known Issues
- [ ] printing multiple array's Example: "print array1, array2"
- [ ] functions with multiple array arguements
- [ ] minor issues with type double

## Expected Output
<pre>
--------------------------------
[MESSAGE] Program Output:
--------------------------------
array pass by reference test: Correct!
int passed by value: Correct!
sumAndDouble: Correct!
scopeTest: Correct!
stringTest: Correct!
fibonacci: Correct!
len of u: Correct!
len of x: Correct!
subscript/append test: Correct!
pop test: Correct!
u[0] test: Correct!
u[1+2] + 5 test: Correct!
g test: Correct!
->EVERYTING WORKS!!!<-
</pre>
## Above Test Input
<pre>
finalResult = 0

def someFunc(b):
    b.append(3)
    return b

b = [1, 2, 3, 4, 5]
someFunc(b)
if(len(b) == 6):
    finalResult = finalResult + 1
    print "array pass by reference test: Correct!"
else:
    print "someFunc(b) is Incorrect!"

def valueTest(a):
    a = 1
    return a
a = 4
valueTest(a)
if(a == 4):
    finalResult = finalResult + 1
    print "int passed by value: Correct!"
else:
    print "valueTest(a) is Incorrect!"


def double(a):
    c = a*2
    return c

def sum(a, b):
    c = a + b
    return c

def sumAndDouble(a, b): #Testing nested functions
    c = sum(a, b)
    z = double(c)
    return z

a = 5
b = 5

if(sumAndDouble(a, b) == 20):
    finalResult = finalResult + 1
    print "sumAndDouble: Correct!"
else:
    print "sumAndDouble is Incorrect!"

j = 5
def scopeTest():
    return j

if(scopeTest() == 5):
    finalResult = finalResult + 1
    print "scopeTest: Correct!"
else:
    print "scopeTest is Incorrect!"

def stringTest(stringValue):
    c = stringValue + "!"
    return c

if(stringTest('testing') == 'testing!'):
    finalResult = finalResult + 1
    print "stringTest: Correct!"
else:
    print "stringTest is Incorrect!"

#Fibonacci number test
def fibonacci(n):
    a = 0
    b = 1
    if n < 0:
        print("Incorrect input")
    elif n == 0:
        return a
    elif n == 1:
        return b
    else:
        for i in range(2, n):
            c = a + b
            a = b
            b = c
        return b

# Driver Program
if(fibonacci(9) == 21):
    finalResult = finalResult + 1
    print "fibonacci: Correct!"
else:
    print "fibonacci is Incorrect!"

u = [1, 2, 3, 4, 5, 6, 7, 8]
x = []

if(len(u) == 8):
    finalResult = finalResult + 1
    print "len of u: Correct!"
else:
    print "len of u is Incorrect!"
if(len(x) == 0):
    finalResult = finalResult + 1
    print "len of x: Correct!"
else:
    print "len of x is Incorrect!"

x.append(1)

if(x[0] == 1):
    finalResult = finalResult + 1
    print "subscript/append test: Correct!"
else:
    print "subscript/append is Incorrect!"

u.pop()

if(len(u) == 7):
    finalResult = finalResult + 1
    print "pop test: Correct!"
else:
    print "pop is Incorrect!"

u[0] = 23

if(u[0] == 23):
    finalResult = finalResult + 1
    print "u[0] test: Correct!"
else:
    print "u[0] is Incorrect!"

n = u[1+2] + 5

if(n == 9):
    finalResult = finalResult + 1
    print "u[1+2] + 5 test: Correct!"
else:
    print "u[1+2] + 5 is Incorrect!"

value = 0
g = ["2", "3", "88"]
for i in range(len(g)):
    if(g[i] == "2" or g[i] == "3" or g[i] == "88"):
        value = value + 1

if(value == 3):
    finalResult = finalResult + 1
    print "g test: Correct!"
else:
    print "g is Incorrect!"

if(finalResult == 13):
    print "->EVERYTING WORKS!!!<-"
else:
    print "SOMETHING FAILED :("
