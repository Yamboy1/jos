# included from an architecture makefile.

include $(PREFIX)/jos/Makefile

ROOT_SOURCES = $(PREFIX)/init.java
ROOT_OBJECTS = $(PREFIX)/init.class

# This way, we explicitly get that we build architecture-specific
# bytecode, but we also get extra .class files (from the architecture-
# independent bytecode) in the arch directories.
#ALL_JAVA_SOURCES = $(ARCH_SOURCES) $(JOS_SOURCES) $(ROOT_SOURCES)
#ALL_JAVA_OBJECTS = $(ARCH_OBJECTS) $(JOS_OBJECTS) $(ROOT_OBJECTS)

# This way loses the feedback but produces the correct tree.
ALL_JAVA_SOURCES = $(JOS_SOURCES) $(ARCH_SOURCES) $(ROOT_SOURCES)
ALL_JAVA_OBJECTS = $(JOS_OBJECTS) $(ARCH_OBJECTS) $(ROOT_OBJECTS) 

$(ROOT_OBJECTS) : %.class : %.java
	@echo
	@echo Building root java files...
	@$(JAVAC) -classpath $$CLASSPATH:$(ARCH_PATH):$(PREFIX) $<
	@echo ... done.
	@echo
