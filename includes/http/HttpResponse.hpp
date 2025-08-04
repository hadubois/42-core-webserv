#pragma once

#include "HttpRequest.hpp"
#include <map>
#include <string>

class HttpResponse {
	private:
		std::string							_version;
		int									_statusCode;
		std::map<int, std::string>			_reasonPhrases;
		std::string							_reasonPhrase;

		std::map<std::string, std::string>	_headers;
		std::string							_contentType;
		size_t								_contentLen;

		std::string							_body;

		std::string							_defaultErrorPage;

	public:
		HttpResponse();
		~HttpResponse();

		const std::string&							getVersion() const ;
		const int&									getStatusCode() const ;
		const std::string							getReasonPhrase(int code) const ;
		const std::string							getReasonPhrase() const ;
		const std::map<std::string, std::string>&	getHeaders() const ;
		const std::string&							getContentType() const ;
		size_t										getContentLen() const ;
		const std::string&							getBody() const ;
		const std::string&							getDefaultErrorPage() const ;

		void	setVersion(std::string version) ;
		void	setCode(int statusCode) ;
		void	insertHeader(std::string key, std::string value);
		void	setContentType(std::string contentType) ;
		void	setContentLen(size_t contentLen) ;
		void	setBody(std::string body) ;
		void	appendBody(std::string str) ;
		void	setDefaultErrorPage(std::string path) ;

		std::string		getCurrentHttpDate() const ;
		std::string		toString() ;
		std::string		generateCgiBody() ;
		std::string		generateBasicHtml(const std::string& content);
		std::string		errorToString(int errCode) ;
		std::string		putRightMessage(std::string const& path, int statusCode, std::string statusPhrase);
};

std::string		buildErrorRes(int errCode);
std::ostream&	operator<<(std::ostream& os, HttpResponse& other) ;
