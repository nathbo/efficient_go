CC=g++
SRCDIR=src
BUILDDIR=build
BINDIR=bin
TARGET=$(BINDIR)/go

SOURCES=$(SRCDIR)/go.cpp $(SRCDIR)/board.cpp $(SRCDIR)/move.cpp
OBJECTS=$(BUILDDIR)/go.o $(BUILDDIR)/board.o $(BUILDDIR)/move.o
CFLAGS=-g
INCLUDE=-I./include/
LIB=-lpthread -lm

$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	@echo "Linking.."
	$(CC) $^ -o $(TARGET) $(LIB)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CC) $(CGLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@echo "Cleaning..."
	$(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean
