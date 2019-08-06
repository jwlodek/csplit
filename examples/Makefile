all:
	gcc basic_csplit_example.c -I../. -o basic_csplit_example
	gcc reading_csv_file.c -I../. -o reading_csv_example
	gcc str_processing_example.c -I../. -o str_processing_example
	gcc read_configure_example.c -I../. -o read_configure_example

debug:
	gcc basic_csplit_example.c -I../. -o basic_csplit_example_db -g
	gcc reading_csv_file.c -I../. -o reading_csv_example_db -g
	gcc str_processing_example.c -I../. -o str_processing_example_db -g
	gcc read_configure_example.c -I../. -o read_configure_example_db -g

clean:
	rm -f basic_csplit_example
	rm -f basic_csplit_example_db
	rm -f reading_csv_example
	rm -f reading_csv_example_db
	rm -f str_processing_example
	rm -f str_processing_example_db
	rm -f read_configure_example
	rm -f read_configure_example_db
