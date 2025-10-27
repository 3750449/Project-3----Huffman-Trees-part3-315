
#ifndef BINSEARCHTREE_BINSEARCHTREE_HPP
#define BINSEARCHTREE_BINSEARCHTREE_HPP

#include <string>
#include <vector>

struct WordTreeNode {

    //store normalized word; count aggregates duplicates
    std::string word;
    int count{1};
    WordTreeNode* left{nullptr};
    WordTreeNode* right{nullptr};
    explicit WordTreeNode(std::string w) : word(std::move(w)) {}

};

class WordBinSearchTree {

    public:

    WordBinSearchTree() {};
    ~WordBinSearchTree();

    void insert(const std::string& word);

    //bulkInsert exists to keep pipeline
    void bulkInsert(const std::vector<std::string>& words);

    bool contains(std::string w) const noexcept;
    int countOf(const std::string& w) const noexcept;

    void inorderCollect(std::vector<std::pair<std::string,int>>& out) const;

    //height/size used for stdout measurements
     std::size_t size() const noexcept;
     unsigned height() const noexcept;

private:

    WordTreeNode* root_{nullptr};

    static void deleteNode(WordTreeNode* n) noexcept;
    static WordTreeNode* insertHelper(WordTreeNode* n, const std::string& w);
    static const WordTreeNode* findNode(const WordTreeNode* n, std::string w) noexcept;
    static void inorderHelper(const WordTreeNode* n, std::vector<std::pair<std::string,int>>& out);

    static std::size_t sizeHelper(const WordTreeNode* n) noexcept;
    static unsigned heightHelper(const WordTreeNode* n) noexcept;

};

#endif //BINSEARCHTREE_BINSEARCHTREE_HPP
