PROJECT=postal-async
ADDON=build/Release/$(PROJECT).node

CC_SRC=$(wildcard src/*.h src/*.cc)

check: lint test

lint:
	./node_modules/.bin/jshint *.js test

test: addon
	./node_modules/.bin/tape test/*.js | ./node_modules/.bin/tap-dot

$(ADDON): $(CC_SRC) CMakeLists.txt
	./node_modules/.bin/cmake-js build

addon: $(ADDON)

prebuild:
	prebuild --backend cmake-js --runtime napi --strip

clean:
	rm -rf build

.PHONY: check lint test addon clean prebuild
