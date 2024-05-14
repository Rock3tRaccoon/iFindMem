TARGET := iphone:clang:latest:16.5

include $(THEOS)/makefiles/common.mk

TOOL_NAME = ifindmem

ifindmem_FILES = main.c mem.c
ifindmem_CFLAGS = -Wno-int-conversion -Wno-format
ifindmem_CODESIGN_FLAGS = -Sent.xml
ifindmem_INSTALL_PATH = /usr/local/bin

include $(THEOS_MAKE_PATH)/tool.mk
