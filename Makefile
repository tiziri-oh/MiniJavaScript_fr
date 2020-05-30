all:
	bison -d bison/parseur.y --report=all -o bison/parseur.tab.c 
	flex -o flex/lex.yy.c flex/lexeur.l 
	gcc -Iinc -Ibison bison/*.c flex/*.c src/*.c *.c -o bin/myJSCompiler 

exp:
	./bin/myJSCompiler exemples/exemple$(num).jsm exemples/exemple$(num).asm

debug:
	./bin/myJSCompiler exemples/exemple$(num).jsm exemples/exemple$(num).asm 1

clean: 
	rm -rf bin/myJSCompiler
	rm -rf flex/lex.yy.c
	rm -rf bison/parseur.tab.*
	rm -rf bison/parseur.output
	rm -rf exemples/*.asm
