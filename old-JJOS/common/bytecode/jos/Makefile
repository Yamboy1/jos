# included from the common makefile

JOS_SOURCES = \
 $(PREFIX)/jos/platform/event/KeyListener.java \
 $(PREFIX)/jos/platform/event/InterruptListener.java \
 $(PREFIX)/jos/platform/event/ScancodeListener.java \
 $(PREFIX)/jos/platform/event/KeyEvent.java \
 $(PREFIX)/jos/platform/event/FocusListener.java \
 $(PREFIX)/jos/platform/event/FocusEvent.java \
 \
 $(PREFIX)/jos/util/Queue.java \
 $(PREFIX)/jos/util/QueueElement.java \
 $(PREFIX)/jos/util/ByteQueue.java \
 $(PREFIX)/jos/util/Set.java \
 \
 $(PREFIX)/jos/system/interrupts.java \
 $(PREFIX)/jos/system/machine.java \
 $(PREFIX)/jos/system/console.java \
 $(PREFIX)/jos/system/consoled.java \
 $(PREFIX)/jos/system/ConsoleDriver.java \
 \
 $(PREFIX)/jos/platform/KeyboardInterpreter.java \
 $(PREFIX)/jos/platform/keyboard.java \

JOS_OBJECTS = \
 $(PREFIX)/jos/platform/event/KeyListener.class \
 $(PREFIX)/jos/platform/event/InterruptListener.class \
 $(PREFIX)/jos/platform/event/ScancodeListener.class \
 $(PREFIX)/jos/platform/event/KeyEvent.class \
 $(PREFIX)/jos/platform/event/FocusListener.class \
 $(PREFIX)/jos/platform/event/FocusEvent.class \
 \
 $(PREFIX)/jos/system/interrupts.class \
 $(PREFIX)/jos/system/machine.class \
 $(PREFIX)/jos/system/console.class \
 $(PREFIX)/jos/system/consoled.class \
 $(PREFIX)/jos/system/ConsoleDriver.class \
 \
 $(PREFIX)/jos/platform/KeyboardInterpreter.class \
 $(PREFIX)/jos/platform/keyboard.class \
 \
 $(PREFIX)/jos/util/Queue.class \
 $(PREFIX)/jos/util/QueueElement.class \
 $(PREFIX)/jos/util/ByteQueue.class \
 $(PREFIX)/jos/util/Set.class

$(JOS_OBJECTS) : %.class : %.java
	@echo 
	@echo Building architecture-independent objects...
	@$(JAVAC) -classpath $$CLASSPATH:$(ARCH_PATH):$(PREFIX) $(JOS_SOURCES)
	@echo ... done.
	@echo
