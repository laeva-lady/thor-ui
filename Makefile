r:
	cmake . -B build
	make -C build
	mv build/loki .
	./loki
