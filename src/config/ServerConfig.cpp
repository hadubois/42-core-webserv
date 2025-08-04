#include "ServerConfig.hpp"
#include "log_macros.hpp"
#include "generic_utils.hpp"

ServerConfig::ServerConfig() {
	_allowedVersion = "HTTP/1.1";
	_root = "./tmp/www";
	_allowedPort = 8080;

	LocationConfig	defaultConfig;
	_locations["/"] = defaultConfig;
}

ServerConfig::ServerConfig(ServerConfig const& other)
	: _allowedMethods(other._allowedMethods),
	  _allowedVersion(other._allowedVersion),
	  _root(other._root),
	  _allowedPort(other._allowedPort),
	  _locations(other._locations),
	  _defaultErrorPage(other._defaultErrorPage),
	  _redirectionPage(other._redirectionPage),
	  _redirectionUrl(other._redirectionUrl),
	  _maxBodySize(other._maxBodySize),
	  _maxHeaderSize(other._maxHeaderSize)
{}

ServerConfig& ServerConfig::operator=(ServerConfig const& other) {
	if (this != &other) {
		_allowedMethods = other._allowedMethods;
		_allowedVersion = other._allowedVersion;
		_root = other._root;
		_allowedPort = other._allowedPort;
		_locations = other._locations;
		_defaultErrorPage = other._defaultErrorPage;
		_redirectionPage = other._redirectionPage;
		_redirectionUrl = other._redirectionUrl;
		_maxBodySize = other._maxBodySize;
		 _maxHeaderSize = other._maxHeaderSize;
	}
	return *this;
}

ServerConfig::~ServerConfig() {}

// ----GETTERS--------------------------------------------------------------- //
const std::set<std::string>&	ServerConfig::getAllowedMethods() const {
	return _allowedMethods;
}

const std::string&				ServerConfig::getAllowedVersion() const {
	return _allowedVersion;
}

const std::string&				ServerConfig::getRoot() const {
	return _root;
}

const int&						ServerConfig::getAllowedPort() const {
	return _allowedPort;
}

const std::map<std::string, LocationConfig>&	ServerConfig::getLocationConfig() const {
	return _locations;
}

const std::string	ServerConfig::getDefaultErrorPage() const {
	return _defaultErrorPage;
}

const std::string	ServerConfig::getRedirectionPage() const {
	return _redirectionPage;
}

const std::string	ServerConfig::getRedirectionUrl() const {
	return _redirectionUrl;
}
const size_t& 	ServerConfig::getMaxBodySize() const { return _maxBodySize; }
const size_t& 	ServerConfig::getMaxHeaderSize() const { return _maxHeaderSize; }

// ----SETTERS--------------------------------------------------------------- //
void	ServerConfig::insertLocationConfig(LocationConfig const& lc) {
	_locations[lc.getPath()] = lc;
}

void	ServerConfig::setDefaultErrorPage(std::string path) {
	_defaultErrorPage = path;
}

void	ServerConfig::setRedirectionPage(std::string page) {
	_redirectionPage = page;
}

void	ServerConfig::setRedirectionUrl(std::string url) {
	_redirectionUrl =url;
}

void	ServerConfig::setMaxBodySize(size_t size) {
	_maxBodySize = size;

}

void	ServerConfig::setMaxHeaderSize(size_t size) {
	_maxHeaderSize = size;
}

// ----METHODS--------------------------------------------------------------- //
const LocationConfig&	ServerConfig::defineBestLocation(std::string uri) const {
	std::map<std::string, LocationConfig>::const_iterator	it;
	std::string												longestPath;
	std::string												currentPath;

	for (it = _locations.begin(); it != _locations.end(); it++) {
		if (uri.find(it->second.getPath()) != std::string::npos) {
			currentPath = it->second.getPath();
			if (currentPath.length() > longestPath.length()) {
				longestPath = currentPath;
			}
		}
	}

	return _locations.find(longestPath)->second;
}

void	ServerConfig::insertMethod(std::string const& method) {
	_allowedMethods.insert(method);
}

std::ostream&	operator<<(std::ostream& os, ServerConfig& other) {
	os  << "[ServerConfig]"<< std::endl
		<< "[AllowedMethods] " << std::endl; printSet(other.getAllowedMethods());
	os  << "AllowedVersion: " << other.getAllowedVersion() << std::endl
		<< "Root: " << other.getRoot() << std::endl
		<< "AllowedPort: " << other.getAllowedPort() << std::endl
		<< "[Locations]" << std::endl; printMapTClass(other.getLocationConfig());
	os  << "DefaultErrorPage: " << other.getDefaultErrorPage() << std::endl
		<< "RedirectionPage: " << other.getRedirectionPage() << std::endl
		<< "MaxBodySize: " << other.getMaxBodySize() << std::endl
		<< "MaxHeaderSize: " << other.getMaxHeaderSize() << std::endl
		<< "RedirectionUrl: " << other.getRedirectionUrl() << std::endl;
	return os;
}
