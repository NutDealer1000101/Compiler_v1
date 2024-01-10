#pragma once

#include "Parser.h"

class Generator {
private:
	void Print_Term(const Expr_Terminal* term) {
		struct Terminal_Visitor {
			Generator* gen;
			void operator()(const Expr_Ident* ident) {
				gen->Print(Token::TokenToString(ident->ident));
			}
			void operator()(const Expr_IntLit* intlit) {
				gen->Print(Token::TokenToString(intlit->intlit));
			}
		};

		Terminal_Visitor visitor = { .gen = this };
		std::visit(visitor, term->var);
	}
	void Print_Expr(const Expr* expr) {
		struct Expr_Visitor {
			Generator* gen;
			void operator()(const Expr_Terminal* term) {
				gen->Print("Expr : Terminal {");
				gen->depth++;
				gen->Print_Term(term);
				gen->depth--;
				gen->Print("}");
			}
			void operator()(const Expr_Binary* binary) {
				gen->Print("Expr : Binary Expression {");
				gen->depth++;
				gen->Print("Operator[TODO]");
				gen->Print("Left {");
				gen->depth++;
				gen->Print_Expr(binary->left);
				gen->depth--;
				gen->Print("Right {");
				gen->depth++;
				gen->Print_Expr(binary->right);
				gen->depth--;
				gen->depth--;
				gen->Print("}");
			}
		};

		Expr_Visitor visitor = { .gen = this };
		std::visit(visitor, expr->var);
	}
	void Print_Stmt(const Stmt* stmt) {
		struct Stmt_Visitor {
			Generator* gen;

			void operator()(const Stmt_Return* ret) const {
				gen->Print("Stmt : Return {");
				gen->depth++;
				gen->Print_Expr(ret->expr);
				gen->depth--;
				gen->Print("}");
			}
			void operator()(const Stmt_IntDec* intdec) const {
				gen->Print("Stmt : Integer Decleration {");
				gen->depth++;
				gen->Print("Identifier: " + Token::TokenToString(intdec->ident));
				gen->Print_Expr(intdec->expr);
				gen->depth--;
				gen->Print("}");
			}
		};

		Stmt_Visitor visitor = { .gen = this };
		std::visit(visitor, stmt->var);
	}
public:
	void Print(const Stmt_Program& AST_root) {
		std::cout << "[GENERATOR] Printing out Abstract Syntax Tree..." << std::endl << std::endl;

		Print("Stmt : Program {");
		depth++;
		for (const Stmt* stmt : AST_root.body) {
			Print_Stmt(stmt);
			Print("");
		}
		depth--;
		Print("}");

		std::cout << std::endl << "[GENERATOR] AST successfully printed" << std::endl;
	}
public:
	void Print(const std::string& str, bool lineBreak = true) const {
		Depth();
		std::cout << str;

		if (lineBreak) std::cout << std::endl;
	}
	void Depth() const {
		for (int i = 0; i < depth; i++) {
			std::cout << "|  ";
		}
	}
	int depth = 0;
};