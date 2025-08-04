#pragma once

#include <string>
#include <map>

class MimeTypes {
	private:
		static std::map<std::string, std::string>	_extToMime;
		static std::map<std::string, std::string>	_mimeToExt;
		static bool									_initialized;

	public:
		static void initMaps();
		static std::string	getMimeFromExt(const std::string& ext);
		static std::string	getExtFromMime(const std::string& mime);
};
