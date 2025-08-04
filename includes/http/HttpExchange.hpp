#pragma once

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Handler.hpp"
#include "Parser.hpp"
#include "Router.hpp"
#include "request_state.hpp"
#include <string>
#include <iostream>

class HttpExchange {
	private:
		HttpRequest		_req;
		HttpResponse	_res;
		Handler			_handler;
		Parser			_parser;
		Router			_router;
		ServerConfig	_sc;
		std::string		_inBuf;
		std::string		_outBuf;

	public:
		HttpExchange() ;
		HttpExchange(ServerConfig const& sc) ;
		~HttpExchange() ;

		HttpRequest&	getReq()  ;
		HttpResponse&	getRes()  ;
		Handler&		getHandler()  ;
		Parser&		getParser()  ;
		Router&		getRouter()  ;
		ServerConfig& getSC()  ;
		std::string&	getInBuf()  ;
		std::string&	getOutBuf()  ;

		const ReqState&		getState() const ;

		void	process(std::string inputDatas) ;
};

std::ostream&	operator<<(std::ostream& oss, HttpExchange& exc) ;
