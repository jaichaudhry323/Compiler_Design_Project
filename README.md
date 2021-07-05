# Compiler_Design_Project

![Working](./fast-slow-1621194130341.gif)

*RUN THESE COMMANDS IN ORDER in Mini_Compiler Folder*<br>
====================================================================================================<br>
################--------TO PRINT CODE OUTPUT---------################<br>
yacc -y -d cal.y<br>
lex cal.l<br>
gcc -c y.tab.c<br>
gcc -c lex.yy.c<br>
gcc y.tab.o lex.yy.o cal_impl.c -o impl<br>
<br>
Impl.exe <input.txt > output.txt                         ## To compile code present in input.txt<br>
<br>
====================================================================================================<br>
################--------TO PRINT THREE ADDRESS CODE---------################<br>
gcc -c y.tab.c<br>
gcc -c lex.yy.c<br>
gcc -c cal_3addr.c<br>
gcc  y.tab.o lex.yy.o cal_3addr.o -o impl3<br>

impl3 <input.txt > output_3.txt                          ## To compile code present in input.txt<br>
<br>
====================================================================================================<br>
################--------TO RUN ABOVE CREATED EXECUTABLES ON INPUT.TXT FILE---------################<br>
Impl.exe <input.txt > output.txt<br>
impl3 <input.txt > output_3.txt<br>
<br>
====================================================================================================<br>
################-------- THE END --------##################<br>
