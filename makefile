all:
	gcc calc_checksum.c crc32/libcrc32.a -o chksum
	gcc main.c crc32/libcrc32.a -o source

clean:
	rm -rf chksum
	rm -rf source
