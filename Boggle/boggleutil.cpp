    #include "boggleutil.h"

    /**
     * Constructs a BoardPos with the given text. Text must have been
     * sanitized by the caller.
     */
    BoardPos::BoardPos(const std::string &text) : text(text){
        visited = false;
    }

    /**
     * Returns the characters on this BoardPos.
     */
    const std::string BoardPos::getText() const {
        return text;
    }
    
    /**
     * Returns whether this BoardPos has been visited or not.
     */
    bool BoardPos::getVisited() const {
        return visited;
    }

    /**
     * Sets whether this BoardPos has been visited or not.
     */
    void BoardPos::setVisited(bool visitedd){
        visited = visitedd;
        return;
    }

    LexNode * Lexicon::getRoot() {
        return this->root;
    }

    /* Method to get all the children of a LexNode attatched to 
     * the char key */
    LexNode* LexNode::getChildren(char key) {
        auto itr = children.find(key);
            if (itr == children.end()) {
                return NULL;
            }
            else {
                return itr->second;
            }
    }

    /* Method to insert a word to Lexicon */
    void Lexicon::insert (const std::string &word) {

        LexNode * curr;

        // Start with root
        curr = root;

        // Cast length of the word to an int for easier computation
        int length = (int)word.length(); 


        // for every letter in word, add child LexNode for letters not present
        for(int i = 0; i<length; i++) {

            char letter = word[i];

            // search for the letter in the children map
            auto childIterator = curr->children.begin();
            childIterator = curr->children.find(letter);
            if(childIterator != curr->children.end()) { // if the iterator does not return end of map, the letter exists in its map
                curr = childIterator->second;           // access the LexNode associated with that letter
            }else{                                      // Else, the letter doesn't exist yet and we must add a lex node for it in curr's child map
                LexNode * newLexNode = new LexNode();

                // If it is the last letter, set end_of_word to true
                if(i == length-1)
                    newLexNode->end_of_word = true;

                // Insert the new node into the map as a pair with the letter
                curr->children.insert(make_pair(letter, newLexNode));

                // Point to the new node
                curr = newLexNode;
            }
        }


    }

    /* Method to check if a word exists in the Lexicon */
    bool Lexicon::find (const std::string &word) {
        if(!isPrefix(word))
            return false;

        LexNode * curr = root;
        int length = (int)word.length();

        for(int i = 0; i<length; i++) {
            char letter = word[i];

            auto childIterator = curr->children.begin();
            childIterator = curr->children.find(letter);

            // Check if the LexNode for the letter is not present return false
            if(childIterator == curr->children.end()) {
                return false;
            }else if(childIterator->first == letter) {
                // if the letter is present, set curr to be the LexNode corresponding and continue
                curr = childIterator->second;
            }
        }

            if(curr->end_of_word)
                return true;
            else
                return false;

    }


    bool Lexicon::isPrefix(std::string word) {

        // Iterate through each letter in word
        // once we reach the end of word 

        LexNode * curr = root;
        int length = (int)word.length();

        for(int i = 0; i<length; i++) {
            char letter = word[i];

            auto childIterator = curr->children.begin();
            childIterator = curr->children.find(letter);

            if(!(childIterator->first))
                return false;
        }

    return true;

    }

    Lexicon::~Lexicon() {
        clearLexicon();
        delete root;
        root = nullptr;
    }

    /* Helper method to recursively remove all nodes */
    void LexNode::removeNodes(LexNode * n){
        for(auto it = n->children.begin(); it != n->children.end(); it++){
            n->removeNodes(it->second);
        }
        delete n;
    }

    /* Helper method for the destructor */
    void Lexicon::clearLexicon(){
        for(auto it = root->children.begin(); it != root->children.end(); it++){
            root->removeNodes(it->second);
        }
        root->children.clear();
    }


