
APP="writer"
CC=$(CROSS_COMPILE)gcc

all: $(APP)

$(APP): writer.c
	$(CC) -o $@ $<

clean:
	rm -f $(APP)
