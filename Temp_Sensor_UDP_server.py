import socket
import sys
from pymongo import MongoClient

connection = MongoClient('192.168.1.101', 27017)  #CLOUD IP

db = connection.iot.dosiot


query ={'device': 'temperature'}


# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Bind the socket to the port
server_address = ('192.168.1.100', 10000)    #EDGE IP

print >>sys.stderr, 'Starting up EDGE on %s port %s' % server_address

sock.bind(server_address)


while True:
    #print >>sys.stderr, '\nwaiting to receive message'
    data, address = sock.recvfrom(4096)
    
    #print >>sys.stderr, 'received %s bytes from %s' % (len(data), address)
    print >>sys.stderr, '*** Current Temperature value %s ***' % data

    print ("\n\n\n\n")
    print ("\n\n\n\n")
	
    db.update(query, {'$set': {'value': data}})

    if data:
        sent = sock.sendto(data, address)
        #print >>sys.stderr, 'sent %s bytes back to %s' % (sent, address)
