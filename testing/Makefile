test: testConedy testPython-Conedy


testPython-Conedy:
	find ./ -mindepth 1 -maxdepth 1 -path '*/.svn' -prune  -o  -type d -exec sh -c "echo {} && make -s --no-print-directory  -C {} testPython-Conedy " \;

testConedy:
	find ./ -mindepth 1 -maxdepth 1 -path '*/.svn' -prune  -o  -type d -exec sh -c "echo {} && make -s --no-print-directory  -C {} testConedy " \;

clean:
	find ./ -mindepth 1 -maxdepth 1 -path '*/.svn' -prune  -o  -type d -exec sh -c "echo {} && make -s --no-print-directory  -C {} clean" \;


submit:
	find ./ -mindepth 1 -maxdepth 1 -path '*/.git' -prune  -o  -type d -exec sh -c "echo {} && make -s --no-print-directory  -C {} submit" \;

