#pragma once

#include "HttpRequest.hpp"
#include "log_macros.hpp"
#include "request_state.hpp"
#include <string>

class Parser {
	private:
		size_t		_contentLength;

	public:
		Parser();
		~Parser();

		const size_t&	getContentLength() const;

		void	parseStartLine(std::string& rawBuf, HttpRequest& req) ;
		void	parseHeaders(std::string& rawBuf, HttpRequest& req) ;
		void	parseBody(std::string& rawBuf, HttpRequest& req) ;

		void		parse(std::string& rawBuf, HttpRequest& req) ;
		bool		handleBoundary(HttpRequest& req) ;
};

std::ostream&	operator<<(std::ostream& os, Parser& other) ;
