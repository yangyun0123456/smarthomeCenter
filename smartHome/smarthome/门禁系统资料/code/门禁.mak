CC = iccavr
CFLAGS =  -IC:\icc\include\ -e -DATMEGA -DATMega16  -l -g -Mavr_enhanced 
ASFLAGS = $(CFLAGS)  -Wa-g
LFLAGS =  -O -LC:\icc\lib\ -g -ucrtatmega.o -bfunc_lit:0x54.0x4000 -dram_end:0x45f -bdata:0x60.0x45f -dhwstk_size:16 -beeprom:1.512 -fihx_coff -S2
FILES = main.o wg26.o util.o tips.o password.o timer.o log.o locker.o ringandscreen.o eeprom.o uart.o iic.o 

�Ž�:	$(FILES)
	$(CC) -o �Ž� $(LFLAGS) @�Ž�.lk   -lcatmega
main.o: C:/icc/include/stdio.h C:/icc/include/stdarg.h C:/icc/include/_const.h C:/icc/include/stdlib.h C:/icc/include/_const.h C:/icc/include/limits.h C:/icc/include/string.h C:/icc/include/_const.h C:/icc/include/iom16v.h C:/icc/include/macros.h\
 D:\smarthome\smarthome\�Ž�ϵͳ����\code/uart.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/wg26.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/util.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/tips.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/timer.h\
 D:\smarthome\smarthome\�Ž�ϵͳ����\code/ringandscreen.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/iic.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/eeprom.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/password.h\
 D:\smarthome\smarthome\�Ž�ϵͳ����\code/locker.h
main.o:	D:\smarthome\smarthome\�Ž�ϵͳ����\code\main.c
	$(CC) -c $(CFLAGS) D:\smarthome\smarthome\�Ž�ϵͳ����\code\main.c
wg26.o: C:/icc/include/iom16v.h C:/icc/include/macros.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/password.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/timer.h
wg26.o:	D:\smarthome\smarthome\�Ž�ϵͳ����\code\wg26.c
	$(CC) -c $(CFLAGS) D:\smarthome\smarthome\�Ž�ϵͳ����\code\wg26.c
util.o: D:\smarthome\smarthome\�Ž�ϵͳ����\code/util.h
util.o:	D:\smarthome\smarthome\�Ž�ϵͳ����\code\util.c
	$(CC) -c $(CFLAGS) D:\smarthome\smarthome\�Ž�ϵͳ����\code\util.c
tips.o: C:/icc/include/iom16v.h C:/icc/include/macros.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/tips.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/util.h
tips.o:	D:\smarthome\smarthome\�Ž�ϵͳ����\code\tips.c
	$(CC) -c $(CFLAGS) D:\smarthome\smarthome\�Ž�ϵͳ����\code\tips.c
password.o: C:/icc/include/iom16v.h C:/icc/include/macros.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/log.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/locker.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/tips.h\
 D:\smarthome\smarthome\�Ž�ϵͳ����\code/timer.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/iic.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/eeprom.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/uart.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/password.h\
 D:\smarthome\smarthome\�Ž�ϵͳ����\code/ringandscreen.h
password.o:	D:\smarthome\smarthome\�Ž�ϵͳ����\code\password.c
	$(CC) -c $(CFLAGS) D:\smarthome\smarthome\�Ž�ϵͳ����\code\password.c
timer.o: C:/icc/include/iom16v.h C:/icc/include/macros.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/timer.h
timer.o:	D:\smarthome\smarthome\�Ž�ϵͳ����\code\timer.c
	$(CC) -c $(CFLAGS) D:\smarthome\smarthome\�Ž�ϵͳ����\code\timer.c
log.o: C:/icc/include/iom16v.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/uart.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/log.h
log.o:	D:\smarthome\smarthome\�Ž�ϵͳ����\code\log.c
	$(CC) -c $(CFLAGS) D:\smarthome\smarthome\�Ž�ϵͳ����\code\log.c
locker.o: C:/icc/include/iom16v.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/timer.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/util.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/tips.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/locker.h
locker.o:	D:\smarthome\smarthome\�Ž�ϵͳ����\code\locker.c
	$(CC) -c $(CFLAGS) D:\smarthome\smarthome\�Ž�ϵͳ����\code\locker.c
ringandscreen.o: C:/icc/include/iom16v.h C:/icc/include/macros.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/util.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/tips.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/timer.h\
 D:\smarthome\smarthome\�Ž�ϵͳ����\code/ringandscreen.h
ringandscreen.o:	D:\smarthome\smarthome\�Ž�ϵͳ����\code\ringandscreen.c
	$(CC) -c $(CFLAGS) D:\smarthome\smarthome\�Ž�ϵͳ����\code\ringandscreen.c
eeprom.o: C:/icc/include/iom16v.h C:/icc/include/macros.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/util.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/iic.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/eeprom.h
eeprom.o:	D:\smarthome\smarthome\�Ž�ϵͳ����\code\eeprom.c
	$(CC) -c $(CFLAGS) D:\smarthome\smarthome\�Ž�ϵͳ����\code\eeprom.c
uart.o: C:/icc/include/iom16v.h C:/icc/include/macros.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/password.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/locker.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/timer.h\
 D:\smarthome\smarthome\�Ž�ϵͳ����\code/uart.h
uart.o:	D:\smarthome\smarthome\�Ž�ϵͳ����\code\uart.c
	$(CC) -c $(CFLAGS) D:\smarthome\smarthome\�Ž�ϵͳ����\code\uart.c
iic.o: C:/icc/include/iom16v.h C:/icc/include/macros.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/util.h D:\smarthome\smarthome\�Ž�ϵͳ����\code/iic.h
iic.o:	D:\smarthome\smarthome\�Ž�ϵͳ����\code\iic.c
	$(CC) -c $(CFLAGS) D:\smarthome\smarthome\�Ž�ϵͳ����\code\iic.c
