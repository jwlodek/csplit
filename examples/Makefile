all:
	gcc basic_csplit_example.c -I../. -o basic_csplit_example
	gcc reading_csv_file.c -I../. -o reading_csv_example

debug:
	gcc basic_csplit_example.c -I../. -o basic_csplit_example_db -g
	gcc reading_csv_file.c -I../. -o reading_csv_example -g

clean:
	rm -f basic_csplit_example
	rm -f basic_csplit_example_db
	rm -f reading_csv_example
	rm -f reading_csv_example_db
	