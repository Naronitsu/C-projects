all: worksheet worksheet_dynamic

worksheet:
	gcc -std=c11 -Wall -Wextra -O2 -g -o worksheet worksheet.c

worksheet_dynamic:
	gcc -std=c11 -Wall -Wextra -O2 -g -o worksheet_dynamic worksheet_dynamic.c

clean:
	rm -f worksheet worksheet_dynamic *.o