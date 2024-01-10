#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <conio.h>
#include <iostream>

#include "Tokenization.h"
#include "Parser.h"
#include "Generation.h"

int main() {
	std::string contents;
	{
		std::stringstream content_stream;
		std::ifstream sourceFile("Test.nut");
		content_stream << sourceFile.rdbuf();
		contents = content_stream.str();
	}

	Lexer lexer(contents);
	std::vector<Token> tokens = lexer.Tokenize();
	lexer.PrintTokens();
	

	Parser parser(tokens);
	Stmt_Program AST = parser.GenerateAST();

	Generator generator;
	generator.Print(AST);

	std::cout << "[Program] Program finished! Press any key to exit..." << std::endl;

	while (!_kbhit());
	return 0;
}