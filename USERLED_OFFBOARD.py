import socket
import sys





from pymongo import MongoClient
#connection = MongoClient("mongodb://127.0.0.1:27017/iot")
connection = MongoClient('192.168.1.101', 27017)  #CLOUD IP

db = connection.iot.dosiot



# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
prevledstatus = " "
server_address = ('192.168.1.104', 7)  #XINU IP
while 1:

	usrledstatus = db.find({'device':'USRLED'})
	
	for p in usrledstatus:
		if prevledstatus == p["value"]:
			continue

		if p["value"] == "USRLED1":
			message = 'USRLED1'
		else:
			message = 'USRLED0'
		prevledstatus = p["value"]

		if message == "USRLED1":
	        	print ("**** USER LED IS ON  ****")
			print ("\n\n\n\n")
		else:
			print ("**** USER LED IS OFF ****")
			print ("\n\n\n\n")

		sent = sock.sendto(message, server_address)
		data, server = sock.recvfrom(4096)
	
    	
sock.close()

