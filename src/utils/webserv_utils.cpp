#include "generic_utils.hpp"
#include "webserv_utils.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#define CODE_PHRASE_FILE "./config/status_codes.txt"

const std::map<std::string, std::string>& getMimeTypeMap() {
	static std::map<std::string, std::string> mime;

	if (mime.empty()) {
		mime[".html"] = "text/html";
		mime[".js"]   = "application/javascript";
		mime[".png"]  = "image/png";
		mime[".jpg"]  = "image/jpeg";
		mime[".jpeg"] = "image/jpeg";
		mime[".gif"]  = "image/gif";
		mime[".txt"]  = "text/plain";
		mime[".pdf"]  = "application/pdf";
	}

	return mime;
}

const std::map<int, std::string>& getStatusCodeMap() {
	static std::map<int, std::string>	statusCode;

	if (statusCode.empty()) {
		statusCode = extractReasonPhrases();
	}

	return statusCode;
}

std::map<int, std::string>	extractReasonPhrases() {
	std::map<int, std::string>	statusCode;
	std::ifstream	ifs(CODE_PHRASE_FILE);
	std::string		line;
	bool			firstLine = true;

	if (!ifs.is_open())
		throw std::runtime_error("Impossible to open status_code.txt");
	if(ifs.peek() == std::ifstream::traits_type::eof())
		throw std::runtime_error("Invalid file format: empty");

	while (std::getline(ifs, line)) {
		std::istringstream	iss(line);
		std::string	sep;
		std::string	phrase;

		if (firstLine) {
			std::string	code;
			iss >> code >> sep >> phrase;
			if (code != "Code" || sep != "|" || phrase != "Reason-Phrase")
				throw std::runtime_error("Invalid file format: header");
			firstLine = false;
		} else {
			int	key;
			iss >> key >> sep;
			if (iss.fail() || sep != "|") {
				throw std::runtime_error("Invalid file format: body");
			}
			std::getline(iss, phrase);
			ft_trim(phrase);
			statusCode[key] = phrase;
		}
	}
	return statusCode;
}

