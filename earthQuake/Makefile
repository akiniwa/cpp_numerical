PROG:=myapp
SRCS:=main.cpp site.cpp
OBJS:=$(SRCS:%.cpp=%.o)

CC :=c++

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^


%.o: %.c
	$(CC) -c $<
