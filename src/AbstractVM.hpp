/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AbstractVM.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frivaton <frivaton@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by marvin            #+#    #+#             */
/*   Updated: 2019/06/26 17:05:05 by frivaton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ABSTRACTVM_HPP
#define ABSTRACTVM_HPP

//#include <iostream>
#include <string>
#include <stack>
#include <exception>
#include <vector>
#include "IOperand.hpp"
#include "MutantStack.hpp"

class AbstractVM;

enum eOperator
{
	Exit,
	Push,
	Pop,
	Add,
	Sub,
	Mul,
	Div,
	Mod,
	Sqrt,
	Dump,
	Print,
	Echo,
	Assert,
	Equal,
	NB_OPERATORS
};

struct Command
{
	eOperator Operator;
	const IOperand  *Operand;
};

typedef IOperand const * (* factoryFuncType) (std::string const &);
typedef void (AbstractVM::*commandFuncType) ();


// *************************************
// AbstractVM
// *************************************

class AbstractVM
{
public:
	AbstractVM();								// canonical
	AbstractVM(std::istream & program);
	virtual ~AbstractVM();						// canonical

	bool Run_Program(std::istream & input);

protected:
	AbstractVM(const AbstractVM & other);				// canonical
	AbstractVM & operator=(const AbstractVM & other);	// canonical

	// grammar
	void				Lexer(std::vector<std::string> &tokens, const std::string &str, const std::string & sep = " \t\r\n");
	void				Parser(const std::vector<std::string> &tokens, Command &cmd);
	const IOperand *	Parser_Operand(const std::string &Words);
	bool				Execute_Command(const Command &cmd);
	void				Run_Line(const std::string &sBuffer);

	// operands
	const IOperand* peekOperand();
	const IOperand* popOperand();
	void 			popOperands();
	void			pushOperand(const IOperand * op);

	// operators
	void operatorExit();
	void operatorPush();
	void operatorPop();
	void operatorAdd();
	void operatorSub();
	void operatorMul();
	void operatorDiv();
	void operatorMod();
	void operatorSqrt();
	void operatorDump();
	void operatorPrint();
	void operatorEcho();
	void operatorAssert();
	void operatorEqual();

	// factory
	static
	IOperand const * createOperand( eOperandType type, std::string const & value );
	// factories
	static IOperand const * createInt8( std::string const & value );
	static IOperand const * createInt16( std::string const & value );
	static IOperand const * createInt32( std::string const & value );
	static IOperand const * createFloat( std::string const & value );
	static IOperand const * createDouble( std::string const & value );

// ** ATTRIBUTES **
protected:
	//memorize IOperand
	MutantStack<const IOperand *> _operandStack;

	const IOperand *	_regA;
	const IOperand *	_regB;

	size_t		_line;
	bool		_run;
	bool		_exit;

	static
	factoryFuncType factories[NB_OPERANDS];
	commandFuncType commands[NB_OPERATORS];

// ** EXCEPTION **
public:
	class AVM_Exception : public std::exception
	{
	public:
		AVM_Exception(const std::string &res);
		AVM_Exception(const AVM_Exception &);
		virtual ~AVM_Exception();
		virtual const char *what() const noexcept;
	private:
		AVM_Exception();
		AVM_Exception &operator=(const AVM_Exception &);

		std::string _reason;
	};

};

#endif
