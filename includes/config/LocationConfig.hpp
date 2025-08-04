#pragma once

#include <iostream>
#include <map>
#include <string>

class LocationConfig {
	private:
		std::string							_path;
		std::string							_root;
		bool								_autoIndex;
		std::string							_indexFile;
		bool								_cgiEnabled;
		std::map<std::string, std::string>	_cgi;
		bool								_uploadEnabled;
		std::string							_uploadPath;

	public:
		LocationConfig();
		LocationConfig( LocationConfig const& other) ;
		LocationConfig& operator=(LocationConfig const& other) ;
		~LocationConfig();

		const std::string&							getPath() const ;
		const std::string&							getRoot() const ;
		const bool&									getAutoIndex() const ;
		const std::string&							getIndexFile() const ;
		const bool&									getCgiEnabled() const ;
		const std::map<std::string, std::string>&	getCgi() const ;
		const std::string							getCgiPathFor(std::string extension) const ;
		const bool&									getUploadEnabled() const ;
		const std::string							getUploadPath() const ;

		void	setPath(std::string const& path) ;
		void	setRoot(std::string const& root) ;
		void	setAutoIndex(bool state) ;
		void	setIndexFile(std::string const& indexFile) ;
		void	setCgiEnabled(bool state);
		void	setUploadEnabled(bool state);
		void	setUploadPath(std::string const& path) ;
		void	insertCgiPath(std::string const& extention, std::string const& path) ;
};

std::ostream&	operator<<(std::ostream& os, LocationConfig const& other) ;
