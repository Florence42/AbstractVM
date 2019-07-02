/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AbstractVM.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frivaton <frivaton@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by marvin            #+#    #+#             */
/*   Updated: 2019/06/27 11:48:06 by frivaton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "AbstractVM.hpp"
#include "Operand.hpp"
#include "Log.hpp"

struct s_op
{
	const std::string name;
	const int nb_operands;
};

static s_op Operators[] =
{
	{ "exit",	0 },
	{ "push",	1 },
	{ "pop",	0 },
	{ "add",	0 },
	{ "sub",	0 },
	{ "mul",	0 },
	{ "div",	0 },
	{ "mod",	0 },
	{ "sqrt",   0 },
	{ "dump",	0 },
	{ "print",	0 },
	{ "echo", 	0 },
	{ "assert",	1 },
	{ "equal", 	0 },
};

AbstractVM::AbstractVM() : _regA(NULL),
						   _regB(NULL),
						   _line(0),
						   _run(true),
						   _exit(false),
						   commands{
							   &AbstractVM::operatorExit,
							   &AbstractVM::operatorPush,
							   &AbstractVM::operatorPop,
							   &AbstractVM::operatorAdd,
							   &AbstractVM::operatorSub,
							   &AbstractVM::operatorMul,
							   &AbstractVM::operatorDiv,
							   &AbstractVM::operatorMod,
							   &AbstractVM::operatorSqrt,
							   &AbstractVM::operatorDump,
							   &AbstractVM::operatorPrint,
							   &AbstractVM::operatorEcho,
							   &AbstractVM::operatorAssert,
							   &AbstractVM::operatorEqual}
{
}

AbstractVM::AbstractVM(std::istream & program) : AbstractVM()
{
	Run_Program(program);
}

AbstractVM::~AbstractVM()
{
	while (!_operandStack.empty())
	{
		const IOperand *op = _operandStack.top();
		_operandStack.pop();
		delete op;
	}
	delete _regA;
	delete _regB;
}

factoryFuncType AbstractVM::factories[NB_OPERANDS] =
{
	&AbstractVM::createInt8,
	&AbstractVM::createInt16,
	&AbstractVM::createInt32,
	&AbstractVM::createFloat,
	&AbstractVM::createDouble
};

IOperand const * AbstractVM::createOperand( eOperandType type, std::string const & value )
{
	return (*factories[type])(value);
}

// factories
IOperand const * AbstractVM::createInt8( std::string const & value )
{
	return new Operand<int8_t,int16_t>(value);
}

IOperand const * AbstractVM::createInt16( std::string const & value )
{
	return new Operand<int16_t,int32_t>(value);
}

IOperand const * AbstractVM::createInt32( std::string const & value )
{
	return new Operand<int32_t,int64_t>(value);
}

IOperand const * AbstractVM::createFloat( std::string const & value )
{
	return new Operand<float,double>(value);
}

IOperand const * AbstractVM::createDouble( std::string const & value )
{
	return new Operand<double,long double>(value);
}

void AbstractVM::Lexer(std::vector<std::string> & tokens, const std::string & line_in, const std::string & sep)
{
	std::string sline;

	// take off the comments
	size_t com = line_in.find_first_of(";"); // line_in.find(";"); -> Invalid read of size 16 in valgrind !!?
	sline.assign(line_in, 0, com);

    size_t begin = sline.find_first_not_of(sep);
    while (begin != std::string::npos)
	{
  		size_t end = sline.find_first_of(sep, begin);
		size_t len = (end == std::string::npos) ? end : end - begin;
		tokens.push_back(sline.substr(begin, len));
		begin = sline.find_first_not_of(sep, end);
	}
}

const IOperand * AbstractVM::Parser_Operand(const std::string &Words)
{
	std::string sType;
	std::string sValue;
	std::vector<std::string> Tokens;

	std::size_t p_open = Words.find_first_of("(");

	if (p_open == std::string::npos)
	{
		throw(AVM_Exception(std::string("(Parser Error) '(' expected in \"" + Words + "\"")));
	}
	else
	{
		Tokens.push_back(Words.substr(0, p_open));
		std::size_t p_close = Words.find_last_of(")");
		if (p_close!= std::string::npos)
		{
			Tokens.push_back(Words.substr(p_open+1, p_close-p_open-1));
		}
		else
		{
			throw(AVM_Exception(std::string("(Parser Error) ')' expected in \"" + Words + "\"")));
		}
	}

	if (Tokens.size() != 2)
	{
		throw(AVM_Exception(std::string("(Parser Error) bad operand \"" + Words + "\"")));
	}

	sType = Tokens[0];

	for (int i = 0; i < NB_OPERANDS; i++)
	{
		//check nb of operand
		if (!sType.compare(Word_Operand[i]))
		{
			return (_run ? createOperand( eOperandType(i), Tokens[1] ) : NULL);
		}
	}

	throw(AVM_Exception(std::string("(Parser Error) Unknown Operand type: \"" + sType + "\"")));
}

void AbstractVM::Parser(const std::vector<std::string> &Tokens, Command &cmd)
{
	const std::string & sOperator = Tokens[0];

	for (int i = 0; i < NB_OPERATORS; i++)
	{
		//check nb of operand
		cmd.Operand = NULL;
		if (!sOperator.compare(Operators[i].name))
		{
			if (Operators[i].nb_operands + 1 != static_cast<int>(Tokens.size()))
			{
				throw(AVM_Exception(std::string("(Parser Error) Too many operands for \"" + sOperator + "\"")));
			}
			if (Operators[i].nb_operands == 1)
			{
				//find the type and the value
				cmd.Operand = Parser_Operand(Tokens[1]);
			}

			cmd.Operator = eOperator(i);
			//std::cout << std::endl << "-----------" << i << "---------" << std::endl;
			return;
		}
	}

	throw(AVM_Exception(std::string("(Parser Error) Unknown Operator ") + sOperator));
}


void AbstractVM::Run_Line(const std::string & sBuffer)
{
	try
	{
		std::vector<std::string> Tokens;

		_line++;

		AbstractVM::Lexer(Tokens, sBuffer);

		if (Tokens.size()) // 0 -> Empty line or comment
		{
			Command cmd;
			AbstractVM::Parser(Tokens, cmd);
			if (_run)
			{
				if (cmd.Operand)
					_operandStack.push(cmd.Operand);
				try
				{
					if (cmd.Operator == Add)
						operatorAdd();
						else
					(this->*commands[cmd.Operator])();

				}
				catch (AVM_Exception & e)
				{
					Err << "Line " << _line << ": " << e.what() << std::endl;
					_run = false;
				}
			}
		}
	}
	catch (AVM_Exception & e)
	{
		Err << "Line " << _line << ": " << e.what() << std::endl;
		_run = false;
	}
}

bool AbstractVM::Run_Program(std::istream & input)
{
	std::string sbuffer;

	Mes << "\u2699  (Execution start)" << std::endl;
	try
	{
		while (!_exit && getline(input, sbuffer))
			Run_Line(sbuffer);

		if (input.bad())
			throw(AVM_Exception(std::string("IO Error!")));

		if (!_run)
		{	// something bad happened
			Mes << "\u274c  (Execution stopped)" << std::endl;
			//operatorDump();
		}
		else
		{	// exec at end
			Mes << "\u2699  (Execution end)" << std::endl;
			if (!_exit)
				throw(AVM_Exception(std::string("(Runtime Error) \"exit\" missing!")));
		}
	}
	catch (AVM_Exception & e)
	{
		Err << "Line " << _line << ": " << e.what() << std::endl;
	}
	return (true);
}

//************** EXCEPTIONS

AbstractVM::AVM_Exception::AVM_Exception(const std::string & reason) : _reason(reason)
{
}

AbstractVM::AVM_Exception::AVM_Exception(const AVM_Exception & e) : _reason(e._reason)
{
}

AbstractVM::AVM_Exception::~AVM_Exception()
{
}

const char *AbstractVM::AVM_Exception::what() const noexcept
{
	return _reason.c_str();
}
