#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

template <typename T>
T readValue() {
	T value = 0;
	while (true) {
		cin >> value;
		if (cin.fail()) {
			cout << "Incorrect input. Enter new value: ";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			return value;
		}
	}
}










//**********************STACK STRUCTYRE**********************
struct Stack {
	string value;
	struct Stack* next;
};

//**********************Контейнер приорететов**********************
map <string, int> weights = {
		{"+", 1},
		{"-", 1},
		{"*", 2},
		{"/", 2},
		{"^", 3},
		{"(", 0},
		{")", 0}
};

//**********************Контейнер переменных**********************
map<string, int> variables = {};


//**********************GET SIZE**********************
int stackSize(Stack* head) {
	int count = 0;
	while (head) {
		count++;
		head = head->next;
	}
	return count;
}

//**********************ADDING A NEW ELEMENT TO AN EMPTY STACK**********************
void stackPush(Stack*& head, string data) {
	Stack* first = new Stack;
	first->value = data;
	first->next = head;
	head = first;
}


//**********************READING AN ELEMENT FROM THE TOP OF THE STACK**********************
string stackPop(Stack*& head) {
	Stack* prev = nullptr;
	if (head == nullptr)
		return 0;
	prev = head;
	string val = head->value;
	head = head->next;
	delete prev;
	return val;
}

//**********************GETTING A REFERENCE TO THE TOP ELEMENT OF THE STACK**********************
Stack* pop(Stack* str) {
	Stack* prev = str->next;
	delete str;
	str = prev;
	return str;
}
//**********************GETTING A REFERENCE TO THE LAST ELEMENT OF THE STACK**********************
Stack* stackLast(Stack* head) {
	if (head == nullptr)
		return nullptr;
	while (head->next)
		head = head->next;
	return head;
}

//**********************FREE MEMORY**********************
void stackClear(Stack* head) {
	Stack* prev = nullptr;
	while (head->next) {
		prev = head;
		head = head->next;
		delete prev;
	}
	delete head;
}

//**********************GETTING A REFERENCE TO THE LAST ELEMENT OF THE STACK(operation > 2)**********************
Stack* stackGetLastButOne(Stack* head) {
	if (head == nullptr || head->next == nullptr)
		return nullptr;
	while (head->next->next)
		head = head->next;
	return head;
}
//**********************PRINT STACK**********************
void stackPrint(Stack* head) {
	while (head) {
		cout << head->value << " ";
		head = head->next;
	}
}
//**********************CHECK STACK (if emty return 0)**********************
bool empty(Stack* str) {
	if (!str) return 1;
	else return 0;
}


//**********************ADDING A NEW ELEMENT TO THE TOP OF THE STACK**********************
void stackPushBack(Stack* head, string data) {
	Stack* last = stackLast(head);
	Stack* next = new Stack;
	next->value = data;
	next->next = nullptr;
	last->next = next;
}


//**********************READING AN ELEMENT FROM THE TOP OF THE STACK(operation > 2)**********************
string stackPopBack(Stack* head) {
	string value;
	Stack* lastbn = nullptr;
	if (!head)
		return 0;

	lastbn = stackGetLastButOne(head);
	value = lastbn->next->value;

	if (lastbn == nullptr) {
		delete head;
		head = nullptr;
	}
	else {
		delete lastbn->next;
		lastbn->next = nullptr;
	}
	return value;
}

//**********************STACK INITIALIZATION:**********************
void stackAdd(Stack*& head, const string& data) {
	if (!head)
		stackPush(head, data);
	else
		stackPushBack(head, data);
}

//**********************CHECK WEIGHT**********************
string stackTake(Stack*& head) {
	if (stackSize(head) > 1)
		return stackPopBack(head);
	else 
		return stackPop(head);
}

//**********************CHECK TOKEN (operation)**********************
bool isOperation(const string& str) {
	auto res = weights.find(str);
	return res != weights.end();
}

//**********************CHECK TOKEN (literal)**********************
bool isLiteral(const string& str) {
	auto res = variables.find(str);
	return res != variables.end();
}

//**********************CHECK TOKEN (number)**********************
bool isNumber(const string& str) {
	string::const_iterator it = str.begin();
	while (it != str.end() && isdigit(*it)) ++it;
	return !str.empty() && it == str.end();
}

//**********************GET TOKENS**********************
vector<string> tokenize(const string& text) {
	vector<string> tokens{};
	istringstream input(text);

	for (string token; getline(input, token, ' ');)
		tokens.push_back(token);
	return tokens;
}

//**********************CHECK WEIGHT**********************
bool MoveOperation(string token, Stack* opstack) {
	Stack* last = stackLast(opstack);
	return weights.find(last->value)->second >= weights.find(token)->second;
}

//**********************CHECK PRIORITY**********************
int _switch(char p) {
	switch (p) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	default: return -1;
	}
}

//**********************CORRECTION OF A MATHEMATICAL EXPRESSION**********************
bool correct(string& str) {
	string token;
	Stack* tok = nullptr;

	if (!empty(tok)) {
		return 0;
	}

	for (int i = 1; i < str.length() - 1; i++) {
		if (str[i] == ' ' && _switch(str[i - 1]) == -1 && _switch(str[i + 1]) == -1)
			return 0;
	}


	for (int i = 0; i < str.length(); i++) {
		if (str[i] == '(') {
			std::string s1(1, str[i]);
			stackAdd(tok, s1);
		}
		if (str[i] == ')')
			if (!empty(tok)) {
				tok = pop(tok);
			}
			else return 0;
	}

	for (int i = 1; i < str.length(); i++) {

		if (_switch(str[i - 1]) > 0 && _switch(str[i]) > 0 && str[i] != '-')
			return 0;
	}
	for (int i = 0, k = 0; i < str.length(); i++) {
		if (_switch(str[i]) > 0) k++;
		if (i == 1 && str[i - 1] != '-' && _switch(str[i - 1]) > 0)
			return 0;
		else if (i == str.length() - 1 && _switch(str[i]) > 0 || i == str.length() - 1 && !k) 
			return 0;
	}

	return 1;
}

//**********************RPN**********************
string reversePolishNotation(string& inpt_str, ofstream& ofs, bool silent = false) {
	variables.clear();
	string current_str;
	vector<string> str_tokens = tokenize(inpt_str);
	Stack* outStack = nullptr;
	Stack* opStack = nullptr;

	for (const auto& token : str_tokens) {

		if (isNumber(token)) {
			stackAdd(outStack, token);
		}

		else if (isOperation(token)) {
			if (!opStack)
				stackPush(opStack, token);
			else if (token == "(")
				stackAdd(opStack, token);
			else if (token == ")") {
				while (opStack && current_str != "(") {
					if (stackSize(opStack) >= 2)
					{
						current_str = stackPopBack(opStack);
					}
					else {
						current_str = stackPop(opStack);
					}
					stackAdd(outStack, current_str);
					Stack* last = stackLast(opStack);
					if (!last)
						throw "Обнаружена непарная скобка";
					current_str = last->value;
				}
				if (!opStack)
					throw "Обнаружена непарная скобка";
				current_str.clear();
				if (stackSize(opStack) >= 2)
				{
					current_str = stackPopBack(opStack);
				}
				else {
					current_str = stackPop(opStack);
				}
			}
			else if (MoveOperation(token, opStack)) {
				if (stackSize(opStack) >= 2)
				{
					stackPushBack(outStack, stackPopBack(opStack));
				}
				else {
					stackPushBack(outStack, stackPop(opStack));
				}
				stackAdd(opStack, token);
			}
			else
				stackPushBack(opStack, token);
		}
		else {
			if (silent) {
				stackAdd(outStack, "1");
			}
			else {
				auto found = variables.find(token);
				if (found == variables.end()) {
					cout << "Введите значение для " + token + ": ";
					int var = readValue<int>();
					variables[token] = var;
					stackAdd(outStack, to_string(var));
				}
				else {
					stackAdd(outStack, to_string(found->second));
				}
			}
		}
		if (!silent) {
			cout << "----------------------------- " << "Token: " + token << " -----------------------------" << "\nвыходная строка: ";
			stackPrint(outStack);
			cout << "\nСтэк : ";
			stackPrint(opStack); 
			cout << endl << endl;
		}
	}

	if (!silent)
		cout << "Перемещение операций в основной стэк...\n";
	while (opStack)
		if (stackSize(opStack) >= 2)
		{
			stackPushBack(outStack, stackPopBack(opStack));
		}
		else {
			stackPushBack(outStack, stackPop(opStack));
		}

	if (!silent) {
		cout << "\nОбратная польская нотация:\t";
		stackPrint(outStack);
		cout << "\n\n\n";
	}
	string outp;
	while (outStack) {
		outp += outStack->value + ' ';
		outStack = outStack->next;
	}
	outp.pop_back();
	return outp;
}


//**********************CALCULATOR**********************
int calculatorRpn(string& rpn, ofstream& ofs) {
	istringstream reader(rpn);
	Stack* outStack = nullptr;
	string token;
	while (!reader.eof()) {
		reader >> token;
		if (isNumber(token)) {
			cout << "Переносим в стэк " << token << endl;
			stackAdd(outStack, token);
		}
		else {
			if (stackSize(outStack) < 2)
				throw "Неверный порядок!";
			int a, b;
			b = stoi(stackTake(outStack));
			a = stoi(stackTake(outStack));

			switch (token[0]) {
			case '+':
					cout << "Забираем числа " << a << " и " << b << " из стека. Складываем. Результат переносим в стек.\n";
					cout << to_string(a) << " + " << to_string(b) << " = " << to_string(a + b) << '\n';
				stackAdd(outStack, to_string(a + b));
				break;
			case '-':
					cout << "Забираем числа " << a << " и " << b << " из стека. Вычитаем. Результат переносим в стек.\n";
					cout << to_string(a) << " - " << to_string(b) << " = " << to_string(a - b) << '\n';
				stackAdd(outStack, to_string(a - b));
				break;
			case '*':
				cout << "Забираем числа " << a << " и " << b << " из стека. Умножаем. Результат переносим в стек.\n";
				cout << to_string(a) << " * " << to_string(b) << " = " << to_string(a * b) << '\n';
				stackAdd(outStack, to_string(a * b));
				break;
			case '/':
					cout << "Забираем числа " << a << " и " << b << " из стека. Делим. Результат переносим в стек.\n";
					cout << to_string(a) << " / " << to_string(b) << " = " << to_string(a / b) << '\n';
				stackAdd(outStack, to_string(a / b));
				break;




			default:
				string errt = "Неизвестный оператор " + token;
				throw errt.c_str();
			}
		}
	}
	if (stackSize(outStack) != 1)
		throw "Invalid stack size";
	return stoi(stackPop(outStack));
}








//**********************MAIN MENU**********************
int main()
{
	setlocale(LC_ALL, "rus");
	ofstream ofs("out.txt");
	while (true) {
		system("cls");
		cout <<
			"Choose the command:\n"
			"(1) - Convert expression into reverse polish notation\n"
			"(2) - Validate and compute mathematical expression\n"
			"(3) - Validate and compute reverse polish notation expression\n"
			"(0) - Exit\n\n";
		cout << "Action >> ";
		int choice = readValue<int>();
		cout << endl;
		switch (choice) {
		case 0:
			return 0;
		case 1: {
			system("cls");
			string inp;
			cout << "Введите математическое выражение: ";
			getline(cin, inp);
			cout << endl << "Перевод в обратную польскую нотацию...\n\n";
			reversePolishNotation(inp, ofs);
			break;
		}
		case 2: {
			system("cls");
			string inp;
			cout << "Введите математическое выражение: ";
			getline(cin, inp);
			if(!correct(inp)) {
				cout << "Expression is invalid!\n";
			}
			else {
				cout << "Expression is valid.\n";
			}
			break;
		}
		case 3: {
			system("cls");
			string inp;
			cout << "Enter reverse polish notation expression: ";
			getline(cin, inp);
			try {
				cout << "Ответ: " << calculatorRpn(inp, ofs) << endl << endl;
				cout << "Expression is valid.\n";
			}
			catch (const char* data) {
				cout << "\nERROR\n" << "Expression is invalid! " << data << endl;
			}
			break;
		}

		default:
			cout << "\nCategory with number " << choice << " does not exist." << endl;
			break;
		}
		system("pause");
	}
}