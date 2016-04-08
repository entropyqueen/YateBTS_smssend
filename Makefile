#
# Makefile for smssend
# 
# SmsSend is a plugin for Yate, allowing to send a sms to a phone number
# 

NAME	= smssend.yate
INSTALL = /usr/local/lib/yate/$(NAME)

CC		= g++
RM		= rm -f
LN		= ln -s
CP		= cp

SRCS	= smssend.cpp
OBJS	= $(SRCS:.cpp=.o)
CFLAGS	= -std=c++11 -fPIC 
LFLAGS	= -shared
LIBS 	=
INCLUDES= -I /usr/local/include/yate/

ifdef DEBUG
	CFLAGS += -g3
endif

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LFLAGS) $(LIBS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

install:
	$(CP) $(NAME) $(INSTALL)

uninstall: fclean
	$(RM) $(INSTALL)

.PHONY: all clean fclean .cpp.o re install
