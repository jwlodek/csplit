#
# Example builder Makefile. 
# Please edit the below common parameters for compilation
#

CC=gcc
IDIR=../.
CFLAGS=-I$(IDIR)
DEBUG_FLAGS=-D CSPLIT_DEBUG -g

all:
	$(CC) basic_csplit_example.c $(CFLAGS) -o basic_csplit_example
	$(CC) reading_csv_file.c $(CFLAGS) -o reading_csv_example
	$(CC) str_processing_example.c $(CFLAGS) -o str_processing_example
	$(CC) read_configure_example.c $(CFLAGS) -o read_configure_example

debug:
	$(CC) $(DEBUG_FLAGS) basic_csplit_example.c $(CFLAGS) -o basic_csplit_example_db
	$(CC) $(DEBUG_FLAGS) reading_csv_file.c $(CFLAGS) -o reading_csv_example_db
	$(CC) $(DEBUG_FLAGS) str_processing_example.c $(CFLAGS) -o str_processing_example_db
	$(CC) $(DEBUG_FLAGS) read_configure_example.c $(CFLAGS) -o read_configure_example_db

clean:
	rm -f basic_csplit_example
	rm -f basic_csplit_example_db
	rm -f reading_csv_example
	rm -f reading_csv_example_db
	rm -f str_processing_example
	rm -f str_processing_example_db
	rm -f read_configure_example
	rm -f read_configure_example_db
