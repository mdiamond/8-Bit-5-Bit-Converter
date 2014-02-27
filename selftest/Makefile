FILES=four.5b four.txt gettysburg.5b gettysburg.txt Makefile smile.5b smile.gif filter.c test.sh 45zero 45zero.5b 760r 760r.5b

5bit: 5bit.c
	gcc -o 5bit 5bit.c

good: 5bit.c
	gcc -o 5bit 5bit.good.c

filter: filter.c
	gcc -o filter filter.c

package: ${FILES}
	rm -rf ../selftest
	mkdir ../selftest || true
	cp ${FILES} ../selftest
	tar -C .. -czf selftest.tar.gz selftest/

testpack:
	rm -rf ~/tmp/selftest/
	tar -xvf selftest.tar.gz -C ~/tmp/
	gcc -o ~/tmp/selftest/5bit 5bit.good.c

# vim: tw=0
