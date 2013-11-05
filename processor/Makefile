processor := processor
processor_objects := $(if $(findstring lr35902,$(processors)),processor-lr35902)
objects += $(processor_objects)

obj/processor-lr35902.o:  $(processor)/lr35902/lr35902.cpp $(call rwildcard,$(processor)/lr35902)
