#pragma once

#include <vector>
#include <string>
#include <optional>
#include <iostream>

/*
	NONE,
	IDENTIFIER,				// x
	INT_LITERAL,			// 255

	IF,						//if
	ELSE,					//else
	WHILE,					//while
	RETURN,					//return
	INT,					//int

	PARENTHESES_BEGIN,		// (
	PARENTHESES_END,		// )
	SQUAREBRACKET_BEGIN,	// [
	SQUAREBRACKET_END,		// ]
	CURLYBRACKET_BEGIN,		// {
	CURLYBRACKET_END,		// }

	COMMA,					// ,
	SEMICOLON,				// ;

	ADD,					// +
	SUB,					// -
	MULTIPLY,				// *
	ASSIGN,					// =
	COMPUND_ADD,			// +=
	COMPUND_SUB,			// -=
	COMPUND_MULTIPLY,		// *=
	INCREMENT,				// ++
	DECREMENT,				// --

	ADDRESS_OF,				// &
	DEREFERENCE,			// *

	NOT,					// !
	AND,					// &&
	OR,						// ||

	EQUAL,					// ==
	NOT_EQUAL,				// !=
	LESS,					// <
	LESS_OR_EQUAL,			// <=
	GREATER,				// >
	GREATER_OR_EQUAL		// >=

*/

// (10 + 5) * 2

enum class TokenType {
	//Other
	OUT_OF_BOUNDS, //Returned when parsing if value outside of token vector tries to be read
	SEMI,
	END_OF_FILE,

	//Primary expressions
	IDENTIFIER,
	INT_LIT,

	//Binary expressions
	ADD,
	SUB,
	MULTIPLY,

	//Keywords
	INT,
	RETURN,

	ASSIGN,

	//Symbols
	OPEN_PAREN,
	CLOSE_PAREN
};

struct Token {
	TokenType type;
	std::string lit;

	int tokenNr;
	int line;
	int col;

	static std::string TypeToString(TokenType t) {
		std::string str;

		switch (t) {
		case TokenType::OUT_OF_BOUNDS:
			str.append("OutOfBounds");
			break;
		case TokenType::SEMI:
			str.append("Semicolon");
			break;
		case TokenType::END_OF_FILE:
			str.append("EndOfFile");
			break;
		case TokenType::IDENTIFIER:
			str.append("Ident");
			break;
		case TokenType::INT_LIT:
			str.append("IntLit");
			break;
		case TokenType::ADD:
			str.append("ADD");
			break;
		case TokenType::SUB:
			str.append("SUB");
			break;
		case TokenType::MULTIPLY:
			str.append("MULTIPLY");
			break;
		case TokenType::INT:
			str.append("IntDec");
			break;
		case TokenType::RETURN:
			str.append("Return");
			break;
		case TokenType::ASSIGN:
			str.append("Assign");
			break;
		case TokenType::OPEN_PAREN:
			str.append("OpenParen");
			break;
		case TokenType::CLOSE_PAREN:
			str.append("CloseParen");
			break;
		default:
			str.append("Unknown");
			break;
		}

		return str;
	}
	static std::string TokenToString(Token t) {
		std::string str;

		str.append(TypeToString(t.type));

		switch (t.type) {
		case TokenType::IDENTIFIER:
			str.append("[" + t.lit + "]");
			break;
		case TokenType::INT_LIT:
			str.append("[" + t.lit + "]");
			break;
		}

		return str;
	}
	static std::string GetPos(Token t) {
		return std::to_string(t.line) + ":" + std::to_string(t.col);
	}
};

class Lexer {
public:
	Lexer(const std::string& src) 
		:
		src(src) 
	{
	}

	std::vector<Token> Tokenize() {
		std::string buf;

		while (At().has_value()) {
			buf.clear();

			if (std::isspace(At().value())) {
				if (At().value() == '\n') {
					line++;
					col = 0;
				}
				Eat();
			}
			else if (At().value() == ';') {
				Eat();
				AddToken(TokenType::SEMI);
			}
			else if (At().value() == '(') {
				Eat();
				AddToken(TokenType::OPEN_PAREN);
			}
			else if (At().value() == ')') {
				Eat();
				AddToken(TokenType::CLOSE_PAREN);
			}
			else if (At().value() == '=') {
				Eat();
				AddToken(TokenType::ASSIGN);
			}
			else if (At().value() == '+') {
				Eat();
				AddToken(TokenType::ADD);
			}
			else if (At().value() == '-') {
				Eat();
				AddToken(TokenType::SUB);
			}
			else if (At().value() == '*') {
				Eat();
				AddToken(TokenType::MULTIPLY);
			}
			else if (std::isalpha(At().value())) {
				buf.push_back(Eat());

				//Read characters into buf
				while (At().has_value() && std::isalnum(At().value())) {
					buf.push_back(Eat());
				}

				//Check if buf is a keyword
				std::optional<Keyword> keyword = GetKeyword(buf);
				if (keyword.has_value()) {
					AddToken(keyword.value().type);	//Add keyword token
				}
				else {
					AddToken(TokenType::IDENTIFIER, buf); //Add identifier with value = buf
				}
			}
			else if (std::isdigit(At().value())) {
				buf.push_back(Eat());

				//Read characters into buf
				while (At().has_value() && std::isdigit(At().value())) {
					buf.push_back(Eat());
				}

				AddToken(TokenType::INT_LIT, buf);
			}
			else {
				std::cerr << "[Lexer] " << "Unrecognized symbol \"" << At().value() << "\" At line " << line << ", col " << col << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		AddToken(TokenType::END_OF_FILE);

		return tokens;
	}
	void PrintTokens() const {
		std::cout << "[LEXER] Printing tokens..." << std::endl;
		for (Token t : tokens) {
			std::cout << std::to_string(t.tokenNr) << "\t" << Token::TokenToString(t) << " " << Token::GetPos(t) << std::endl;
		}
		std::cout << "[LEXER] All tokens printed successfully" << std::endl << std::endl;;
	}
private:
	struct Keyword {
		std::string str;
		TokenType type;
	};

	//Contains the reserved keywords and their TokenType
	const std::vector<Keyword> keywords{
		{"int", TokenType::INT},
		{"return", TokenType::RETURN}
	};
private:
	std::optional<char> At(size_t offset = 0) const {
		if (index + offset >= src.length()) {
			return {};
		}
		else {
			return src.at(index + offset);
		}
	}
	char Eat() {
		char c = src.at(index);
		index++;
		return c;
	}

	void AddToken(TokenType type, const std::string& lit = "") {
		tokens.push_back({ .type = type, .lit = lit, .tokenNr = tokenCount, .line = line, .col = col });
		tokenCount++;
		col++;
	}

	//If str is equal to a keyword, return the keyword, else return nothing
	std::optional<Keyword> GetKeyword(const std::string& str) {
		for (auto& k : keywords) {
			if (k.str == str) {
				return k;
			}
		}

		return {};
	}

	const std::string& src;
	std::vector<Token> tokens;

	size_t index = 0;
	int tokenCount = 0;
	int line = 0;
	int col = 0;
};