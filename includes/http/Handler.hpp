#pragma once

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <string>
#include <vector>

class Handler {
	public:
		Handler();
		~Handler();

		void	handle(HttpRequest& req, HttpResponse& res) ;
		void	setMimeType(std::string path, HttpResponse& res) ;

		void		handleCgi(HttpRequest& req, HttpResponse& res) ;
		void		handleCgiPost(HttpRequest& req, HttpResponse& res) ;
		std::string	execute_cgi(std::string program_executor, std::string cgi_path);

		void	handleGet(HttpRequest& req, HttpResponse& res);
		void	handleFile(HttpRequest& req, HttpResponse& res) ;
		void	handleAutoIndex(HttpRequest& req, HttpResponse& res) ;

		void		handlePost(HttpRequest& req, HttpResponse& res);
		void		uploadFile(HttpRequest& req, HttpResponse& res) ;
		std::string	generateUniqueTimeStamp() ;
		std::string	generateTmpFilename() ;
		std::string	generateUniqueFilename(HttpRequest& req) ;

		void	handleDelete(HttpRequest& req, HttpResponse& res);
		void	deleteProcess(HttpRequest& req, HttpResponse& res);
		bool	deleteRecursive(std::string path) ;
};
