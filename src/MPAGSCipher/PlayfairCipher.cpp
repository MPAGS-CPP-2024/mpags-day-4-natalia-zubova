#include "PlayfairCipher.hpp"

#include <algorithm>
#include <iostream>
#include <string>

using namespace std;

PlayfairCipher::PlayfairCipher(const std::string& key)
{
    setKey(key);
}

std::string PlayfairCipher::applyCipher(const std::string& inputText,
                                        const CipherMode cipherMode) const
{
    std::string outputText{inputText};

    // Change J -> I

    // lambda function to change J to I
    auto changeJtoI = [](char c) {
        if (c == 'J') {
            return 'I';
        }
        return c;
    };

    // Use transform to change all Js to I
    std::transform(outputText.begin(), outputText.end(), outputText.begin(),
                   changeJtoI);

    // If repeated chars in a digraph add an X or Q if XX
    for (size_t i = 0; i < outputText.size(); i += 2) {
        if (outputText[i] == outputText[i + 1]) {
            if (outputText[i] == 'X') {
                outputText.insert(i + 1, "Q");
            } else {
                outputText.insert(i + 1, "X");
            }
        }
    }

    // if the size of input is odd, add a trailing Z
    if (outputText.size() % 2 != 0) {
        outputText += 'Z';
    }

    // Loop over the input in Digraphs
    auto iter = outputText.begin();
    while (iter != outputText.end()) {
        switch (cipherMode) {
            case CipherMode::Encrypt: {
                // - Find the coords in the grid for each digraph
                auto coordsFirstLetter = charLookup_.at(*iter);
                auto coordsSecondLetter = charLookup_.at(*(iter + 1));

                // RULES:
                // 1. If letters are on the same row,
                // replace with letter to the right
                // 2. If in same column, replace with
                // letters directly below
                // 3. If they form a rectangle, replace
                // with ones from corner on the same row

                if (coordsFirstLetter.first == coordsSecondLetter.first) {
                    // same row
                    coordsFirstLetter.second =
                        (coordsFirstLetter.second + 1) % gridSize_;
                    coordsSecondLetter.second =
                        (coordsSecondLetter.second + 1) % gridSize_;
                } else if (coordsFirstLetter.second ==
                           coordsSecondLetter.second) {
                    // same column
                    coordsFirstLetter.first =
                        (coordsFirstLetter.first + 1) % gridSize_;
                    coordsSecondLetter.first =
                        (coordsSecondLetter.first + 1) % gridSize_;
                } else {
                    // form a rectangle
                    std::swap(coordsFirstLetter.second,
                              coordsSecondLetter.second);
                }
                
                // - Find the letter associated with the new coords
                *iter = coordLookup_.at(coordsFirstLetter);
                *(iter + 1) = coordLookup_.at(coordsSecondLetter);

                iter += 2;
                break;
            }
            case CipherMode::Decrypt: {
                // - Find the coords in the grid for each digraph
                auto coordsFirstLetter = charLookup_.at(*iter);
                auto coordsSecondLetter = charLookup_.at(*(iter + 1));


                // RULES:
                // 1. If letters are on the same row,
                // replace with letter to the left
                // 2. If in same column, replace with
                // letters directly above
                // 3. If they form a rectangle, replace
                // with ones from corner on the same row

                if (coordsFirstLetter.first == coordsSecondLetter.first) {
                    // same row
                    coordsFirstLetter.second =
                        (coordsFirstLetter.second + 4) % gridSize_;
                    coordsSecondLetter.second =
                        (coordsSecondLetter.second + 4) % gridSize_;
                } else if (coordsFirstLetter.second ==
                           coordsSecondLetter.second) {
                    // same column
                    coordsFirstLetter.first =
                        (coordsFirstLetter.first + 4) % gridSize_;
                    coordsSecondLetter.first =
                        (coordsSecondLetter.first + 4) % gridSize_;
                } else {
                    // form a rectangle
                    std::swap(coordsFirstLetter.second,
                              coordsSecondLetter.second);
                }

                // - Find the letter associated with the new coords
                *iter = coordLookup_.at(coordsFirstLetter);
                *(iter + 1) = coordLookup_.at(coordsSecondLetter);

                iter += 2;
                break;
            }
            default: {
                break;
            }
        }
    }

    // return output text
    return outputText;
}

void PlayfairCipher::setKey(const std::string& key)
{
    // store the original key
    key_ = key;

    // append the alphabet
    key_ += std::string(alphabet_.cbegin(), alphabet_.cend());

    // make sure the key is upper case
    std::transform(key_.cbegin(), key_.cend(), key_.begin(), ::toupper);

    // remove non-alpha characters

    // lambda function to check if a character is not alphabetic
    auto notAlpha = [](char c) { return !isalpha(c); };

    // remove_if will move all non-alpha characters to the end of the string
    auto iter = std::remove_if(key_.begin(), key_.end(), notAlpha);

    // actually erase
    key_.erase(iter, key_.end());

    // Change J -> I

    // lambda function to change J to I
    auto changeJtoI = [](char c) {
        if (c == 'J') {
            return 'I';
        }
        return c;
    };

    // transform to change all Js to I
    std::transform(key_.cbegin(), key_.cend(), key_.begin(), changeJtoI);

    // remove duplicated letters
    std::string lettersSeen;

    // lambda function to check if a character is a duplicate
    auto notDuplicate = [&lettersSeen](char c) {
        if (lettersSeen.find(c) == std::string::npos) {
            lettersSeen += c;
            return false;
        }
        return true;
    };

    // remove_if will move all duplicate characters to the end of the string
    auto iterDup = std::remove_if(key_.begin(), key_.end(), notDuplicate);
    key_.erase(iterDup, key_.end());

    // store the coords of each letter
    // into two maps: one for the letter to coords,
    // one for coords to letter
    for (size_t i = 0; i < key_.size(); ++i) {
        size_t row = i / gridSize_;
        size_t col = i % gridSize_;
        auto coords = std::make_pair(row, col);
        charLookup_[key_[i]] = coords;
        coordLookup_[coords] = key_[i];
    }
}