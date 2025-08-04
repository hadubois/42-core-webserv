/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_management.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadubois <hadubois@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:59:30 by lle-duc           #+#    #+#             */
/*   Updated: 2025/07/31 11:16:56 by hadubois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"
#include "log_macros.hpp"

int get_file_size(std::string filename)
{
    int bytes;
    std::ifstream fs;

    fs.open(filename.c_str(), std::ifstream::binary);
    if (!fs.is_open())
    {
        DEBUG("Open failed -> " << filename);
        return (1);
    }
    fs.seekg(0, fs.end);
    bytes = fs.tellg();
    fs.close();
    return (bytes);
}

std::string		build_directory_indexing_page(std::string directory_path)
{
    std::ifstream fs;
	std::string cmd = "tree -H ";
    char buffer[1024];
    std::string body;

	cmd = cmd + directory_path + " > ./tmp_directory_indexing.html";
	std::system(cmd.c_str());
    fs.open("./tmp_directory_indexing.html", std::ifstream::in);
    if (!fs.is_open())
    {
        DEBUG("Open failed -> " << "./tmp_directory_indexing.html");
        return NULL;
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
    return (body);
}
