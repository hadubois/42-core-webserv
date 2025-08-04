#pragma once

#include "HttpRequest.hpp"
#include "ServerConfig.hpp"

class Router {
	public:
		Router() ;
		~Router() ;

		void	validateRequest(ServerConfig& sConfig, HttpRequest& req) ;
		void	validateStartLine(ServerConfig& sConfig, HttpRequest& req) ;
		void	validateHeaders(ServerConfig& sConfig, HttpRequest& req) ;

		void	route(ServerConfig& config, HttpRequest& req) ;

		void	defineRequestType(ServerConfig& sc, HttpRequest& req);
		bool	isDirectory(std::string const& path) ;
		bool	isFile(std::string const& path) ;
		bool	fileExist(std::string const& path) ;
		bool	cgiMatch(LocationConfig const& config, HttpRequest& req) const ;
		void	checkMethodSemantic(HttpRequest& req) ;
		void	checkGet(HttpRequest& req) ;
};
