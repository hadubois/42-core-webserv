/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_management.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lle-duc <lle-duc@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 12:17:27 by lle-duc           #+#    #+#             */
/*   Updated: 2025/06/25 12:21:01 by lle-duc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void    print_errors(std::string str)
{
    std::cout << "[ERROR] : " << str << std::endl;
}

void    print_logs(std::string str)
{
    std::cout << "[LOGS]" << str << std::endl;
}
