// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   Log.hpp                                            :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: dhayez <marvin@42.fr>                      +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2019/06/26 11:02:27 by dhayez            #+#    #+#             //
//   Updated: 2019/06/26 11:02:32 by dhayez           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>

#define C_RED "\033[31m"
#define C_GRN "\033[32m"
#define C_YEL "\033[33m"
#define C_BLU "\033[34m"
#define C_MAG "\033[35m"
#define C_CYA "\033[36m"
#define C_WHT "\033[37m"
#define C_LRD "\033[91m"
#define C_LCY "\033[96m"
#define C_NRM "\033[0m"

class CLog
{
public:
	CLog(std::ostream & str, const char *col = "");

	virtual ~CLog();			// canonical

	template <typename T>
	CLog & operator<<(const T &t) // template code
	{
		if (_display)
			_ostr << _col << t << C_NRM;
		return *this;
	}

	CLog & operator<<(std::ostream &(*f)(std::ostream &));
	CLog & operator<<(std::ostream &(*f)(std::ios &));
	CLog & operator<<(std::ostream &(*f)(std::ios_base &));

	void enable();
	void disable();

private:
	CLog();										// canonical
	CLog(const CLog & other);					// canonical
	CLog & operator=(const CLog & other);		// canonical

	std::ostream & _ostr;
	const char* _col;
	bool _display; //display information message
};

extern CLog Mes;
extern CLog Log;
extern CLog Err;

#endif
