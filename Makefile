.PHONY: all run clean

all:
	$(MAKE) -C KR1

run:
	$(MAKE) -C KR1 run

clean:
	$(MAKE) -C KR1 clean
