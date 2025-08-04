#include "HttpExchange.hpp"
#include "ServerConfig.hpp"
#include "ansi_colors.hpp"
#include "generic_utils.hpp"
#include "log_macros.hpp"
#include <iostream>
#include <sstream>
#include <vector>

// ----CONSTRUCTOR/DESTRUCTOR------------------------------------------------ //
HttpExchange::HttpExchange() {}

HttpExchange::HttpExchange(ServerConfig const& sc) : _sc(sc) {
	_req.setMaxBodySize(_sc.getMaxBodySize());
	_req.setMaxHeaderSize(_sc.getMaxHeaderSize());
}

HttpExchange::~HttpExchange() {}

// ----GETTERS--------------------------------------------------------------- //
HttpRequest&	HttpExchange::getReq()  { return _req; }
std::string&	HttpExchange::getInBuf()  { return _inBuf; }
Handler&		HttpExchange::getHandler()  { return _handler; }
Parser&			HttpExchange::getParser()  { return _parser; }
Router&			HttpExchange::getRouter()  { return _router; }
ServerConfig&	HttpExchange::getSC()  { return _sc; }
HttpResponse&	HttpExchange::getRes()  { return _res; }
std::string&	HttpExchange::getOutBuf()  { return _outBuf; }

const ReqState&		HttpExchange::getState() const { return _req.getState(); }

// ----METHODS--------------------------------------------------------------- //
void	HttpExchange::process(std::string	inputDatas) {
	_inBuf += inputDatas;
	INFO( "<INBUF>\n" PURPLE << _inBuf << RST );
	_res.setDefaultErrorPage(_sc.getDefaultErrorPage());

	if (_req.getState() != RS_BODY_DONE || _req.getState() != RS_NO_BODY) {
		INFO(CLR "PARSING..." RST);
		_parser.parse(_inBuf, _req);
	}

	if (_req.getState() == RS_BODY_DONE || _req.getState() == RS_NO_BODY) {
		INFO(CLR "ROUTING..." RST);
		_router.route(_sc, _req);
	}

	if(_req.getState() == RS_ROUTING_DONE) {
		INFO(CLR "HANDLING..." RST);
		_handler.handle(_req, _res);
	}

	if (_req.getState() == RS_CLOSE) {
		_outBuf = _res.errorToString(_req.getErrCode());
		return ;
	} else if (_req.getState() == RS_READY) {
		_outBuf = _res.toString();
	}

	if (_req.getState() == RS_READY || _req.getState() == RS_CLOSE)
		INFO("<OUTBUF>\n" YELLOW << _outBuf << RST);
}

// ----NON-MEMBER------------------------------------------------------------ //
std::ostream&	operator<<(std::ostream& os, HttpExchange& exc) {
	os  << "HttpRequest: " << exc.getReq() << std::endl
		<< "HttpResponse: " << exc.getRes() << std::endl
		<< "Parser: " << exc.getParser() << std::endl
		<< "ServerConfgi: " << exc.getSC() << std::endl
		<< "inBuf: " << exc.getInBuf() << std::endl
		<< "outBuf: " << exc.getOutBuf() << std::endl;
	return os;
}
