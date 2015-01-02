compile: 	
	g++ main.cpp -o main

runK:
	./main search_strings.txt search_text.txt K

runB:
	./main search_strings.txt search_text.txt B  

clean:
	rm -f main main.o 