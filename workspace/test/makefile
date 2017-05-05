CC = g++
# ¿É´ÐÎ¼þ
TARGET = test
# CÎ¼þ
SRCS = test.cpp
# Ŀ±ê¼þ
OBJS = $(SRCS:.cpp=.o)
# ¿â¼þ
DLIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui
# t½Óª¿É´ÐÎ¼þ
$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(DLIBS)  
clean:
	rm -rf $(TARGET) $(OBJS)
# ±à¹æ $@´ú±ê¼þ $< ´úһ¸öµÎ¼þ
%.o:%.cpp
	$(CC) -o $@ -c $<
