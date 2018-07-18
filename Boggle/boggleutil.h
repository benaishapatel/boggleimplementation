// Kyle Barron-Kraus <kbarronk>

#ifndef BOGGLEUTIL_H
#define BOGGLEUTIL_H

#include <cctype>
#include <algorithm>
#include <map>
#include <stdlib.h>
#include <memory>
#include <string>
#include <utility>
#include <stdio.h>

using namespace std;

/**
 * Class that represents a position on the Boggle Board.
 *
 * Each position contains its sanitized string that represents the
 * characters on that board position, as well as a boolean that
 * marks the board position as visited by a given search algorithm.
 */
class BoardPos {
  public:
    /**
     * Constructs a BoardPos with the given text. Text must have been
     * sanitized by the caller.
     */
    BoardPos(const std::string &text);

    /**
     * Returns the characters on this BoardPos.
     */
    const std::string getText() const;
    
    /**
     * Returns whether this BoardPos has been visited or not.
     */
    bool getVisited() const;

    /**
     * Sets whether this BoardPos has been visited or not.
     */
    void setVisited(bool visitedd);

  private:
    /**
     * The characters that this BoardPos contains.
     */
    const std::string text;

    /**
     * Whether this BoardPos has been visited or not.
     */
    bool visited;
};

/* Private class for a node in the Lexicon */
class LexNode {

friend class Lexicon;

public:
    bool isEndOfWord(){ return end_of_word; }

    LexNode() {
        end_of_word = false;
    }


    /* Method to return the children map of the node */
    LexNode* getChildren(char key); 
   
    /* Helper method for destructor; recursively removes nodes*/
    void removeNodes(LexNode * n);
    
private:
    // Map of letters to the child LexNodes nodes corresponding to those letters
    std::map <char, LexNode *> children;

    // A field to indicate end of word
    bool end_of_word;

}; // LexNode

/**
 * Implement class for Multiway Trie
 */

 class Lexicon {

 friend class LexNode;

 public:

    // Constructor
    Lexicon(){
        root = new LexNode();
    }

    ~Lexicon();

    /* Helper method for destructor*/
    void clearLexicon();

    /* Method to return the root */
    LexNode * getRoot();

    // Method to insert a word to Lexicon
    void insert(const std::string &word);

    // Method to check if a word exists in the Lexicon
    bool find(const std::string &word);

    /* Method to determine if a word is a prefix or not */
    bool isPrefix(std::string word);

 private:

    // Pointer to the root of multiway-trie
    LexNode *root;

}; // Lexicon



#endif // BOGGLEUTIL_H


