yacc -d -y 1905060.y
g++ -w -c -o y.o y.tab.c
flex 1905060.l
g++ -w -c -o l.o lex.yy.c
g++ y.o l.o -lfl a.out
./a.out input.txt

#yacc -d 1905060.y
#flex 1905060.l
#g++ -fpermissive -fsanitize=address -g y.tab.c lex.yy.c -o a.out
#./a.out syntax_semantic_error.c
