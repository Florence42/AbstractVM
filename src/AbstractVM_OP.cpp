/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AbstractVM_OP.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frivaton <frivaton@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 13:30:14 by frivaton          #+#    #+#             */
/*   Updated: 2019/06/27 11:25:18 by frivaton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "AbstractVM.hpp"
#include "Operand.hpp"
#include "Log.hpp"


//*********** OPERAND STACK HANDLING


// push one operand
void AbstractVM::pushOperand(const IOperand * op)
{
	if (op == NULL)
		throw(AVM_Exception("(Runtime Error) Push on NULL operand (this never happens)"));

	Log << " -> PUSH " << op->toString() << std::endl;
	try
	{
		_operandStack.push(op);
	}
	catch(std::exception & e)
	{
		throw(AVM_Exception("(Runtime Error) Push on full stack (?)"));
	}

}


// peek ONE operand
const IOperand* AbstractVM::peekOperand()
{
	if (_operandStack.empty())
		throw(AVM_Exception("(Runtime Error) Peek on empty stack."));

	const IOperand *op = _operandStack.top();

	if (op == NULL)
		throw(AVM_Exception("(Runtime Error) Top operand is NULL (this never happens)"));

	return op;
}

// pop ONE operand
const IOperand* AbstractVM::popOperand()
{
	if (_operandStack.empty())
		throw(AVM_Exception("(Runtime Error) Pop on empty stack."));

	const IOperand *op = _operandStack.top();

	_operandStack.pop();

	if (op == NULL)
		throw(AVM_Exception("(Runtime Error) Top operand is NULL (this never happens)"));

	return op;
}

// pop TWO operands in "registers" A & B
void AbstractVM::popOperands()
{
	if (_operandStack.size() < 2)
		throw(AVM_Exception("(Runtime Error) Stack too short for operation."));

	_regB = popOperand();
	_regA = popOperand();

	eOperandType opAt = _regA->getType();
	eOperandType opBt = _regB->getType();
	eOperandType maxt = std::max(opAt, opBt);

	if (opAt != maxt)
	{	// upgrade operand
		const IOperand *tmp = _regA;
		_regA = AbstractVM::createOperand(maxt, _regA->toString());
		delete tmp;
	};

	if (opBt != maxt)
	{	// upgrade operand
		const IOperand *tmp = _regB;
		_regB = AbstractVM::createOperand(maxt, _regB->toString());
		delete tmp;
	};
}


//*********** OPERATORS

void AbstractVM::operatorExit()
{
	_exit = true;
}

void AbstractVM::operatorPush()
{
	// (already done)
	const IOperand *op = peekOperand();
	Log << "PUSH " << op->toString() << std::endl;
}

void AbstractVM::operatorPop()
{
	const IOperand *op = popOperand();
	Log << "POP " << op->toString() << std::endl;
	delete op;
}

void AbstractVM::operatorDump()
{
	// execute
	Log << "DUMP:" << (_operandStack.empty() ? "(empty stack)" : "") << std::endl;

	for (auto it = _operandStack.rbegin(); it != _operandStack.rend(); it++)
	{
		Mes << "\u2630 ";
		std::cout << (*it)->toString();
		Mes << " (" << (*it)->getTypeString() << ")";
		std::cout << std::endl;
	}
}

void AbstractVM::operatorAdd()
{
	popOperands();

	Log << "ADD " << _regA->toString() << " " << _regB->toString() << std::endl;

	pushOperand( (*_regA) + (*_regB) );

	delete _regA; _regA = NULL;
	delete _regB; _regB = NULL;
}

void AbstractVM::operatorSub()
{
	popOperands();

	Log << "SUB " << _regA->toString() << " " << _regB->toString() << std::endl;

	pushOperand((*_regA) - (*_regB));

	delete _regA; _regA = NULL;
	delete _regB; _regB = NULL;
}

void AbstractVM::operatorMul()
{
	popOperands();

	Log << "MUL " << _regA->toString() << " " << _regB->toString() << std::endl;

	pushOperand((*_regA) * (*_regB));

	delete _regA; _regA = NULL;
	delete _regB; _regB = NULL;
}

void AbstractVM::operatorDiv()
{
	popOperands();

	Log << "DIV " << _regA->toString() << " " << _regB->toString() << std::endl;

	pushOperand((*_regA) / (*_regB));

	delete _regA; _regA = NULL;
	delete _regB; _regB = NULL;
}

void AbstractVM::operatorMod()
{
	popOperands();

	Log << "MOD " << _regA->toString() << " " << _regB->toString() << std::endl;

	pushOperand((*_regA) % (*_regB));

	delete _regA; _regA = NULL;
	delete _regB; _regB = NULL;
}

void AbstractVM::operatorSqrt()
{
	_regA = popOperand();

	Log << "SQRT " << _regA->toString() << std::endl;

	pushOperand(_regA->sqrt());

	delete _regA;
	_regA = NULL;
	delete _regB;
	_regB = NULL;
}

void AbstractVM::operatorPrint()
{
	const IOperand *op = peekOperand();

	Log << "PRNT " << op->toString() << std::endl;

	if (op->getType() == Int8)
	{
		std::cout << static_cast<char>(std::atoi(op->toString().c_str()));
	}
	else
		throw(AVM_Exception("(Runtime Error) Print on non-int8."));
}

void AbstractVM::operatorAssert()
{
	_regA = popOperand();
	_regB = peekOperand();

	const std::string & strA = _regA->toString();
	const std::string & strB = _regB->toString();

	Log << "ASSERT " << strA << " " << strB << std::endl;

	bool eq = (strA == strB);

	delete _regA;	_regA = NULL;
					_regB = NULL;

	if (!eq)
		throw(AVM_Exception("(Runtime Error) Failed assertion."));
}

void AbstractVM::operatorEqual()
{
	_regA = popOperand();
	_regB = peekOperand();

	const std::string & strA = _regA->toString();
	const std::string & strB = _regB->toString();

	Log << "EQU " << strA << " " << strB << std::endl;

	bool eq = (strA == strB);

	delete _regA;	_regA = NULL;
					_regB = NULL;

	if (!eq)
		throw(AVM_Exception("(Runtime Error) Failed equality."));
}

void AbstractVM::operatorEcho()
{
	Log << "ECHO" << std::endl;
	std::string sval(peekOperand()->toString());
	std::cout << sval << std::endl;
}
