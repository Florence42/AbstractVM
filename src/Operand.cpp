/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Operand.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frivaton <frivaton@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by marvin            #+#    #+#             */
/*   Updated: 2019/06/27 11:35:38 by frivaton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include "Operand.hpp"
#include "Log.hpp"
#include "AbstractVM.hpp"

// default constructors, set type once & for all

template <>
Operand<int8_t,int16_t>::Operand() : _type(Int8), _value(0), _stype(Word_Operand[_type]), _svalue("0") {}
template <>
Operand<int16_t,int32_t>::Operand() : _type(Int16), _value(0), _stype(Word_Operand[_type]), _svalue("0") {}
template <>
Operand<int32_t,int64_t>::Operand() : _type(Int32), _value(0), _stype(Word_Operand[_type]), _svalue("0") {}
template <>
Operand<float,double>::Operand() : _type(Float), _value(0.f), _stype(Word_Operand[_type]), _svalue("0") {}
template <>
Operand<double,long double>::Operand() : _type(Double), _value(0.0), _stype(Word_Operand[_type]), _svalue("0") {}

// modulo
template <>
int16_t op_mod(int16_t a, int16_t b) { return a % b; }
template <>
int32_t op_mod(int32_t a, int32_t b) { return a % b; }
template <>
int64_t op_mod(int64_t a, int64_t b) { return a % b; }
template <>
double op_mod(double a, double b) { return std::fmod(a,b); }
template <>
long double op_mod(long double a, long double b) { return std::fmodl(a,b); }

// sqrt
template <>
int16_t op_sqrt(int16_t a) { return static_cast<int16_t>(sqrt(static_cast<double>(a))); }
template <>
int32_t op_sqrt(int32_t a) { return static_cast<int32_t>(sqrt(static_cast<double>(a))); }
template <>
int64_t op_sqrt(int64_t a) { return static_cast<int64_t>(sqrt(static_cast<double>(a))); }
template <>
double op_sqrt(double a) { return std::sqrt(a); }
template <>
long double op_sqrt(long double a) { return std::sqrtl(a); }

// abs
template <>
int16_t op_abs(int16_t a) { return std::abs(a); }
template <>
int32_t op_abs(int32_t a) { return std::abs(a); }
template <>
int64_t op_abs(int64_t a) { return std::abs(a); }
template <>
double op_abs(double a) { return std::fabs(a); }
template <>
long double op_abs(long double a) { return std::fabs(a); }

// string to num type
template <>
int16_t op_fromString(const std::string & s) { return std::stoi(s); }
template <>
int32_t op_fromString(const std::string & s) { return std::stol(s); }
template <>
int64_t op_fromString(const std::string & s) { return std::stol(s); }
template <>
double op_fromString(const std::string & s) { return std::stod(s); }
template <>
long double op_fromString(const std::string & s) { return std::stold(s); }

// regex string value
// note:
// "[-]?[0-9]+([eE][+]?[0-9]+)?" won't work with std::stoi
// exponential notation if for float/double
// 1e2 is a double, 1e2f a  float
// int n = 1e2;   -->   int n = (int)1e2;

template <>
bool op_checkFormat<int8_t>(const std::string & str)
{
	return regex_match(str, std::regex("[-]?[0-9]+"));
}

template <>
bool op_checkFormat<int16_t>(const std::string & str)
{
	return regex_match(str, std::regex("[-]?[0-9]+"));
}

template <>
bool op_checkFormat<int32_t>(const std::string & str)
{
	return regex_match(str, std::regex("[-]?[0-9]+"));
}

template <>
bool op_checkFormat<float>(const std::string & str)
{
	return regex_match(str, std::regex("[-+]?[0-9]+(.[0-9]+)?([eE][-+]?[0-9]+)?"));
}

template <>
bool op_checkFormat<double>(const std::string & str)
{
	return regex_match(str, std::regex("[-+]?[0-9]+(.[0-9]+)?([eE][-+]?[0-9]+)?"));
}
