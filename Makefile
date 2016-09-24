# To build exifviewer

objs = exifviewer.o exif_tool.o exif_tagdb.o
execute = exifviewer

CC = gcc
RM = rm

PF = $(shell uname -s)

ifneq ($(PF), Linux*)
	EXT=.exe
endif

.PHONY: $(execute)
$(execute):$(objs)
	$(CC) -o $@ $^

$(objs):%.o:%.c
	$(CC) -c $^

.PHONY:clean
clean:
	$(RM) -f $(objs) $(execute)$(EXT)
