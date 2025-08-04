#pragma once

#include "LocationConfig.hpp"
#include "request_state.hpp"
#include <iostream>
#include <map>
#include <string>
#include <set>

class HttpRequest {
	private:
		std::string							_method;
		std::string							_uri;
		std::string							_url;
		int									_port;
		std::string							_version;

		std::map<std::string, std::string>	_headers;

		std::string							_body;

		e_type								_type;
		ReqState							_state;
		int									_errCode;
		LocationConfig						_lc;
		std::string							_redirUrl;
		std::string							_fileName;
		size_t								_maxBodySize;
		size_t								_maxHeaderSize;
		std::string							_cgiExecutor;

	public:
		HttpRequest() ;
		~HttpRequest() ;

		const std::string&							getMethod() const ;
		const std::string&							getUri() const ;
		const std::string&							getUrl() const ;
		const int&									getPort() const ;
		const std::string&							getVersion() const ;
		const std::map<std::string, std::string>&	getHeaders() const ;
		const std::string&							getBody() const ;
		const ReqState&								getState() const ;
		const int&									getErrCode() const ;
		e_type										getType() const ;
		const LocationConfig&						getLocationConfig() const ;
		const std::string&							getRedirUrl() const ;
		const std::string&							getFileName() const ;
		const size_t&								getMaxBodySize() const ;
		const size_t&								getMaxHeaderSize() const ;
		const std::string							getCgiExecutor() const;

		void	setMethod(std::string const& method) ;
		void	setUri(std::string const& uri) ;
		void	setUrl(std::string const& url) ;
		void	setPort(int const& port) ;
		void	setVersion(std::string const& version) ;
		void	setBody(std::string const& str) ;
		void	insertHeader(std::string const& key, std::string const& value);
		void	appendBody(char& body) ;
		void	setState(ReqState	state) ;
		void	setErrCode(int	errCode) ;
		void	setType(e_type type) ;
		void	setLocationConfig(LocationConfig const& lc) ;
		void	setRedirUrl(std::string const& redirUrl ) ;
		void	setFileName(std::string const& fileName) ;
		void	setMaxBodySize(size_t size) ;
		void	setMaxHeaderSize(size_t size) ;
		void	setCgiExecutor(std::string const& executor) ;

		const std::string	findHeader(std::string const& key) const ;
		void				printHeaders() const ;
};

std::ostream&	operator<<(std::ostream& os, HttpRequest& other) ;
