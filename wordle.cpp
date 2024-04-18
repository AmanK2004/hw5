#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
// #include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
std::set<std::string> wordleHelper(std::string in, std::string floating, int pos);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here 
    // Call wordleHelper to generate possible words
    std::set<std::string> words = wordleHelper(in, floating, 0);

    // Initialize the result set
    std::set<std::string> result;

    // Iterate over the generated words
    for (const std::string& word : words) {
        // If the word is in the dictionary, insert it into the result set
        if (dict.find(word) != dict.end()) {
            result.insert(word);
        }
    }

    // Return the result set
    return result;
}

// Define any helper functions here
std::set<std::string> wordleHelper(std::string in, std::string floating, int pos) {
    std::set<std::string> words;

    // Base case: If the input string is empty, return an empty set
    if (in.empty()) {
        return words;
    }

    // Count the number of dashes in the input string
    unsigned int counter = 0;
    for (char c : in) {
        if (c == '-') {
            counter++;
        }
    }

    // If there are no dashes, the input itself is a valid word, insert it into the set and return
    if (counter == 0) {
        words.insert(in);
        return words;
    }

    // If the current character is a dash, replace it with characters from the floating string
    if (in[pos] == '-') {
        // Iterate over each character in the floating string
        for (unsigned int i = 0; i < floating.length(); i++) {
            char c = floating[i];
            // Replace the dash with the current character
            in[pos] = c;
            // Create a new floating string without the current character
            std::string newFloating = floating.substr(0, i) + floating.substr(i + 1);
            // Recursively call wordleHelper with the updated string and position
            std::set<std::string> words_temp = wordleHelper(in, newFloating, pos + 1);
            // Insert the generated words into the set
            words.insert(words_temp.begin(), words_temp.end());
            // Reset the character back to a dash for the next iteration
            in[pos] = '-';
        }

        // If there are still more dashes to fill and characters in the alphabet, fill them
        if (floating.length() < counter) {
            // Iterate over each lowercase alphabet character
            for (int i = 'a'; i <= 'z'; ++i) {
                char c = static_cast<char>(i);
                // Check if the character is not in the floating string
                bool notInFloating = true;
                for (char f : floating) {
                    if (f == c) {
                        notInFloating = false;
                        break;
                    }
                }
                // If the character is not in the floating string, use it
                if (notInFloating) {
                    // Replace the dash with the current character
                    in[pos] = c;
                    // Recursively call wordleHelper with the updated string and position
                    std::set<std::string> words_temp = wordleHelper(in, floating, pos + 1);
                    // Insert the generated words into the set
                    words.insert(words_temp.begin(), words_temp.end());
                    // Reset the character back to a dash for the next iteration
                    in[pos] = '-';
                }
            }
        }
    } else {
        // If the current character is not a dash, move to the next position recursively
        std::set<std::string> words_temp = wordleHelper(in, floating, pos + 1);
        // Insert the generated words into the set
        words.insert(words_temp.begin(), words_temp.end());
    }

    // Return the set of possible words
    return words;
}