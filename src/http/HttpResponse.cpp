#include "HttpResponse.hpp"
#include "generic_utils.hpp"
#include "log_macros.hpp"
#include "webserv_utils.hpp"
#include <cstdlib>
#include <ctime>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#define PATH "./tmp/www/cgi_response.html"

HttpResponse::HttpResponse()
:
	_version("HTTP/1.1"),
	_statusCode(200),
	_contentType("text/plain"),
	_contentLen(0)
{
	_reasonPhrases = getStatusCodeMap();
}

HttpResponse::~HttpResponse() {}

// ----GETTERS--------------------------------------------------------------- //
const std::string&	HttpResponse::getVersion() const { return _version; }
const int&			HttpResponse::getStatusCode() const { return _statusCode; }

const std::string	HttpResponse::getReasonPhrase(int code) const {
	std::map<int, std::string>::const_iterator it = _reasonPhrases.find(code);
	return it->second;
}

const std::string	HttpResponse::getReasonPhrase() const {
	std::map<int, std::string>::const_iterator it = _reasonPhrases.find(_statusCode);
	return it->second;
}

const std::map<std::string, std::string>&	HttpResponse::getHeaders() const {
	return _headers;
}

const std::string&	HttpResponse::getContentType() const {
	return _contentType;
}

size_t				HttpResponse::getContentLen() const {
	return _body.length();
}

const std::string&	HttpResponse::getBody() const { return _body; }

const std::string&	HttpResponse::getDefaultErrorPage() const {
	return _defaultErrorPage;
}

// ----SETTERS--------------------------------------------------------------- //
void	HttpResponse::setVersion(std::string version) { _version = version; }
void	HttpResponse::setCode(int statusCode) { _statusCode = statusCode; }

void	HttpResponse::insertHeader(std::string key, std::string value) {
	_headers[key] = value;
}

void	HttpResponse::setContentType(std::string contentType) {
	_contentType = contentType;
}

void	HttpResponse::setContentLen(size_t length) {
	_contentLen = length;
}

void	HttpResponse::setBody(std::string body) { _body = body; }
void	HttpResponse::appendBody(std::string str) { _body += str; }

void	HttpResponse::setDefaultErrorPage(std::string path) {
	_defaultErrorPage = path;
}

// ----METHODS--------------------------------------------------------------- //

std::string	HttpResponse::getCurrentHttpDate() const {
	std::time_t now = std::time(NULL);
	std::tm* gmt = std::gmtime(&now);

	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", gmt);

	return std::string(buffer);
}

// ----BUILD----------------------------------------------------------------- //
std::string		HttpResponse::toString() {
	std::string	buf, startLine, headers;
	startLine = _version + " " + itoa(_statusCode) + " " + getReasonPhrase(_statusCode) + "\r\n";

	insertHeader("Date", getCurrentHttpDate());
	insertHeader("Content-Length", itoa(getContentLen()));
	std::map<std::string, std::string>::const_iterator	it;
	for (it = _headers.begin(); it != _headers.end(); it++) {
		headers += it->first + ": " + it->second + "\r\n";
	}
	headers += "\r\n";

	buf = startLine + headers + _body;
	return buf;
}

std::string	HttpResponse::errorToString(int errCode) {
	setCode(errCode);
	insertHeader("Content-Type", "text/html");
	_body = putRightMessage(_defaultErrorPage, _statusCode, getReasonPhrase(_statusCode));

	std::string response = toString();
	return response;
}

std::string	HttpResponse::putRightMessage(std::string const& path, int statusCode, std::string statusPhrase) {
	std::fstream	fs(path.c_str());
	std::string		buf;

	if (!fs.is_open()){
		buf = generateBasicHtml("Error " + itoa(statusCode) + ": " + statusPhrase);
		return buf;
	}
	else {
		std::string	line;
		while (std::getline(fs, line)) {
			if (line.find("OUPS") != std::string::npos) {
				buf += "<h1>" + statusPhrase + "</h1>";
			} else if (line.find("ERREUR") != std::string::npos ) {
				buf += "<h1>" + itoa(statusCode) + "</h1>";
			} else {
				buf += line;
			}
			buf += "\n";
		}
			buf = buf.substr(0, buf.length() - 1);

	}
	return buf;
}

std::string	HttpResponse::generateCgiBody() {
	std::string path(PATH);
	std::fstream	fs(path.c_str());
	std::string		buf;

	insertHeader("Content-Type", "text/html");
	if (!fs.is_open()) {
		buf = generateBasicHtml("CGI RETURN: " + _body);
		return buf;
	}
	else {
		std::string	line;
		while (std::getline(fs, line)) {
			if (line.find("REPLACE") != std::string::npos) {
				buf += "<h1>" + _body + "</h1>";
			} else {
				buf += line;
			}
			buf += "\n";
		}
			buf = buf.substr(0, buf.length() - 1);

	}
	return buf;
}
#include <string>

std::string	HttpResponse::generateBasicHtml(const std::string& content) {
	std::string html =
		"<html lang=\"en\">\n"
		"<body style=\"background-color: #6B46C1; color: #EDF2F7;\">\n"
		"  <div style=\"display: flex; align-items: center; justify-content: center; height:100%\">\n"
		"    <div>\n"
		"      <h1>" + content + "</h1>\n"
		"    </div>\n"
		"  </div>\n"
		"</body>\n"
		"</html>\n";
	return html;
}

// ----NON-MEMBER------------------------------------------------------------ //
std::string	buildErrorRes(int errCode) {
	HttpResponse	res;
	res.setCode(errCode);
	res.insertHeader("Content-Type", "text/html");
	res.setBody(res.generateBasicHtml("Error " + itoa(errCode) + ": " + res.getReasonPhrase(errCode)));
	std::string response = res.toString();
	return response;
}

std::ostream&	operator<<(std::ostream& os, HttpResponse& other) {
	os  << "[HttpResponse]"<< std::endl
		<< "Version: " << other.getVersion() << std::endl
		<< "StatusCode: " << other.getStatusCode() << std::endl
		<< "ReasonPhrase: " << other.getReasonPhrase() << std::endl
		<< "[Headers]" << std::endl; printMap(other.getHeaders());
	os  << "ContentType: " << other.getContentType() << std::endl
		<< "ContentLen: " << other.getContentLen() << std::endl
		<< "Body: " << other.getBody() << std::endl
		<< "DefaultErrorPage: " << other.getDefaultErrorPage() << std::endl;
	return os;
}
