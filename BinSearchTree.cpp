
//Desmond A.Farley-Williams

#include "BinSearchTree.hpp"
#include <algorithm> // for sort() --
#include <string>
#include <iostream>
#include <queue>

WordBinSearchTree::~WordBinSearchTree() { deleteNode(root_); }

//Post-order delete to free entire tree and avoid leaks
void WordBinSearchTree::deleteNode(WordTreeNode* n) noexcept {

    if (!n)
        return;

    deleteNode(n->left);
    deleteNode(n->right);

    delete n;
}

// Why: Standard BST insert; duplicates increment count
WordTreeNode* WordBinSearchTree::insertHelper(WordTreeNode* n, const std::string& w) {

    if (!n)
        return new WordTreeNode(w);

    if (w < n->word)
        n->left = insertHelper(n->left, w);

    else if (w > n->word)
        n->right = insertHelper(n->right, w);

    else n->count++;

    return n;
}

//Single entry point to mutate root
void WordBinSearchTree::insert(const std::string& word) {

    root_ = insertHelper(root_, word);

}

//Deterministic bulk load; caller controls order
void WordBinSearchTree::bulkInsert(const std::vector<std::string>& words){

    for (const auto& w : words) insert(w);

}

//Iterative find to avoid recursion errors
const WordTreeNode* WordBinSearchTree::findNode(const WordTreeNode* n, std::string w) noexcept {

    while (n) {

        if (w<n->word)
            n=n->left;

        else if (w>n->word)
            n=n->right;

        else return n;
    }

    return nullptr;
}

//used by tests content of tree
bool WordBinSearchTree::contains(std::string w) const noexcept {

    return findNode(root_, w) != nullptr;

}

//used to count of content in tree
int WordBinSearchTree::countOf(const std::string& w) const noexcept {

    const auto* n = findNode(root_, w);
    return n ? n->count : -1;

}

//Inorder traversal produces (word,count)
void WordBinSearchTree::inorderHelper(const WordTreeNode* n, std::vector<std::pair<std::string,int>>& out) {

    if (!n)
        return;

    inorderHelper(n->left,out);
    out.emplace_back(n->word,n->count);
    inorderHelper(n->right,out);

}

void WordBinSearchTree::inorderCollect(std::vector<std::pair<std::string,int>>& out) const {

    out.clear();
    inorderHelper(root_, out);

}

//size of tree
std::size_t WordBinSearchTree::sizeHelper(const WordTreeNode* n) noexcept {

    if (!n)
        return 0;

    return 1 + sizeHelper(n->left) + sizeHelper(n->right);
}

//height of tree
unsigned WordBinSearchTree::heightHelper(const WordTreeNode* n) noexcept {

    if (!n)
        return 0;

    unsigned lh = heightHelper(n->left);
    unsigned rh = heightHelper(n->right);

    return 1 + ( lh > rh ? lh : rh );
}

std::size_t WordBinSearchTree::size() const noexcept {

    return sizeHelper(root_);
}

unsigned WordBinSearchTree::height() const noexcept {

    return heightHelper(root_);
}
