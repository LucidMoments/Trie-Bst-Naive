#pragma once
#include <iostream>
#include <algorithm>
#include <string>
#include <optional>
#include <vector>
using namespace std;
const int M = 256;
const int S = 257;

int Hash(const string& str)
{
    int h = 0, len = str.length();
    for (int i = 0; i < len; i++)
        h = ((h * S) % M + str[i]) % M;

    return h;
}

class MyMap
{
    struct abox;
    struct box
    {
        int val;
        string tag;
        struct box* next;
        struct abox* theabox;

        box(const string& s, struct box* n) :val(0), tag(s), next(n) {}
    }**A;
    int N;

    struct abox
    {
        struct abox* next;
        struct abox* prev;
        struct box* thebox;
        abox(struct abox* n, struct box* t)
        {
            if ((next = n))
                n->prev = this;
            prev = nullptr;
            thebox = t;
        }
    }*head;

public:
    MyMap() :A(new struct box* [M]), N(0)
    {
        for (int i = 0; i < M; i++)
            A[i] = nullptr;
        head = nullptr;

    }
    ~MyMap()
    {
        for (int i = 0; i < M; i++)
        {
            struct box* ptr = A[i];
            while (ptr)
            {
                box* guy2del = ptr;
                ptr = ptr->next;
                delete guy2del;
            }
        }

        while (head)
        {
            abox* p = head;
            head = p->next;
            delete p;
        }


        delete[] A;
    }

    int& operator[](const string& s)
    {
        int h = Hash(s);
        if (!check(s))
        {
            A[h] = new box(s, A[h]);
            N++;
            head = new abox(head, A[h]);
            A[h]->theabox = head;

            return A[h]->val;
        }
        else
        {
            struct box* ptr = A[h];
            while (s != ptr->tag)
            {
                ptr = ptr->next;
            }
            return ptr->val;
        }
    }
    int getN()
    {
        return N;
    }
    void remove(const string& s)
    {
        if (!check(s))
            return;

        int h = Hash(s);
        struct box** pptr;

        for (pptr = &(A[h]); *pptr && (*pptr)->tag != s; *pptr = (*pptr)->next);
        abox* aptr = (*pptr)->theabox;
        if (aptr->next)
            (aptr->next)->prev = aptr->prev;
        if (aptr->prev)
            (aptr->prev)->next = aptr->next;
        else
            head = aptr->next;
        delete aptr;


        struct box* theguy2del = *pptr;
        *pptr = theguy2del->next;
        delete theguy2del;
        N--;
    }
    bool check(const string& s)
    {
        int h = Hash(s);
        struct box* ptr = A[h];
        while (ptr && ptr->tag != s)
            ptr = ptr->next;
        if (!ptr)
            return false;
        else
            return true;
    }
    void print()
    {
        for (int i = 0; i < M; i++)
        {
            struct box* ptr = A[i];
            while (ptr)
            {
                box* guy2del = ptr;
                ptr = ptr->next;
                cout << guy2del->tag << endl;
            }
        }
    }
};

class Trie
{
public:

    std::vector<Trie*> children;
    int prefix_count;
    int leaf_count;

    Trie()
    {
        children.resize(26, NULL);
        prefix_count = 0;
        leaf_count = 0;
    }

    void insert_util(string& word, int i)
    {
        int it = word[i] - 'a';
        if (it < 0)
        {
            it = abs(it) - 31;
        }
        if (i == word.size())
        {
            this->prefix_count++;
            this->leaf_count++;
            return;
        }
        this->prefix_count++;
        if (!this->children[word[i] - 'a'])
            this->children[word[i] - 'a'] = new Trie();
        this->children[word[i] - 'a']->insert_util(word, i + 1);
    }


    void insert(string word)
    {
        if (!word.size())
            return;
        insert_util(word, 0);
    }
    bool search_util(Trie* node, string& word, int i)
    {
        if (!node)
            return false;
        if (i == word.size())
            return node->leaf_count > 0;
        return search_util(node->children[word[i] - 'a'], word, i + 1);
    }

    bool search(string word)
    {
        if (!word.size())
            return true;
        return search_util(this, word, 0);
    }

    bool startsWith_util(Trie* node, string& prefix, int i)
    {
        if (!node)
            return false;
        if (i == prefix.size())
            return node->prefix_count > 0;
        return startsWith_util(node->children[prefix[i] - 'a'], prefix, i + 1);
    }
    bool startsWith(string prefix)
    {
        if (!prefix.size())
            return true;
        return startsWith_util(this, prefix, 0);
    }
};
struct BSTreeNode {
    optional<string> key;
    struct BSTreeNode* left;
    struct BSTreeNode* right;

    BSTreeNode() {
        key = {};
        left = nullptr;
        right = nullptr;
    }

    const BSTreeNode* findString(const string& k) const {
        if (!this->key.has_value()) {
            return nullptr;
        }

        if (k == this->key.value())
            return this;

        if (k < this->key.value()) {
            if (!this->left) {
                return nullptr;
            }
            return this->left->findString(k);
        }
        else {
            if (!this->right) {
                return nullptr;
            }
            return this->right->findString(k);
        }
    }

    void insertString(const string& k) {
        if (!key.has_value()) {
            key = { k };
            return;
        }

        else {
            if (k < this->key.value()) {
                if (!this->left) {
                    this->left = new BSTreeNode();
                }
                this->left->insertString(k);
            }
            else {
                if (!this->right) {
                    this->right = new BSTreeNode();
                }
                this->right->insertString(k);
            }
        }
    }

    void printString() const {
        if (this->key.has_value()) {
            cout << this->key.value() << "\n";
        }
        else {
            cout << "Empty node\n";
        }
    }
};

class Naive {
private:
    std::vector<std::string> strings{};

public:
    void insertString(const std::string& str) {
        
        if (!findString(str)) {
            
            strings.push_back(std::string(str));
        }
    }

    bool findString(const std::string& str) const {
        for (auto v : strings) {
            if (str == v) {
                return true;
            }
        }
        return false;
    }
};
