#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;

/* INPUT_LIN_TAM: max number of symbols in line
 * MAX_TABLE_LEN: max keyword length/size
 */
#define INPUT_LIN_TAM 1000
#define MAX_TABLE_LEN 20

char keywords[][MAX_TABLE_LEN] = { "alias", "{", "}", "defined?", "do", "then", "if", "elsif", "else", "unless", "case",
"when", "until", "ensure", "return", "yield", "begin", "end", "def", "undef","and", "not", "or", "true", "false", "nil","for" "while", "\0" };
char ruby_method_op[][MAX_TABLE_LEN] = { ".", "::", "super", "\0" };
char ruby_func_op[][MAX_TABLE_LEN] = { "(", ")", "\0", };
char ruby_term_stmt_op[][MAX_TABLE_LEN] = { ";", "\n", "\0" };
char ruby_global_id[][MAX_TABLE_LEN] = { "$",  "\0" };
char ruby_op_id[][MAX_TABLE_LEN] = { "!", "?", "\0" };
char ruby_var_id[][MAX_TABLE_LEN] = { "@@", "@", "\0" };
char ruby_sep_op[][MAX_TABLE_LEN] = { ",", "\0" };
char ruby_comment_begin[][MAX_TABLE_LEN] = { "=begin", "\0" };
char ruby_comment_end[][MAX_TABLE_LEN] = { "=end", "\0" };
char ruby_print_op[][MAX_TABLE_LEN] = { "puts", "print", "\0" };
char ruby_class_op[][MAX_TABLE_LEN] = { "class", "\0" };
char ruby_list_op[][MAX_TABLE_LEN] = { "[", "]", "\0" };
char punctuation[][MAX_TABLE_LEN] = { "[", "]","(", ")", ",", ";",":", ".","\0" };

// global vars to check if comments/strings are closed correct
bool global_comment_open = false;
bool global_string_open = false;
bool open_for_variable = false;

/*TOKENS NAMES :
->Array that contains the names of the tokens
->Each token name corresponds to one of the categories listed in the arrays
 stated above
*/
char ruby_table[][MAX_TABLE_LEN] = { "UNKNOWN", "OPERATOR", "KEYWORD", "STRING LITERAL", "INTEGER LITERAL",
"FLOAT LITERAL", "IDENTIFIER", "COMMENT", "PUNCTUATION", "PRINT_OP" };
int color_table[] = { 12, 14, 9, 13, 9, 11, 7, 10, 7, 12 };
// declaration of methods
void flush_input_lin(char* input_lin, int tam);
int check_if_keyword(char* lex, char list[][MAX_TABLE_LEN]);
int check_if_id(char* lex);
void check_token(char* lex, int* tok, int* len);
void print_lexem(char* lex, int tam);
void print_token(int tok);
void read_keywords();

int main() {
	int lin_num = 1; // counter of strings
	int char_itr = 0; //used to cycle through characters 
	int tmp_itr = 0; //same func as char_itr
	int t_tok, t_len; //id of token and lexem length(size)
	char* input_lin = (char*)malloc(INPUT_LIN_TAM * sizeof(char));
	HANDLE hConsole;
	int k = 0;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ofstream outputFile("output.txt");
	ifstream inputFile("input.txt");
	while (inputFile) {
		inputFile.getline(input_lin, 255);
		char_itr = 0; // resets char_itr
		//SetConsoleTextAttribute(hConsole, k < 254? k++ : k = 0);
		//std::cout << lin_num++ << "   " << input_lin << std::endl; // print line
		SetConsoleTextAttribute(hConsole, 253);
		cout << (k < 9 ? "   " : (k < 99 ? "  ":" ")) << ++k << " ";
		SetConsoleTextAttribute(hConsole, 12);
		cout << " ";
		while (input_lin[char_itr] != '\n' && input_lin[char_itr] != '\0') {
			t_tok = 0; //token is not known
			t_len = 0; // lexem size = 0

			check_token(&input_lin[char_itr], &t_tok, &t_len); //checks current lexem is token

			/*t_tok <0 was agreed as a universal identifier that that line should be ignored*/
			if (t_tok < 0) {
				break;
			}
			else {
				//SetConsoleTextAttribute(hConsole, color_table[t_tok]);
				SetConsoleTextAttribute(hConsole, color_table[t_tok]);
				print_lexem(&input_lin[char_itr], t_len);
				//std::cout << " -> ";
				//print_token(t_tok);
				//std::cout << std::endl;
				outputFile << ruby_table[t_tok] << " -> ";
				for (int i = char_itr; i < t_len + char_itr; i++)
					outputFile << input_lin[i];
				outputFile << endl;
				char_itr += t_len;
			}
		}
		// clear input_line buffer
		flush_input_lin(&input_lin[0], char_itr);
		std::cout << std::endl;
	}
	outputFile.close();
	inputFile.close();
	if (global_comment_open) {
		std::cout << "PLEASE END COMMENT" << std::endl;
	}
	if (global_string_open) {
		std::cout << "PLEASE CLOSE STRINGS" << std::endl;
	}
	system("pause");
	return 0;
}


bool is_digit(char i) {
	return (i >= '0' && i <= '9');
}
bool is_letter(char i) {
	return (i >= 'a' && i <= 'z');
}
bool is_big_letter(char i) {
	return (i >= 'A' && i <= 'Z');
}

bool check_if_continue_id(char symbol) {
	return (symbol == '_' || is_digit(symbol)) || is_letter(symbol) || is_letter(symbol);
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

void read_keywords() {
	ifstream inputFile("input.txt");
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
	if (lex[0] == '#') return strlen(lex);
	else if (check_if_keyword(lex, ruby_comment_begin) > 0) {
		global_comment_open = true;
		return strlen(lex);
	}
	else if (check_if_keyword(lex, ruby_comment_end) > 0) {
		global_comment_open = false;
		return strlen(lex);
	}
	else if (global_comment_open) return strlen(lex);
	return 0;
}


//checks if lexem is identifier
// /[a-zA-Z_]{a-zA-Z0-9_}/
int check_if_id(char* lex) {
	int i = 0;
	if (!is_big_letter(lex[i]) && lex[i] != '_' && !is_letter(lex[i]))
		return -1;
	i++;
	while (is_digit(lex[i]) || is_big_letter(lex[i]) || lex[i] == '_' || is_letter(lex[i]))
		i++;
	return skip_spaces_after_word(lex, i);
}

//checks if lexem is integer
int check_if_int(char* lex) {
	int i = 0;
	if (lex[i] == '-') {
		if (!is_digit(lex[i + 1])) return 0;
		else i++;
	}
	while (is_digit(lex[i])) {
		i++;
	}
	if (lex[i] == '.' && !is_letter(lex[i + 1]))
		return skip_spaces_after_word(lex, i);
	return skip_spaces_after_word(lex, i);
}


// checks if lexem is float
int check_if_float(char* lex) {
	int i = 0;
	if (lex[i] == '-') {
		i++;
		if (!is_digit(lex[i + 1])) return 0;
	}
	while (is_digit(lex[i])) i++;
	if ((lex[i] == '.' && i > 0) && !is_letter(lex[i + 1])) {
		i++;
		while (is_digit(lex[i])) i++;
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
	case '|':
	case '&':
	case '<':
	case '>':
		i++;
		if (lex[i] == lex[i - 1]) {
			i++;
			if (lex[i] == '=') i++;
		}
		else if (lex[i] == '=') {
			i++;
			if (lex[i - 2] == '<' && lex[i] == '>')
				i++;
		}
		return skip_spaces_after_word(lex, i++);
		break;
	case '^':
		i++;
		return skip_spaces_after_word(lex, i++);
		break;
	case '=':
		i++;
		if (lex[i] == '=') {
			i++;
			if (lex[i] == '=') i++;
		}
		return skip_spaces_after_word(lex, i++);
		break;
	case '*':
	case '/':
	case '%':
		i++;
		if (lex[i] == '=') {
			i++;
		}
		else if (lex[i] == lex[i - 1] && lex[i] == '*') {
			i++;
			if (lex[i] == '=') i++;
		}
		return skip_spaces_after_word(lex, i++);
		break;
	case '[':
		if (lex[i + 1] == ']') {
			i += 2;
			if (lex[i] == '=') i++;
		}
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

	*len = check_if_comment(lex);
	if (*len > 0) {
		*tok = 7;
		return;
	}
	*len = check_if_string(lex);
	if (*len > 0) {
		*tok = 3;
		return;
	}
	*len = check_if_keyword(lex, keywords);
	if (*len > 0) {
		*tok = 2;
		return;
	}
	*len = check_if_float(lex);
	if (*len > 0) {
		*tok = 5;
		return;
	}
	*len = check_if_int(lex);
	if (*len > 0) {
		*tok = 4;
		return;
	}
	*len = check_if_operator(lex);
	if (*len > 0) {
		*tok = 1;
		return;
	}
	*len = check_if_keyword(lex, punctuation);
	if (*len > 0) {
		*tok = 8;
		return;
	}
	*len = check_if_id(lex);
	if (*len > 0) {
		*tok = 6;
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