

#include "lexer.h"
#include "Project4.h"
#include <cstdio>
#include <cstdlib>
#define MAX_VARS 100

int cnt = 0;
Token t;
LexicalAnalyzer lexer;

struct StatementNode* parse_body();
struct ValueNode* table[MAX_VARS];




void error(string prt, int lin, int c) 
{
	cout << "Error occuring at " << prt << ", line " << lin << " with char " << c << "\n";
	exit(0);
}

struct var* id_list() 
{
	t = lexer.GetToken();
	struct var* temp = new var;
	if (t.token_type == SEMICOLON) {
		return temp;
	}
	else if (t.token_type == ID) {
		struct ValueNode* hold = new ValueNode;
		hold->name = t.lexeme;
		hold->value = 0;
		temp->itm = hold;
		temp->nxt = NULL;

	}
	else { error("idList expecting ID or ;", t.line_no, t.token_type); }

	t = lexer.GetToken();
	if (t.token_type == COMMA) {
		temp->nxt = id_list();
	}
	else if (t.token_type != SEMICOLON) { error("expecting idList  , or ;", t.line_no, t.token_type); }

	return temp;
}

struct StatementNode* parse_assign() 
{
	struct StatementNode* stmt = new StatementNode;
	struct AssignmentStatement* hld = new AssignmentStatement;
	bool check = false;
	stmt->type = ASSIGN_STMT;
	t = lexer.GetToken();


	if (t.token_type == ID) 
	{
		struct var* cycler = list;
		while (cycler) {
			if (t.lexeme == cycler->itm->name)
			{
				hld->left_hand_side = cycler->itm;
				check = true;
			}
			cycler = cycler->nxt;
		}

		if (!check) 
		{
			struct var* add = list;
			struct var* newVar = new var;
			struct ValueNode* newitem = new ValueNode;
			newitem->name = t.lexeme;
			newitem->value = 0;

			newVar->itm = newitem;
			newVar->nxt = NULL;
			while (add->nxt) 
			{
				add = add->nxt;
			}
			add->nxt = newVar;
			hld->left_hand_side = newitem;
		}
		//cout << t.lexeme << " checking\n";
		t = lexer.GetToken();
		if (t.token_type == EQUAL) {
			check = false;
			t = lexer.GetToken();
			if (t.token_type == ID) {
				cycler = list;
				while (cycler) {
					if (cycler->itm->name == t.lexeme) {
						hld->operand1 = cycler->itm;
					}
					cycler = cycler->nxt;
				}
			}
			else if (t.token_type == NUM) {
				struct ValueNode* tempNum = new ValueNode;
				tempNum->value = atoi(t.lexeme.c_str());
				tempNum->name = "";
				hld->operand1 = tempNum;

			}
			else { error("assign_stmt expecting NUM and ID", t.line_no, t.token_type); }

			t = lexer.GetToken();

			if (t.token_type == MULT) 
			{
				hld->op = OPERATOR_MULT;
			}

			else if (t.token_type == DIV) 
			{
				hld->op = OPERATOR_DIV;
			}

			else if (t.token_type == PLUS) 
			{
				hld->op = OPERATOR_PLUS;
			}

			else if (t.token_type == MINUS) 
			{
				hld->op = OPERATOR_MINUS;
			}
			else if (t.token_type == SEMICOLON) {
				hld->op = OPERATOR_NONE;
				stmt->assign_stmt = hld;
				return stmt;
			}
			else { error("assign_stmt expexting exp or ;", t.line_no, t.token_type); }
		}
		else 
		{ 
			error("assign_stmt expecting =", t.line_no, t.token_type); 
		}

		t = lexer.GetToken();
		if (t.token_type == ID) {
			cycler = list;
			while (cycler) {
				if (cycler->itm->name == t.lexeme) {
					hld->operand2 = cycler->itm;
				}
				cycler = cycler->nxt;
			}
		}
		else if (t.token_type == NUM) 
		{
			struct ValueNode* tempNum = new ValueNode;
			tempNum->value = atoi(t.lexeme.c_str());
			tempNum->name = "";
			hld->operand2 = tempNum;
		}

		else
		{
			error("assign_stmt expecting ID or NUM", t.line_no, t.token_type); 
		}

		stmt->assign_stmt = hld;
	}
	else 
	{ 
		error("assign_stmt expecting an ID", t.line_no, t.token_type); 
	}

	t = lexer.GetToken();

	if (t.token_type != SEMICOLON) 
	{ 
		error(" expecting assign_stmt  ;", t.line_no, t.token_type); 
	}

	return stmt;
}


struct StatementNode* parse_body() 
{
	struct StatementNode* stmt = new StatementNode;
	t = lexer.GetToken();
	if (t.token_type == LBRACE) {
		stmt = stmt_list();
	}
	else { error("stmt_list expecting {", t.line_no, t.token_type); }

	t = lexer.GetToken();
	if (t.token_type != RBRACE) { error("stmt_list expecting }", t.line_no, t.token_type); }

	return stmt;
}


struct StatementNode* parse_print() 
{
	struct StatementNode* stmt = new StatementNode;
	stmt->type = PRINT_STMT;
	struct PrintStatement* hld = new PrintStatement;
	t = lexer.GetToken();

	if (t.token_type == PRINT) 
	{
		t = lexer.GetToken();

		if (t.token_type == ID) 
		{
			struct var* cycler = list;
			while (cycler) {
				if (cycler->itm->name == t.lexeme) 
				{
					hld->id = cycler->itm;
				}

				cycler = cycler->nxt;
			}
		}
		else 
		{ 
			error("parse_print expecting ID", t.line_no, t.token_type); 
		}
	}

	else 
	{ 
		error("parse_print is  expecting a PRINT", t.line_no, t.token_type); 
	}

	t = lexer.GetToken();

	if (t.token_type != SEMICOLON) 
	{ 
		error("parse_print is expecting a SEMICOLON", t.line_no, t.token_type); 
	}
	stmt->print_stmt = hld;
	return stmt;
}


struct StatementNode* parse_while() 
{
	struct StatementNode* stmt = new StatementNode;
	struct IfStatement* hld = new IfStatement;
	struct var* cycler;

	t = lexer.GetToken();
	stmt->type = IF_STMT;

	if (t.token_type == WHILE) {
		t = lexer.GetToken();
		if (t.token_type == ID) {
			cycler = list;
			while (cycler) {
				if (cycler->itm->name == t.lexeme) {
					hld->condition_operand1 = cycler->itm;
				}
				cycler = cycler->nxt;
			}
		}
		else if (t.token_type == NUM) {
			struct ValueNode* tempNum = new ValueNode;
			tempNum->value = atoi(t.lexeme.c_str());
			tempNum->name = "";
			hld->condition_operand1 = tempNum;
		}
		else 
		{ 
			error("parse_while checking ID or NUM", t.line_no, t.token_type); 
		}

		t = lexer.GetToken();

		if (t.token_type == LESS)
		{
			hld->condition_op = CONDITION_LESS;
		}

		else if (t.token_type == GREATER) 
		{
			hld->condition_op = CONDITION_GREATER;
		}

		else if (t.token_type == NOTEQUAL) 
		{
			hld->condition_op = CONDITION_NOTEQUAL;
		}
		else { error("parse_while checking condition", t.line_no, t.token_type); }

		t = lexer.GetToken();

		if (t.token_type == ID) 
		{
			cycler = list;

			while (cycler) 
			{
				if (cycler->itm->name == t.lexeme) 
				{
					hld->condition_operand2 = cycler->itm;
				}
				cycler = cycler->nxt;
			}
		}

		else if (t.token_type == NUM)
		{
			struct ValueNode* tempNum = new ValueNode;
			tempNum->value = atoi(t.lexeme.c_str());
			tempNum->name = "";
			hld->condition_operand2 = tempNum;
		}
		else 
		{ 
			error("parse_while checking 2nd ID or NUM", t.line_no, t.token_type); 
		}

	}
	else 
	{ 
		error("parse_while checking WHILE", t.line_no, t.token_type);
	}

	hld->true_branch = parse_body();

	struct StatementNode* add = new StatementNode;
	struct GotoStatement* go = new GotoStatement;

	stmt->if_stmt = hld;
	go->target = stmt;
	add->type = GOTO_STMT;
	add->goto_stmt = go;

	struct StatementNode* cycstmt = hld->true_branch;
	while (cycstmt->next) {
		cycstmt = cycstmt->next;
	}
	cycstmt->next = add;
	return stmt;
}


struct StatementNode* parse_if()
{
	struct StatementNode* stmtNode = new StatementNode;
	struct IfStatement* hld = new IfStatement;
	struct var* cycler;

	t = lexer.GetToken();
	stmtNode->type = IF_STMT;

	if (t.token_type == IF) 
	{
		t = lexer.GetToken();
		if (t.token_type == ID)
		{
			cycler = list;
			while (cycler) 
			{
				if (cycler->itm->name == t.lexeme) 
				{
					hld->condition_operand1 = cycler->itm;
				}
				cycler = cycler->nxt;
			}
		}

		else if (t.token_type == NUM) {
			struct ValueNode* tempNum = new ValueNode;
			tempNum->value = atoi(t.lexeme.c_str());
			tempNum->name = "";
			hld->condition_operand1 = tempNum;
		}

		else 
		{ 
			error("parse_if checking ID or NUM", t.line_no, t.token_type);
		}

		t = lexer.GetToken();
		if (t.token_type == LESS) {
			hld->condition_op = CONDITION_LESS;
		}

		else if (t.token_type == GREATER) {
			hld->condition_op = CONDITION_GREATER;
		}

		else if (t.token_type == NOTEQUAL) {
			hld->condition_op = CONDITION_NOTEQUAL;
		}

		else { error("parse_if checking condition", t.line_no, t.token_type); }

		t = lexer.GetToken();

		if (t.token_type == ID) {
			cycler = list;
			while (cycler) {
				if (cycler->itm->name == t.lexeme) {
					hld->condition_operand2 = cycler->itm;
				}
				cycler = cycler->nxt;
			}
		}

		else if (t.token_type == NUM)
		{
			struct ValueNode* tempNum = new ValueNode;
			tempNum->value = atoi(t.lexeme.c_str());
			tempNum->name = "";
			hld->condition_operand2 = tempNum;
		}

		else 
		{ 
			error("parse_if checking 2nd ID or NUM", t.line_no, t.token_type);
		}

	}
	else 
	{
		error("parse_if checking IF", t.line_no, t.token_type); 
	}
	hld->true_branch = parse_body();
	stmtNode->if_stmt = hld;
	return stmtNode;
}


struct StatementNode* parse_case(struct ValueNode* val, struct StatementNode* after) 
{
	struct StatementNode* stmtNode = new StatementNode;
	struct IfStatement* tempNode = new IfStatement;
	struct ValueNode* numNode = new ValueNode;
	struct StatementNode* cycler;

	t = lexer.GetToken();

	if (t.token_type != CASE) 
	{ 
		error("parse_case expecting CASE", t.line_no, t.token_type);
	}

	t = lexer.GetToken();

	if (t.token_type != NUM) 
	{ 
		error("parse_case expecting NUM", t.line_no, t.token_type);
	}

	numNode->value = atoi(t.lexeme.c_str());
	numNode->name = "";
	tempNode->condition_operand1 = val;
	tempNode->condition_operand2 = numNode;
	tempNode->condition_op = CONDITION_NOTEQUAL;

	t = lexer.GetToken();

	if (t.token_type != COLON) 
	{ 
		error("parse_case expecting :", t.line_no, t.token_type); 
	}

	tempNode->false_branch = parse_body();

	cycler = tempNode->false_branch;

	//int i = 0;
	while (cycler->next) 
	{
		//cout << i++ << " check " << temp->condition_operand2->value << "\n";
		cycler = cycler->next;
	}

	cycler->next = after;
	//if(after->next) { cout << "check\n";}
	stmtNode->type = IF_STMT;
	stmtNode->if_stmt = tempNode;
	stmtNode->next = after;

	return stmtNode;
}

struct StatementNode* case_list(struct ValueNode* val, struct StatementNode* after) 
{
	struct StatementNode* stmtNode = new StatementNode;
	stmtNode->type = IF_STMT;
	t = lexer.GetToken();

	if (t.token_type == CASE)
	{
		lexer.UngetToken(t);
		stmtNode = parse_case(val, after);

		stmtNode->if_stmt->true_branch = case_list(val, after);
	}
	else if (t.token_type == DEFAULT) 
	{
		t = lexer.GetToken();
		if (t.token_type != COLON)
		{
			error("case_list expecting :", t.line_no, t.token_type);
		}

		stmtNode = parse_body();
		struct StatementNode* cycle = stmtNode;

		while (cycle->next) 
		{
			cycle = cycle->next;
		}

		cycle->next = after;
	}

	else 
	{
		lexer.UngetToken(t);
		stmtNode = after;
	}

	return stmtNode;
}


struct StatementNode* parse_switch(struct StatementNode* after) 
{
	struct StatementNode* stmtNode = new StatementNode;
	struct var* cycler = new var;
	struct ValueNode* hld;

	t = lexer.GetToken();

	if (t.token_type == SWITCH) 
	{
		t = lexer.GetToken();

		if (t.token_type == ID) 
		{
			cycler = list;
			while (cycler)
			{
				if (t.lexeme == cycler->itm->name) 
				{
					hld = cycler->itm;
				}
				cycler = cycler->nxt;
			}
		}
		t = lexer.GetToken();
		if (t.token_type == LBRACE) {
			stmtNode = case_list(hld, after);
		}
		else 
		{ 
			error("checking parse_switch  {", t.line_no, t.token_type); 
		}

		t = lexer.GetToken();

		if (t.token_type != RBRACE)
		{
			error("checking parse_switch  }", t.line_no, t.token_type);
		}
	}

	return stmtNode;
}

struct StatementNode* stmt_list()
{
	struct StatementNode* stmtNode = new StatementNode;
	t = lexer.GetToken();

	if (t.token_type == ID)
	{
		lexer.UngetToken(t);
		stmtNode = parse_assign();

		t = lexer.GetToken();

		if (t.token_type == ID || t.token_type == IF || t.token_type == SWITCH || t.token_type == WHILE || t.token_type == PRINT) 
		{
			lexer.UngetToken(t);
			stmtNode->next = stmt_list();
			return stmtNode;
		}

		else if (t.token_type == RBRACE)
		{
			lexer.UngetToken(t);
			return stmtNode;
		}

		else 
		{
			error("Stmt_list checking next", t.line_no, t.token_type);
		}
	}

	else if (t.token_type == IF) {
		lexer.UngetToken(t);
		stmtNode = parse_if();
		struct StatementNode* no_OP = new StatementNode;
		no_OP->type = NOOP_STMT;
		struct StatementNode* appendtrue = stmtNode->if_stmt->true_branch;

		while (appendtrue->next) 
		{
			appendtrue = appendtrue->next;
		}

		appendtrue->next = no_OP;
		stmtNode->if_stmt->false_branch = no_OP;
		stmtNode->next = no_OP;

		t = lexer.GetToken();
		if (t.token_type == ID || t.token_type == IF || t.token_type == SWITCH || t.token_type == WHILE || t.token_type == PRINT)
		{
			lexer.UngetToken(t);
			stmtNode->next->next = stmt_list();
			return stmtNode;
		}

		else if (t.token_type == RBRACE) 
		{
			lexer.UngetToken(t);
			return stmtNode;
		}

		else 
		{ 
			error("Stmt_list checking next", t.line_no, t.token_type); 
		}
	}
	else if (t.token_type == SWITCH) 
	{
		struct StatementNode* after = new StatementNode;
		struct GotoStatement* go = new GotoStatement;
		after->type = GOTO_STMT;
		after->goto_stmt = go;

		lexer.UngetToken(t);
		stmtNode = parse_switch(after);
		t = lexer.GetToken();

		if (t.token_type == ID || t.token_type == IF || t.token_type == SWITCH || t.token_type == WHILE || t.token_type == PRINT)
		{
			lexer.UngetToken(t);
			go->target = stmt_list();
			//if (after){ cout << "check\n";}
			stmtNode->next = after;
			return stmtNode;
		}

		else if (t.token_type == RBRACE)
		{
			lexer.UngetToken(t);
			return stmtNode;
		}

		else
		{ 
			error("checking next Stmt_list ", t.line_no, t.token_type); 
		}
	}

	else if (t.token_type == PRINT) {
		lexer.UngetToken(t);
		stmtNode = parse_print();
		t = lexer.GetToken();

		if (t.token_type == ID || t.token_type == IF || t.token_type == SWITCH || t.token_type == WHILE || t.token_type == PRINT) 
		{
			lexer.UngetToken(t);
			stmtNode->next = stmt_list();
			return stmtNode;
		}
		else if (t.token_type == RBRACE)
		{
			lexer.UngetToken(t);
			return stmtNode;
		}

		else 
		{ 
			error("checking next Stmt_list ", t.line_no, t.token_type);
		}
	}

	else if (t.token_type == WHILE)
	{
		lexer.UngetToken(t);
		stmtNode = parse_while();
		struct StatementNode* no_OP = new StatementNode;
		no_OP->type = NOOP_STMT;

		struct StatementNode* appendtrue = stmtNode->if_stmt->true_branch;

		while (appendtrue->next)
		{
			appendtrue = appendtrue->next;
		}

		appendtrue->next = no_OP;
		stmtNode->if_stmt->false_branch = no_OP;
		stmtNode->next = no_OP;

		t = lexer.GetToken();

		if (t.token_type == ID || t.token_type == IF || t.token_type == SWITCH || t.token_type == WHILE || t.token_type == PRINT) 
		{
			lexer.UngetToken(t);
			stmtNode->next->next = stmt_list();
			return stmtNode;
		}

		else if (t.token_type == RBRACE)
		{
			lexer.UngetToken(t);
			return stmtNode;
		}

		else 
		{ 
			error("Stmt_list checking for statement", t.line_no, t.token_type); 
		}

	}
}




struct StatementNode* parse_generate_intermediate_representation()
{
	struct StatementNode* program = new StatementNode;
	list = new var;

	t = lexer.GetToken();
	if (t.token_type == ID) {
		lexer.UngetToken(t);
		list = id_list();
	}

	else 
	{ 
		lexer.UngetToken(t);
	}

	t = lexer.GetToken();

	if (t.token_type == LBRACE)
	{
		lexer.UngetToken(t);
		program = parse_body();
	}

	else
	{
		error("expecting PGIR {, ", t.line_no, t.token_type);
	}
	struct StatementNode* chk = program;
	return program;
}