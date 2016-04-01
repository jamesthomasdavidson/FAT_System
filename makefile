all:
	gcc programs/p1.c -o diskinfo
	gcc programs/p2.c -o disklist
	gcc programs/p3.c -o diskget
	gcc programs/p4.c -o diskput
p1:
	gcc programs/p1.c -o diskinfo

p2:
	gcc programs/p2.c -o disklist

p3:
	gcc programs/p3.c -o diskget

p4:
	gcc programs/p4.c -o diskput
shell:
	gcc programs/diskshell.c -lreadline -o diskshell
clean:
	rm diskinfo
	rm disklist
	rm diskget
	rm diskput
	rm diskshell

