#include "provided.h"
#include <string>
#include <iostream>		//delete later
using namespace std;

const int LETTERS_IN_ALPHABET = 26;

class TranslatorImpl
{
public:
	TranslatorImpl();
	~TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;

private:
	struct MapNode
	{
		char translatedLetters[LETTERS_IN_ALPHABET];
		MapNode* next;
	};

	int changeLetterToIndex(char c) const;

	MapNode* head;
	int m_numMappings;

};

TranslatorImpl::TranslatorImpl()
	: m_numMappings(0)
{
	head = new MapNode;
	for (int i = 0; i < LETTERS_IN_ALPHABET; i++)
		head->translatedLetters[i] = '?';

	head->next = nullptr;
}

TranslatorImpl::~TranslatorImpl()
{
	MapNode* p = head;

	while (p->next != nullptr)
	{
		MapNode* q = p->next;
		p->next = q->next;
		delete q;
	}

	delete p;
}

int TranslatorImpl::changeLetterToIndex(char c) const
{
	char ch = toupper(c);
	int index = 0;

	switch (ch)
	{
	case 'A':
		index = 0;
		break;
	case 'B':
		index = 1;
		break;
	case 'C':
		index = 2;
		break;
	case 'D':
		index = 3;
		break;
	case 'E':
		index = 4;
		break;
	case 'F':
		index = 5;
		break;
	case 'G':
		index = 6;
		break;
	case 'H':
		index = 7;
		break;
	case 'I':
		index = 8;
		break;
	case 'J':
		index = 9;
		break;
	case 'K':
		index = 10;
		break;
	case 'L':
		index = 11;
		break;
	case 'M':
		index = 12;
		break;
	case 'N':
		index = 13;
		break;
	case 'O':
		index = 14;
		break;
	case 'P':
		index = 15;
		break;
	case 'Q':
		index = 16;
		break;
	case 'R':
		index = 17;
		break;
	case 'S':
		index = 18;
		break;
	case 'T':
		index = 19;
		break;
	case 'U':
		index = 20;
		break;
	case 'V':
		index = 21;
		break;
	case 'W':
		index = 22;
		break;
	case 'X':
		index = 23;
		break;
	case 'Y':
		index = 24;
		break;
	case 'Z':
		index = 25;
		break;
	default:
		break;
	}


	return index;
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
	//if sizes aren't the same return false
	if (ciphertext.size() != plaintext.size())
		return false;

	//if either one contains a non-letter return false
	for (int i = 0; i < ciphertext.size(); i++)
	{
		if (!(isalpha(ciphertext[i]) && isalpha(plaintext[i])))
			return false;
	}


	for (int j = 0; j < ciphertext.size(); j++)
	{
		int cipherIndex = changeLetterToIndex(ciphertext[j]);

		//if one ciphertext letter would map to two or more different plaintext letters: return false
		if (isalpha(head->translatedLetters[cipherIndex]) && head->translatedLetters[cipherIndex] != toupper(plaintext[j]))
			return false;
			

		//if two or more different ciphertext letters map to the same plaintext letter: return false
		for (int k = 0; k < LETTERS_IN_ALPHABET; k++)
		{
			if (k != cipherIndex && head->translatedLetters[k] == toupper(plaintext[j]))
				return false;
				
		}
	}


	
	MapNode* q = new MapNode;				//create a new MapNode after it passed all error checks


	//copy the previous Mapping's tranlations (the first one will copy the all '?' mapping)
	for (int i = 0; i < LETTERS_IN_ALPHABET; i++)
		q->translatedLetters[i] = head->translatedLetters[i];
		
	//update the new MapNode with translations
	for (int m = 0; m < ciphertext.size(); m++)
	{
		int cipherIndex = changeLetterToIndex(ciphertext[m]);

		q->translatedLetters[cipherIndex] = toupper(plaintext[m]);
	}
	
	q->next = head;
	head = q;
	m_numMappings++;		//successfully pushed a mapping

    return true;
}

bool TranslatorImpl::popMapping()
{
	if (m_numMappings <= 0)
		return false;

	MapNode* p = head;
	head = p->next;
	delete p;
	m_numMappings--;		//successfully popped a mapping

    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
	string translation;

	for (int i = 0; i < ciphertext.size(); i++)
	{
		if (!isalpha(ciphertext[i]))
		{
			translation += ciphertext[i];
			continue;
		}

		int cipherIndex = changeLetterToIndex(ciphertext[i]);

		if (isalpha(head->translatedLetters[cipherIndex]))
		{
			if (islower(ciphertext[i]) != 0)
				translation += tolower(head->translatedLetters[cipherIndex]);
			else
				translation += head->translatedLetters[cipherIndex];
		}
		else if (head->translatedLetters[cipherIndex] == '?')
		{
			translation += head->translatedLetters[cipherIndex];
		}

	}


	return translation;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
