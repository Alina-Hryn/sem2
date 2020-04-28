#include <iostream>
#include <fstream>
#include <map>
using namespace std;

/* INPUT_LIN_TAM: max number of symbols in line
 * MAX_TABLE_LEN: max keyword length/size
 */
#define INPUT_LIN_TAM 1000
#define MAX_TABLE_LEN 20


map <string, char*> tokens;

//keywords and categories
//ДОБАВИТЬ BITWISE OPERATORS : |, & , ^
char ruby_operators[][MAX_TABLE_LEN] = { "..", "||", "|", "^", "&&", "&", "<=>", "==",
"===", ">", ">=", "<", "<=", "<<", ">>", "+@", "-@", "[]", "+", "-", "**", "*", "/", "%",
"[]=", "\0" };
char ruby_op_assign[][MAX_TABLE_LEN] = { "+=", "-=", "*=", "/=", "%=", "**=",
"&=", "|=", "^=", "<<=", ">>=", "&&=", "||=", "=", "\0" };
char ruby_stmt_op[][MAX_TABLE_LEN] = { "alias", "{", "}", "defined?", "do", "then", "if", "elsif", "else", "unless", "case",
"when", "until", "ensure", "return", "yield", "begin", "end", "def", "undef", "\0" };
char ruby_method_op[][MAX_TABLE_LEN] = { ".", "::", "super", "\0" };
char ruby_func_op[][MAX_TABLE_LEN] = { "(", ")", "\0", };
char ruby_term_stmt_op[][MAX_TABLE_LEN] = { ";", "\n", "\0" };
char ruby_global_id[][MAX_TABLE_LEN] = { "$",  "\0" };
char ruby_op_id[][MAX_TABLE_LEN] = { "!", "?", "\0" };
char ruby_var_id[][MAX_TABLE_LEN] = { "@@", "@", "\0" };
char ruby_logic_op[][MAX_TABLE_LEN] = { "and", "not", "or" "\0" };
char ruby_bool_lit[][MAX_TABLE_LEN] = { "true", "false", "nil", "\0" };
char ruby_loop_op[][MAX_TABLE_LEN] = { "for" "while", "\0" };
char ruby_sep_op[][MAX_TABLE_LEN] = { ",", "\0" };
char ruby_comment_begin[][MAX_TABLE_LEN] = { "=begin", "\0" };
char ruby_comment_end[][MAX_TABLE_LEN] = { "=end", "\0" };
char ruby_print_op[][MAX_TABLE_LEN] = { "puts", "print", "\0" };
char ruby_class_op[][MAX_TABLE_LEN] = { "class", "\0" };
char ruby_list_op[][MAX_TABLE_LEN] = { "[", "]", "\0" };

// global vars to check if comments/strings are closed correct
bool global_comment_open = false;
bool global_string_open = false;
bool open_for_variable = false;

/*TOKENS NAMES :
->Array that contains the names of the tokens
->Each token name corresponds to one of the categories listed in the arrays
 stated above
*/
char ruby_table[][MAX_TABLE_LEN] = { "UNKNOWN", "OPERATOR", "SUM_OP",
"SUB_OP", "MULT_OP", "DIV_OP", "MOD_OP", "EXP_OP", "ASSIGN_OPERATOR",
"KEYWORD", "BLOCK_OP", "COND_OP", "COMMAND_OP", "METHOD_OP", "FUNCTION_OP",
"END_STATEMENT_OP", "GLOBAL_ID", "OPERATION_ID", "VAR_ID", "LOGIC_OP", "BOOL_OP",
"LOOP_OP", "SEP_OP", "STRING_LIT", "INT_LIT", "FLOAT_LIT", "PRINT_OP", "CLASS_OP", "LIST_ASSOC_OP", "IDENTIFIER", "COMMENT", "PLUS OR MINUS" };


// declaration of methods
void flush_input_lin(char* input_lin, int tam);
int check_if_keyword(char* lex, char list[][MAX_TABLE_LEN]);
int check_if_id(char* lex);
void check_token(char* lex, int* tok, int* len);
void print_lexem(char* lex, int tam);
void print_token(int tok);


int main() {
	int lin_num = 1; // counter of strings
	int char_itr = 0; //used to cycle through characters 
	int tmp_itr = 0; //same func as char_itr
	int t_tok, t_len; //id of token and lexem length(size)
	char* input_lin = (char*)malloc(INPUT_LIN_TAM * sizeof(char)); // buffer variable
	//maintains the line currently being analyzed

   /*
	ofstream MyFile("input.txt");
	MyFile << "Files can be tricky, but it is fun enough!";

	MyFile.close();

	*/
	ofstream outputFile("output.txt");
	ifstream inputFile("input.txt");
	// while (std::cin.getline(input_lin, INPUT_LIN_TAM)) { //reads input line by line
	while (inputFile) {
		inputFile.getline(input_lin, 255);
		// skip empty line
		if (input_lin[0] == '\n' || input_lin[0] == '\0') {
			lin_num++;
			continue;
		}
		// continue if is not empty
		char_itr = 0; // resets char_itr
		std::cout << ">> lin[" << lin_num++ << "]: " << input_lin << std::endl; // print line
		//while it is not the end of line
		while (input_lin[char_itr] != '\n' && input_lin[char_itr] != '\0') {
			t_tok = 0; //token is not known
			t_len = 0; // lexem size = 0

			check_token(&input_lin[char_itr], &t_tok, &t_len); //checks current lexem is token

			// if multi-line comment or string is open doesn`t analyze
			if (global_comment_open || global_string_open) {
				char_itr++;
				break;
			}
			/*t_tok <0 was agreed as a universal identifier that that line should be ignored*/
			if (t_tok < 0) {
				break;
			}
			// otherwise print lexem - token
			else {
				print_lexem(&input_lin[char_itr], t_len);
				std::cout << " -> ";
				print_token(t_tok);
				std::cout << std::endl;
				for (int i = char_itr; i < t_len + char_itr; i++)
					outputFile << input_lin[i];
				outputFile << " -> " << ruby_table[t_tok] << endl;
				/*
				the check_tok function changes the value of t_tok and t_len in its parameters.
				That's because the lexical analyzer analyzes char by char.
				In case the lexeme token has already been identified, the returned value of t_len
				already informs the value that must increment char_itr,
				otherwise the algorithm would analyze the same lexeme several times,
				once for each letter:
				*/
				char_itr += t_len;
			}
		}
		// clear input_line buffer
		flush_input_lin(&input_lin[0], char_itr);
		std::cout << std::endl;
	}
	outputFile.close();
	inputFile.close();
	/* aqui, ao chegar no final do programa, checa se a var. global_comment_open
	*  foi fechada, senão, isso indica que um comentário não foi corretamente finalizado:
	*/
	// checks if comment is ended 
	// if not => print that comment has wrong end
	if (global_comment_open) {
		std::cout << "PLEASE END COMMENT" << std::endl;
	}
	// checks if strings were closed
	if (global_string_open) {
		std::cout << "PLEASE CLOSE STRINGS" << std::endl;
	}
	system("pause");
	return 0;
}

bool check_if_continue_id(char symbol) {
	return (symbol == '_' || (symbol >= '0' && symbol <= '9')) || (symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z');
}

int skip_spaces_after_word(char* lex, int i) {
	while (lex[i] == ' ') i++;
	return i;
}

// clears the buffer in the input_lin
void flush_input_lin(char* input_lin, int tam) {
	for (int i = 0; i < tam; i++) {
		input_lin[i] = '\n';
	}
}


//checks if lexem is keyword in the list [][MAX_TABLE_LEN]
int check_if_keyword(char* lex, char list[][MAX_TABLE_LEN]) {
	int list_i = 0;
	bool found;
	int i;
	while (list[list_i][0] != '\0') {
		i = 0;
		found = true;
		while (list[list_i][i] != '\0') {
			if (list[list_i][i] != lex[i]) {
				found = false;
				break;
			}
			i++;
		}
		if ((list[list_i][i] == '\0' && found) && !check_if_continue_id(lex[i])) {
			return skip_spaces_after_word(lex, i);
		}
		list_i++;
	}
	return 0;
}


//checks if lexem is comment
int check_if_comment(char* lex) {
	if (lex[0] == '#') return 1; // comment in one line
   // checks if it is multi-line comment (begins from =begin)
	else if (check_if_keyword(lex, ruby_comment_begin) > 0) {
		global_comment_open = true; // change global var to show that comment is open
		return 1; //=>shows that it is comment
	}
	// checks if comment ends =end and close comment
	else if (check_if_keyword(lex, ruby_comment_end) > 0) {
		global_comment_open = false; // change to show it is closed
		return 1;
	}
	return 0;
}


//checks if lexem is identifier
// /[a-zA-Z_]{a-zA-Z0-9_}/
int check_if_id(char* lex) {
	int i = 0;
	if (lex[i] < 'A' || (lex[i] > 'Z' && lex[i] != '_' && lex[i] < 'a') || lex[i] > 'z')
		return -1;
	i++;
	while ((lex[i] >= '0' && lex[i] <= '9') || (lex[i] >= 'A' && lex[i] <= 'Z') || lex[i] == '_' || (lex[i] >= 'a' && lex[i] <= 'z'))
		i++;
	return skip_spaces_after_word(lex, i);
}

//checks if lexem is integer
int check_if_int(char* lex) {
	int i = 0;
	if (lex[0] == '-') {
		if (lex[1] < '0' || lex[1] > '9') return 0;
		else i++;
	}
	while (lex[i] >= '0' && lex[i] <= '9') {
		i++;
	}
	if (lex[i] == '.' && (lex[i + 1] >= 'a' && lex[i + 1] <= 'z'))
		return skip_spaces_after_word(lex, i);
	return skip_spaces_after_word(lex, i);
}

bool is_figit(char i) {
	return (i >= '0' && i <= '9');
}
// checks if lexem is float
int check_if_float(char* lex) {
	int i = 0;
	if (lex[i] == '-') {
		i++;
		if (lex[i++] < '0' || lex[1] > '9') return 0;
	}
	while (lex[i] >= '0' && lex[i] <= '9') i++;
	if ((lex[i] == '.' && i > 0) && (lex[i + 1] >= '0' && lex[i + 1] <= '9')) {
		i++;
		while (lex[i] >= '0' && lex[i] <= '9') i++;
	}
	else return 0;
	return skip_spaces_after_word(lex, i);
}

int check_if_operator(char* lex) {
	int i = 0;
	switch (lex[i])
	{
	case '+':
	case '-':
		i++;
		if (lex[i] == '@' || lex[i] == '=')
			i++;
		return skip_spaces_after_word(lex, i++);
		break;
	default:
		return 0;
	}
}

// checks if lexem is string
int check_if_string(char* lexem) {
	int i = 0;

	if (lexem[i] == '"' || (lexem[i] == '}' && open_for_variable)) {
		global_string_open = true;
		if (lexem[i] == '}') open_for_variable = false;
		i++;
		while ((lexem[i] != '"' && lexem[i] != '\n') && (lexem[i] != '#' && lexem[i + 1] != '{')) i++;
		if (lexem[i] == '#' && lexem[i + 1] == '{') {
			i += 2;
			open_for_variable = true;
			global_string_open = false;
			return skip_spaces_after_word(lexem, i);
		}
		if (lexem[i] == '"') global_string_open = false;
	}
	else if (lexem[i] == '\'') {
		i++;
		while (lexem[i] != '\'' && lexem[i] != '\n') i++;
		if (lexem[i] == '\'') global_string_open = false;
	}
	else if (lexem[i] == '`') {
		i++;
		while (lexem[i] != '`' && lexem[i] != '\n') i++;
		if (lexem[i] == '`') global_string_open = false;
	}
	else if (lexem[i] == '`') {
		i++;
		while (lexem[i] != '`' && lexem[i] != '\n') i++;
		if (lexem[i] == '`') global_string_open = false;
	}
	if (i > 0) i++;
	return skip_spaces_after_word(lexem, i);
}

void check_token(char* lex, int* tok, int* len) {
	*len = check_if_operator(lex);
	if (*len > 0) {
		*tok = 31;
		return;
	}
	*len = check_if_comment(lex);
	if (*len > 0) {
		*tok = -1;
		return;
	}
	*len = check_if_string(lex);
	if (*len > 0) {
		*tok = 23;
		return;
	}
	*len = check_if_keyword(lex, ruby_operators);
	if (*len > 0) {
		*tok = 1;
		return;
	}
	*len = check_if_keyword(lex, ruby_func_op);
	if (*len > 0) {
		*tok = 14;
		return;
	}
	*len = check_if_keyword(lex, ruby_stmt_op);
	if (*len > 0) {
		*tok = 9;
		return;
	}
	*len = check_if_float(lex);
	if (*len > 0) {
		*tok = 25;
		return;
	}
	*len = check_if_int(lex);
	if (*len > 0) {
		*tok = 24;
		return;
	}


	*len = check_if_keyword(lex, ruby_op_assign);
	if (*len > 0) {
		*tok = 8;
		return;
	}
	*len = check_if_keyword(lex, ruby_method_op);
	if (*len > 0) {
		*tok = 13;
		return;
	}
	*len = check_if_keyword(lex, ruby_term_stmt_op);
	if (*len > 0) {
		*tok = 15;
		return;
	}
	*len = check_if_keyword(lex, ruby_global_id);
	if (*len > 0) {
		*tok = 16;
		return;
	}
	*len = check_if_keyword(lex, ruby_op_id);
	if (*len > 0) {
		*tok = 17;
		return;
	}
	*len = check_if_keyword(lex, ruby_var_id);
	if (*len > 0) {
		*tok = 18;
		return;
	}
	*len = check_if_keyword(lex, ruby_logic_op);
	if (*len > 0) {
		*tok = 19;
		return;
	}
	*len = check_if_keyword(lex, ruby_bool_lit);
	if (*len > 0) {
		*tok = 20;
		return;
	}
	*len = check_if_keyword(lex, ruby_loop_op);
	if (*len > 0) {
		*tok = 21;
		return;
	}
	*len = check_if_keyword(lex, ruby_sep_op);
	if (*len > 0) {
		*tok = 22;
		return;
	}
	*len = check_if_keyword(lex, ruby_print_op);
	if (*len > 0) {
		*tok = 26;
		return;
	}
	*len = check_if_keyword(lex, ruby_class_op);
	if (*len > 0) {
		*tok = 27;
		return;
	}
	*len = check_if_keyword(lex, ruby_list_op);
	if (*len > 0) {
		*tok = 28;
		return;
	}
	*len = check_if_id(lex);
	if (*len > 0) {
		*tok = 29;
		return;
	}
	*tok = 0;
	*len = 1;
	return;
}


void print_lexem(char* lex, int tam) {
	for (int i = 0; i < tam; i++) {
		std::cout << lex[i];
	}
}


void print_token(int tok) {
	std::cout << ruby_table[tok];
}