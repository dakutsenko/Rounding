TARGET = round

$(TARGET): main.cpp
	g++ -Wall -ansi -pedantic -O3 $^ -o $@

.PHONY: clean
clean:
	-rm $(TARGET)
