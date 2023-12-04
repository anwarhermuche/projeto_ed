CC = g++
CFLAGS = -std=c++11 -Wall
TARGET = programa
SRCS = Binario.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
CSV_URL = https://dl.dropboxusercontent.com/scl/fi/1heo1enuqwq42i2bee6bn/Data7602DescendingYearOrder.csv?rlkey=tv0yjw9n3stx787byu53mjozn&dl=0

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:	
	rm -f $(OBJS) $(TARGET) *.bin

baixar_dados:
	curl -o Data7602DescendingYearOrder.csv -L $(CSV_URL)
