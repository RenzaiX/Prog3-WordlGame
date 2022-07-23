/*--------------------------------------------
   Program 3: Wordle Reload
    CS 141, Spring 2022, UIC

    The objective of this game is to guess the randomly selected
    word within a given number of attempts. You can select either
    a three or five word board.
    At the conclusion of the game, stats will be displayed.
    Indicators will be given if characters of the user entered
    word are reflected in the guessed word.
  - If the character is in the correct position, the character
    will display as an uppercase value.
  - If the character is within the random word, the character
    will display as a lowercase value.
  - If you enter a character that is not in the word, an asterisk '*'
    will display.

		       
   Course: CS 141, Spring 2022
   System: Visual Studio
   Author: Mykola Turchak
    
 ---------------------------------------------*/

#include <iostream>   // For Input and Output
#include <fstream>    // For file input and output
#include <vector>     // Input file is stored in a vector
#include <cassert>    // For the assert statement used to validate file open
#include <cctype>     // Allows using the tolower() function
#include <ctime>      // Needed to set the random number seed to be the current time

using namespace std;  // So that we don't need to preface cin and cout with std::


//function opens file and checks if the word entered by user is in file, otherwise return false.
bool isValidWord(string word) {
    ifstream inStream;         

    //open file              
    inStream.open("wordlewords.txt");       
    assert( inStream.fail() == false );  

    //turn the whole word to lower case letters
    for (size_t i = 0; i < word.size(); i++)
        word.at(i) = tolower(word.at(i));
    

    string curWord;   

    //check if word is in the file and return true if it is.
    while (inStream >> curWord) {  

        if (curWord == word) {
            inStream.close();
            return true;
        }    
    }
    inStream.close();

    return false;
}



//functuin displays menu
void displayWelcomeMessage()
{
    cout       
        << "Select a menu option: \n"
        << "  1. To play Wordle Reload 3 letter play \n"
        << "  2. To play Wordle Reload 5 letter play \n"
        << "  3. Exit the program \n"
        << "Your choice --> ";

}



//function opens file, adding all words of certain length (depending on user's choice of game) to the vector and then randomly chooses the word that user needs to guess
string readRandomWord(size_t len) {

    //initialze variables
    vector<string> dictionary;
    ifstream inStream;    

    //open file                   
    inStream.open("wordlewords.txt");       
    assert( inStream.fail() == false );     
    
    //clearn dictionary
    dictionary.clear();
    
    string curWord;  

    //move all words to dictionary                      
    while (inStream >> curWord) {   
        if (curWord.size() == len)        
            dictionary.push_back(curWord);
    }
    inStream.close();

    assert(dictionary.size() != 0);

    //get random word out of the whole dicitonary
    return dictionary.at(rand() % dictionary.size());
}



//main function where everything is ran
int main() {
    
    //set random number to be the same
    srand(1);
    
    //outputs rules of the game
    cout << "Program 3: Wordle Reload" << endl
        <<  "CS 141, Spring 2022, UIC" << endl << endl

        << "The objective of this game is to guess the randomly selected "  << endl
        << "word within a given number of attempts. You can select either "  << endl
        << "a three or five word board. " << endl
        << "At the conlusion of the game, stats will be displayed. " << endl
        << "Indicators will be given if characters of the user entered " << endl
        << "word are reflected in the guessed word. " << endl
        << "- If the character is in the correct position, the character " << endl
        << "will display as an uppercase value. " << endl
        << "- If the character is within the random word, the character " << endl
        << "will display as a lowercase value. " << endl 
        << "- If you enter a character that is not in the word, an asterisk '*' " << endl
        << "will display." << endl << endl << endl;

    //innitializing and taking the input
    size_t wins = 0;
    size_t maxStreak = 0;
    size_t curStreak = 0;
    size_t gameState = 0;
    size_t totalTime = 0;
    size_t numOfGames = 0;
    size_t wordLen = 0;
    size_t attempts = 10;
    size_t maxAttempts = 0;
    size_t maxSecs = 0;

    string averageTime = "N/A";
    string randWord; // tolower
    string prevField = "";

    bool isWordGuessed = false;
    vector<char> field(10);
    time_t startTime = 0; //time(NULL);
    
    //while loop that runs until user wins or looses
    while(true) {    

        //entry menu option
        if (gameState == 0) {
            displayWelcomeMessage();

            //taking the input on what user wants to play 
            cin >> gameState;

            //outputing the rules and setting starting values for the game with 3 letter word
            if (gameState == 1) {
                cout << endl << endl << "To get started, enter your first 3 letter word.\n"
                    << "You have 4 attempts to guess the random word.\n"
                    << "The timer will start after your first word entry.\n"
                    << "Try to guess the word within 20 seconds. " << endl;

                maxSecs = 20;
                maxAttempts = 4;
                wordLen = 3;
                prevField = "";
                field.reserve(wordLen);
                randWord = readRandomWord(wordLen);
            } 

            //outputing the rules and setting starting values for the game with 5 letter word
            else if (gameState == 2) {
                cout << endl << endl << "To get started, enter your first 5 letter word.\n"
                    << "You have 6 attempts to guess the random word.\n"
                    << "The timer will start after your first word entry.\n"
                    << "Try to guess the word within 40 seconds. " << endl;

                maxSecs = 40;
                maxAttempts = 6;
                wordLen = 5;
                prevField = "";
                field.reserve(wordLen);
                randWord = readRandomWord(wordLen);
            }
            //outputing overal game information 
            else if (gameState == 3) {
                cout << "Overall Stats:" << endl
                    << "  - You guessed: " << wins << endl
                    << "  - Your longest streak is: " << maxStreak << endl
                    << "  - Average word completion time: " << averageTime << endl
                    << "Exiting program";
                break;
            }
            attempts = maxAttempts;
        }

        //set positions until wordLen to "*"
        for (size_t i = 0; i < wordLen; i++){
            field.at(i) = '*';
        }
        
        //ask user to enter a word
        cout << endl << "Please enter word --> ";

        //start timer here
        if (startTime == 0) {
            startTime = time(NULL);
        }

        //if user runs out of time, reset some values and output loosing message
        if (time(NULL) - startTime > maxSecs) {
            cout << "You ran out of time!";
            totalTime += time(NULL) - startTime;
            numOfGames += 1;
            curStreak = 0;
            gameState = 0;
            continue;
        }

        //innitializing and taking the input
        string word;
        cin >> word;

        //check if word is of right length and output error message if not
        if (word.size() != wordLen) {
            cout << "Invalid word entry - please enter a word that is " << wordLen << " characters long." << endl;
            continue;
        }

        //check if word is valid and output error message if not
        if (!isValidWord(word)) {
            cout << "\nNot a playable word, please select another word." << endl;
            continue;
        }

        //each letter is being compared to all other letter in randWord. If letters match we check their positions. If positions match, we turn that letter to upercase, then add it to the vector.
        for (int i = 0; i < word.size(); i++) {
            char c1 = tolower(word.at(i));
            for (int j = 0; j < randWord.size(); j++) {
                char c2 = tolower(randWord.at(j));
                if (c1 == c2) {
                    if (i == j) 
                        c1 = toupper(c1);
                    field.at(i) = c1;
                }              
            }
        }
        
        
        //adding all of users entries from last rounds to the string
        prevField += " ";
        for (size_t i = 0; i < wordLen; i++)
            prevField += string(" [ ") + field.at(i) + " ]";
        prevField += "\n";
        
        //outprint two endlines
        cout << "\n\n";
        cout << prevField;

        attempts--;

        bool isDone = true; 
        
        //check if all letters are uppercase in the vector
        for (size_t i = 0; i < wordLen; i++)
            isDone = isDone && isupper(field.at(i));

        //if game is finished, adjust values and output winning message
        if (isDone) {
            totalTime += time(NULL) - startTime;
            numOfGames += 1;
            averageTime = to_string(totalTime/numOfGames);
            cout << endl << "Nice Work!  You guessed the correct word\n"
            << "  - You completed the board in: " << averageTime << " seconds.\n"  
            << "  - It took you " << (maxAttempts - attempts) << "/" << maxAttempts << " attempts.\n";
            gameState = 0;
            wins += 1;
            curStreak += 1;

            //check if there is a new max streak
            if (curStreak > maxStreak)
                maxStreak = curStreak;
            continue;
        }

        //checks if user ran out of attempts and outputs message if he did
        if (attempts <= 0) {
            totalTime += time(NULL) - startTime;
            numOfGames += 1;
            gameState = 0;
            curStreak = 0;
            startTime = 0;
            cout << "Maximum amount of attempts have been reached. Try again.\n";
        }
    }
    
    return 0;
}
