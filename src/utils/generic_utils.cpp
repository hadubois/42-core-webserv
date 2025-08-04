#include "generic_utils.hpp"
#include  "node.hpp"
std::string itoa(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string	trim(const std::string& str) {
	size_t start = str.find_first_not_of(" \t\r\n");
	if (start == std::string::npos)
		return "";

	size_t end = str.find_last_not_of(" \t\r\n");
	return str.substr(start, end - start + 1);
}

void	ft_trim(std::string& str) {
	size_t start = str.find_first_not_of(" \t\r");
	if (start == std::string::npos) {
		str.clear();
		return;
	}
	size_t end = str.find_last_not_of(" \t\r");

	str = str.substr(start, end - start + 1);
}
