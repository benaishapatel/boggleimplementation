#include "boggleplayer.h"
#include "boggleboard.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
     * Constructs a BogglePlayer with an uninitialized board and lexicon.
     * Both must be initialized with data before use.
     */
    BogglePlayer::BogglePlayer() {
        this->lexicon = new Lexicon();
        lexicon_built = false;
        board_built = false;
        rows = 0;
        cols = 0;
    }

    bool BogglePlayer::lexIsBuilt() {
        return lexicon_built;
    }

    /**
     * Initializes the BogglePlayer's Lexicon using the supplied word
     * list. Words are inserted in a case-insensitive manner.
     */
    void BogglePlayer::buildLexicon(const std::set<std::string> &word_list) {
        
        this->word_list = word_list;
        // Initialize lexicon
        if(lexicon_built) 
            delete lexicon;

        for(std::set<std::string>::const_iterator it = word_list.begin(); it != word_list.end(); ++it) {
            lexicon->insert(*it);
        }
        lexicon_built = true;
    }


    /**
     * Initializes the BogglePlayer's internal board representation
     * using the supplied multidimentional array.
     */
    void BogglePlayer::setBoard(unsigned int rows, unsigned int cols,
            std::string **diceArray) {

        if(board_built) {
            board.clear();
        }
 
        string toAdd;

        this->rows = rows;
        this->cols = cols;

        // convert all to lowercase and add to board vector
        for( int i = 0; i<this->rows; i++ ) {
            for( int j = 0; j<this->cols; j++ ) {
                toAdd = setLowerCase(diceArray[i][j]);
                board.push_back(BoardPos(toAdd));
            }
        }

        board_built = true;
    }
    
    /**
     * Populates the supplied set with the words in the BogglePlayer's
     * lexicon that appear on the board and are longer than
     * minimum_word_length.
     *
     * Returns false if either the board or the lexicon has not been
     * initialized. Returns true otherwise.
     */
    bool BogglePlayer::getAllValidWords(unsigned int minimum_word_length,
            std::set<std::string> *words) {

        if(!board_built)
            return false;
        if(!lexicon_built)
            return false;

        LexNode *curr = lexicon->getRoot();

        for (unsigned int r = 0; r < rows; r++) {
            for (unsigned int c = 0; c < cols; c++) {
                
                int i = mapIndex(r,c);
                
                board[i].setVisited(true);
                //call helper function
                getWords(r, c, curr, "", words, minimum_word_length);
                
                for (unsigned int i = 0; i < board.size(); i++) {
                    board[i].setVisited(false);
                }
            }

        }
        
        return true;
    }

    /* Helper function for getAllValidWords */
    void BogglePlayer::getWords(int row, int col, LexNode *curr, std::string 
        word_matched, std::set<std::string> *words, unsigned int minimum_word_length)
    {
      
        int index = mapIndex(row, col);
        string text = board[index].getText();
  
        // checks if text is in lexicon
        for (unsigned int i = 0; i < text.size(); i++) {
            curr = curr->getChildren(text[i]);
            if (curr == NULL) {
                board[index].setVisited(false);
                return;
            }
        }
 
        word_matched.append(text);

        // check if word found
        if (curr->isEndOfWord() == true && word_matched.size() >= minimum_word_length) 
        {
            words->insert(word_matched);
        }

        //loop through neighbors
        for (int r = -1; r <= 1; r++) {
            for (int c = -1; c <= 1; c++) {

                int nextR = row + r;
                int nextC = col + c;
      
                if (nextR < rows && nextC < cols) {
                    int nextI = mapIndex(nextR, nextC);
                    if (!board[nextI].getVisited()) {
                        //set visited true and call getWords recursively
                        board[nextI].setVisited(true);
                        getWords(nextR, nextC, curr, word_matched, words, minimum_word_length);

                    }
                }
            }

        }
        board[index].setVisited(false);
    }

        
    /**
     * Determines if the given word is in the BogglePlayer's lexicon.
     * The lexicon is searched in a case-insensitive fashion.
     *
     * Returns true if the word is in the lexicon, and false if not.
     */
    bool BogglePlayer::isInLexicon(const std::string &word_to_check) {
        if(lexicon_built)
            return lexicon->find(word_to_check);

        return false;
    }

    /**
     * Determines if the given word is on the BogglePlayer's board.
     * The board is searched in a case-insensitive fashion.
     *
     * Returns a vector of integers representing the positions on the board
     * that make up the word, if the word exists on the board. Otherwise,
     * returns an empty vector.
     */

    std::vector<int> BogglePlayer::isOnBoard(const std::string &word_to_check) {

        vector<int> returnVector;
        
        string wordtoCheck = setLowerCase(word_to_check);
        if (board_built == false)
            return returnVector;

        bool found = false;

        //go over all board pos with iterator and set all visited to false
        for(auto iterator = board.begin(); iterator != board.end(); ++iterator) {
           (*iterator).setVisited(false); 
        }

        for(unsigned int x = 0; x < rows; ++x)
        {
            for(unsigned int y = 0; y < cols; ++y)
            {
                found = findWord(x, y, wordtoCheck, 0, &returnVector);
                if(found == true) {
                    return returnVector;
                }  
            }
        }
            
        return returnVector;

    }

    /**
     * Returns a custom board for the boggle ui. The board is loaded
     * from the file custboard.txt in the current working directory.
     * The input format is expected to follow that outlined in README_brd.
     */
    void BogglePlayer::getCustomBoard(std::string **&new_board,
            unsigned int *rows, unsigned int *cols) {
        setBoard(*rows, *cols, new_board);
    }

    /* Helper method for setBoard
     * sets the board diceArray to lowercase 
     * uses standard c++ function strlen */

     std::string BogglePlayer::setLowerCase(std::string string) {
        std::string newString;
        int length = string.length();
        for( int i = 0; i<length; i++ ) {
            newString += tolower(string[i]);
        }
        return newString;
     }

    /* Helper method to map the index of the current letter to the 
     * board vector */
    int BogglePlayer::mapIndex(int row, int col) {
        if(row < 0 || col < 0 || row >= this->rows || col >= this->cols) {
            return -1;
        }
        else {
            return row*(this->cols) + col;
        }
    }

    /* Helper method used in isOnBoard to see if the word is actually there
     * calls recursively
     * returns true if word is found and matches
     */

    bool BogglePlayer::findWord(unsigned int row, unsigned int col, const string &word_to_check, 
            unsigned int word_letter, vector<int> *positions) 
    {

        if(row>=rows || col >= cols){
            return false;
        }
        if (word_letter > word_to_check.length()){
            return false;
        }
        if (word_letter == word_to_check.length()){
            return true;
        }

        bool found = 0;

        //array to go through neighbors
        int pos[8][2] = {-1,-1,-1,0,-1,1,0,-1,0,1,1,-1,1,0,1,1};
        for(int i = 0; i<8; i++){

            //make current r and c the neighbors before calling
            int r = row + pos[i][0];
            int c = col + pos[i][1];

            //find index
            int index = mapIndex(r,c);

            if(board[index].getVisited()==false){

                string curStr = board[index].getText();

                int size = curStr.size();

                string str = word_to_check.substr(word_letter, size);
                //see if string is equal
                if(curStr.compare(str)==0){ 
                    board[index].setVisited(true);
                    positions->push_back(index); 
                    word_letter+=size;
                    found = findWord(r,c,word_to_check, word_letter, positions);
                    if(found){
                        return true;
                    }
                    else{
                        //reset visited and pop
                        board[index].setVisited(false);
                        positions->pop_back();
                        word_letter-=size;
                    }
                }
            }
        }
        return false;
    }





