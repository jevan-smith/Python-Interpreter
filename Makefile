.SUFFIXES: .o .cpp .x

CFLAGS = -ggdb -std=c++17
objects =  Functions.o TypeDescriptor.o Token.o Tokenizer.o Parser.o ArithExpr.o SymTab.o Statements.o main.o

statement.x: $(objects)
	g++ $(CFLAGS) -o statement.x $(objects)

.cpp.o:
	g++ $(CFLAGS) -c $< -o $@


Functions.o: Functions.cpp Functions.hpp TypeDescriptor.hpp
TypeDescriptor.o: TypeDescriptor.cpp TypeDescriptor.hpp
Token.o:  Token.cpp Token.hpp
Tokenizer.o: Tokenizer.cpp Tokenizer.hpp
ArithExpr.o: ArithExpr.cpp ArithExpr.hpp TypeDescriptor.hpp Token.hpp SymTab.hpp
SymTab.o: SymTab.cpp SymTab.hpp TypeDescriptor.hpp
Parser.o: Parser.cpp Token.hpp Parser.hpp Tokenizer.hpp SymTab.hpp ArithExpr.hpp Statements.hpp
Statements.o: Statements.cpp Statements.hpp TypeDescriptor.hpp Functions.hpp ArithExpr.hpp Token.hpp SymTab.hpp
main.o: main.cpp Token.hpp Tokenizer.hpp Parser.hpp SymTab.hpp ArithExpr.hpp Statements.hpp TypeDescriptor.hpp

clean:
	rm -fr *.o *~ *.x
