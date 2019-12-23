#include<iostream>
#include<string>
#include<fstream>
#include<vector>
using namespace std;

#define ALPHABET_CHARS 26
#define MAX_LENGTH 10

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

vector<string> Tree_GenerateWord(NODE* p_root, char* chars, int n);
//--------------------------------------------------------------------

int main() {
	NODE* p_root = Tree_ReadFromFile("test.txt");
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
	return p_root;
}
