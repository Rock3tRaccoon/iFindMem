DEBUG = 0
FINALPACKAGE = 1
TARGET = iphone:clang:latest:16.5

include $(THEOS)/makefiles/common.mk

APPLICATION_NAME = iFindMemory

iFindMemory_FILES = main.m fwAppDelegate.m fwRootViewController.m
iFindMemory_CFLAGS = -fobjc-arc
iFindMemory_CODESIGN_FLAGS = -Sent.xml
iFindMemory_FRAMEWORKS = UIKit CoreGraphics

include $(THEOS_MAKE_PATH)/application.mk
SUBPROJECTS += ifindmemcore/
include $(THEOS_MAKE_PATH)/aggregate.mk
THEOS_DEVICE_IP = iPhoneRoot
THEOS_DEVICE_PORT = 2222
THEOS_DEVICE_USER = root