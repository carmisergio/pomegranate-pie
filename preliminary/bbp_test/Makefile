EXECUTABLE = bbp_test
SOURCES = bbp_test.cpp hex_aggregator.hpp

CC=g++
CARGS=-Wall
LIBS=-lgmp -lgmpxx


$(EXECUTABLE): $(SOURCES)
	$(CC) $(CARGS) -o $(EXECUTABLE) $(SOURCES) $(LIBS)
	
clean:
	rm $(EXECUTABLE)
	
run: $(EXECUTABLE)
	./$(EXECUTABLE)