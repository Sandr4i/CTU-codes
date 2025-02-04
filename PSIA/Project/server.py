import time
import socket
import hashlib
import zlib

# IP = socket.gethostbyname(socket.gethostname())
IP = "127.0.0.1"
# SEND_PORT = 15001 
# RECEIVE_PORT = 14000
SEND_PORT = 14001 
RECEIVE_PORT = 15000
REC = (IP, RECEIVE_PORT)
SEND = (IP, SEND_PORT)
SIZE = 1024
FORMAT = "latin-1"

server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server.bind(REC)

def main():
    print("Server started")
    print("Server is listening...")
    idx = -1
    print("Packet ID:", idx+1)
    data, address = server.recvfrom(SIZE)
    checkCRC(data, idx)
    fname = data[8:].decode(FORMAT).rstrip()
    print(f"{(IP, SEND_PORT)} Connected") 
    print("File name recieved: " + fname)
    receiveData(fname)
    
    
    f = open("get/" + fname, "rb")
    data = f.read()
    ret  = receiveHASH(data)
    if ret == "SHA256 hash verification failed":
        server.sendto("Hash verification failed".encode("latin-1"), (IP, SEND_PORT))
        while (ret == "SHA256 hash verification failed"):
            receiveData(fname)
            f = open("get/" + fname, "rb")
            f.read()
            ret  = receiveHASH(f)
        print("Hash is ok!")
    server.sendto("File data received".encode("latin-1"), (IP, SEND_PORT))
    server.close()
    print(f"{(IP, SEND_PORT)} Disconnected")
    
def receiveData(fname):
    file = open("get/" + fname, "wb")
    id = 0
    while True:
        data, addr = server.recvfrom(SIZE)
        if not data:
            break
        print("Packet ID:", id+1)
        checkCRC(data, id)
        file.write(data[8:])
        id+=1
    file.close()

def createPacket(data, idx):
    crc = zlib.crc32(data)
    return idx.to_bytes(4, byteorder='big') + crc.to_bytes(4, byteorder='big') + data

def checkCRC(data, id):
    idx = int.from_bytes(data[:4], byteorder='big')
    print(idx, id+1)
    while (idx != id + 1):
        print("Packet ID is wrong")
        print(idx, id+1)
        packet = createPacket("NO".encode(FORMAT), id+1)
        server.sendto(packet, SEND)
        time.sleep(0.001)
        data, addr = server.recvfrom(SIZE)
        idx = int.from_bytes(data[:4], byteorder='big')
    crc = int.from_bytes(data[4:8], byteorder='big')
    ret = compareCRC(data[8:], crc)
    while (ret != "OK"):
        print("CRC is not OK")
        packet = createPacket("NO".encode(FORMAT), id+1)
        server.sendto(packet, SEND)
        time.sleep(0.001)
        data, addr = server.recvfrom(SIZE)
        crc = int.from_bytes(data[4:8], byteorder='big')
        ret = compareCRC(data[8:], crc)
    packet = createPacket("OK".encode(FORMAT), id+1)
    server.sendto(packet, SEND)
    time.sleep(0.001)
        

def compareCRC(fdata, crc):
    ret = "NO"
    crc32_hash = zlib.crc32(fdata)
    if crc == crc32_hash:
        print("CRC is OK")
        ret = "OK"
    return ret

def receiveHASH(data):
    sha256_hash = hashlib.sha256(data).digest()
    sha256_recv, addr = server.recvfrom(SIZE)
    checkCRC(sha256_recv, 0)
    print(sha256_hash)
    if sha256_recv == sha256_hash:
        print("SHA256 hash verified")
        server.sendto("OK".encode("latin-1"), (IP, SEND_PORT))
    else:
        server.sendto("NO".encode("latin-1"), (IP, SEND_PORT))
        print("SHA256 hash verification failed")

if __name__ == "__main__":
    main()