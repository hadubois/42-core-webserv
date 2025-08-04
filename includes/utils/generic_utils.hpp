#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

std::string itoa(int value) ;
std::string trim(const std::string& str) ;
void	ft_trim(std::string& str) ;

template <typename T, typename U>
void	printMap(std::map<T, U>const& m) {
	for (typename std::map<T, U>::const_iterator it = m.begin(); it != m.end(); it++) {
		std::cout << it->first << "->" << it->second << std::endl;
	}
}

template <typename T, class U>
void	printMapTClass(std::map<T, U>const& m) {
	for (typename std::map<T, U>::const_iterator it = m.begin(); it != m.end(); it++) {
		std::cout << it->first << "->" << it->second << std::endl;
	}
}

template <typename T>
void	printSet(std::set<T>const& m) {
	for (typename std::set<T>::const_iterator it = m.begin(); it != m.end(); it++) {
		std::cout << *it << std::endl;
	}
}
