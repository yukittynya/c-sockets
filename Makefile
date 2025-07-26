CC = clang
FLAGS = -O3 -ffast-math -flto
CHOICES = server client

build/bin/%: build/%/main.o | build/bin
	$(CC) $(FLAGS) $? -o $@
	chmod +x $@

build/%/main.o: src/%/main.c | build/%/
	$(CC) $(FLAGS) -c $? -o $@

build:
	mkdir -p build

build/bin:
	mkdir -p build/bin

build/%/: build
	mkdir -p build/$*

.PHONY: $(CHOICES)
$(CHOICES): %: build/bin/%

.PRECIOUS: build/%/main.o

.PHONY: clean
clean:
	rm -rvf build/
