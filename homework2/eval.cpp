#include "Map.h"
#include <string>
#include <stack>
#include <iostream>
#include <cassert>
using namespace std;

int evaluate(string infix, const Map& values, string& postfix, int& result);
// Evaluates an integer arithmetic expression
//   If infix is a syntactically valid infix integer expression whose
//   only operands are single lower case letters (whether or not they
//   appear in the values map), then postfix is set to the postfix
//   form of the expression; otherwise postfix may or may not be
//   changed, result is unchanged, and the function returns 1.  If
//   infix is syntactically valid but contains at least one lower
//   case letter operand that does not appear in the values map, then
//   result is unchanged and the function returns 2.  If infix is
//   syntactically valid and all its lower case operand letters
//   appear in the values map, then if evaluating the expression
//   (using for each letter in the expression the value in the map
//   that corresponds to it) attempts to divide by zero, then result
//   is unchanged and the function returns 3; otherwise, result is
//   set to the value of the expression and the function returns 0.
void infixToPostfix(string infix, string& postfix);
bool lowerOrEqualPrecedence(char op1, char op2);


int evaluate(string infix, const Map& values, string& postfix, int& result)
{
	//checking for errors
	if (infix.size() == 0)
		return 1;

	int i = 0;
	/*
	while (i < infix.size())
	{
		if (!(islower(infix[i])) && infix[i] != '+' && infix[i] != '-' && infix[i] != '*' && infix[i] != '/' && infix[i] != ' ' && infix[i] != '(' && infix[i] != ')')
			return 1;

		if (islower(infix[i]))
		{
			int temp = i;
			for (int k = i + 1; infix[k] == ' ' && k < infix.size(); k++)
				temp++;

			if (islower(infix[temp + 1]) || infix[temp + 1] == '(')
				return 1;
		}

		if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/' || infix[i] == ')')
		{
			if (i == 0)
				return 1;
		}

		if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/')
		{
			int temp = i;
			for (int k = i + 1; infix[k] == ' ' && k < infix.size(); k++)
				temp++;

			if (!(islower(infix[temp + 1])) && infix[temp + 1] != ' ' && infix[temp + 1] != '(')
				return 1;
		}

		if (infix[i] == '(')
		{
			int temp = i;
			for (int k = i + 1; infix[k] == ' ' && k < infix.size(); k++)
				temp++;

			if (!islower(infix[temp + 1]))
				return 1;
		}


		if (islower(infix[i]))
		{
			if (!(values.contains(infix[i])))
			{
				infixToPostfix(infix, postfix);
				return 2;
			}
		}

		if (infix[i] == '/')
		{
			int checkZero = 1;
			int temp = i;
			for(int j = i + 1; infix[j] != ' ' && j < infix.size(); j++)
				temp++;

			values.get(infix[temp + 1], checkZero);
			
			if (checkZero == 0)
				return 3;
		}
		i++;
	}
	*/

	infixToPostfix(infix, postfix);

	//evaluting postfix
	stack<int> operandStack;

	for (int i = 0; i < postfix.size(); i++)
	{
		char ch = postfix[i];

		if (islower(ch))
		{
			int value;
			if (values.get(postfix[i], value))
				operandStack.push(value);
			else
				return 2;
		}
		else
		{
			if (operandStack.size() < 2)
				return 1;

			int operand2 = operandStack.top();
			operandStack.pop();
			int operand1 = operandStack.top();
			operandStack.pop();
			int temp;

			switch (ch)
			{
				

			case '+':
				temp = operand1 + operand2;
				operandStack.push(temp);
				break;
			case '-':
				temp = operand1 - operand2;
				operandStack.push(temp);
				break;
			case '*':
				temp = operand1 * operand2;
				operandStack.push(temp);
				break;
			case '/':
				if (operand2 == 0)
					return 3;

				temp = operand1 / operand2;
				operandStack.push(temp);
				break;
			default:
				return 1;
			}
		}
	}

	if (operandStack.size() != 1)
		return 1;

	result = operandStack.top();			//last one remaining should be the result
	operandStack.pop();


	return 0;
}

void infixToPostfix(string infix, string& postfix)
{
	postfix = "";
	stack<char> operatorStack;

	for (int i = 0; i < infix.size(); i++)
	{
		char ch = infix[i];

		if (ch == ' ') 
		{
			;
		}
		else
		{
			if (islower(ch))
				postfix += ch;
			else
			{
				switch (ch)
				{
				case '(':
					operatorStack.push(ch);
					break;
				case ')':
					while (operatorStack.top() != '(')
					{
						postfix += operatorStack.top();
						operatorStack.pop();
					}
					operatorStack.pop();
					break;
				case '+':
				case '-':
				case '*':
				case '/':
					while (!operatorStack.empty() && operatorStack.top() != '(' && lowerOrEqualPrecedence(ch, operatorStack.top()))
					{
						postfix += operatorStack.top();
						operatorStack.pop();
					}
					operatorStack.push(ch);
					break;
				}
			}
		}	
	}

	while (!operatorStack.empty())
	{
		postfix += operatorStack.top();
		operatorStack.pop();
	}
}

bool lowerOrEqualPrecedence(char op1, char op2)
{
	if (op1 == '+' || op1 == '-')
		return true;
	if (op1 == '*' || op1 == '/')
	{
		if (op2 == '+' || op2 == '-')
			return false;
		if (op2 == '*' || op2 == '/')
			return true;
	}

	return false;
}

int main()
{
	char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
	int  vals[] = { 3,  -9,   6,   2,   4,   1 };
	Map m;
	for (int k = 0; vars[k] != '#'; k++)
		m.insert(vars[k], vals[k]);
	string pf;
	int answer;
	assert(evaluate("a+ e", m, pf, answer) == 0 &&
		pf == "ae+"  &&  answer == -6);
	answer = 999;
	assert(evaluate("", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a i", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("ai", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("()", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("y(o+u)", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+E", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("(a+(i-o)", m, pf, answer) == 1 && answer == 999);
	// unary operators not allowed:
	assert(evaluate("-a", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a*b", m, pf, answer) == 2 &&
		pf == "ab*"  &&  answer == 999);
	assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0 &&
		pf == "yoau-*+"  &&  answer == -1);
	answer = 999;
	assert(evaluate("o/(y-y)", m, pf, answer) == 3 &&
		pf == "oyy-/"  &&  answer == 999);
	assert(evaluate(" a  ", m, pf, answer) == 0 &&
		pf == "a"  &&  answer == 3);
	assert(evaluate("((a))", m, pf, answer) == 0 &&
		pf == "a"  &&  answer == 3);
	cout << "Passed all tests" << endl;
}