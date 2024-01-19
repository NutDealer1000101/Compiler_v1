#pragma once

#include "Tokenization.h"
#include "Arena.h"

#include <variant>
#include <assert.h>

struct Stmt;
struct Expr;

struct Expr_Ident {
	Token ident{ .type = TokenType::IDENTIFIER, .lit = ""};
};

struct Expr_IntLit {
	Token intlit{ .type = TokenType::INT_LIT, .lit = "" };
};

struct Expr_Paren {
	Expr* expr;
};

struct Expr_Terminal {
	std::variant<Expr_Ident*, Expr_IntLit*, Expr_Paren*> var;
};

struct Expr_Binary {
	Expr* left;
	Expr* right;
	std::string op;
};

struct Expr {
	std::variant<Expr_Terminal*, Expr_Binary*> var;
};

struct Stmt_Return {
	// return [expr];
	Expr* expr;
};

struct Stmt_IntDec {
	// int [ident] = [expr];
	Token ident;
	Expr* expr;
};

struct Stmt {
	std::variant<Stmt_IntDec*, Stmt_Return*> var;
};

// Program
struct Stmt_Program {
	std::vector<Stmt*> body;
};


//-------------------Parser-------------------------------------//

class Parser {
private:
	Stmt* parse_Stmt() {
		if (TypeAt() == TokenType::RETURN) {
			// return [expr];
			auto stmt_return = arena.alloc<Stmt_Return>();

			Expect(TokenType::RETURN);
			stmt_return->expr = parse_Expr();
			Expect(TokenType::SEMI);

			auto stmt = arena.alloc<Stmt>();
			stmt->var = stmt_return;

			return stmt;
		} 
		else if (TypeAt() == TokenType::INT) {
			// int [ident] = [expr];
			Stmt_IntDec* stmt_intdec = arena.alloc<Stmt_IntDec>();
			
			Expect(TokenType::INT);
			stmt_intdec->ident = Expect(TokenType::IDENTIFIER);

			Expect(TokenType::ASSIGN);
			stmt_intdec->expr = parse_Expr();
			Expect(TokenType::SEMI);

			auto stmt = arena.alloc<Stmt>();
			stmt->var = stmt_intdec;

			return stmt;
		}
		else {
			PrintError("Invalid Statement!");
			return {};
		}
	}

	Expr_Terminal* parse_Terminal() {
		if (TypeAt() == TokenType::OPEN_PAREN) {
			Eat();
			Expr* expr = parse_Expr();
			Expect(TokenType::CLOSE_PAREN);

			Expr_Paren* paren = arena.alloc<Expr_Paren>();
			paren->expr = expr;

			Expr_Terminal* term = arena.alloc<Expr_Terminal>();
			term->var = paren;

			return term;
		}
		else if (TypeAt() == TokenType::INT_LIT) {
			Expr_IntLit* intlit = arena.alloc<Expr_IntLit>();
			intlit->intlit = Expect(TokenType::INT_LIT);

			Expr_Terminal* expr = arena.alloc<Expr_Terminal>();
			expr->var = intlit;

			return expr;
		}
		else if (TypeAt() == TokenType::IDENTIFIER) {
			Expr_Ident* ident = arena.alloc<Expr_Ident>();
			ident->ident = Expect(TokenType::IDENTIFIER);

			Expr_Terminal* expr = arena.alloc<Expr_Terminal>();
			expr->var = ident;

			return expr;
		}
		else {
			PrintError("Invalid Expression!");
			return {};
		}
	}
	Expr* parse_Expr(int min_prec = 0) {
		Expr_Terminal* term_lhs = parse_Terminal();

		auto expr_lhs = arena.alloc<Expr>();
		expr_lhs->var = term_lhs;

		while (true) {
			Token currentToken = At();
			if (currentToken.type == TokenType::OUT_OF_BOUNDS ||
				!IsBinaryOp(currentToken.type) ||
				BinaryPrecedence(currentToken.type) < min_prec) 
			{
				break;
			}

			Token op = Eat();
			int prec = BinaryPrecedence(op.type);
			int nextMinPrec = prec + 1;

			auto expr_rhs = parse_Expr(nextMinPrec);
			
			auto binary = arena.alloc<Expr_Binary>();
			auto expr_lhs2 = arena.alloc<Expr>();
			expr_lhs2->var = expr_lhs->var;

			binary->left = expr_lhs2;
			binary->right = expr_rhs;
			if (op.type == TokenType::ADD) {
				binary->op = "+";
			}
			else if (op.type == TokenType::SUB) {
				binary->op = "-";
			}
			else if (op.type == TokenType::MULTIPLY) {
				binary->op = "*";
			}
			else {
				PrintError("Unknown operator!");
			}

			expr_lhs->var = binary;
		}

		return expr_lhs;
	}
public:
	Parser(std::vector<Token> tokens) 
		:
		tokens(std::move(tokens)),
		arena(1024 * 1024 * 4)
	{
	}
	
	Stmt_Program GenerateAST() {
		std::cout << "[PARSER] Beginning parsing..." << std::endl;
		Stmt_Program program;

		while (!EndOfFile()) {
			program.body.push_back(parse_Stmt());
		}

		std::cout << "[PARSER] Parsing successfull" << std::endl << std::endl;
		return program;
	}
private:
	Token At(size_t offset = 0) const {
		if (index + offset >= tokens.size()) {
			return Token(TokenType::OUT_OF_BOUNDS);
		}
		else {
			return tokens.at(index + offset);
		}
	}
	TokenType TypeAt(size_t offset = 0) const {
		return At(offset).type;
	}
	Token Eat() {
		Token t = tokens.at(index);
		index++;
		return t;
	}

	Token ExpectCustom(TokenType tk, const std::string& err) {
		if (TypeAt() == tk) {
			return Eat();
		}
		else {
			PrintError(err);
			return Token(TokenType::OUT_OF_BOUNDS);
		}
	}

	Token Expect(TokenType tk) {
		if (TypeAt() == tk) {
			return Eat();
		}
		else {
			std::string err = "Unexpected Token at " + Token::GetPos(At()) + "\t";
			err.append("Expected Token \"" + Token::TypeToString(tk) + "\", got \"" + Token::TokenToString(At()) + "\"");
			PrintError(err);
			return Token(TokenType::OUT_OF_BOUNDS);
		}
	}

	bool EndOfFile() const {
		return TypeAt() == TokenType::END_OF_FILE;
	}

	void PrintError(const std::string& err) {
		std::cerr << "[PARSER] " << err << std::endl;
		exit(EXIT_FAILURE);
	}

	std::vector<Token> tokens;
	size_t index = 0;
	ArenaAllocator arena;
};