# kompilator c
CCOMP = gcc
# konsolidator
LOADER = gcc
# opcje optymalizacji:
# wersja do debugowania
OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
#OPT = -O3 -fopenmp
# pliki naglowkowe
INC = -I./include
# biblioteki
#LIB = -L../pomiar_czasu -lpomiar_czasu -lm -lpthread
LIB = -L./lib -lpomiar_czasu -lm -lpthread
run: liczenie_calki clean
liczenie_calki: liczenie_calki.o
	$(LOADER) $(OPT) liczenie_calki.o -o liczenie_calki $(LIB)
liczenie_calki.o: liczenie_calki.c include/pomiar_czasu.h
	$(CCOMP) -c $(OPT) liczenie_calki.c $(INC)
clean:
	rm -f *.o
