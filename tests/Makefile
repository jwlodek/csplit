tests:
	wget https://github.com/Snaipe/Criterion/releases/download/v2.3.3/criterion-v2.3.3-linux-x86_64.tar.bz2
	tar -xjf criterion-v2.3.3-linux-x86_64.tar.bz2
	mv criterion-v2.3.3 criterion
	rm *.tar.bz2
	gcc csplit_core_tests.c -I../. -I./criterion/include/. -L./criterion/lib/. -o csplit_core_tests -lcriterion
	LD_LIBRARY_PATH=./criterion/lib:$LD_LIBRARY_PATH ./csplit_core_tests
clean:
	rm -f csplit_core_tests
