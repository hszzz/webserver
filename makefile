.PHONY : all clean

CC := gcc
RM := rm -rf
TARGET := httpd

all : $(TARGET)

%.o : %.cc
	$(CC) -c $< -o $@

$(TARGET) : *.o
	$(CC) $< -o $@

clean : 
	$(RM) $(TARGET)

