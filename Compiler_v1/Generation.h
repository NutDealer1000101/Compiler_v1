#pragma once

#include "Parser.h"

class Generator {
private:
	void gen_Ident(Token t) {
		Print("Identifier: ", false);
		std::cout << t.lit << std::endl;
	}

	void gen_IntLit(Token t) {
		Print("Integer Literal: ", false);
		std::cout << t.lit << std::endl;
	}


	void gen_Expr(Expr* expr) {
		if (std::holds_alternative<Expr_Ident*>(expr->var)) {
			gen_Ident(std::get<Expr_Ident*>(expr->var)->ident);
		} 
		else if (std::holds_alternative<Expr_IntLit*>(expr->var)) {
			gen_IntLit(std::get<Expr_IntLit*>(expr->var)->intlit);
		} 
		else {
			std::cerr << "[GENERATOR] Error: Bad variant or sum shit" << std::endl;
			exit(EXIT_FAILURE);
		}
	}



	void gen_Stmt_Return(Stmt_Return* stmt) {
		Print("(Statement) Return {");
		depth++;
		gen_Expr(stmt->expr);
		depth--;
		Print("}");
	}
	void gen_Stmt_IntDec(Stmt_IntDec* stmt) {
		Print("(Statement) Integer Declaration {");
		depth++;
		gen_Ident(stmt->ident);
		gen_Expr(stmt->expr);
		depth--;
		Print("}");
	}
	void gen_Stmt(Stmt* stmt) {
		if (std::holds_alternative<Stmt_Return*>(stmt->var)) {
			gen_Stmt_Return(std::get<Stmt_Return*>(stmt->var));
		}
		else if (std::holds_alternative<Stmt_IntDec*>(stmt->var)) {
			gen_Stmt_IntDec(std::get<Stmt_IntDec*>(stmt->var));
		}
		else {
			std::cerr << "[GENERATOR] Error: Bad variant or sum shit" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
public:
	void Gen(const Stmt_Program& AST_root) {
		std::cout << "[GENERATOR] Printing out Abstract Syntax Tree..." << std::endl << std::endl;
		std::cout << "Program {" << std::endl;
		depth++;
		for (Stmt* stmt : AST_root.body) {
			gen_Stmt(stmt);
			std::cout << std::endl;
		}
		depth--;
		std::cout << "}" << std::endl;
		std::cout << std::endl << "[GENERATOR] AST successfully printed" << std::endl;
	}
private:
	void Print(const std::string& str, bool lineBreak = true) const {
		Depth();
		std::cout << str;

		if (lineBreak) std::cout << std::endl;
	}
	void Depth() const {
		for (int i = 0; i < depth; i++) {
			std::cout << "   ";
		}
	}
	int depth = 0;
};