#include "Handler.hpp"
#include "MimeTypes.hpp"
#include "ansi_colors.hpp"
#include "log_macros.hpp"
#include "request_state.hpp"
#include "webserv.hpp"
#include "webserv_utils.hpp"
#include "generic_utils.hpp"
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

Handler::Handler() {}

Handler::~Handler() {}

void	Handler::handle(HttpRequest& req, HttpResponse& res) {
	std::string	method = req.getMethod();

	if (req.getType() == REDIRECTION) {
		res.insertHeader("Location", req.getRedirUrl());
		res.setCode(302);
		req.setState(RS_READY);
		return;
	} else if (method == "GET") {
		handleGet(req, res);
	} else if (method == "POST") {
		handlePost(req, res);
	} else if (method == "DELETE") {
		handleDelete(req, res);
	}

	setMimeType(req.getUrl(), res);
}

void	Handler::setMimeType(std::string path, HttpResponse& res) {
	std::string::size_type dot = path.rfind(".");
	if (dot == std::string::npos)
		return ;

	std::string ext = path.substr(dot);
	const std::map<std::string, std::string>& mime = getMimeTypeMap();
	std::map<std::string, std::string>::const_iterator it = mime.find(ext);
	if (it != mime.end())
		res.insertHeader("Content-Type", it->second);

	return ;
}
// ----CGI------------------------------------------------------------------- //
void	Handler::handleCgi(HttpRequest& req, HttpResponse& res) {
	std::map<std::string, std::string> cgi;
	std::string	buf = execute_cgi(req.getCgiExecutor(), req.getUrl());
	if (buf == "") {
		ERROR("500 Internal Server Error: cgi failed");
		req.setErrCode(500);
		req.setState(RS_CLOSE);
		return ;
	}
	res.appendBody(buf);
	res.setBody(res.generateCgiBody());
	req.setState(RS_READY);
}

std::string	Handler::generateTmpFilename() {
	std::ostringstream oss;
	oss << "/tmp/webserv_" << std::time(NULL) << "_" << rand() << ".tmp";
	return oss.str();
}

void	Handler::handleCgiPost(HttpRequest& req, HttpResponse& res) {
	std::string	tmpFilename = "tmp_" + req.getFileName();
	tmpFilename += MimeTypes::getExtFromMime(req.findHeader("Content-Type"));
	std::ofstream	ofs(tmpFilename.c_str(), std::ofstream::out | std::ofstream::binary);
	if (!ofs.is_open()) {
		ERROR("500 Internal Server Error: failed to create tmp file");
		req.setErrCode(500);
		req.setState(RS_CLOSE);
		return;
	}
	ofs << req.getBody();
	ofs.close();

	std::string	buf = execute_cgi(req.getCgiExecutor(), tmpFilename);
	if (buf == "") {
		ERROR("500 Internal Server Error: cgi post failed");
		req.setErrCode(500);
		req.setState(RS_CLOSE);
		return ;
	} else {
		res.appendBody(buf);
		req.setState(RS_READY);
	}

	 if (unlink(tmpFilename.c_str()) != 0)
		 ERROR("Can't remove temporary file" << tmpFilename);
}

std::string Handler::execute_cgi(std::string program_executor, std::string cgi_path)
{
	int        pipefd[2];
	pid_t    pid;
	char buffer[1024];
	int bytes;

	std::cout << program_executor << " and " << cgi_path << std::endl;
	if (pipe(pipefd) == -1)
	{
		perror("[CGI EXECUTION]");
		return (NULL);
	}
	pid = fork();
	if (pid == 0)
	{
		std::vector<const char *> arg_v;
		close(pipefd[0]);
		dup2(pipefd[1], 1);
		close(pipefd[1]);
		arg_v.push_back(program_executor.c_str());
		arg_v.push_back(cgi_path.c_str());
		arg_v.push_back(NULL);
		if (execve(program_executor.c_str(), const_cast<char**>(&arg_v[0]), NULL) == -1)
		{
			return ("");
		}
	}
	else
	{
		std::string str;
		close(pipefd[1]);
		waitpid(0, NULL, 0);
		while (true)
		{

			if ((bytes = read(pipefd[0], buffer, sizeof(buffer))) == -1)
			{
				return (str);
			}
			if (bytes == 0)
			{
				return (str);
			}
			str += buffer;
		}
		close(pipefd[0]);
	}
	return ("");
}

// ----GET------------------------------------------------------------------- //
void	Handler::handleGet(HttpRequest& req, HttpResponse& res) {
	if (req.getType() == CGI) {
		handleCgi(req, res);
	} else if (req.getType() == REG_FILE) {
		handleFile(req, res);
	} else if (req.getType() == DIRECTORY) {
		std::string url = req.getUrl();
		if (access(((url+"index.html").c_str()), F_OK) == 0) {
			req.setUrl(url + "index.html");
			handleFile(req, res);

		} else if (req.getLocationConfig().getAutoIndex()) {
			handleAutoIndex(req, res);

		} else {
			ERROR("403 Forbidden: dir without auto index");
			req.setErrCode(403);
			req.setState(RS_CLOSE);
			return ;
		}
	}
}

void	Handler::handleFile(HttpRequest& req, HttpResponse& res) {
	std::ifstream file(req.getUrl().c_str(), std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		ERROR("403 Forbidden: can't open file" << req.getUrl());
		req.setErrCode(403);
		req.setState(RS_CLOSE);
		return ;
	}

	std::ostringstream content;
	content << file.rdbuf();
	res.setBody(content.str());
	file.close();
	req.setState(RS_READY);
}

void	Handler::handleAutoIndex(HttpRequest& req, HttpResponse& res) {
    std::ifstream fs;
	std::string directory_path = req.getUrl();
	std::string cmd = "(cd " + directory_path + "; tree -H " + directory_path + ") > ./tmp_directory_indexing.html";
    char buffer[1024];
    std::string body;

	std::system(cmd.c_str());
    fs.open("./tmp_directory_indexing.html", std::ifstream::in);
    if (!fs.is_open())
    {
        ERROR("Open failed -> " << "./tmp_directory_indexing.html");
		ERROR("403 Forbidden: can't create auto_index_file");
		req.setErrCode(403);
		req.setState(RS_CLOSE);
		return ;
    }
    while (true)
    {
        int gcount = 0;
        fs.read(buffer, sizeof(buffer));
        gcount = fs.gcount();
        if (gcount <= 0)
        {
			break;
        }
        body += buffer;
		std::memset(buffer, '\0', 1024);
    }
    std::remove("./tmp_directory_indexing.html");

	res.setBody(body);
	req.setState(RS_READY);
}

// ----POST------------------------------------------------------------------ //
void	Handler::handlePost(HttpRequest& req, HttpResponse& res) {
	if (req.getLocationConfig().getUploadEnabled()) {
			uploadFile(req, res);
	} else {
		ERROR("405 Method Not Allowed: post without upload enabled");
		req.setErrCode(405);
		req.setState(RS_CLOSE) ;
		return ;
	}
}

void	Handler::uploadFile(HttpRequest& req, HttpResponse& res) {
	if (req.getLocationConfig().getUploadPath().empty()
			|| access(req.getLocationConfig().getUploadPath().c_str(), W_OK) != 0) {
		ERROR("403 Forbidden: upload with invalid path or rights");
		req.setErrCode(403);
		req.setState(RS_CLOSE) ;
		return ;
	}

	std::string uploadPath = req.getLocationConfig().getUploadPath();
	std::string fileName = generateUniqueFilename(req);
	std::string fullPath = uploadPath + "/" + fileName;

	std::ofstream ofs(fullPath.c_str(), std::ios::binary);
	if (!ofs) {
		ERROR("500 Internal Server Error: failed de upload file");
		req.setErrCode(500);
		req.setState(RS_CLOSE);
		return ;
	}

	ofs.write(req.getBody().c_str(), req.getBody().size());
	ofs.close();

	res.setCode(201);
	std::string response = "File uploaded as: " + fileName;
	res.setBody(res.generateBasicHtml(response));
	req.setState(RS_READY);
	return ;
}

bool	findExtention() {
	return false;
}

std::string	Handler::generateUniqueFilename(HttpRequest& req) {
	std::string	time = generateUniqueTimeStamp();
	std::string	filename = req.getFileName();
	if (filename.empty()) {
		filename = "upload";
	}
	std::string	ext;
	return time + "_" + filename + ext;
}

std::string	Handler::generateUniqueTimeStamp() {
	static int		count;
	static time_t	old;
	time_t			now = time(0);
	if (now == old) {
		count++;
	} else {
		old = now;
		count = 0;
	}
	struct tm	tstruct;
	char		buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &tstruct);
	std::string	res(buf);
	if (count > 0)
		res += "_" + itoa(count++);
	return res;
}

// ----DELETE---------------------------------------------------------------- //
void	Handler::handleDelete(HttpRequest& req, HttpResponse& res) {
	if (req.getType() == CGI) {
		ERROR("405 Method Not Allowed: delete a cgi");
		req.setErrCode(405) ;
		req.setState(RS_CLOSE) ;
	} else if (access(req.getUrl().c_str(), W_OK) != 0) {
		ERROR("403 Forbidden: do not have write access");
		req.setErrCode(403) ;
		req.setState(RS_CLOSE) ;
	} else {
		deleteProcess(req, res);
	}
}

void	Handler::deleteProcess(HttpRequest& req, HttpResponse& res) {
	int	state = 0;
	if (req.getType() == REG_FILE) {
		if(unlink(req.getUrl().c_str()) == 0)
			state = 1;
		else
			state = 0;
	} else
		state = deleteRecursive(req.getUrl());

	if (state == 0) {
		ERROR("500 Internal Server Error: delete failed");
		req.setErrCode(500) ;
		req.setState(RS_CLOSE) ;
	} else {
		res.setCode(205);
		res.insertHeader("Content-Type", "text/plain");
		res.setBody("Delete success: " + req.getUrl());
		req.setState(RS_READY);
	}
}

bool Handler::deleteRecursive(std::string path) {
	DIR	*dirp = opendir(path.c_str());
	if (!dirp) {
		ERROR("opendir(): " << path);
		return false;
	}

	errno = 0;
	struct dirent	*dt;
	while ((dt = readdir(dirp)) != 0) {
		std::string name = dt->d_name;
		if (name != "." && name != "..") {
			std::string fullPath = path + "/" + name;
			if (dt->d_type == DT_DIR)
				deleteRecursive(fullPath);
			else if(dt->d_type == DT_REG) {
				if (unlink(fullPath.c_str()) != 0) {
					ERROR("unlink(): " << fullPath);
					return false;
				}
			}
		}
	}

	if (!dt) {
		if (errno != 0) {
			ERROR("readdir()" << path);
			if (closedir(dirp) != 0)
				ERROR("closedir()" << path);
			return false;
		}
	}

	closedir(dirp);
	if (rmdir(path.c_str()) == -1) {
		ERROR("rmdir()" << path);
		return false;
	}

	return true;
}
