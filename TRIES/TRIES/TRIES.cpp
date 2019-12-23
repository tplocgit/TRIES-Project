#include<iostream>
#include<string>
#include<fstream>

using namespace std;

#define ALPHABET_CHARS 26

int8_t getIndex(char c);

// Each node contains hash table with hash function h(k) = k - 97;
struct TRIES_NODE {
	TRIES_NODE* m_nodes[ALPHABET_CHARS];
	bool m_endWord;
	void Deallocate();
	TRIES_NODE*& operator[](int index);
	void setFlag(bool flag);
	void create();
};

struct TRIES_TREE {
	TRIES_NODE* p_root;
	void Insert(string word, int depth, TRIES_NODE* cur);
	void CreateWord(char* chars, int n);
	void create(string file_name);
};


int main() {
	TRIES_TREE Root;
	Root.create("Test.txt");
	Root.p_root->Deallocate();
	cout << "cc\n";
	system("pause");
	return 0;
}

void TRIES_NODE::Deallocate() {
	if (!this)
		return;
	for (int i = 0; i < ALPHABET_CHARS; i++) {
		if (this) {
			(*this)[i]->Deallocate();
			delete[] this;
		}
	}
}

int8_t getIndex(char c) {
	int8_t res;
	if ('a' <= c && c <= 'z')
		res = c - 'a';
	else
		res = -1;
	return res;
}

TRIES_NODE*& TRIES_NODE::operator[](int index) {
	return this->m_nodes[index];
}

void TRIES_NODE::setFlag(bool flag) {
	this->m_endWord = flag;
}

void TRIES_NODE::create() {
	for (int i = 0; i < ALPHABET_CHARS; i++)
		this->m_nodes[i] = nullptr;
	this->m_endWord = true;
}

void TRIES_TREE::Insert(string word, int depth, TRIES_NODE* cur) {
	cur = (*cur)[int(getIndex(word.front()))];
	if (cur) {
		Insert(word.substr(1, word.size() - 1), depth + 1, cur);
		cur->setFlag(false);
	}
	else {
		if (word.end() - word.begin() == 1) {
			cur = new TRIES_NODE;
			cur->create();
		}
		else {
			cur = new TRIES_NODE;
			cur->create();
			Insert(word.substr(1, word.size() - 1), depth + 1, cur);
		}
	}
}

void TRIES_TREE::CreateWord(char* chars, int n) {

}

void TRIES_TREE::create(string file_name) {
	ifstream fin;
	fin.open(file_name);
	if (!fin.is_open())
		cout << "Error XXX: File not found.\n";
	else {
		this->p_root = new TRIES_NODE;
		this->p_root->create();
		string word;
		while (!fin.eof()) {
			fin >> word;
			this->Insert(word, 0, this->p_root);
		}
		fin.close();
	}
}
