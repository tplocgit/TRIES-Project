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

//vector<string> Tree_GenerateWord(NODE* p_root, char* chars, int n);

void Tree_GetStrings(NODE* p_root, vector<string>& result, string word);

vector<NODE> Tree_FindNodesWithPrefix(NODE*p_root, string word, string prefix);

bool IsAvailable(string word, NODE* p_root);
//--------------------------------------------------------------------

int main() {
	NODE* p_root = Tree_ReadFromFile("Test.txt");
	string word;
	cout << "Enter word: ";
	cin >> word;
	if (IsAvailable(word, p_root))
		cout << "yes\n";
	else cout << "no\n";
	Tree_Deallocate(p_root);
	if (IsAvailable(word, p_root))
		cout << "yes\n";
	else cout << "no\n";
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

/*vector<string> Tree_GenerateWord(NODE* p_root, char* chars, int n) {

}*/

void Tree_GetStrings(NODE* p_root, vector<string>& result, string word) {

}

/*NODE FindKeyWithPrefix(string word, string prefix, int length) {

}*/

bool IsAvailable(string word, NODE* p_root) {
	if (p_root && word.empty() && p_root->m_endWord)
		return true;
	else if(!p_root || word.empty())
		return false;
	int index = Tree_getIndex(word.front());
	if (p_root && p_root->m_nodes[index]) {
		return IsAvailable(word.substr(1, word.length() - 1), p_root->m_nodes[index]);
	}
	return false;
}