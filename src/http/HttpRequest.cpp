#include "HttpRequest.hpp"
#include "generic_utils.hpp"
#include "log_macros.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib>

HttpRequest::HttpRequest()
	:
	_port(-1),
	_state(RS_IDLE),
	_errCode(0)
{}

HttpRequest::~HttpRequest() {}

// ----GETTERS--------------------------------------------------------------- //
const std::string&	HttpRequest::getMethod() const { return _method; }
const std::string&	HttpRequest::getUri() const { return _uri; }
const std::string&	HttpRequest::getUrl() const { return _url; }
const int&			HttpRequest::getPort() const { return _port; }
const std::string&	HttpRequest::getVersion() const { return _version; }

const std::map<std::string, std::string>&	HttpRequest::getHeaders() const {
	return _headers;
}

const std::string&	HttpRequest::getBody() const { return _body; }
const ReqState&		HttpRequest::getState() const { return _state; }
const int&			HttpRequest::getErrCode() const { return _errCode; }
e_type				HttpRequest::getType() const { return _type; }

const LocationConfig&	HttpRequest::getLocationConfig() const {
	return _lc;
}

const std::string&							HttpRequest::getRedirUrl() const {
	return _redirUrl;
}
const std::string&	HttpRequest::getFileName() const { return _fileName; }
const size_t& 	HttpRequest::getMaxBodySize() const { return _maxBodySize; }
const size_t& 	HttpRequest::getMaxHeaderSize() const { return _maxHeaderSize; }
const std::string	HttpRequest::getCgiExecutor() const { return _cgiExecutor; }

// ----SETTERS--------------------------------------------------------------- //
void	HttpRequest::setMethod(std::string const& method) { _method = method; }
void	HttpRequest::setUri(std::string const& uri) { _uri = uri; }
void	HttpRequest::setUrl(std::string const& url) { _url = url; }
void	HttpRequest::setPort(int const& port) { _port = port; }

void	HttpRequest::setVersion(std::string const& version) {
	_version = version;
}

void	HttpRequest::insertHeader(std::string const& key, std::string const& value) {
	_headers[key] = value;
}

void	HttpRequest::appendBody(char& body) { _body.push_back(body); }
void	HttpRequest::setBody(std::string const& str) { _body = str; }
void	HttpRequest::setState(ReqState	state) { _state = state; }
void	HttpRequest::setErrCode(int	errCode) { _errCode = errCode; }
void	HttpRequest::setType(e_type	type) { _type = type; }
void	HttpRequest::setLocationConfig(LocationConfig const& lc) { _lc = lc; }
void	HttpRequest::setRedirUrl(std::string const& redirUrl ) { _redirUrl = redirUrl; }
void	HttpRequest::setFileName(std::string const& fileName ) { _fileName = fileName; }
void	HttpRequest::setMaxBodySize(size_t size) {
	_maxBodySize = size;
}

void	HttpRequest::setMaxHeaderSize(size_t size) {
	_maxHeaderSize = size;
}

void	HttpRequest::setCgiExecutor(std::string const& executor) {
	_cgiExecutor = executor;
}

// ----METHODS--------------------------------------------------------------- //
const std::string	HttpRequest::findHeader(std::string const& key) const {
	std::map<std::string, std::string>::const_iterator	it = _headers.find(key);

	if (it != _headers.end())
		return it->second;
	return "";
}

void	HttpRequest::printHeaders() const {
	std::map<std::string, std::string>::const_iterator	it;

	for (it = _headers.begin(); it != _headers.end(); it++) {
		std::cout << it->first << ":" << it->second << std::endl;
	}
}

std::ostream&	operator<<(std::ostream& os, HttpRequest& other) {
	os  << "[HttpRequest]"<< std::endl
		<< "Method: " << other.getMethod() << std::endl
		<< "Uri: " << other.getUri() << std::endl
		<< "Url: " << other.getUrl() << std::endl
		<< "Port: " << other.getPort() << std::endl
		<< "Version: " << other.getVersion() << std::endl
		<< "[Headers]" << std::endl; printMap(other.getHeaders());
	os  << "Body: " << other.getBody() << std::endl
		<< "Type: " << other.getType() << std::endl
		<< "State: " << other.getState() << std::endl
		<< "ErrCode: " << other.getErrCode() << std::endl
		<< "LocationConfig: " << other.getLocationConfig() << std::endl
		<< "RedirUrl: " << other.getRedirUrl() << std::endl
		<< "FileName: " << other.getFileName() << std::endl
		<< "MaxBodySize: " << other.getMaxBodySize() << std::endl
		<< "MaxHeaderSize: " << other.getMaxHeaderSize() << std::endl
		<< "CgiExecutor: " << other.getCgiExecutor() << std::endl;
	return os;
}
