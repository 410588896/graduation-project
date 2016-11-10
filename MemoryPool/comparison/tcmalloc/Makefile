CC 	= g++ -g 

FLAG = -lpthread -ltcmalloc

FOO := $(wildcard ./*.cpp)
BAR := $(basename $(FOO))
OBJ := $(addsuffix .o,$(BAR))
INC := $(wildcard ./*.h)

.cpp.o: 
	$(CC) -c -g $<

MemoryPool:$(OBJ)
	@echo "Compiling......"
	$(CC) -o $@ $^ $(FLAG) 
	@echo "Done......"

sinclude $(FOO:.cpp=.d)
%d:%cpp
	@echo "Create depend"
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ ,g' < $@.$$$$ > $@; \
	$(RM) $@.$$$$

release:MemoryPool
	strip MemoryPool

clean:
	@echo "Cleaning......"
	rm -f *.o;rm -f *.d;rm -f MemoryPool;rm -f *~
	@echo "Done......"


