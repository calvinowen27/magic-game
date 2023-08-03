SRCDIR = src
INCLUDEDIR = include/game
OBJDIR = build/obj
DBGDIR = build/debug/
RLSDIR = build/release/

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -pedantic -I$(INCLUDEDIR)

LDFLAGS = -L./lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lUser32

EXECUTABLE = magic

debug: $(OBJECTS) $(DBGDIR)
	rm -rf $(DBGDIR)content
	mkdir -p $(DBGDIR)content
	cp -r content $(DBGDIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(DBGDIR)$(EXECUTABLE)

release: $(OBJECTS) $(RLSDIR)
	rm -rf $(RLSDIR)content
	mkdir -p $(RLSDIR)content
	cp -r content $(RLSDIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(RLSDIR)$(EXECUTABLE)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(INCLUDEDIR)/%.hpp $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $@

$(DBGDIR):
	mkdir -p $@

$(RLSDIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -f $(EXECUTABLE) $(OBJECTS)