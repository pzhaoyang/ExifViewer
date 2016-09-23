# To build exifviewer

objs = exifviewer.o exif_tool.o exif_tagdb.o
execute = exifviewer


.PHONY: $(execute)
$(execute):$(objs)
	$(CC) -o $@ $^

%.o:%.c
	$(CC) -c $^

.PHONY:clean
clean:
	$(RM) -f $(objs) $(execute)
