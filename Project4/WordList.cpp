#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;

private:
	MyHash<string, vector<string>> m_wordListHash;

	string showLetterPattern(string word) const;
};


bool WordListImpl::loadWordList(string filename)
{
	m_wordListHash.reset();

	ifstream infile(filename);
	if (!infile)		        // Did opening the file fail?
		return false;

	std::string s;
	bool isWord = true;
	while (getline(infile, s))
	{
		for (int i = 0; i < s.size(); i++)
		{
			if (!(isalpha(s[i]) || s[i] == '\''))
				isWord = false;
		}

		if (isWord)
		{
			string letterPatternKey = showLetterPattern(s);

			vector<string>* vec = m_wordListHash.find(letterPatternKey);

			if (vec == nullptr)
			{
				vec = new vector<string>;
				vec->push_back(s);
			}
			else
			{
				vec->push_back(s);
			}

			m_wordListHash.associate(letterPatternKey, *vec);
		}

		isWord = true;
	}

    return true;
}

string WordListImpl::showLetterPattern(string word) const
{
	string alreadySeen;
	string letterPattern;
	string labels = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int indexOfLabel = 0;
	bool checkedCurrent = false;

	for (int i = 0; i < word.size(); i++)
	{
		checkedCurrent = false;

		if (alreadySeen == "")
		{
			alreadySeen += word[i];
			letterPattern += labels[indexOfLabel];
			indexOfLabel++;
			continue;
		}

		for (int j = 0; j < alreadySeen.size(); j++)
		{
			if (tolower(word[i]) == tolower(alreadySeen[j]))
			{
				letterPattern += labels[j];
				checkedCurrent = true;
			}
		}

		if (!checkedCurrent)
		{
			alreadySeen += word[i];
			letterPattern += labels[indexOfLabel];
			indexOfLabel++;
		}
	}

	return letterPattern;
}

bool WordListImpl::contains(string word) const
{
	bool isCorrect = false;

	const vector<string>* wordContainer = m_wordListHash.find(showLetterPattern(word));

	// return false if the hash doesn't exist
	if (wordContainer == nullptr)
		return false;
	
	for (int i = 0; i < wordContainer->size(); i++)
	{
		for (int j = 0; j < wordContainer->at(i).size() && j < word.size(); j++)
		{
			if (tolower(wordContainer->at(i)[j]) == tolower(word[j]))
				isCorrect = true;
			else
			{
				isCorrect = false;
				break;
			}
		}
		
	}

    return isCorrect;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
	vector<string> candidates;

	//all cases of returning blank vectors
	if (cipherWord.size() != currTranslation.size())
		return vector<string>();

	for (int i = 0; i < cipherWord.size(); i++)
	{
		if (!(isalpha(cipherWord[i]) || cipherWord[i] == '\''))
			return vector<string>();
	}

	for (int j = 0; j < currTranslation.size(); j++)
	{
		if (!(isalpha(currTranslation[j]) || currTranslation[j] == '\'' || currTranslation[j] == '?'))
			return vector<string>();
	}

	//get the letter pattern of the cipher word
	string cipherPattern = showLetterPattern(cipherWord);
	const vector<string>* samePattern = m_wordListHash.find(cipherPattern);

	bool mightBeCandidate = false;

	for (int k = 0; k < samePattern->size(); k++)
	{
		for (int l = 0; l < cipherPattern.size(); l++)
		{
			//if the current translation has a letter and the lowercase letter in the translation 
			//is the same as the lowercase letter of the word in the hash of the same pattern
			if (isalpha(currTranslation[l]) && (tolower(currTranslation[l]) == tolower(samePattern->at(k)[l])))
			{
				//if the cipherWord isn't a letter then return a blank vector
				if (!isalpha(cipherWord[l]))
					return vector<string>();

				mightBeCandidate = true;
			}
			//if the current translation is unknown and the character at the word in the hash of the same pattern at that index is a letter
			else if (currTranslation[l] == '?' && isalpha(samePattern->at(k)[l]))
			{
				//if the cipherWord isn't a letter then return a blank vector
				if (!isalpha(cipherWord[l]))
					return vector<string>();

				mightBeCandidate = true;
			}
			//if the current translation shows an apostrophe and so doess the corresponding index of the word
			else if (currTranslation[l] == '\'' && samePattern->at(k)[l] == '\'')
			{
				//if the cipherWord isn't an apostrophe then return a blank vector
				if (cipherWord[l] != '\'')
					return vector<string>();

				mightBeCandidate = true;
			}
			else
			{
				mightBeCandidate = false;
				break;
			}
		}

		if (mightBeCandidate)
		{
			candidates.push_back(samePattern->at(k));
			mightBeCandidate = false;
		}
	}


    return candidates;
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
	return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
   return m_impl->findCandidates(cipherWord, currTranslation);
}