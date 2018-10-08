#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const string& s) const;
private:
	string m_separators;
};

TokenizerImpl::TokenizerImpl(string separators)
	: m_separators(separators)
{}

vector<string> TokenizerImpl::tokenize(const string& s) const
{
	string token;
	vector<string> tokenVector;
	bool hitSeparator = false;

	for (int i = 0; i < s.size(); i++)
	{
		for (int j = 0; j < m_separators.size(); j++)
		{
			if (s[i] == m_separators[j])
				hitSeparator = true;
		}

		if (!hitSeparator)
		{
			token += s[i];
			hitSeparator = false;
		}
		else
		{
			if (token != "")
			{
				tokenVector.push_back(token);
				token = "";
			}
			hitSeparator = false;
		}

	}

	return tokenVector;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const string& s) const
{
    return m_impl->tokenize(s);
}