CC=cc
OPTIONS=-g
COMPILE=$(CC) $(OPTIONS)
DEST=bin
DS= Dev/c/ds.c
FILES= Dev/c/delta-lisp/reader.c Dev/c/delta-lisp/reader-test.c

$(DEST)/reader-test: $(FILES)
	$(COMPILE) $(FILES) $(DS) -o $(DEST)/reader-test

reader-test: $(DEST)/reader-test
	$<

