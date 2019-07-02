// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   MutantStack.hpp                                    :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: dhayez <marvin@42.fr>                      +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2019/06/26 11:01:35 by dhayez            #+#    #+#             //
//   Updated: 2019/06/26 11:01:36 by dhayez           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef MUTANTSTACK_HPP
#define MUTANTSTACK_HPP

// **************************************
// MutantStack: std::stack with iterators
// **************************************

template <typename T>
class MutantStack : public std::stack<T>
{
public:
	MutantStack(void) : std::stack<T>() {}
	virtual ~MutantStack(void) {}

	// provide copy constructors
	MutantStack(std::stack<T> const &obj) : std::stack<T>(obj) {}
	MutantStack(MutantStack const &obj) : std::stack<T>(obj) {}

	// use the base class operator=
	using std::stack<T>::operator=;

	// container_type is a type which maps to Container
	// which itself holds the iterator type
	typedef typename std::stack<T>::container_type::iterator iterator;

	// http://www.cplusplus.com/reference/stack/stack/
	// c is the container, and a member of std::stack
	auto begin(void) { return std::begin(std::stack<T>::c); }
	auto end(void) { return std::end(std::stack<T>::c); }

	//reverse iterators (c++14)
	auto rbegin(void) { return std::rbegin(std::stack<T>::c); }
	auto rend(void) { return std::rend(std::stack<T>::c); }
};

#endif
