ROOTDIR := $(PWD)
CC := gcc
CFLAGS := -lcurl
BUILD_DIR := $(PWD)/build
objects = main.o alpine_init.o

container: $(objects)
	$(CC) -o $(BUILD_DIR)/$@ $(foreach file,$(objects),$(BUILD_DIR)/$(file)) $(CFLAGS)

$(objects): %.o: %.c
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR)
	$(CC) -c $^ -o $(BUILD_DIR)/$@ $(CFLAGS) 
	@cd -

