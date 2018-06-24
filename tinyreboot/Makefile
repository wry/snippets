TARGET := rbt
STRIP := strip
OBJDUMP := objdump
AWK := awk --non-decimal-data
# --non-decimal-data isn't needed w/ busybox awk

all: $(TARGET)

# hd
rbt: rbt.S
	$(CC) -nostdlib -o $@ $<
	$(STRIP) -s $@
	$(STRIP) --remove-section=.note.gnu.build-id $@

# bd
splice: rbt true
	@$(OBJDUMP) -h ./rbt | $(AWK) -v S=$$(objdump -h ./true | $(AWK) --non-decimal-data '{ if($$0~/\.text/) {print ("0x"$$6)+0 }}') '{ if($$0~/\.text/){print "dd if=./rbt of=./true bs=1 count="(("0x"$$3)+0)" skip="(("0x"$$6)+0)" seek="S" conv=notrunc" }}' 

.PHONY: clean
clean:
	$(RM) rbt true
