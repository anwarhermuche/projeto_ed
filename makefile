CC = g++
CFLAGS = -std=c++11 -Wall
TARGET = programa
SRCS = Binario.cpp main.cpp
OBJS = $(SRCS:.cpp=.o)
CSV_URL = https://drive.google.com/uc?export=download&id=1avWyde6u_YkENRsIwJDl1nELfiIPhu6A

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:	
	rm -f $(OBJS) $(TARGET) *.bin

baixar_dados:
	curl -o Data7602DescendingYearOrder.csv -L $(CSV_URL)
