TARGET := iphone:clang:latest:16.5
INSTALL_TARGET_PROCESSES = iFindMem

include $(THEOS)/makefiles/common.mk

APPLICATION_NAME = iFindMem

iFindMem_CODESIGN_FLAGS = -Sent.xml
iFindMem_FILES = main.m RevAppDelegate.m RevRootViewController.m
iFindMem_FRAMEWORKS = UIKit CoreGraphics
iFindMem_CFLAGS = -fobjc-arc

include $(THEOS_MAKE_PATH)/application.mk
include $(THEOS_MAKE_PATH)/aggregate.mk
