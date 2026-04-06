CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
TARGET = jeu
SRCS = main.cpp Joueur.cpp board.cpp carteTrain.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all
