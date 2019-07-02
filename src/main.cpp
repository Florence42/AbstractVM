/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frivaton <frivaton@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by marvin            #+#    #+#             */
/*   Updated: 2019/06/06 15:36:42 by frivaton         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "AbstractVM.hpp"
#include "IOperand.hpp"
#include "Log.hpp"


bool readProgramFromInput(std::istream &input, std::stringstream & prg)
{
	std::string sbuffer;

	while (getline(input, sbuffer))
	{
		//add the input to ss
		prg << sbuffer << "\n";
		std::size_t found = sbuffer.find(";;");
		if (found != std::string::npos)
			return (true);
	}
	if (input.bad())
	{
		Err << "IO error " << std::endl;
		return (false);
	}
	else if (!input.eof())
	{
		Err << "eof error (?)" << std::endl;
		return (false);
	}
	return (true);
}

bool readProgramFromFile(const std::string & filename, std::stringstream & prg)
{
	std::ifstream fichier(filename, std::ios::in);

	if (fichier)
	{
		prg << fichier.rdbuf();
		fichier.close();
	}
	else
	{
		Err << "File \"" << filename << "\" not found!" << std::endl;
		return (false);
	}
	return (true);
}

void usage(const char * exe)
{
	std::cout << C_GRN << "Usage : " << exe << " [-raw] [-log] [-h] [file]+" << C_NRM << std::endl;
}

int main(int ac, char **av)
{
	std::vector<const char *> files;

	Log.disable();

	for (int i = 1; i < ac; i++)
	{
		if ( av[i] == std::string("-raw") )
		{
			Mes.disable();
		}
		else
		if ( av[i] == std::string("-log") )
		{
			Log.enable();
		}
		else
		if ( av[i] == std::string("-h") || av[i] == std::string("-help") )
		{
			usage(av[0]);
			return 0;
		}
		else
		{
			files.push_back(av[i]);
		}
	}

	if (files.size()==0)
	{
		std::stringstream program;

		Mes << "Input:" << std::endl;
		if (readProgramFromInput(std::cin, program))
		{
			AbstractVM avm(program);
		}
	}

	for (auto f=files.begin(); f!=files.end(); f++)
	{
		std::stringstream program;

		if (readProgramFromFile(*f, program))
		{
			Mes << "\nLoaded: " << *f << std::endl;
			AbstractVM avm(program);
		}
	}

	Mes << "\nHave a nice day.  \u26F3  " << std::endl;
	return (0);
}
