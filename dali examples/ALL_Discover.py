#
# program to read the info about all devices on a DALI bus
# loop from 0-63 to get on/off/level and dali type
#

import RPi.GPIO as GPIO
import time
import serial
import sys

ser = serial.Serial(
	port ='/dev/ttyS0',
	baudrate = 19200,
	parity=serial.PARITY_NONE,
	stopbits=serial.STOPBITS_ONE,
	bytesize=serial.EIGHTBITS,
	timeout=1,
)

bus = 1

def serchannel():
	global bus
	x = ser.read(1)
	bus = 1
	if x == '1' :
		bus = 2
		x=ser.read(1)
	if x == '2' :
		bus = 3
		x=ser.read(1)
	if x == '3' :
		bus = 4
		x=ser.read(1)
	return x	


GPIO.setmode(GPIO.BCM)
GPIO.setup(5,GPIO.IN)
GPIO.setup(6,GPIO.IN)
x=ser.read(1000)

print('\n      ATX LED Consultants Inc - Fixture Read UPC, Version, SN \n')

ser.write('v\n')  # lets get version number first
x=ser.read()
if x == "" :
	print(' Missing Firmware or HAT \n')
	exit()

x=ser.read(2)
print('HW version: %s'%(x))
y=ser.read(2)
fwv = int(y,10)
print('FW version: %s'%(y))
hwtype=0
y=ser.read(2)
if y != "\n" :
	hwtype=int(y,10)

if hwtype == 2 :
	print('AL-DALI-Hat: 2 Channel Isolated no power')
if hwtype == 4 :
	print('AL-DALI-Hat: 4 Channel Isolated no power')

if hwtype == 0 :
	if GPIO.input(6) == 0:
		print('Power: Primary Power missing')
	else:
		print('Power: Primary Power OK')

	if GPIO.input(5) == 0:
		print('Power: Secondary Power missing')
	else:
		print('Power: Secondary Power OK')


	print('AL-DALI-Hat: single channel')
y=ser.read(2)
z=1
lasti=0

while z==1:
	x=ser.read(100)
	ser.write('p\n')  # now check if Power is good
	x=ser.read(1)
	print('\n Start a scan '),

	if x == 'P' :
		x=ser.read(2)
		if x != "00" :
			y = ser.read(1)
			print(' A = %s.%s Volts'%(x,y)),
			x=ser.read(2)
			y = ser.read(1)
			print(' B = %s.%s Volts'%(x,y)),
		else :
			x=ser.read(4)

		x=ser.read(1)
		if x == '1' :
			print('     Switch is ON\n')
		if x == '0' :
			print('     Switch is OFF\n')
		x=ser.read(1)
	i=0
	print('Scan DALI bus 1')
	ser.write('d\n')  # now check if Power is good
	x=ser.read(1)
	if x == 'D' :
		x=ser.read(1)
		y=int(x,16)
		if y == 2 :
			print('DALI BUS: power is good\n')
		if y == 0 :
			print('DALI BUS: power is lost\n')
		if y == 1 :
			print('DALI BUS: power is stuck high')
		if y != 2 :
			i = 64		
		x=ser.read(2)
	else :
		i = 64

	while ( i< 64 ):

		a = ( i * 2 ) + 1
		ser.write('h%02xA0\n'%(a))
		x=ser.read(1)
		if x == 'H' :
			data=ser.read(5)
			print('%s:H%s'%(bus,data)),
			x=ser.read(1)
		if x == 'D' :
			ser.write('d\n')  # now reset power
			ser.read(10)
			i=64
			x = 'N'
		if x == 'N' :
			ser.read(1)
			ser.write('h%02xAB\n'%(a))
			x=ser.read(1)
			if x == 'J' :
				if i != (lasti+1) :
					print('   Skipped Address')
				lasti = i
				x=ser.read(2)
				y=int(x,16)
				print('address %2d is AL-WS-010v at level %d : ' % (i,y ))
			x=ser.read(1)
		if x == 'J' :
			if i != (lasti+1) :
				print('   Skipped Address')
			lasti = i
			x=ser.read(2)
			y=int(x,16)
			x=ser.read(1)
			ser.write('h%02xA6\n'%(a))
			x=ser.read(1)
			z="  "
			if( x == 'J'):
				z=ser.read(2)
			x=ser.read(1)
			ser.write('h%02x99\n'%(a))
			x=ser.read(1)
			dt="  "
			zerov = ""
			if( x == 'J'):
				dt=ser.read(2)
				dti = int(dt,16)
				zerov = ""
				if dti != 0 :
					if hwtype == 1 :
						if fwv > 7 :
							x=ser.read(1) # newlink
							ser.write('u\n')  # write uxx where XX is the address 0-64
							x=ser.read(6)  # u00000 throw away
							zerov = "logic 0 = " + ser.read(4) + "mV"
							x=ser.read(4)  # 000 throw away
			x=ser.read(1) # \n
			print('address %2d is Model %s Type %s level %3d %s'%(i,z,dt,y,zerov))

		i = i+1

	if hwtype > 1 :
		x=ser.read(100)
		i=0
		print('Scan DALI bus 2')
		ser.write('1d\n')  # now check if Power is good
		ser.read(1)
		x=ser.read(1)
		if x == 'D' :
			x=ser.read(1)
			y=int(x,16)
			if y == 2 :
				print('DALI BUS: power is good')
			if y == 0 :
				print('DALI BUS: power is lost')
			if y == 1 :
				print('DALI BUS: power is stuck high')
			if y != 2 :
				i = 64
			x=ser.read(2)
		else :
			i = 64

		while ( i< 64):
			a = ( i * 2 ) + 1
			ser.write('1h%02xA0\n'%(a))
			x=serchannel()
			if x=='J' :
				x=ser.read(2)
				y=int(x,16)
				ser.read(1)
				ser.write('1h%02xA6\n'%(a))
				x=serchannel()
				z="  "
				if( x == "J"):
					z=ser.read(2)
				x=ser.read(1)
				ser.write('1h%02x99\n'%(a))
				x=serchannel()
				dt="  "
				if( x == "J"):
					dt=ser.read(2)
				x=ser.read(1)
				print('Address %s:%2d is Model %s Type %s level %3d'%(bus,i,z,dt,y))
				i = i+1
				continue
			if x == 'N':
				ser.read(1)
				ser.write('1h%02xAB\n'%(a))
				x=serchannel()
				if x == 'J' :
					if i != (lasti+1) :
						print('Skipped Address')
					lasti = i
					x=ser.read(2)
					y=int(x,16)
					print('address %2d is AL-WS-010v at level %d : ' % (i,y ))
				ser.read(1)
				i = i+ 1
				continue
			if x=='D' :
				ser.write('1d\n')  # now reset power
				i=64
				X = ser.read(10)
				print('D %s'%(x)),
			if x!='D' :
				print('Type %s'%(x)),
				y=ser.read(10)
				print(' data %s'%(y))
			ser.read(1)
			i = i+1
	if hwtype > 3 :
		x=ser.read(100)
		i=0
		print('Scan DALI bus 3')
		ser.write('2d\n')  # now check if Power is good
		x=ser.read(1)
		x=ser.read(1)
		if x == 'D' :
			x=ser.read(1)
			y=int(x,16)
			if y == 2 :
				print('DALI BUS: power is good')
			if y == 0 :
				print('DALI BUS: power is lost')
			if y == 1 :
				print('DALI BUS: power is stuck high')
			if y != 2 :
				i = 64
			x=ser.read(2)
		else :
			i = 64

		while ( i< 64):
			a = ( i * 2 ) + 1
			ser.write('2h%02xA0\n'%(a))
			x=ser.read(1)
			t=i
			if x=='2' :
				x=ser.read(1)
				t=300+i;
             		if x == 'J' :
				x=ser.read(2)
				y=int(x,16)
				x=ser.read(1)
				ser.write('2h%02xA6\n'%(a))
				x=ser.read(2)
				z="  "
				if( x == "2J"):
					z=ser.read(2)
				x=ser.read(1)
				ser.write('2h%02x99\n'%(a))
				x=ser.read(2)
				dt="  "
				if( x == "2J"):
					dt=ser.read(2)
				x=ser.read(1)
				print('address %2d is Model %s Type %s level %3d'%(t,z,dt,y))
			else :
				if x=='D' :
					ser.write('2d\n')  # now reset power
					i=64
					X = ser.read(10)
					print('D %s'%(x)),
				if x=='N' :
					ser.read(1)
				if x!='N' :
					print('T %s'%(x)),
					y=ser.read(10)
					print('d %s'%(y))

			i = i+1
	if hwtype > 3 :
		x=ser.read(100)
		i=0
		print('Scan DALI bus 4')
		ser.write('3d\n')  # now check if Power is good
		x=ser.read(1)
		x=ser.read(1)
		if x == 'D' :
			x=ser.read(1)
			y=int(x,16)
			if y == 2 :
				print('DALI BUS: power is good')
			if y == 0 :
				print('DALI BUS: power is lost')
			if y == 1 :
				print('DALI BUS: power is stuck high')
			if y != 2 :
				i = 64
			x=ser.read(2)
		else :
			i = 64

		while ( i< 64):
			a = ( i * 2 ) + 1
			ser.write('3h%02xA0\n'%(a))
			x=ser.read(1)
			t=i
			if x=='3' :
				x=ser.read(1)
				t=400+i;
             		if x == 'J' :
				x=ser.read(2)
				y=int(x,16)
				x=ser.read(1)
				ser.write('3h%02xA6\n'%(a))
				x=ser.read(2)
				z="  "
				if( x == "3J"):
					z=ser.read(2)
				x=ser.read(1)
				ser.write('3h%02x99\n'%(a))
				x=ser.read(2)
				dt="  "
				if( x == "3J"):
					dt=ser.read(2)
				x=ser.read(1)
				print('address %2d is Model %s Type %s level %3d'%(t,z,dt,y))
			else :
				if x=='D' :
					ser.write('3d\n')  # now reset power
					i=64
					X = ser.read(10)
					print('D %s'%(x)),
				if x=='N' :
					ser.read(1)
				if x!='N' :
					print('T %s'%(x)),
					y=ser.read(10)
					print('d %s'%(y))

			i = i+1

	ser.write('q\n')
	x = ser.read(20)
	print('%s'%x)	
	z=0
