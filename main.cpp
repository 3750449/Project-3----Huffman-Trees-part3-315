#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>

#include "Scanner.hpp"
#include "utils.hpp"

#include <iomanip> // for .freq
#include "BinSearchTree.hpp"

#include <algorithm> // for sort() --

#include "HuffmanTree.hpp" // for Huffman Tree

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    const std::string dirName = std::string("input_output");
    const std::string inputFileName = std::string(argv[1]);

    // for the directory name
    const std::string inputFullPath = dirName + "/" + inputFileName;

    const std::string inputFileBaseName = baseNameWithoutTxt(inputFileName);

    // build the path to the .tokens output file.
    const std::string wordTokensFileName = dirName + "/" + inputFileBaseName + ".tokens";

    const std::string freqFileName   = dirName + "/" + inputFileBaseName + ".freq";
    const std::string hdrFileName    = dirName + "/" + inputFileBaseName + ".hdr";
    const std::string codeFileName   = dirName + "/" + inputFileBaseName + ".code";

    // The next several if-statement make sure that the input file, the directory exist
    // and that the output file is writeable.
    //update: switched the order
    if (error_type status; (status = directoryExists(dirName)) != NO_ERROR )
        exitOnError(status, dirName);

    if (error_type status; (status = regularFileExistsAndIsAvailable(inputFullPath)) != NO_ERROR)
        exitOnError(status, inputFullPath); // changed for inputFullPath use


    if (error_type status; (status = canOpenForWriting(wordTokensFileName)) != NO_ERROR)
        exitOnError(status, wordTokensFileName);

    // if sets for the other file errors
    if (error_type st; (st = canOpenForWriting(freqFileName)) != NO_ERROR)
        exitOnError(st, freqFileName);
    if (error_type st; (st = canOpenForWriting(hdrFileName)) != NO_ERROR)
        exitOnError(st, hdrFileName);
    if (error_type st; (st = canOpenForWriting(codeFileName)) != NO_ERROR)
        exitOnError(st, codeFileName);

    std::vector<std::string> words;

    // changed for wordTokensFileName
    fs::path tokensFilePath(wordTokensFileName); // create a file system path using the output file.
    {
        Scanner scn(tokensFilePath);
        if (error_type status; (status = scn.tokenize(words, tokensFilePath)) != NO_ERROR)
            exitOnError(status, wordTokensFileName);
    }


    //part 2 stuff

    //Build BST
    WordBinSearchTree bst;
    bst.bulkInsert(words);

    std::vector<std::pair<std::string,int>> countsLex;
    bst.inorderCollect(countsLex);

    //create placeholders and find BST values
    unsigned height = bst.height();
    std::size_t unique = bst.size();
    std::size_t total  = words.size();
    int minFreq = 0;
    int maxFreq = 0;

    if (!countsLex.empty()) {

        minFreq = maxFreq = countsLex.front().second;

        for (auto& [w,c] : countsLex) {

            if (c < minFreq)
                minFreq = c;

            if (c > maxFreq)
                maxFreq = c;

        }

    }

    //Print values
    std::cout << "BST height: "       << (countsLex.empty()?0:height) << '\n'
              << "BST unique words: " << (countsLex.empty()?0:unique) << '\n'
              << "Total tokens: "     << total                        << '\n'
              << "Min frequency: "    << (countsLex.empty()?0:minFreq)<< '\n'
              << "Max frequency: "    << (countsLex.empty()?0:maxFreq)<< '\n';



    // Sort and write .freq (count desc, word asc; newline even when empty)
    //update: changed for empty line insert
    {
        const std::string freqPath = dirName + "/" + inputFileBaseName + ".freq";
        std::ofstream out(freqPath);

        if (!out)
            exitOnError(UNABLE_TO_OPEN_FILE_FOR_WRITING, freqPath);

        if (countsLex.empty()) {

            out << '\n'; // required: single trailing newline on empty

            if (!out)
                exitOnError(FAILED_TO_WRITE_FILE, freqPath);

        } else {

            auto v = countsLex;
            std::sort(v.begin(), v.end(), [](const auto& a, const auto& b){

                if (a.second != b.second) return a.second > b.second; // count desc

                return a.first < b.first;                              // word asc
            });

            for (const auto& [w, c] : v) {

                out << std::setw(10) << c << ' ' << w << '\n';

                if (!out)
                    exitOnError(FAILED_TO_WRITE_FILE, freqPath);

            }
        }
    }


    // .hdr
    HuffmanTree ht = HuffmanTree::buildFromCounts(countsLex);
    {
        std::ofstream out(hdrFileName);

        if (!out)
            exitOnError(UNABLE_TO_OPEN_FILE_FOR_WRITING, hdrFileName);

        error_type e = ht.writeHeader(out); // single-word to "0", newline even when empty

        if (e != NO_ERROR)
            exitOnError(e, hdrFileName);

    }

    // .code
    {

        std::ofstream out(codeFileName);

        if (!out)
            exitOnError(UNABLE_TO_OPEN_FILE_FOR_WRITING, codeFileName);

        error_type e = ht.encode(words, out, 80);

        if (e != NO_ERROR)
            exitOnError(e, codeFileName);

    }

    return 0;
}
