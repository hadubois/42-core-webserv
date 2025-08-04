#include "LocationConfig.hpp"
#include "generic_utils.hpp"
#include <stdexcept>

LocationConfig::LocationConfig()
	:
	_path("/"),
	_root("./tmp/www/"),
	_autoIndex(false),
	_indexFile("index.html"),
	_cgiEnabled(false)
{}

LocationConfig::LocationConfig( LocationConfig const& other)
:
	_path(other.getPath()),
	_root(other.getRoot()),
	_autoIndex(other.getAutoIndex()),
	_indexFile(other.getIndexFile()),
	_cgiEnabled(other.getCgiEnabled()),
	_cgi(other.getCgi()),
	_uploadEnabled(other.getUploadEnabled()),
	_uploadPath(other.getUploadPath())
{}

LocationConfig& LocationConfig::operator=(LocationConfig const& other) {
	_path = other.getPath();
	_root = other.getRoot();
	_autoIndex = other.getAutoIndex();
	_indexFile = other.getIndexFile();
	_cgiEnabled = other.getCgiEnabled();
	_cgi = other.getCgi();
	_uploadEnabled = other.getUploadEnabled();
	_uploadPath = other.getUploadPath();
	return *this;
}

LocationConfig::~LocationConfig() {}

// ----GETTERS--------------------------------------------------------------- //
const std::string&	LocationConfig::getPath() const { return _path; }
const std::string&	LocationConfig::getRoot() const { return _root; }
const bool&			LocationConfig::getAutoIndex() const { return _autoIndex; }
const std::string&	LocationConfig::getIndexFile() const { return _indexFile; }

const bool&			LocationConfig::getCgiEnabled() const {
	return _cgiEnabled;
}

const std::map<std::string, std::string>&	LocationConfig::getCgi() const {
	return _cgi;
}

const std::string	LocationConfig::getCgiPathFor(std::string extension) const {
	std::map<std::string, std::string>::const_iterator	it;
	for (it = _cgi.begin(); it != _cgi.end(); it++) {
		if (it->first == extension)
			return it->second;
	}
	return "";
}

const bool&			LocationConfig::getUploadEnabled() const {
	return _uploadEnabled;
}

const std::string	LocationConfig::getUploadPath() const { return _uploadPath; }

// ----SETTERS--------------------------------------------------------------- //
void	LocationConfig::setPath(std::string const& path) {
	if (path.substr(path.length() - 1) != "/")
		throw std::runtime_error("bad location");
	else
		_path = path;
}

void	LocationConfig::setRoot(std::string const& root) { _root = root; }
void	LocationConfig::setAutoIndex(bool state) { _autoIndex = state; }

void	LocationConfig::setIndexFile(std::string const& indexFile) {
	_indexFile = indexFile;
}

void	LocationConfig::setUploadPath(std::string const& path) {
	_uploadEnabled = true;
	_uploadPath = path;
}

void	LocationConfig::insertCgiPath(std::string const& extention, std::string const& path) {
	// _cgiEnabled = true;
	_cgi[extention] = path;
}

void	LocationConfig::setCgiEnabled(bool state) 
{
	_cgiEnabled = state;
}

void	LocationConfig::setUploadEnabled(bool state) 
{
	_uploadEnabled = state;
}

std::ostream&	operator<<(std::ostream& os, LocationConfig const& other) {
	os  << "[LocationConfig]"<< std::endl
		<< "Path: " << other.getPath() << std::endl
		<< "Root: " << other.getRoot() << std::endl
		<< "AutoIndex: " << other.getAutoIndex() << std::endl
		<< "IndexFile: " << other.getIndexFile() << std::endl
		<< "CgiEnabled: " << other.getCgiEnabled() << std::endl
		<< "[Cgi] \n"; printMap(other.getCgi());
	os  << "UploadEnabled: " << other.getUploadEnabled() << std::endl
		<< "UploadPath: " << other.getUploadPath() << std::endl;
	return os;
}
