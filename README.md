# Webserv
This project is about creating a HTTP/1.1 server from scratch in C++. Also unersdant what's beside the most used protocol in the internet.

## Table of Contents
- [Features](##Features)
- [Installation](##Installation)
- [Usage](##Usage)
- [Technologies Used](##TechnologiesUsed)
- [Contributing](##Contributing)
- [Liscence](##License)

## Features
- Deserve a fully static website, with default error pages.
- Non blocking server using `epoll` that handle all I/O requests.
- Clients can upload and delete files and use `python` CGI.
- Accurate status code like defined in [RFC 9112](https://datatracker.ietf.org/doc/html/rfc9112)

## Installation
1. Git clone the repository `https://github.com/hadubois/42-core-webserv.git`
2. Then go to inside the folder `cd-core-webserv`
3. Build with `make`

## Usage
`./webserv [configuration file]`

There are severals configurations files in `/config/`. You can try it all or custom you server parameters.

## Technologies Used
- All functionality is implemented in C++ 98.
- cgi execution: `execve`, `pipe`, `strerror`, `errno`, `dup`,
`dup2`, `fork`
- socket configuration: `socketpair`, `htons`, `htonl`, `epoll`, `epoll_create`, `epoll_ctl`, `epoll_wait`, `socket`, `accept`, `listen`, `send`, `recv`, `chdir`, `bind`, `connect`,
- file manipulations:  `getaddrinfo`, `setsockopt`, `getsockname`,
`getprotobyname`, `fcntl`, `close`, `read`, `write`, `waitpid`,
`kill`, `signal`, `access`, `stat`, `open`, `opendir`, `readdir`, `closedir`

## Contributing
 ![Photo de @ThatsLucas](https://images.weserv.nl/?url=https://github.com/ThatsLucas.png?size=80&h=80&w=80&fit=cover&mask=circle)

## Licence
[![Licence 42](https://img.shields.io/badge/Paris-grey?style=for-the-badge&logo=data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiPz4KPHN2ZyB3aWR0aD0iNTdweCIgaGVpZ2h0PSI0MHB4IiB2aWV3Qm94PSIwIDAgNTcgNDAiIHZlcnNpb249IjEuMSIgeG1sbnM9Imh0dHA6Ly93d3cudzMub3JnLzIwMDAvc3ZnIiB4bWxuczp4bGluaz0iaHR0cDovL3d3dy53My5vcmcvMTk5OS94bGluayI+CiAgICA8dGl0bGU+NDIgRmluYWwgc2lnbGUgc2V1bDwvdGl0bGU+CiAgICA8ZGVmcz4KICAgICAgICA8ZmlsdGVyIGlkPSJmaWx0ZXItMSI+CiAgICAgICAgICAgIDxmZUNvbG9yTWF0cml4IGluPSJTb3VyY2VHcmFwaGljIiB0eXBlPSJtYXRyaXgiIHZhbHVlcz0iMCAwIDAgMCAxLjAwMDAwMCAwIDAgMCAwIDEuMDAwMDAwIDAgMCAwIDAgMS4wMDAwMDAgMCAwIDAgMS4wMDAwMDAgMCI+PC9mZUNvbG9yTWF0cml4PgogICAgICAgIDwvZmlsdGVyPgogICAgICAgIDxwb2x5Z29uIGlkPSJwYXRoLTIiIHBvaW50cz0iMCAwLjIwNDUzNjA4MiAzMS42MjY2NTg1IDAuMjA0NTM2MDgyIDMxLjYyNjY1ODUgMzkuOTc1MjU3NyAwIDM5Ljk3NTI1NzciPjwvcG9seWdvbj4KICAgIDwvZGVmcz4KICAgIDxnIGlkPSJQYWdlLTEiIHN0cm9rZT0ibm9uZSIgc3Ryb2tlLXdpZHRoPSIxIiBmaWxsPSJub25lIiBmaWxsLXJ1bGU9ImV2ZW5vZGQiPgogICAgICAgIDxnIGlkPSJIb21lLXBhZ2UiIHRyYW5zZm9ybT0idHJhbnNsYXRlKC0yMC4wMDAwMDAsIC0xMTkuMDAwMDAwKSI+CiAgICAgICAgICAgIDxnIGlkPSI0Mi1GaW5hbC1zaWdsZS1zZXVsIiB0cmFuc2Zvcm09InRyYW5zbGF0ZSgwLjAwMDAwMCwgODYuMDAwMDAwKSIgZmlsdGVyPSJ1cmwoI2ZpbHRlci0xKSI+CiAgICAgICAgICAgICAgICA8ZyB0cmFuc2Zvcm09InRyYW5zbGF0ZSgyMC4wMDAwMDAsIDMzLjAwMDAwMCkiPgogICAgICAgICAgICAgICAgICAgIDxnIGlkPSJHcm91cC0zIj4KICAgICAgICAgICAgICAgICAgICAgICAgPG1hc2sgaWQ9Im1hc2stMyIgZmlsbD0id2hpdGUiPgogICAgICAgICAgICAgICAgICAgICAgICAgICAgPHVzZSB4bGluazpocmVmPSIjcGF0aC0yIj48L3VzZT4KICAgICAgICAgICAgICAgICAgICAgICAgPC9tYXNrPgogICAgICAgICAgICAgICAgICAgICAgICA8ZyBpZD0iQ2xpcC0yIj48L2c+CiAgICAgICAgICAgICAgICAgICAgICAgIDxwb2x5bGluZSBpZD0iRmlsbC0xIiBmaWxsPSIjMDAwMTA1IiBtYXNrPSJ1cmwoI21hc2stMykiIHBvaW50cz0iMzEuNjI2NjU4NSAwLjIwNDUzNjA4MiAyMS4wODQxNjE2IDAuMjA0NTM2MDgyIDAgMjEuMDk2OTA3MiAwIDI5LjU1MzgxNDQgMjEuMDg0MTYxNiAyOS41NTM4MTQ0IDIxLjA4NDE2MTYgNDAgMzEuNjI2NjU4NSA0MCAzMS42MjY2NTg1IDIxLjA5NjkwNzIgMTAuNTQyMDgwOCAyMS4wOTY5MDcyIDMxLjYyNjY1ODUgMC4yMDQ1MzYwODIiPjwvcG9seWxpbmU+CiAgICAgICAgICAgICAgICAgICAgPC9nPgogICAgICAgICAgICAgICAgICAgIDxwb2x5bGluZSBpZD0iRmlsbC00IiBmaWxsPSIjMDAwMTA1IiBwb2ludHM9IjM1LjM0ODgzNzIgMTAuMjMyNTU4MSA0NS41ODEzOTUzIDAgMzUuMzQ4ODM3MiAwIDM1LjM0ODgzNzIgMTAuMjMyNTU4MSI+PC9wb2x5bGluZT4KICAgICAgICAgICAgICAgICAgICA8cG9seWxpbmUgaWQ9IkZpbGwtNSIgZmlsbD0iIzAwMDEwNSIgcG9pbnRzPSI1Ni43NDQxODYgMTAuNTQyNDk2OSA1Ni43NDQxODYgMCA0Ni41MTE4Mjk5IDAgNDYuNTExODI5OSAxMC41NDI0OTY5IDM2LjI3OTA2OTggMjEuMDg0OTkzOSAzNi4yNzkwNjk4IDMxLjYyNzkwNyA0Ni41MTE4Mjk5IDMxLjYyNzkwNyA0Ni41MTE4Mjk5IDIxLjA4NDk5MzkgNTYuNzQ0MTg2IDEwLjU0MjQ5NjkiPjwvcG9seWxpbmU+CiAgICAgICAgICAgICAgICAgICAgPHBvbHlsaW5lIGlkPSJGaWxsLTYiIGZpbGw9IiMwMDAxMDUiIHBvaW50cz0iNTYuNzQ0MTg2IDIxLjM5NTM0ODggNDYuNTExNjI3OSAzMS42Mjc5MDcgNTYuNzQ0MTg2IDMxLjYyNzkwNyA1Ni43NDQxODYgMjEuMzk1MzQ4OCI+PC9wb2x5bGluZT4KICAgICAgICAgICAgICAgIDwvZz4KICAgICAgICAgICAgPC9nPgogICAgICAgIDwvZz4KICAgIDwvZz4KPC9zdmc+)](https://42.fr/en/homepage/)
