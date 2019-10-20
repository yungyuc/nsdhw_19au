# CXX = g++ -Wall -Wextra -I/usr/include/python3.7m


CXX = g++ -Wall -Wextra `python-config --cflags` `python-config --ldflags` 

calc_angle:
	$(CXX) -fPIC -shared calc_angle.cpp -o calc_angle.so

# line: linemain.o line.o mkdirs
# 	$(CXX) $(BUILD_DIR)/linemain.o $(BUILD_DIR)/line.o -o $(OUTPUT_DIR)/line

# linemain.o: linemain.cpp line.h mkdirs
# 	$(CXX) -c linemain.cpp -o $(BUILD_DIR)/linemain.o

# line.o: line.h line.cpp mkdirs
# 	$(CXX) -c line.cpp -o $(BUILD_DIR)/line.o

clear:
	rm -f calc_angle.so

test: calc_angle
	python calc_angle_test.py