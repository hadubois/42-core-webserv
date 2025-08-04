#include "Parser.hpp"
#include "generic_utils.hpp"
#include <sstream>
#include <cstdlib>
#include <cstring>

#define MULTIPART "multipart/form-data"
#define BOUNDARY  "boundary="
#define CRLF      "\r\n\r\n"
#define ENDMARK   "--\r\n\r\n"

Parser::Parser() {}
Parser::~Parser() {}

const size_t&	Parser::getContentLength() const { return _contentLength; }

void		Parser::parse(std::string& rawBuf, HttpRequest& req) {
	size_t		pos;

	if (req.getState() == RS_IDLE
			&& (pos = rawBuf.find("\r\n")) != std::string::npos) {
		parseStartLine(rawBuf, req);
		if (req.getErrCode() != 0)
			return ;
	}

	if (req.getState() == RS_START_LINE_READ
			&& (pos = rawBuf.find("\r\n\r\n")) != std::string::npos) {
		parseHeaders(rawBuf, req);
		if (req.getErrCode() != 0)
			return ;
	}

	if (req.getState() == RS_HEADERS_READ
			&& (req.findHeader("Content-Length") != ""
				|| req.findHeader("Transfert-Encoding") != "")) {
		parseBody(rawBuf, req);
		if (req.getErrCode() != 0) {
			return ;
		}
	} else {
		req.setState(RS_NO_BODY);
	}

	if (req.getState() == RS_NO_BODY && !rawBuf.empty()) {
		req.setState(RS_CLOSE);
		req.setErrCode(400);
		return ;
	}
}

void	Parser::parseStartLine(std::string& rawBuf, HttpRequest& req) {
	size_t	pos = rawBuf.find("\r\n");
	if (pos == std::string::npos) {
		ERROR("parseStartLine: \r\n not found");
		req.setErrCode(400);
		req.setState(RS_CLOSE);
		return ;
	}

	std::string	startLine = rawBuf.substr(0, pos);
	if (startLine.empty()) {
		ERROR("parseStartLine: empty start line");
		req.setErrCode(400);
		req.setState(RS_CLOSE);
		return ;
	}
	rawBuf.erase(0, pos + 2);

	std::stringstream	ss1(startLine);
	std::string			method, uri, version;
	ss1 >> method >> uri >> version;
	if (method.empty() || uri.empty() || version.empty()) {
		ERROR("parseStartLine: empty start line arguments");
		req.setErrCode(400);
		req.setState(RS_CLOSE);
		return ;
	}
	req.setMethod(method);
	req.setUri(uri);
	req.setVersion(version);

	req.setState(RS_START_LINE_READ);
}

void	Parser::parseHeaders(std::string& rawBuf, HttpRequest& req) {
	std::string	headersBuf;
	size_t		endPos;

	if ((endPos = rawBuf.find("\r\n\r\n")) != std::string::npos) {
		headersBuf = rawBuf.substr(0, endPos + 2);
		rawBuf.erase(0, endPos + 4);
	}

	if (headersBuf.length() + 2 > req.getMaxHeaderSize()) {
		ERROR("400 Bad Request: invalid header length");
		req.setErrCode(400);
		req.setState(RS_CLOSE);
		return ;
	}

	std::stringstream	ss(headersBuf);
	std::string			line;
	while (std::getline(ss, line)) {
		if (line.find("\r") == std::string::npos) {
			ERROR("400 Bad Request: invalid headers: no CRLF");
			req.setErrCode(400);
			req.setState(RS_CLOSE);
			return ;
		}
		size_t	pos = std::string::npos;

		if((pos = line.find(":")) != std::string::npos) {
			std::string key = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			value = trim(value);

			if (key.find(" ") != std::string::npos
					|| key.empty()
					|| value.empty()) {
				ERROR("431 Request Header Fields Too Large: invalid headers: space found");
				req.setErrCode(431);
				req.setState(RS_CLOSE);
				return ;
			}

			req.insertHeader(key, value);
		} else {
			ERROR("400 Bad Request: `:` not found");
			req.setErrCode(400);
			req.setState(RS_CLOSE);
			return ;
		}
	}

	if (req.findHeader("Host") == "") {
		ERROR("400 Bad Request: Host not found");
		req.setErrCode(400);
		req.setState(RS_CLOSE);
		return ;
	}

	req.setState(RS_HEADERS_READ);
}

void	Parser::parseBody(std::string& rawBuf, HttpRequest& req) {
	std::string	value;
	if ((value = req.findHeader("Content-Length")) != "") {
		if (req.findHeader("Transfer-Encoding") != "") {
			ERROR("400 Bad Request: both Content-Length and Transfert-Encoding");
			req.setErrCode(400);
			req.setState(RS_CLOSE);
			return;
		}

		for (size_t i = 0; i < value.length() ; ++i) {
			if (!std::isdigit(value[i])) {
				ERROR("400 Bad Request: not only digit in content-length");
				req.setErrCode(400);
				req.setState(RS_CLOSE);
				req.setState(RS_CLOSE);
				return ;
			}
		}

		std::istringstream iss(value);
		size_t len = 0;
		iss >> len;
		if (iss.fail()) {
			ERROR("400 Bad Request: not only digit in content-length");
			req.setErrCode(400);
			req.setState(RS_CLOSE);
			return ;
		}

		_contentLength = len;
		if (_contentLength == 0) {
			ERROR("400 Bad Request: Content-Length == 0");
			req.setErrCode(400);
			req.setState(RS_CLOSE);
			return ;
		}

		if (len >= req.getMaxBodySize()) {
			ERROR("413 Payload Too Large: invalid body length");
			req.setErrCode(413);
			req.setState(RS_CLOSE);
			return ;
		}

		for (size_t i = 0; !rawBuf.empty() && req.getBody().length() < _contentLength; i++) {
			req.appendBody(rawBuf[0]);
			rawBuf.erase(0, 1);
		}

		if (req.getBody().length() == _contentLength) {
			if (!rawBuf.empty()) {
				ERROR("400 Bad Request: more data than expected: [" << rawBuf << "]");
				req.setErrCode(400);
				req.setState(RS_CLOSE);
				return ;
			} else {
				req.setState(RS_BODY_DONE);
			}
		}

	} else if (req.findHeader("Transfert-Encoding") != "") {
		ERROR("501 Not implemented: TE != chunked");
		req.setErrCode(501);
		req.setState(RS_CLOSE);
		return ;
	}

	if (req.getState() == RS_BODY_DONE && req.findHeader("Content-Type").find(MULTIPART) != std::string::npos) {
		if (!handleBoundary(req)) {
			req.setErrCode(400);
			req.setState(RS_CLOSE);
		}
	}
}

bool	Parser::handleBoundary(HttpRequest& req) {
	std::string	content_type = req.findHeader("Content-Type");
	size_t		pos;
	pos = content_type.find(BOUNDARY);
	if (pos == std::string::npos) {
		ERROR("400 Bad Request: boundary not found");
		return false;
	}

	std::string	buf = req.getBody();
	std::string	boundary = "--" + content_type.substr(pos + std::strlen(BOUNDARY));
	pos = buf.find(boundary);
	size_t	endPos = buf.find(CRLF);
	if (pos == std::string::npos || endPos == std::string::npos) {
		ERROR("400 Bad Request: bad buf structure");
		return false;
	}
	std::string	headers = buf.substr(pos + boundary.length(), endPos - boundary.length() + strlen(CRLF));
	buf.erase(0, endPos + std::strlen(CRLF));

	std::string	endMark = boundary + "--";
	endPos = buf.find(endMark.c_str());
	if (pos == std::string::npos) {
		ERROR("400 Bad Request: bad body structure: no end boundary");
		return false;
	}
	std::string	body = buf.substr(0, endPos);
	buf.erase(0, endPos + boundary.length() + std::strlen(CRLF));
	if (!buf.empty()) {
		ERROR("400 Bad Request: bad body structure: datas after end boundary mark");
		return false;
	}

	req.setBody(body);

	std::istringstream	stream(headers);
	std::string			line;
	std::string			name;
	while (std::getline(stream, line)) {
		if (line.find("Content-Disposition:") == 0) {
			size_t namePos = line.find("name=\"");
			if (namePos != std::string::npos) {
				size_t nameEnd = line.find("\"", namePos + 6);
				name = line.substr(namePos + 6, nameEnd - (namePos + 6));
			}
			size_t filePos = line.find("filename=\"");
			if (filePos != std::string::npos) {
				size_t fileEnd = line.find("\"", filePos + 10);
				req.setFileName(line.substr(filePos + 10, fileEnd - (filePos + 10)));
			}
		} else if (line.find("Content-Type:") == 0) {
			req.insertHeader("Content-Type", line.substr(strlen("Content-Type: ")));
		}
	}

	if (req.getState() == RS_CLOSE)
		return false;

	return true;
}

std::ostream&	operator<<(std::ostream& os, Parser& other) {
	os  << "[Parser]"<< std::endl
		<< "getContentLength: " << other.getContentLength() << std::endl;
	return os;
}
