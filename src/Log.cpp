/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Log.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frivaton <frivaton@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/15 18:48:34 by dhayez            #+#    #+#             */
/*   Updated: 2019/06/27 11:58:03 by frivaton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Log.hpp"

CLog Mes(std::cout, C_LCY);
CLog Log(std::clog, C_CYA);
CLog Err(std::cerr, C_LRD);

CLog::CLog(std::ostream &str, const char *col)
: _ostr(str), _col(col), _display(true)
{

};

CLog::~CLog(){}; // canonical

CLog & CLog::operator<<(std::ostream &(*f)(std::ostream &))
{
	if (_display)
		f(_ostr);
	return *this;
}

CLog & CLog::operator<<(std::ostream &(*f)(std::ios &))
{
	if (_display)
		f(_ostr);
	return *this;
}
CLog & CLog::operator<<(std::ostream &(*f)(std::ios_base &))
{
	if (_display)
		f(_ostr);
	return *this;
}

void CLog::enable()
{
	_display = true;
}

void CLog::disable()
{
	_display = false;
}
