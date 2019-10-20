CXX = g++ -Wall -Wextra

MKDIR_CMD = mkdir -p

BUILD_DIR = build
OUTPUT_DIR = out

line: linemain.o line.o mkdirs
	$(CXX) $(BUILD_DIR)/linemain.o $(BUILD_DIR)/line.o -o $(OUTPUT_DIR)/line

linemain.o: linemain.cpp line.h mkdirs
	$(CXX) -c linemain.cpp -o $(BUILD_DIR)/linemain.o

line.o: line.h line.cpp mkdirs
	$(CXX) -c line.cpp -o $(BUILD_DIR)/line.o

mkdirs:
	$(MKDIR_CMD) $(BUILD_DIR)
	$(MKDIR_CMD) $(OUTPUT_DIR)

clear:
	rm -rf $(BUILD_DIR)/ $(OUTPUT_DIR)/

run: line
	./$(OUTPUT_DIR)/line