#include "MimeTypes.hpp"

std::map<std::string, std::string> MimeTypes::_extToMime;
std::map<std::string, std::string> MimeTypes::_mimeToExt;
bool MimeTypes::_initialized = false;

void MimeTypes::initMaps() {
	if (_initialized)
		return;

	_extToMime[".html"] = "text/html";
	_extToMime[".css"]  = "text/css";
	_extToMime[".js"]   = "application/javascript";
	_extToMime[".json"] = "application/json";
	_extToMime[".jpg"]  = "image/jpeg";
	_extToMime[".jpeg"] = "image/jpeg";
	_extToMime[".png"]  = "image/png";
	_extToMime[".gif"]  = "image/gif";
	_extToMime[".txt"]  = "text/plain";
	_extToMime[".pdf"]  = "application/pdf";
	_extToMime[".xml"]  = "application/xml";

	for (std::map<std::string, std::string>::iterator it = _extToMime.begin(); it != _extToMime.end(); ++it)
		_mimeToExt[it->second] = it->first;

	_initialized = true;
}

std::string MimeTypes::getMimeFromExt(const std::string& ext) {
	initMaps();
	std::map<std::string, std::string>::iterator it = _extToMime.find(ext);
	if (it != _extToMime.end())
		return it->second;
	return "application/octet-stream";
}

std::string MimeTypes::getExtFromMime(const std::string& mime) {
	initMaps();
	std::map<std::string, std::string>::iterator it = _mimeToExt.find(mime);
	if (it != _mimeToExt.end())
		return it->second;
	return "";
}
