#! /usr/bin/python
# a simple tcp server
import socket,os
import time
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
sock.bind(('127.0.0.1', 2301))  
sock.listen(5)  
connection,address = sock.accept()  

while True:  
    buf = connection.recv(100)  
    print(buf)
    # time.sleep(0.5)
    connection.send(buf)    		
    # 05 00 e5 50 66 55 00 00 10 27 00 00 00 00 00 00 e8 03 fa 22 ff 7f 00 00
    
connection.close()
