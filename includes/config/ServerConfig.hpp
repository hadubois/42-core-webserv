#pragma once

#include "LocationConfig.hpp"
#include <iostream>
#include <map>
#include <set>
#include <string>

class ServerConfig {
	private:
		std::set<std::string>					_allowedMethods;
		std::string								_allowedVersion;
		std::string								_root;
		int										_allowedPort;
		std::map<std::string, LocationConfig>	_locations;
		std::string								_defaultErrorPage;
		std::string								_redirectionPage;
		std::string								_redirectionUrl;
		size_t									_maxBodySize;
		size_t									_maxHeaderSize;

	public:
		ServerConfig() ;
		ServerConfig(ServerConfig const& other) ;
		ServerConfig& operator=(ServerConfig const& other) ;
		~ServerConfig() ;

		const std::set<std::string>&					getAllowedMethods() const ;
		const std::string&								getAllowedVersion() const ;
		const std::string&								getRoot() const ;
		const int&										getAllowedPort() const ;
		const std::map<std::string, LocationConfig>&	getLocationConfig() const ;
		const std::map<std::string, std::string>&		getMimeTypeMap();
		const std::string								getDefaultErrorPage() const ;
		const std::string								getRedirectionPage() const ;
		const std::string								getRedirectionUrl() const ;
		const size_t& 									getMaxBodySize() const ;
		const size_t& 									getMaxHeaderSize() const ;

		void					setDefaultErrorPage(std::string path) ;
		void					setRedirectionPage(std::string page) ;
		void					setRedirectionUrl(std::string url) ;
		void					setMaxBodySize(size_t size) ;
		void					setMaxHeaderSize(size_t size) ;

		void					insertLocationConfig(LocationConfig const& lc) ;
		void					insertMethod(std::string const& method) ;
		const LocationConfig&	defineBestLocation(std::string uri) const ;
};

std::ostream&	operator<<(std::ostream& os, ServerConfig& other) ;
