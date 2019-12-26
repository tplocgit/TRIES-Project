#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<time.h>
using namespace std;
#define ALPHABET_CHARS 26
#define MAX_LENGTH 10
#define  LOG_STATUS false

// Each node contains hash table with hash function h(k) = k - 97;
//--------------------------------------------------------------------
struct NODE {
	NODE* m_nodes[ALPHABET_CHARS];
	bool m_endWord;
};

int Tree_getIndex(char c);

char Tree_getChar(int index);

NODE* Tree_createNODE();

void Tree_InsertWord(NODE*& p_root, string word);

NODE* Tree_ReadFromFile(string path);

void Tree_Deallocate(NODE*& p_root);

vector<string> Tree_GenerateWord(NODE* p_root, string data);

void Tree_GetStrings(NODE* p_root, vector<string>& result, string word);

bool Tree_IsAvailable(string word, NODE* p_root);

int TwoPower(int n);

vector<string> String_getSubString(string s);

void String_permutation(string data, vector<string>& result, string tmp, bool* checker);

vector<string> String_Generator(string s);

bool isExist(string s, vector<string> arr);

string RemoveSpace(string line);

void cc(NODE* p_root, string data, vector<string>& result, string tmp, vector<bool> checker);
//--------------------------------------------------------------------

int main() {
	NODE* p_root = Tree_ReadFromFile("dic.txt");
	string line;
	getline(cin, line, '\n');
	string data = RemoveSpace(line);
	vector<string> gen;
	string tmp;
	vector<bool> checker(data.length(), true);
	cc(p_root, data, gen, tmp,checker);
	cout << gen.size() << endl;
	if (!gen.empty())
		for (string s : gen)
			cout << s << endl;
	Tree_Deallocate(p_root);
	system("pause");
	return 0;
}

vector<string> Tree_GenerateWord(NODE* p_root, string data) {
	if (LOG_STATUS) {
		cout << "Tree_GenerateWord working...\n";
	}
	vector<string> gen = String_Generator(data);
	vector<string> res;
	for (auto it = gen.begin(); it != gen.end(); ++it)
		if (Tree_IsAvailable(*it, p_root))
			res.push_back(*it);
	if (LOG_STATUS)
		cout << "Tree_GenerateWord complete!\n";
	return res;
}

void Tree_Deallocate(NODE*& p_root) {
	if (!p_root)
		return;
	for (int i = 0; i < ALPHABET_CHARS; i++) {
		if (p_root->m_nodes[i]) {
			Tree_Deallocate(p_root->m_nodes[i]);
			delete[] p_root->m_nodes[i];
			p_root->m_nodes[i] = nullptr;
		}
	}
}

int Tree_getIndex(char c) {
	int res;
	if ('a' <= c && c <= 'z')
		res = c - 'a';
	else
		res = -1;
	return res;
}

char Tree_getChar(int index) {
	char c;
	if (0 <= index && index <= 25)
		c = index + 'a';
	else
		c = '\0';
	return c;
}

NODE* Tree_createNODE() {
	NODE* p_node = new NODE;
	for (int i = 0; i < ALPHABET_CHARS; i++)
		p_node->m_nodes[i] = nullptr;
	p_node->m_endWord = false;
	return p_node;
}

void Tree_InsertWord(NODE*& p_root, string word) {
	NODE* p_cur = p_root;
	for (auto it = word.begin(); it != word.end(); ++it) {
		int index = Tree_getIndex(*it);
		if (!p_cur->m_nodes[index])
			p_cur->m_nodes[index] = Tree_createNODE();
		p_cur = p_cur->m_nodes[index];
	}
	p_cur->m_endWord = true;
}

NODE* Tree_ReadFromFile(string path) {
	if (LOG_STATUS)
		cout << "Tree_ReadFromFile working...\n";
	ifstream fin;
	NODE* p_root = nullptr;
	fin.open(path);
	if (!fin.is_open())
		cout << "Error XXX: File not found.\n";
	else {
		p_root = Tree_createNODE();
		string word;
		while (!fin.eof()) {
			std::getline(fin, word, '\n');
			if (word.length() >= 3)
				Tree_InsertWord(p_root, word);
		}
		fin.close();
	}
	if (LOG_STATUS)
		cout << "Tree_ReadFromFile complete!\n";
	return p_root;
}

void Tree_GetStrings(NODE* p_root, vector<string>& result, string word) {
	if (p_root) {
		for (int i = 0; i < ALPHABET_CHARS; ++i) {
			if (p_root->m_nodes[i]) {
				char c = Tree_getChar(i);
				word.push_back(c);
				if (p_root->m_nodes[i]->m_endWord)
					result.push_back(word);
				Tree_GetStrings(p_root->m_nodes[i], result, word);
				word.pop_back();
			}
		}
	}
}

bool Tree_IsAvailable(string word, NODE* p_root) {
	if (p_root && word.empty() && p_root->m_endWord)
		return true;
	else if (!p_root || word.empty())
		return false;
	int index = Tree_getIndex(word.front());
	if (p_root && p_root->m_nodes[index]) {
		return Tree_IsAvailable(word.substr(1, word.length() - 1), p_root->m_nodes[index]);
	}
	return false;
}

int TwoPower(int n) {
	int subBits = 0;
	for (int i = 0; i < n; ++i)
		subBits |= 1 << i;
	return subBits + 1;
}

vector<string> String_getSubString(string s) {
	if (LOG_STATUS)
		cout << "String_getSubString working...\n";
	int subBits = TwoPower(s.length()); // 2^n
	vector<string> result;
	if (!s.empty()) {
		for (int i = 0; i < subBits; ++i) {
			string data;
			for (int j = 0; j < s.length(); ++j)
				if ((i >> j) & 1)
					data.push_back(s[j]);
			if (!data.empty())
				result.push_back(data);
		}
	}
	if (LOG_STATUS)
		cout << "String_getSubString complete! Got: " << result.size() << " sub strings\n";
	return result;
}

void String_permutation(string data, vector<string>& result, string tmp, bool* checker) {
	if (tmp.size() == data.size()) {
		result.push_back(tmp);
		return;
	}
	for (int i = 0; i < data.size(); ++i) {
		if (checker[i]) {
			checker[i] = false;
			tmp.push_back(data[i]);
			String_permutation(data, result, tmp, checker);
			checker[i] = true;
			tmp.pop_back();
		}
	}
}

vector<string> String_Generator(string s) {
	if (LOG_STATUS)
		cout << "String_Generator working...\n";

	vector<string> subArr = String_getSubString(s);
	vector<string> res;
	if (!subArr.empty()) {
		int count = 0;
		for (string s : subArr) {
			++count;
			if (LOG_STATUS)
				cout << "Permuting " << count << "th sub string...";
			if (s.length() >= 3) {
				bool* checker = new bool[s.length()];
				for (int i = 0; i < s.length(); ++i)
					checker[i] = true;
				vector<string> subRes;
				string tmp;
				String_permutation(s, subRes, tmp, checker);
				if (!subRes.empty())
					for (string s : subRes)
						if (!isExist(s, res))
							res.push_back(s);
				delete[] checker;
			}
			if (LOG_STATUS)
				cout << "==> complete!\n";
		}
	}
	if (LOG_STATUS)
		cout << "String_Generator complete!\n";
	return res;
}

bool isExist(string s, vector<string> arr) {
	if (LOG_STATUS)
		cout << "Checking " << s << " in array....";
	for (string a : arr)
		if (s == a) {
			if (LOG_STATUS)
				cout << "=> complete!\n";
			return true;
		}
	if (LOG_STATUS)
		cout << "=> complete!\n";
	return false;
}

string RemoveSpace(string line) {
	if (LOG_STATUS)
		cout << "RemoveSpace working...\n";
	string s;
	for (char c : line)
		if ('a' <= c && c <= 'z')
			s.push_back(c);
	if (LOG_STATUS)
		cout << "RemoveSpace complete!\n";
	return s;
}

void cc(NODE* p_root, string data, vector<string>& result, string tmp, vector<bool> checker) {
	if (p_root) {
		for (auto it = data.begin(); it != data.end(); ++it) {
			int i = Tree_getIndex(*it);
			int j = it - data.begin();
			if (p_root->m_nodes[i] && checker[j]) {
				tmp.push_back(*it);
				checker[j] = false;
				if (p_root->m_nodes[i]->m_endWord) {
					if (!isExist(tmp, result))
						result.push_back(tmp);
				}
				cc(p_root->m_nodes[i], data, result, tmp, checker);
				tmp.pop_back();
				checker[j] = true;
			}
		}
	}
}