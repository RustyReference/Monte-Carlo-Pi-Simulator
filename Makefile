main:
	gcc -fsanitize=address -Wall -Wextra -g -o main main.c functions.c result_functions.c stats.c -lpthread

test:
	gcc -fsanitize=address -Wall -Wextra -g -o test test.c functions.c

clean:
	rm main 
	rm -r main.dSYM

cleanTest: 
	rm test
	rm -r test.dSYM
