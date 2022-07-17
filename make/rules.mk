#
# Переменная DIRS содержит
# пути для поиска исходников
# и заголовочных файлов. Задается
# в Makefile нижнего уровня
#
VPATH := $(DIRS)

CC := $(CROSS_COMPILE)gcc
LD := $(CROSS_COMPILE)gcc

# CFLAGS += -Wall -g $(TARGET) -D$(IPC) -D$(IPC_TYPE) -D$(OS) -D$(DEF_TYPES) $(GPROF) $(DZY_VER)
CFLAGS += -Wall -O2 $(TARGET) -D$(IPC) -D$(IPC_TYPE) -D$(OS) -D$(DEF_TYPES) $(GPROF) $(DZY_VER)
CXXFLAGS += $(CFLAGS)

INCLUDES := $(shell find src -type d)
vpath %.cpp $(sort $(INCLUDES))
vpath %.c $(sort $(INCLUDES))

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CC) $(CXXFLAGS) -c -MMD $< -o $@	

$(BUILD_DIR)/%.o: %.CPP | $(BUILD_DIR)
	$(CC) $(CXXFLAGS) -c -MMD $< -o $@	

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -MMD $< -o $@	

$(BUILD_DIR):
	mkdir -p $@

include $(wildcard $(BUILD_DIR)/*.d)