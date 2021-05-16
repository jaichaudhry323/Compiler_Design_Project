# Compiler_Design_Project

![Working](./fast-slow-1621194130341.gif)

*RUN THESE COMMANDS IN ORDER*
====================================================================================================
################--------TO PRINT CODE OUTPUT---------################
yacc -y -d cal.y
lex cal.l
gcc -c y.tab.c
gcc -c lex.yy.c
gcc y.tab.o lex.yy.o cal_impl.c -o impl

Impl.exe <input.txt > output.txt                         ## To compile code present in input.txt

====================================================================================================
################--------TO PRINT THREE ADDRESS CODE---------################
gcc -c y.tab.c
gcc -c lex.yy.c
gcc -c cal_3addr.c
gcc  y.tab.o lex.yy.o cal_3addr.o -o impl3

impl3 <input.txt > output_3.txt                          ## To compile code present in input.txt

====================================================================================================
################--------TO RUN ABOVE CREATED EXECUTABLES ON INPUT.TXT FILE---------################
Impl.exe <input.txt > output.txt
impl3 <input.txt > output_3.txt

====================================================================================================

################-------- THE END --------##################
