ROOTDIR := $(PWD)
CC := gcc
CFLAGS := -lcurl -g
BUILD_DIR := $(PWD)/build
SRC_DIR := $(PWD)/src
objects = main.o alpine_init.o cgroup_config.o argparse.o

container: $(objects)
	$(CC) -o $(BUILD_DIR)/$@ $(foreach file,$(objects),$(BUILD_DIR)/$(file)) $(CFLAGS)

$(objects): %.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR)
	$(CC) -c $^ -o $(BUILD_DIR)/$@ $(CFLAGS) 
	@cd -

clean:
	@rm -rf $(BUILD_DIR)