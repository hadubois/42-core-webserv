#pragma once

#include <map>
#include <string>

const std::map<std::string, std::string>&	getMimeTypeMap();

const std::map<int, std::string>&			getStatusCodeMap() ;
std::map<int, std::string>					extractReasonPhrases() ;
