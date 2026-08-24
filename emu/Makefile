SOURCES := $(wildcard source/*.c) $(wildcard source/**/*.c)
OBJECTS := $(patsubst source/%.c,bin/%.o,$(SOURCES))
PLAT    := $(shell uname -s)

ifeq ($(PLAT),NetBSD)
	override CPPFLAGS += -I/usr/X11R7/include -I/usr/pkg/include
	override LDFLAGS += -L/usr/X11R7/lib -L/usr/pkg/lib -Wl,-R/usr/X11R7/lib -Wl,-R/usr/pkg/lib
else ifeq ($(PLAT),windows)
	CC := x86_64-w64-mingw32-gcc
	override LDLIBS += -lkernel32 -l:libSDL2.a -lole32 -loleaut32 -limm32
	override LDLIBS += -lsetupapi -lversion -lgdi32 -lwinmm -lopengl32
else ifeq ($(PLAT), Linux)
	override CFLAGS += -D_GNU_SOURCE
	override LDLIBS += -lSDL2
endif
LD := $(CC)

OUT := y8emu

ifeq ($(STATIC), y)
	override CFLAGS += -static -static-libgcc -static-libstdc++
endif

override CFLAGS += -std=c99 -Wall -Wextra -Wuninitialized -Wundef -pedantic -Ilib -Werror=return-type
override LDLIBS += -lm

ifeq ($(BUILD),release)
	override CFLAGS   += -O3
	#override CPPFLAGS += -NDEBUG
else
	override CFLAGS += -O0 -ggdb -fno-omit-frame-pointer
	override LDFLAGS += -O0 -ggdb -fno-omit-frame-pointer
	ifeq ($(ASAN),y)
		override CFLAGS += -fsanitize=address,undefined
		override LDFLAGS += -fsanitize=address,undefined
	endif
endif

.SECONDEXPANSION:

deps.filter := %.c %.h
deps.option := -MM
define deps
$$(filter $$(deps.filter),,$$(shell $(CC) $(CFLAGS) $(CPPFLAGS) -E $(deps.option) $(1)))
endef

all: $(OUT)
	@:

run: $(OUT)
	'$(dir $<)$(notdir $<)' $(RUNFLAGS)

y8emu: $(OBJECTS)
	$(LD) $(LDFLAGS) $^ $(LDLIBS) -o $@

bin/:
	mkdir -p bin

bin/PlatinumSrc_%.o: lib/PlatinumSrc/%.c $(call deps,lib/PlatinumSrc/%.c) | bin/
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

bin/%.o: source/%.c $(call deps,source/%.c) | bin/
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -c -o $@

clean:
	-rm -r bin

distclean: clean
	rm $(OUT)

.PHONY: all run clean distclean install
