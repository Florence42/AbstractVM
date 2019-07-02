/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frivaton <frivaton@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by marvin            #+#    #+#             */
/*   Updated: 2019/06/27 11:24:44 by frivaton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERAND_HPP
#define OPERAND_HPP

#include <iostream>
#include <regex>
#include "AbstractVM.hpp"
#include "IOperand.hpp"
#include "Log.hpp"

template <typename T> T op_mod(T, T);
template <typename T> T op_abs(T);
template <typename T> T op_sqrt(T);
template <typename T> T op_fromString(const std::string &);
template <typename T> bool op_checkFormat(const std::string &);

template <typename NumType, typename SupType>
class Operand : public IOperand
{
public:

	Operand(const std::string & sval) : Operand()	// (constructor delegation, c++11)
	{
		this->setValue(sval);
	}

	Operand(const Operand & other) : Operand()		// canonical
	{
		*this = other;
	}

	virtual ~Operand()								// canonical
	{
	}

	Operand & operator=(const Operand & other)		// canonical
	{
		this->setValue(other.getValue());
		return (*this);
	}

	Operand & operator=(const IOperand & other)
	{
		if (other.getPrecision() < this->getPrecision())
			this->setValue(other.toString());
		else
			throw(AbstractVM::AVM_Exception("(Runtime Error) operand demotion! (this never happens)"));
		return (*this);
	}

	// IOperand
	int getPrecision( void ) const // Precision of the type of the instance
	{
		return (int)_type;
	}

	eOperandType getType( void ) const // Type of the instance
	{
		return _type;
	}

	const std::string& getTypeString(void) const // Type of the instance
	{
		return _stype;
	}

	IOperand const * operator+( IOperand const & rhs ) const // Sum
	{
		const Operand<NumType,SupType>* rhso(dynamic_cast<const Operand<NumType,SupType>*>(&rhs));
		if (rhso == NULL)
			throw(AbstractVM::AVM_Exception("(Runtime Error) different types! (this never happens)"));

		SupType a = static_cast<SupType>(this->getValue());
		SupType b = static_cast<SupType>(rhso->getValue());

		SupType r = a + b;

		checkBounds(r);

		return new Operand<NumType,SupType>(static_cast<NumType>(r));
	}

	IOperand const * operator-( IOperand const & rhs) const // Diffference
	{
		const Operand<NumType,SupType>* rhso(dynamic_cast<const Operand<NumType,SupType>*>(&rhs));
		if (rhso == NULL)
			throw(AbstractVM::AVM_Exception("(Runtime Error) different types! (this never happens)"));

		SupType a = static_cast<SupType>(this->getValue());
		SupType b = static_cast<SupType>(rhso->getValue());

		SupType r = a - b;

		checkBounds(r);

		return new Operand<NumType,SupType>(static_cast<NumType>(r));
	}

	IOperand const *operator*(IOperand const &rhs) const // Product
	{
		const Operand<NumType,SupType>* rhso(dynamic_cast<const Operand<NumType,SupType>*>(&rhs));
		if (rhso == NULL)
			throw(AbstractVM::AVM_Exception("(Runtime Error) different types! (this never happens)"));

		SupType a = static_cast<SupType>(this->getValue());
		SupType b = static_cast<SupType>(rhso->getValue());

		SupType r = a * b;

		checkBounds(r);

		return new Operand<NumType,SupType>(static_cast<NumType>(r));
	}

	IOperand const *operator/(IOperand const &rhs) const // Product
	{
		const Operand<NumType,SupType>* rhso(dynamic_cast<const Operand<NumType,SupType>*>(&rhs));
		if (rhso == NULL)
			throw(AbstractVM::AVM_Exception("(Runtime Error) different types! (this never happens)"));

		SupType a = static_cast<SupType>(this->getValue());
		SupType b = static_cast<SupType>(rhso->getValue());

		if (b == 0)
			throw(AbstractVM::AVM_Exception("(Runtime Error) divide by zero (" + _stype + ")."));

		SupType r = a / b;

		checkBounds(r);

		return new Operand<NumType,SupType>(static_cast<NumType>(r));
	}

	IOperand const *operator%(IOperand const &rhs) const // Product
	{
		const Operand<NumType,SupType>* rhso(dynamic_cast<const Operand<NumType,SupType>*>(&rhs));
		if (rhso == NULL)
			throw(AbstractVM::AVM_Exception("(Runtime Error) different types! (this never happens)"));

		SupType a = static_cast<SupType>(this->getValue());
		SupType b = static_cast<SupType>(rhso->getValue());

		if (b == 0)
			throw(AbstractVM::AVM_Exception("(Runtime Error) modulo zero (" + _stype + ")."));

		SupType r = op_mod<SupType>(a,b);

		checkBounds(r);

		return new Operand<NumType,SupType>(static_cast<NumType>(r));
	}

	IOperand const *sqrt() const // Product
	{
		if (_value < 0)
			throw(AbstractVM::AVM_Exception("(Runtime Error) sqrt on negative value"));

		SupType r = op_sqrt<SupType>(_value);

		checkBounds(r);

		return new Operand<NumType, SupType>(static_cast<NumType>(r));
	}
	
	std::string const & toString( void ) const	// String representation of the instance
	{
		return (_svalue);
	}

protected:

	Operand();	//	canonical

	Operand(NumType val) : Operand()
	{
		this->setValue(val);
	}

	void setValue(const std::string & sval)
	{
		try
		{
			if (!op_checkFormat<NumType>(sval))
				throw(AbstractVM::AVM_Exception("(Runtime Error) bad format " + sval + " (" + _stype + ")."));

			SupType r = op_fromString<SupType>(sval);

			if (r < std::numeric_limits<NumType>::lowest() || r > std::numeric_limits<NumType>::max())
				throw(AbstractVM::AVM_Exception("(Runtime Error) value out of range (" + _stype + ")."));
			else
			if (op_abs(r) < std::numeric_limits<NumType>::min())
				throw(AbstractVM::AVM_Exception("(Runtime Error) value below range (" + _stype + ")."));

			std::stringstream ss;
			ss << +static_cast<NumType>(r); // '+' because int8_t would print as a char
			_svalue = ss.str();
			_value = r;
		}
		catch (const std::out_of_range &) // might be thrown by op_fromString() (stoi() etc.)
		{
			throw(AbstractVM::AVM_Exception("(Runtime Error) value out of range (" + _stype + ")."));
		}
		catch (const std::exception &Exp)
		{
			throw(AbstractVM::AVM_Exception(Exp.what()));
		}
	}

	void checkBounds(SupType v) const
	{
		if (v < std::numeric_limits<NumType>::lowest() || v > std::numeric_limits<NumType>::max())
			throw(AbstractVM::AVM_Exception("(Runtime Error) overflow (" + _stype + ")."));
		else
		if (op_abs(v) < std::numeric_limits<NumType>::min())
			throw(AbstractVM::AVM_Exception("(Runtime Error) underflow (" + _stype + ")."));
	}

	void setValue(NumType val)
	{
		std::stringstream ss;
		ss << +val;
		_svalue = ss.str();
		_value = val;
	}

	NumType getValue() const
	{
		return _value;
	}

private:

	eOperandType 	_type;
	NumType			_value;

	std::string		_stype; //(for debug)
	std::string		_svalue;
};

#endif
