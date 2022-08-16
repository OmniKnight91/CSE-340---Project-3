

using namespace std;
#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>
#include <stdlib.h>

#include "compiler.h"


#define ALLOC(t) (t*) calloc(1, sizeof(t))


struct var 
{
	struct ValueNode * itm;
	struct var * nxt;

}*list;


void error(string part, int line, int c);


struct StatementNode* parse_body();
struct var* id_list();
struct StatementNode* parse_assign();
struct StatementNode* parse_print();
struct StatementNode* parse_while();
struct StatementNode* parse_if();
struct StatementNode* parse_case(struct ValueNode* val, struct StatementNode* after);
struct StatementNode* case_list(struct ValueNode* val, struct StatementNode* after);
struct StatementNode* parse_switch(struct StatementNode* after);
struct StatementNode* stmt_list();
struct StatementNode* parse_body();
struct StatementNode* parse_generate_intermediate_representation();
//struct StatementNode* parse_for();
