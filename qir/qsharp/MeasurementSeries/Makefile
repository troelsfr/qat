all: ungrouped

ungrouped: QSharpVersion/qir/Example.ll
	${QAT_BINARY} --apply -S -O3 --always-inline --unroll-loops --disable-grouping --profile default QSharpVersion/qir/Example.ll  > ungrouped.ll
	
	

QSharpVersion/qir/Example.ll:
	cd QSharpVersion && make qir/Example.ll

clean:
	cd QSharpVersion && make clean
	rm -f combined.ll optimized.ll
	rm -f validation.json