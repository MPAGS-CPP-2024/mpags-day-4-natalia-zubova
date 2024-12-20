#ifndef MPAGSCIPHER_PLAYFAIRCIPHER_HPP
#define MPAGSCIPHER_PLAYFAIRCIPHER_HPP

#include "CipherMode.hpp"

#include <cstddef>
#include <string>
#include <vector>
#include <map>

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */
class PlayfairCipher {
  public:
    /**
     * \brief Create a new PlayfairCipher, converting the given string into the key
     *
     * \param key the string to convert into the key to be used in the cipher
     */
    explicit PlayfairCipher(const std::string& key);

    /**
     * \brief Set the key to be used in the cipher
     * 
     * \param key the key to use in the cipher
     * 
     * \return void
     */
    void setKey(const std::string& key);

    /**
     * \brief Apply the cipher to the provided text
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    std::string applyCipher(const std::string& inputText,
                            const CipherMode cipherMode) const;
  private:
    std::string key_{""};
    const std::vector<char> alphabet_{
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
   
    const std::size_t alphabetSize_{alphabet_.size()};

    const std::size_t gridSize_{5};
    
    /// Lookup table to go from the character to the coordinates

    /// Type definition for the coordinates in the 5x5 table
    using PlayfairCoords = std::pair<std::size_t, std::size_t>;

    /// Lookup table to go from the character to the coordinates
    std::map<char, PlayfairCoords> charLookup_;

    /// Lookup table to go from the coordinates to the character
    std::map<PlayfairCoords, char> coordLookup_;

};

#endif // MPAGSCIPHER_PLAYFAIRCIPHER_HPP