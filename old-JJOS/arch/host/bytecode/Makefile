# included from ../nativecode/Makefile
# note that this Makefile uses a pre-built
# java class library at /jdk/lib/classes.zip;
# at some point, classpath should be integrated into
# this build.

ARCH_PATH = ../bytecode

ARCH_SOURCES = \
 $(ARCH_PATH)/jos/platform/driver/console.java \
 $(ARCH_PATH)/jos/platform/driver/consoleNative.java \
 $(ARCH_PATH)/jos/platform/driver/consoleVGA.java \
 $(ARCH_PATH)/jos/platform/driver/keyboard.java \
 $(ARCH_PATH)/jos/platform/driver/keyboardd.java \
 $(ARCH_PATH)/jos/platform/driver/KeyboardInterpreter.java \
 $(ARCH_PATH)/jos/platform/driver/VGA.java \
 $(ARCH_PATH)/jos/platform/driver/standardVGA.java

ARCH_OBJECTS = \
 $(ARCH_PATH)/jos/platform/driver/console.class \
 $(ARCH_PATH)/jos/platform/driver/consoleNative.class \
 $(ARCH_PATH)/jos/platform/driver/consoleVGA.class \
 $(ARCH_PATH)/jos/platform/driver/keyboard.class \
 $(ARCH_PATH)/jos/platform/driver/keyboardd.class \
 $(ARCH_PATH)/jos/platform/driver/KeyboardInterpreter.class \
 $(ARCH_PATH)/jos/platform/driver/VGA.class \
 $(ARCH_PATH)/jos/platform/driver/standardVGA.class \

$(ARCH_OBJECTS) : %.class : %.java
	@echo
	@echo Building architecture-specific Java objects...
	@$(JAVAC) -classpath $$CLASSPATH:$(ARCH_PATH):../../../common/bytecode $(ARCH_SOURCES) -d $(ARCH_PATH)
	@echo ... done.
	@echo

TARGET_ROOT = $(shell pwd)
PREFIX = ../../../common/bytecode
include $(PREFIX)/Makefile

target_zip : $(TARGET_ZIP)

$(TARGET_ZIP) : %.zip : $(ALL_JAVA_OBJECTS)
	@echo Copying in class library....
	@cp /jdk/lib/classes.zip $(TARGET_ROOT)/$(TARGET_ZIP)
#	@echo Copying in classpath library....
#	@cp ../bytecode/classpath.zip $(TARGET_ROOT)/$(TARGET_ZIP)
	@echo Zipping architecture-independent classes...
	@cd $(PREFIX); find . -name '*.class' | sed -e's/\.\///' | zip $(TARGET_ROOT)/$(TARGET_ZIP) -0 -@
	@echo Zipping architecture-specific classes...
	@cd $(ARCH_PATH); find . -name '*.class' | sed -e's/\.\///' | zip $(TARGET_ROOT)/$(TARGET_ZIP) -0 -@
	@echo ... done.
