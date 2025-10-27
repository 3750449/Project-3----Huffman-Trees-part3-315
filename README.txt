## Student Information

**Name:** Desmond Farley-Williams
**Student ID:** 008508702
**Repository:**https://github.com/3750449/Project-3----Huffman-Trees-part3-315.git

---

## Collaboration & Sources

- **cppreference.com** — consulted for `std::fstream`, `std::tolower`, `std::filesystem`
- **geeksforgeeks.org** — cross-checked C++ I/O examples and syntax

---

## Implementation Details — **Project 3, Part 3**


Implements **Scanner → BST → PriorityQueue (vector+sort) → HuffmanTree → Encoder**, producing:
- `<base>.tokens`
- `<base>.freq`
- `<base>.hdr`  *(preorder of leaves; `word<space>code` per line; always newline at end)*
- `<base>.code` *(ASCII `0/1` only; hard wrap at exactly 80 chars/line; final newline)*

**Determinism & Rules**
- Tokenization matches spec: lowercase ASCII; tokens are `[a–z]+` or `[a–z]+'[a–z]+`; separators include digits, whitespace, punctuation, hyphens/dashes, and any non-ASCII byte
- BST inorder returns `(word,count)` in **lexicographic order**.
- `.freq` sorted by **count descending**, tie by **word ascending**, formatted with `std::setw(10) << count << ' ' << word`
- Priority queue is implemented as a **vector kept sorted** by the same comparator (so **min is at back**)
- Huffman construction always attaches **left=a (0), right=b (1)** when merging the two minima
- **Single-word edge case:** that word’s code is `"0"`
- **Empty input:** all outputs exist and contain exactly one trailing newline

### Files Implemented

- `Scanner.hpp` / `Scanner.cpp` — tokenization (file writer)
- `BinSearchTree.hpp` / `BinSearchTree.cpp` — word frequency for BST
- `PriorityQueue.hpp` / `PriorityQueue.cpp` — vector + sort (simulated PQ)
- `utils.hpp` / `utils.cpp` — error codes, filesystem checks, error reporting
- `main.cpp` — driver wiring modules and writing outputs
- `CMakeLists.txt` — build configuration
- `HuffmanTree.hpp` / `HuffmanTree.cpp` — build Huffman tree, write header

### Scanner

- **Normalization rules**

  - Lowercase ASCII only.
  - Tokens match: `[a–z]+` **or** `[a–z]+'[a–z]+` (apostrophes only **inside**)
  - Separators: digits, whitespace, punctuation/symbols, all hyphens/dashes, any non-ASCII byte
  - Drop leading/trailing apostrophes (e.g., `'tis` → `tis`, `rock-n-roll` → `rock`, `n`, `roll`)
  - `tokenize(vector<string>&)` → in-memory tokens
  - `tokenize(vector<string>&, path outputFile)` → calls in-memory version, writes one token per line
  - file ends with a single trailing newline (even when empty)
  - `readWord(istream&)` → scanner primitive implementing the rules

### BST (word → count)

- **Node:** `word`, `count`, `left`, `right`
- **Insert:** `insert`, `bulkInsert` (duplicates increment count)
- **Traversal:** `inorderCollect` → `vector<pair<string,int>>` in ascending order
- **Metrics:** `height()` (empty = 0), `size()`

### PriorityQueue (vector+sort stand-in)

- Builds from BST’s `(word,count)` vector
- Sorted invariant `.freq`:
- **Primary:** count **descending** (alphabetical)

**HuffmanTree**
- `buildFromCounts(countsLex)` — build using the PQ loop; **left=0**, **right=1**; parent’s `key_word = min(left.key_word, right.key_word)`
- `writeHeader(os)` — **preorder over leaves**; line is `word<space>code\n`; if tree empty, write a single `\n`
- `encode(tokens, os, 80)` — builds codebook once; streams codes; **wraps at 80 columns**, always ends with a newline

---

## Outputs

- **`<base>.tokens`**
- One token per line
- **Ends with exactly one trailing newline** (even if the input is empty)

- **`<base>.freq`**
- Each line: `std::setw(10) << count << ' ' << word << '\n'`
- Sorted by count desc, tie by word asc
- Ends with newline, including the empty case (file is a single `\n`)

**`<base>.hdr`**
- **Preorder over leaves**; `word<space>code`
- **Single-word** → that word gets code `"0"`
- Always ends with a trailing newline; empty → single `\n`

**`<base>.code`**
- ASCII `0/1` only; **hard wrap exactly 80 chars per line**
- Always ends with a trailing newline; empty tokens → single `\n`

### Required stdout (exact labels/order)

BST height: H
BST unique words: U
Total tokens: T
Min frequency: MIN
Max frequency: MAX

Empty input defines:

H = 0, U = 0, T = 0, MIN = 0, MAX = 0

---

## Testing & Status

### Sample run (The Bells)

BST height: 12
BST unique words: 49
Total tokens: 83
Min frequency: 1
Max frequency: 11

**Excerpt of `<base>.freq` (format + ordering):**

    11 bells
    11 the
     4 of
     3 a
     3 time
     3 tinkle
     2 from
     2 in
     2 that
     2 to
     2 with
     1 air
     1 all
     1 allan
     1 and
     1 crystalline
     1 delight
     1 edgar
     1 foretells
     1 hear
     1 heavens
     1 how
     1 icy
     1 jingling
     1 keeping
     1 melody
     1 merriment
     1 musically
     1 night
     1 oversprinkle
     1 poe
     1 rhyme
     1 runic
     1 s
     1 seem
     1 silver
     1 sledges
     1 so
     1 sort
     1 stars
     1 their
     1 they
     1 tinkling
     1 tintinnabulation
     1 twinkle
     1 wells
     1 what
     1 while
     1 world

### Empty-input test

Create `input_output/empty.txt` (0 bytes), then run. Expect:

- Stdout:

BST height: 0
BST unique words: 0
Total tokens: 0
Min frequency: 0
Max frequency: 0

- Files:

- `empty.tokens` → **exactly one newline**
- `empty.freq` → **exactly one newline**
- `empty.hdr`  → **exactly one newline**
- `empty.code` → **each is a single newline**

---

## How to Compile & Run

### Plain g++

g++ -std=c++20 -Wall *.cpp -o huff_phase3
./huff_phase3 <file>.txt

mkdir build && cd build
cmake ..
make
./huff_phase3 <file>.txt

