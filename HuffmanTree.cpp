
#include "HuffmanTree.hpp"
#include <unordered_map>

HuffmanTree::~HuffmanTree(){ destroy(root_); }

void HuffmanTree::destroy(HufNode* n) noexcept {

    if(!n)
        return;

    destroy(n->left);
    destroy(n->right);
    delete n;

}

HuffmanTree HuffmanTree::buildFromCounts(const std::vector<std::pair<std::string,int>>& counts) {

    HuffmanTree huffTree;

    if (counts.empty())
        return huffTree;

    std::vector<HufNode*> leaves;

    leaves.reserve(counts.size());

    for (const auto& kv : counts)
        leaves.push_back(new HufNode(kv.first, kv.second));

    PriorityQueue pq(std::move(leaves));

    if (pq.size() == 1) {

        huffTree.root_ = pq.extractMin();

        return huffTree;
    }

    while (pq.size() >= 2){

        auto* a=pq.extractMin();
        auto* b=pq.extractMin();

        std::string key = (a->key_word < b->key_word ? a->key_word : b->key_word);

        pq.insert(new HufNode(a->freq + b->freq, key, a, b));

    }

    huffTree.root_=pq.extractMin();

    return huffTree;
}

void HuffmanTree::assignCodesDFS(const HufNode* n, std::string& p, std::vector<std::pair<std::string,std::string>>& out) {

    if(!n)
        return;

    if(n->isLeaf()) {

        out.emplace_back(n->word, p.empty()? "0" : p);

        return;
    }

    p.push_back('0');

    assignCodesDFS(n->left, p, out);
    p.back()='1';

    assignCodesDFS(n->right, p, out);
    p.pop_back();

}

void HuffmanTree::assignCodes(std::vector<std::pair<std::string,std::string>>& out) const {

    out.clear();

    std::string p;

    assignCodesDFS(root_, p, out);

}

void HuffmanTree::writeHeaderPreorder(const HufNode* n, std::string& p, std::ostream& outStream) {

    if(!n)
        return;

    if(n->isLeaf()) {

        outStream << n->word << ' ' << (p.empty()? "0":p) << '\n';

        return;
    }

    p.push_back('0');
    writeHeaderPreorder(n->left, p, outStream);
    p.back()='1';
    writeHeaderPreorder(n->right, p, outStream);
    p.pop_back();

}

error_type HuffmanTree::writeHeader(std::ostream& outStream) const {

    if(!outStream.good())
        return FAILED_TO_WRITE_FILE;

    // empty with single newline
        if (!root_) {

            outStream.put('\n');

            return outStream.good() ? NO_ERROR : FAILED_TO_WRITE_FILE;
            }

        std::string p;

    writeHeaderPreorder(root_, p, outStream);

    return outStream.good() ? NO_ERROR : FAILED_TO_WRITE_FILE;
}

error_type HuffmanTree::encode(const std::vector<std::string>& tokens, std::ostream& out, int wrap_cols) const {

    std::vector<std::pair<std::string,std::string>> codes;

    assignCodes(codes);

    std::unordered_map<std::string,std::string> m;

    for(auto& kv: codes) {

        m.emplace(kv.first, kv.second);

    }

        //newline when tokens zero
        if (tokens.empty()) {

            out.put('\n');

            if (!out.good())
                return FAILED_TO_WRITE_FILE;

            return NO_ERROR;
            }

    int col = 0;

    for (const auto& t : tokens) {

        auto it = m.find(t);

        //if( it==m.end() )
        //return FAILED_TO_WRITE_FILE;
        if( it==m.end() )
            return ERR_TYPE_NOT_FOUND;

        for (char ch : it->second) {

            out.put(ch);

            if(!out.good())
                return FAILED_TO_WRITE_FILE;

            if( ++col == wrap_cols) {

                out.put('\n');

                if(!out.good())
                    return FAILED_TO_WRITE_FILE;

                col = 0;

            }

        }

    }

    if( col != 0) {

        out.put('\n');

        if(!out.good())
            return FAILED_TO_WRITE_FILE;

    }

    return NO_ERROR;
}

unsigned HuffmanTree::heightHelper(const HufNode* n) noexcept {

    if(!n)
        return 0;

    unsigned lh = heightHelper(n->left), rh = heightHelper(n->right);

    return 1+(lh > rh ? lh : rh);
}

unsigned HuffmanTree::height() const noexcept
{

    return heightHelper(root_);
}
