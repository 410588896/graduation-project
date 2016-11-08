CC 	= g++

FOO := $(wildcard ./*.cpp)
BAR := $(basename $(FOO))
OBJ := $(addsuffix .o,$(BAR))
INC := $(wildcard ./*.h)

.cpp.o: 
	$(CC) -c -g $<

BloomFilter:$(OBJ)
	@echo "Compiling......"
	$(CC) -o $@ $^ 
	@echo "Done......"

sinclude $(FOO:.cpp=.d)
%d:%cpp
	@echo "Create depend"
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

release:BloomFilter
	strip BloomFilter

clean:
	@echo "Cleaning......"
	rm -f *.o;rm -f *.d;rm -f BloomFilter;rm -f *~
	@echo "Done......"


