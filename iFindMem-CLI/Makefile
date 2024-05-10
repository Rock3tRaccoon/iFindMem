TARGET := iphone:clang:latest:16.5
INSTALL_TARGET_PROCESSES = RevelariOS
ARCH = arm64e

include $(THEOS)/makefiles/common.mk

APPLICATION_NAME = RevelariOS

RevelariOS_CODESIGN_FLAGS = -Sent.xml
RevelariOS_FILES = mem.h
RevelariOS_FRAMEWORKS = UIKit CoreGraphics
RevelariOS_CFLAGS = -fobjc-arc

include $(THEOS_MAKE_PATH)/application.mk
include $(THEOS_MAKE_PATH)/aggregate.mk