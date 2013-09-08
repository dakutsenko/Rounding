TARGET = round

$(TARGET): main.cpp
	g++ -Wall $^ -o $@

.PHONY: clean
clean:
	-rm $(TARGET)
