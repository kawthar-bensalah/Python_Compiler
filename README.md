# Python_Compiler

Compile Python code through five phases:

 1- Lexicalanalysis;  
 2- Syntaxanalysis;  
 3- Semantic analysis;  
 4- Optimisationphase;  
 5- Machine code generation.


## Description

The goal of this project is to build a Python compiler passing through the different phases
of compilation, namely lexical analysis using the FLEX tool and syntaxic-semantic analysis
using the BISON tool, intermediate code generation, optimization, etc.
As well as the generation of the machine code will be done in C language.
Parallel processing concerning the management of the symbol table and the
processing of the various errors are also carried out during the analysis phases of the
compilation process.


## Getting Started

### Requirements

* Linux OS.
* Flex.
* Bison.

### Executing program

* To install Flex Package on Ubuntu   
```
sudo apt-get update -y    
sudo apt-get install -y flex  
```

* To install Bison Package on Ubuntu   

```
sudo apt-get update -y  

sudo apt-get install -y bison  
```

* To compile the program:   
```
$ flex TP.l  

$ gcc lex.yy.c -o TP -lfl  
```
                  
* To run the program:  
```
$ ./TP 
```
 
* To stop the program: 
```
$ Ctrl+c 
```
 
* The compilation of the bison file is done as follows:   
```
Bison -d bison.y  
gcc bison.tab.c lex.yy.c -o executable name   
./executable name  
/  
 
```

## Authors

BENSALAH Kawthar 
bensalahkawthar9@gmail.com

