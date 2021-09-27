mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
mkfile_dir := $(dir $(mkfile_path))

NODE=-I/usr/include/node -I$(mkfile_dir)node_modules/node-addon-api/ -I$(shell node -p "require('node-addon-api').include")
INCLUDE=$(NODE)
LIBS=

BUILD_DIR=$(CURDIR)/build

event.node: clean
	mkdir -p $(BUILD_DIR)
	g++ lib/lib.cpp -shared -fPIC -o $(BUILD_DIR)/event.node -Wall -Wextra $(LIBS) $(INCLUDE)

clean:
	rm -rf $(BUILD_DIR)
