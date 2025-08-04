#include "Router.hpp"
#include "MimeTypes.hpp"
#include "log_macros.hpp"
#include <fstream>
#include <string>
#include <set>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

Router::Router() {}

Router::~Router() {}

void	Router::route(ServerConfig& config, HttpRequest& req) {
	validateRequest(config, req);
	if (req.getState() == RS_CLOSE)
		return ;

	defineRequestType(config, req);
	if (req.getState() == RS_CLOSE) {
		return ;
	}

	req.setState(RS_ROUTING_DONE);
}

// ----VALIDATION------------------------------------------------------------ //
void	Router::validateRequest(ServerConfig& config, HttpRequest& req) {
	validateStartLine(config, req);
	if (req.getState() == RS_START_LINE_DONE)
		validateHeaders(config, req);
}

void	Router::validateStartLine(ServerConfig& config, HttpRequest& req) {
	std::set<std::string>::const_iterator	mIt = config.getAllowedMethods().find(req.getMethod());
	if (mIt == config.getAllowedMethods().end()) {
		ERROR("405 Method Not Allowed");
		req.setErrCode(405);
		req.setState(RS_CLOSE);
		return ;
	}

	if (req.getVersion().substr(0, 5) != "HTTP/") {
		ERROR("400 Bad Request: incorrect version syntax");
		req.setErrCode(400);
		req.setState(RS_CLOSE);
		return ;
	} else if (req.getVersion() != config.getAllowedVersion()) {
		ERROR("505 Vesrion Not Supported");
		req.setErrCode(505);
		req.setState(RS_CLOSE);
		return ;
	}

	req.setState(RS_START_LINE_DONE);
}

void	Router::validateHeaders(ServerConfig& config, HttpRequest& req) {
	(void)config;
	if (req.getMethod() == "POST" && req.findHeader("Content-Length") == "") {
		ERROR("400: Bad Request: method POST without Content-Length");
		req.setErrCode(400);
		req.setState(RS_CLOSE);
		return ;
	}
	req.setState(RS_HEADERS_DONE);
}

// ----DEFINE-TYPE----------------------------------------------------------- //
void	Router::defineRequestType(ServerConfig& sc, HttpRequest& req) {
	std::string		uri = req.getUri();
	LocationConfig	lc = sc.defineBestLocation(uri);
	req.setLocationConfig(lc);
	std::string		trimUri = uri.substr(lc.getPath().length());
	std::string		url = lc.getRoot() + trimUri;
	req.setUrl(url);

	if (!sc.getRedirectionPage().empty() && sc.getRedirectionPage() == req.getUrl()) {
		if (sc.getRedirectionUrl().empty()) {
			ERROR("500 Internal Error: empty redirection url");
			req.setErrCode(500);
			req.setState(RS_CLOSE);
			return ;
		}
		req.setRedirUrl(sc.getRedirectionUrl());
		req.setType(REDIRECTION);
	} else if (fileExist(url)) {
		if (isDirectory(url)) {
			req.setType(DIRECTORY);
			if (url.compare(url.length() - 1, 0, "/") == 0) {
				ERROR("301 Moved Permanently: directory without `/`");
				req.setErrCode(301);
				req.setState(RS_CLOSE);
				return ;
			}
			if (req.getMethod() == "POST") {
				if (cgiMatch(lc, req))
					req.setType(CGI);
			}
		} else if (isFile(url)) {
			if (lc.getCgiEnabled()
					&& cgiMatch(lc, req)
					/* TODO ?? && access(req.getUrl().c_str(), X_OK) == 0*/) {
				req.setType(CGI);
			} else {
				req.setType(REG_FILE);
			}
		} else {
			ERROR("403 Forbidden: url");
			req.setErrCode(403);
			req.setState(RS_CLOSE);
			return ;
		}
	} else {
		ERROR("404 Not Found: url: " << url);
		req.setErrCode(404);
		req.setState(RS_CLOSE);
		return ;
	}
}

bool	Router::fileExist(std::string const& path) {
	struct stat	s;
	if ((stat(path.c_str(), &s)) == 0)
		return true;
	return false;
}

bool	Router::isDirectory(std::string const& path) {
	struct stat	s;
	if ((stat(path.c_str(), &s)) == 0)
		return (S_ISDIR(s.st_mode));
	return false;
}

bool	Router::isFile(std::string const& path) {
	struct stat	s;
	if ((stat(path.c_str(), &s)) == 0)
		return (S_ISREG(s.st_mode));
	return false;
}

bool	Router::cgiMatch(LocationConfig const& lc, HttpRequest& req) const {
	std::string	fullPath = req.getUrl();
	if (req.getMethod() == "POST")
		fullPath = req.getFileName();
	std::map<std::string, std::string>	cgi = lc.getCgi();
	std::map<std::string, std::string>::const_iterator	it;

	for (it = cgi.begin(); it != cgi.end(); it++) {
		std::string	ext = it->first;
		if (ext.length() > fullPath.length())
			continue ;
		else if (fullPath.compare(fullPath.length() - ext.length(), ext.length(), ext) == 0) {
			req.setCgiExecutor(it->second);
			return true;
		}
	}
	return false;
}
