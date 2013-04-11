.PHONY: build

build:
	@echo "Uploading ./build/CoArduino.hex..."
	"avrdude" "-C/usr/share/arduino/hardware/tools/avrdude.conf" -q -q -V -patmega328p -carduino -P/dev/ttyACM1 -b115200 -D "-Uflash:w:./build/CoArduino.hex:i"
	
