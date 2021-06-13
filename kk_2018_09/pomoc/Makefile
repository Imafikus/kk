test: test.cpp	
	clang++ -o $@ $< `llvm-config --cxxflags --ldflags --system-libs --libs core`

.PHONY: clean

clean:
	rm -f *~ test
