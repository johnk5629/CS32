#include "provided.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>			//delete later
using namespace std;

class DecrypterImpl
{
public:
	DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
	WordList wlist;
	Translator translate;
	Tokenizer token;

	string crackMessage(string ciphertext_message, vector<string>& output);
};

DecrypterImpl::DecrypterImpl()
	: token(" ,;:.!()[]{}-\"#$%^&")
{}

bool DecrypterImpl::load(string filename)
{
    return wlist.loadWordList(filename);
}

string DecrypterImpl::crackMessage(string ciphertext_message, vector<string>& output)
{
	vector<string> cipherWords = token.tokenize(ciphertext_message);
	vector<string> translatedWords;

	for (int i = 0; i < cipherWords.size(); i++)
		translatedWords.push_back(translate.getTranslation(cipherWords[i]));

	//trying to find the word with the most unknowns
	int indexOfMostUnknown;
	int highestUnknown = 0;

	for (int j = 0; j < translatedWords.size(); j++)
	{
		int currentUnknown = 0;

		for (int k = 0; k < translatedWords[j].size(); k++)
		{
			if ((translatedWords[j])[k] == '?')
				currentUnknown++;
		}

		if (currentUnknown > highestUnknown)
		{
			highestUnknown = currentUnknown;
			indexOfMostUnknown = j;
		}
	}

	vector<string> possibleCandidates = wlist.findCandidates(cipherWords[indexOfMostUnknown], translatedWords[indexOfMostUnknown]);


	if (possibleCandidates.empty())
	{
		translate.popMapping();
		return "";
	}

	for (int k = 0; k < possibleCandidates.size(); k++)
	{
		//if the mapping is invalid this will return false and go to the next iteration of the for loop
		if (translate.pushMapping(cipherWords[indexOfMostUnknown], possibleCandidates[k]))
		{
			//re-translate the words with the new mapping
			string translation = translate.getTranslation(ciphertext_message);

			//check if there is at least one fully translated word
			vector<string> newTokens = token.tokenize(translation);

			vector<bool> fullTranslations;

			for (int j = 0; j < newTokens.size(); j++)
			{
				int currentUnknown = 0;

				for (int k = 0; k < newTokens[j].size(); k++)
				{
					if ((newTokens[j])[k] == '?')
						currentUnknown++;
				}

				if (currentUnknown == 0)
					fullTranslations.push_back(true);
			}

			//if there are no full translations: pop this current map then go on with the next iteration of the for loop
			if (fullTranslations.empty())
			{
				translate.popMapping();
				continue;
			}
			//if all the words are fully translated: return this translated vector
			else if (fullTranslations.size() == translatedWords.size())
			{
				translate.popMapping();
				output.push_back(translation);
			}
			//if at least one are fully translated continue on and recursively call the function
			else if (!fullTranslations.empty())
			{
				crackMessage(ciphertext_message, output);
			}

		}
	}

	translate.popMapping();
	return "";  // If it ever reaches here then pop the current mapping and just return

}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
	vector<string> crackedText;

	crackMessage(ciphertext, crackedText);

	sort(crackedText.begin(), crackedText.end());

	return crackedText;
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
   return m_impl->crack(ciphertext);
}
