#include <iostream>
#include <cassert>
#include <chrono>
#include <fstream>
#include <functional>
#include <utility>
#include <algorithm>
#include <vector>
#include "lib.h"

long timeit_nano(std::function<void()> totime) {
    using std::chrono::steady_clock;
    using std::chrono::duration;
    using std::chrono::time_point;

    time_point start = steady_clock::now();
    totime();
    time_point end = steady_clock::now();
    duration<long, std::nano> elapsed = end - start;
    return elapsed.count();
}

std::vector<std::string> read_file(std::string path)
{
    std::string line;
    std::ifstream myfile(path);
    if (myfile.fail()) {
        cout << "Failed to open \'" << path << "\'\n";
        std::exit(1);
    }
    vector<std::string> to_return;
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            
            if (line.back() == '\r') {
                line.pop_back();
            }
            to_return.push_back(line);
        }
        myfile.close();
    }
    return to_return;
}


int main()
{
    Trie trie;
    MyMap hash;
    BSTreeNode* bst = new BSTreeNode();
    Naive naive{};
    std::vector<std::string> dictionarry = read_file("file.txt");

    {
        long trie_timeittook = 0;
        long hash_timeittook = 0;
        long bst_timeittook = 0;
        long naive_timeittook = 0;
        for (long unsigned int i = 0; i < dictionarry.size(); i++)
            trie_timeittook += timeit_nano([&, i] {trie.insert(dictionarry[i]); });
        for (long unsigned int i = 0; i < dictionarry.size(); i++)
            hash_timeittook += timeit_nano([&, i] { hash[dictionarry[i]] = i; });
        for (long unsigned int i = 0; i < dictionarry.size(); i++)
            bst_timeittook += timeit_nano([&, i] { bst->insertString(dictionarry[i]); });
        for (long unsigned int i = 0; i < dictionarry.size(); i++)
            naive_timeittook += timeit_nano([&, i] { naive.insertString(dictionarry[i]); });
        cout << dictionarry.size() << " " << trie_timeittook << " " << hash_timeittook << " " << bst_timeittook << " " << naive_timeittook << endl;
    }

    vector<string> text = read_file("text.txt");

    int size = text.size() / 100;
    int n, i = 1;
    for (n = 10; n < text.size(); i++)
    {
        long trie_timeittook = 0;
        long hash_timeittook = 0;
        long bst_timeittook = 0;
        long naive_timeittook = 0;
        for (int j = 0; j < n; j++)
        {
            hash_timeittook += timeit_nano([&, j] {hash.check(text[j]); });
        }

        for (int j = 0; j < n; j++)
        {
            trie_timeittook += timeit_nano([&, j] {trie.search(text[j]); });
        }
        for (int j = 0; j < n; j++)
        {
            bst_timeittook += timeit_nano([&, j] {
                auto found = bst->findString(text[j]);
                /*
                if (!found) {
                    cout << "Not found\n";
                } else {
                    found->printString();
                }
                */
                });
        }
        for (int j = 0; j < n; j++) {
            naive_timeittook += timeit_nano([&, j] {
                auto found = naive.findString(text[j]);
                });
        }

        cout << n << " " << trie_timeittook << " " << hash_timeittook << " " << bst_timeittook << " " << naive_timeittook << endl;
        n = i * size;
    }


    return 0;
}
