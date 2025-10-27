//
// Created by Ali Kooshesh on 9/27/25.
//

#include "Scanner.hpp"

#include <utility>
#include <iostream>
#include <fstream>

#include <cctype> // i add for tolower

#include "utils.hpp"

// Scanner
// Initializes the Scanner with a given path.
// passes a path ending in ".tokens",
// read from the corresponding ".txt" file in the same directory
// Example: input_output/ch01.tokens → input_output/ch01.txt
Scanner::Scanner(std::filesystem::path inputPath) {

    // You complete this...

    inputPath_ = std::move(inputPath);

        if (inputPath_.has_parent_path()) {

            auto dir  = inputPath_.parent_path();
            auto base = inputPath_.stem().string();

            inputPath_ = dir / (base + ".txt");

        }

}

// Returns true if 'c' is a valid ASCII character (0–127).
bool Scanner::is_ascii(unsigned char c) {

    return c < 128;
}

// Converts a single character to lowercase.
char Scanner::to_lower(unsigned char c) {

    return std::tolower(c);
}

// Removes leading and trailing apostrophes from a token.
void Scanner::trim_edges(std::string& s) {

    if (!s.empty() && s.front() == '\'')
        s.erase(s.begin());

    if (!s.empty() && s.back() == '\'')
        s.pop_back();

}

// readWord()
// Reads the next valid token from the input stream,
// Returns an empty string when no more tokens remain.
//Valid tokens: [a–z]+ or [a–z]+’[a–z]+, Lowercase only, drop non-ASCII bytes,
//handle apostrophes followed by a letter
std::string Scanner::readWord(std::istream& in) {

    std::string tok;
    int ci;

    // find first char
    while ((ci = in.get()) != -1) {

        char c = ci;

        // Skip any non-ASCII characters
        if (!is_ascii(c))
            continue;

        c = to_lower(c);

        //if lower, then is found
        if (c >= 'a' && c <= 'z')
        {

            tok.push_back(c);
            break;

        }

    }

    // EOF reached
    if (tok.empty())
        return {};

    //check next chars
    while ((ci = in.peek()) != -1) {

        char c = ci;

        if (!is_ascii(c))
        {

            in.get();
            break;

        }

        c = to_lower(c);

        // connect chars in token string
        if (c >= 'a' && c <= 'z') {

            in.get();

            tok.push_back(c);

        //if apostrophe is found
        } else if (c == '\'') {

            in.get(); // consume apostrophe

            int ni = in.peek(); // check the next char

            if (ni == -1)
                break;

            char n = ni;

            if (!is_ascii(n))
                break;

            n = to_lower(n);

            if (n >= 'a' && n <= 'z') {

                tok.push_back('\'');

                in.get(); // takes the following letter

                tok.push_back(n);

            } else {

                break;

            }

        } else {

            break; // separator

        }

    }

    //remove stray apostrophes
    trim_edges(tok);

    return tok;
}

// tokenize()
// Reads tokens from the file into the provided vector,
// Returns NO_ERROR on success or an error_type value if could not be opened or found
error_type Scanner::tokenize(std::vector<std::string>& words) {

    words.clear();

    if (inputPath_.has_parent_path() && !std::filesystem::exists(inputPath_.parent_path()))
        return DIR_NOT_FOUND;

    if (!std::filesystem::exists(inputPath_))
        return FILE_NOT_FOUND;

    std::ifstream in(inputPath_, std::ios::binary);

    if (!in)
        return UNABLE_TO_OPEN_FILE;

    //calls readWord till empty
    while (true) {

        std::string w = readWord(in);

        if (w.empty())
            break;

        words.push_back(std::move(w));

    }

    return NO_ERROR;
}

// tokenize()
//writes one token per line to the output file, returns an error_type value if writing fails
error_type Scanner::tokenize(std::vector<std::string>& words, const std::filesystem::path& outputFile) {

    auto err = tokenize(words);

    if (err!=NO_ERROR)
        return err;

    std::ofstream out(outputFile);

    if (!out)
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;


      if (words.empty()) {

          // single trailing newline on empty
                out << '\n';

                if (!out)
                    return FAILED_TO_WRITE_FILE;

      } else
        {

          //display 1 token per line
          for (const auto& w : words)
            {

                out<<w<<'\n';

                if (!out)
                    return FAILED_TO_WRITE_FILE;

            }

        }

    return NO_ERROR;
}
