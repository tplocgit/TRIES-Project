#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<time.h>
using namespace std;
#define ALPHABET_CHARS 26
#define MAX_LENGTH 10
#define LOG_STATUS false

// Each node contains hash table with hash function h(k) = k - 97;
//--------------------------------------------------------------------
struct NODE {
	NODE* m_nodes[ALPHABET_CHARS];
	bool m_endWord;
};

int Tree_getIndex(char c);

NODE* Tree_createNODE();

void Tree_InsertWord(NODE*& p_root, string word);

NODE* Tree_ReadFromFile(string path);

void Tree_Deallocate(NODE*& p_root);

bool isExist(string s, vector<string> arr);

string RemoveSpace(string line);

void Tree_GenerateWord(NODE* p_root, string data, vector<string>& result, string tmp, vector<bool> checker);

void Tree_RadixSort(vector<string>& data);

void Tree_CountingSort(vector<string>& data, int exp);
//--------------------------------------------------------------------

int main() {
	NODE* p_root = Tree_ReadFromFile("Dic.txt");
	string line;
	getline(cin, line, '\n');
	string data = RemoveSpace(line);
	vector<string> gen;
	string tmp;
	vector<bool> checker(data.length(), true);
	Tree_GenerateWord(p_root, data, gen, tmp, checker);
	cout << gen.size() << endl;
	if (!gen.empty()) {
		Tree_RadixSort(gen);
		for (string s : gen)
			cout << s << endl;
	}
	Tree_Deallocate(p_root);
	system("pause");
	return 0;
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
	for (char c : line) {
		if ('a' <= c && c <= 'z')
			s.push_back(c);
		else if ('A' <= c && c <= 'Z')
			s.push_back(c + ' ');
	}
	if (LOG_STATUS)
		cout << "RemoveSpace complete!\n";
	return s;
}

void Tree_GenerateWord(NODE* p_root, string data, vector<string>& result, string tmp, vector<bool> checker) {
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
				Tree_GenerateWord(p_root->m_nodes[i], data, result, tmp, checker);
				tmp.pop_back();
				checker[j] = true;
			}
		}
	}
}

void Tree_RadixSort(vector<string>& data) {
	int max = data.front().length();
	for (auto it = data.begin() + 1; it != data.end(); ++it) {
		if (it->length() > max)
			max = it->length();
	}
	for (int i = 0; i < max; ++i)
		Tree_CountingSort(data, i);
}

void Tree_CountingSort(vector<string>& data, int exp) {
	vector<int> count(ALPHABET_CHARS, 0);
	vector<string> tmp(data.size());

	for (auto it = data.begin(); it != data.end(); ++it) {
		if (int(it->length()) - exp > 0) {
			int i = (*it)[int(it->length() - exp) - 1] - 'a' + 1;
			if (i >= 26)
				i--;
			count[i]++;
		}
		else
			count[0]++;
	}

	for (int i = 1; i < ALPHABET_CHARS; ++i) {
		int j = i - 1;
		count[i] += count[j];
	}

	for (auto it = data.rbegin(); it != data.rend(); ++it) {
		if (int(it->length()) - exp > 0) {
			int i = (*it)[int(it->length() - exp) - 1] - 'a' + 1;
			if (i >= 26)
				i--;
			tmp[--count[i]] = *it;
		}
		else {
			tmp[--count[0]] = *it;
		}
	}

	for (int i = 0; i < data.size(); i++)
		data[i] = tmp[i];
}