# Creating main program
main: main.o store.o flatfilestore.o postgresqlstore.o sqlite3store.o
	$(CXX) -g -o $@ $^

# How to compile .cc into .o
%.o: $.cc
	$(CXX) -g -Wall -c -o $@ $<

# Cleaning of artifacts
clean:
	rm -f main *.o

# Extra dependencies should be added here to express that FILE.o not only
# depends on FILE.cc but also on the local headers it includes. That's left
# to the reader.
