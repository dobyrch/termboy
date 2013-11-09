name := termboy

processors := arm gsu hg51b lr35902 r6502 r65816 spc700 upd96050
include processor/Makefile
include ananke/Makefile
include gb/Makefile

ui_objects := ui-ethos ui-configuration ui-interface ui-utility
ui_objects += ui-input
ui_objects += ruby
ui_objects += $(if $(call streq,$(platform),win),resource)

# platform
ifeq ($(platform),x)
  ruby := video.glx video.xv video.sdl
  ruby += audio.alsa audio.openal audio.oss audio.pulseaudio audio.pulseaudiosimple audio.ao
  ruby += input.sdl input.x
else ifeq ($(platform),osx)
  ruby := video.cgl
  ruby += audio.openal
  ruby += input.carbon
else ifeq ($(platform),win)
  ruby := video.direct3d video.wgl video.directdraw video.gdi
  ruby += audio.directsound audio.xaudio2
  ruby += input.rawinput input.directinput
endif

# ruby
include ruby/Makefile
link += $(rubylink)

# rules
objects := $(ui_objects) $(objects)
objects := $(patsubst %,obj/%.o,$(objects))

obj/ui-ethos.o: $(ui)/ethos.cpp $(call rwildcard,$(ui)/)
obj/ui-configuration.o: $(ui)/configuration/configuration.cpp $(call rwildcard,$(ui)/)
obj/ui-interface.o: $(ui)/interface/interface.cpp $(call rwildcard,$(ui)/)
obj/ui-utility.o: $(ui)/utility/utility.cpp $(call rwildcard,$(ui)/)
obj/ui-input.o: $(ui)/input/input.cpp $(call rwildcard,$(ui)/)

obj/ruby.o: ruby/ruby.cpp $(call rwildcard,ruby/*)
	$(compiler) $(rubyflags) -c $< -o $@

obj/resource.o: $(ui)/resource.rc
ifeq ($(arch),win32)
	windres --target=pe-i386 $(ui)/resource.rc obj/resource.o
else
	windres $(ui)/resource.rc obj/resource.o
endif

# targets
build: $(objects)
ifeq ($(platform),x)
	$(compiler) -o out/$(name)-bin $(objects) $(link)
endif

install:
	sudo install -D -m 755 $(name) /usr/bin/$(name)
	sudo install -D -m 755 out/$(name)-bin /usr/share/$(name)/$(name)-bin
	sudo install -D -m 644 profile/boot.rom /usr/share/$(name)/boot.rom
	sudo install -D -m 644 profile/manifest.bml /usr/share/$(name)/manifest.bml
	sudo install -D -m 644 font.psfu.gz /usr/share/$(name)/font.psfu.gz

uninstall:
	sudo rm /usr/bin/$(name)
	sudo rm -r /usr/share/$(name)/
