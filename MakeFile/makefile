CXX = g++
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG

TARGET = enterprise_server
SOURCES = main.cpp
HEADERS = main.hpp

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) -o $(TARGET) $(SOURCES)

debug: $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)
	rm -f enterprise_database.dat
	rm -f enterprise_backup.dat
	rm -f system_log.txt

install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

run: $(TARGET)
	./$(TARGET)

build-run: $(TARGET) run

help:
	@echo "Available targets:"
	@echo "  all       - Build release version"
	@echo "  debug     - Build with debug symbols"
	@echo "  clean     - Remove built files and data"
	@echo "  install   - Install to /usr/local/bin"
	@echo "  uninstall - Remove from /usr/local/bin"
	@echo "  run       - Run the server"
	@echo "  build-run - Build and run"
	@echo "  help      - Show this help"

.PHONY: all debug clean install uninstall run build-run help
