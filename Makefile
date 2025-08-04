NAME = webserv
LIB  = lib$(NAME).a

SRCDIR = src
OBJDIR = obj
INCDIR = includes/ includes/http/ includes/utils includes/config

# Source Files
SRC = src/main.cpp \
	  src/socket_manager.cpp \
	  src/file_management.cpp \
	  src/errors/error_management.cpp \
	  src/server/Server.cpp \
	  src/server/Server_functions.cpp \
	  src/Monitor/Monitor.cpp \
	  src/parse_config.cpp \
	  src/http/HttpExchange.cpp \
	  src/http/HttpRequest.cpp \
	  src/http/HttpResponse.cpp \
	  src/http/Parser.cpp \
	  src/http/Router.cpp \
	  src/http/Handler.cpp \
	  src/utils/generic_utils.cpp \
	  src/utils/Client.cpp \
	  src/utils/webserv_utils.cpp \
	  src/config/ServerConfig.cpp \
	  src/config/LocationConfig.cpp \
	  src/config/MimeTypes.cpp \

OBJ = $(SRC:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Compiler and Flags
CC = c++
CFLAGS = -Wall -Wextra -Werror -gdwarf-4 -std=c++98 $(addprefix -I, $(INCDIR))

LOG ?= 0
ifeq ($(LOG), 1)
	CFLAGS += -DLOG
else
	CFLAGS += #-DNDBUG
endif

# Compilation mode
VERBOSE ?= 0
ifeq ($(VERBOSE),1)
  V :=
else
  V := @
endif

# Colors
RED     := "\033[1;31m"
GREEN   := "\033[1;32m"
RESET   := "\033[0m"

# Default Rule
all: $(NAME)

lib: $(OBJ)
	$(V) $(AR) -crs $(LIB) $^

# Dependency Files
DEP = $(OBJ:.o=.d)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(V) mkdir -p $(dir $@); #Object Directory Rule
	$(V)$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEP)

# Linking Rule
$(NAME): $(OBJ)
	$(V)$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
	$(V)echo $(GREEN)"[$(NAME)] Executable created ✅"$(RESET)

# Clean Rules
clean:
	$(V)echo $(RED)'[$(NAME)] Cleaning objects'$(RESET)
	$(V)rm -rf $(OBJDIR)
	$(V) $(RM) $(LIB)

fclean: clean
	$(V)echo $(RED)'[$(NAME)] Cleaning all files'$(RESET)
	$(V)rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re lib
.DEFAULT_GOAL := all
