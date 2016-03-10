all:
	g++ Node.cpp Stack.cpp StackHelper.cpp Token.cpp LexicalAnalyser.cpp parser.cpp main.cpp -o p1

cl:
	rm -f *.0 p1
