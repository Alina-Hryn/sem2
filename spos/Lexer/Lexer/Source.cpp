#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
using namespace std;

/* INPUT_LIN_TAM: max number of symbols in line
 * MAX_TABLE_LEN: max keyword length/size
 */
#define INPUT_LIN_TAM 1000
#define MAX_TABLE_LEN 20

char ruby_comment_begin[][MAX_TABLE_LEN] = { "=begin", "\0" };
char ruby_comment_end[][MAX_TABLE_LEN] = { "=end", "\0" };
char punctuation_[] = { '[', ']','(', ')', ',', ';','::', ':', '.', '?','{', '}', '\0' };
char keywords[50][MAX_TABLE_LEN] = {};

// global vars to check if comments/strings are closed correct
bool global_comment_open = false;
bool global_string_open = false;
bool open_for_variable = false;


char token_table[][MAX_TABLE_LEN] = { "UNKNOWN", "OPERATOR", "KEYWORD", "STRING LITERAL", "INTEGER LITERAL",
"FLOAT LITERAL", "IDENTIFIER", "COMMENT", "PUNCTUATION" };
int token_color_table[] = { 12, 14, 8, 13, 9, 11, 7, 10, 3 };

class Token {
private:
	string token_name;
	int token_type;
	int color;
public:
	static int size_of_array;
	Token() {
		this->token_name = "";
	}
	void add(string name, int token_type) {
		this->token_name = name;
		this->token_type = token_type;
		this->color = token_color_table[this->token_type];
	}
	void get_token() {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
		cout << this->token_name;
	}
};


// declaration of methods
void flush_input_lin(char* input_lin, int tam);
int check_if_token(char* lex, char list[][MAX_TABLE_LEN]);
int check_if_id(char* lex);
void check_token(char* lex, int* tok, int* len);
void print_lexem(char* lex, int tam);
void print_token(int tok);
void read_tokens();

int main() {
	int lin_num = 1; // counter of strings
	int char_itr = 0; //used to cycle through characters 
	int tmp_itr = 0; //same func as char_itr
	int t_tok, t_len; //id of token and lexem length(size)
	char* input_lin = (char*)malloc(INPUT_LIN_TAM * sizeof(char));
	HANDLE hConsole;
	int k = 0;
	read_tokens();
	Token tokens[1];
	int tokens_size = 0;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	ofstream outputFile("output.txt");
	ifstream inputFile("input.txt");
	while (inputFile) {
		inputFile.getline(input_lin, 255);
		char_itr = 0; // resets char_itr
		//SetConsoleTextAttribute(hConsole, k < 254? k++ : k = 0);
		//std::cout << lin_num++ << "   " << input_lin << std::endl; // print line
		SetConsoleTextAttribute(hConsole, 245);
		cout << (k < 9 ? "   " : (k < 99 ? "  " : " ")) << ++k << " ";
		SetConsoleTextAttribute(hConsole, 12);
		cout << " ";
		while (input_lin[char_itr] != '\n' && input_lin[char_itr] != '\0') {
			t_tok = 0; //token is not known
			t_len = 0; // lexem size = 0

			check_token(&input_lin[char_itr], &t_tok, &t_len); //checks current lexem is token
			if (t_tok < 0) {
				break;
			}
			else {
				outputFile << token_table[t_tok] << " -> ";
				string s;
				for (int i = char_itr; i < t_len + char_itr; i++) {
					outputFile << input_lin[i];
					s.push_back(input_lin[i]);
				}
				tokens[tokens_size].add(s, t_tok);
				tokens[tokens_size].get_token();
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

void read_tokens() {
	int size = 0;
	ifstream inputFile("keywords.txt");
	char text[MAX_TABLE_LEN];
	while (inputFile) {
		inputFile.getline(text, MAX_TABLE_LEN);
		int i = 0;
		while (text[i] != '\n' && text[i] != '\0') {
			keywords[size][i] = text[i];
			i++;
		}
		size++;
	}
	keywords[size][0] = '\0';


}

//checks if lexem is keyword in the list [][MAX_TABLE_LEN]
int check_if_token(char* lex, char list[][MAX_TABLE_LEN]) {
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
	else if (check_if_token(lex, ruby_comment_begin) > 0) {
		global_comment_open = true;
		return strlen(lex);
	}
	else if (check_if_token(lex, ruby_comment_end) > 0) {
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
	if (lex[i] == '$') i++;
	else if (lex[i] == '@') {
		if (lex[i + 1] == '@') i++;
		i++;
	}
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
	if ((lex[i] == '.' && i > 0) && !is_letter(lex[i + 1]) && lex[i + 1] != '.') {
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
	case '!':
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
	case '.':
		if (lex[i + 1] == '.') {
			i += 2;
		}
		return skip_spaces_after_word(lex, i++);
		break;
	default:
		return 0;
	}
}
int check_if_punctuation(char* lex) {
	int i = 0;
	int k = 0;
	for (i = 0; i < sizeof(punctuation_); i++)
		if (lex[k] == punctuation_[i]) k++;
	return k;
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
	*len = check_if_punctuation(lex);
	if (*len > 0) {
		*tok = 8;
		return;
	}
	*len = check_if_token(lex, keywords);
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
	std::cout << token_table[tok];
}