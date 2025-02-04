import time
import socket
import zlib
import hashlib

# IP = socket.gethostbyname(socket.gethostname())
IP = "127.0.0.1"
SEND_PORT = 14000
RECEIVE_PORT = 15001
REC = (IP, RECEIVE_PORT)
SEND = (IP, SEND_PORT)
SIZE = 1024
FORMAT = "latin-1"
client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
client.bind(REC)
client.settimeout(0.25)

def main():
    fname = input("Enter file name: ")
    idx = 0
    packet = creatPacket(fname.encode(FORMAT), idx)
    client.sendto(packet, SEND)
    time.sleep(0.001)
    print("Packet ID:", idx)
    CRCanswer(fname.encode(FORMAT), idx)
    print("File name was sent")
    sendData(fname)
    print("File was sent")
    #to-do change hash verification
    sendHASH(fname)
    while (True):
        try:
            answer, addr = client.recvfrom(SIZE)
            break
        except socket.timeout:
            sendHASH(fname)
    answer = answer.decode("latin-1").rstrip()
    if answer == "NO":
        client.sendto("No".encode("latin-1"), (IP, SEND_PORT))
        start_time = time.time()
        while answer == "No":
            sendData(fname)
            print("File was sent")
            sendHASH(fname)
            answer, addr = client.recvfrom(SIZE)
            answer = answer.decode("latin-1").rstrip()
            elapsed_time = time.time() - start_time
            if (elapsed_time > 4): 
                    print("Time out") 
                    exit()
        client.sendto("Yes".encode("latin-1"), (IP, SEND_PORT))
        print("File was successfuly received!")
    else:
        client.sendto("Yes".encode("latin-1"), (IP, SEND_PORT))
        print("File was successfuly received!") 
    client.close()

def sendData(fname):
    f = open(fname, "rb")
    fdata = f.read(SIZE-8)
    idx = 1
    while True:
        if not fdata:
            break
        print("Packet ID:", idx)
        packet = creatPacket(fdata, idx)
        client.sendto(packet, SEND)
        time.sleep(0.001)
        CRCanswer(fdata, idx)
        fdata = f.read(SIZE-8)
        idx += 1
    client.sendto("".encode("latin-1"), (IP, 15000))

def CRCanswer(data, idx):
    while True:
        try:
            data, addr = client.recvfrom(SIZE)
            answer = data[8:]
            crc32_hash = zlib.crc32(answer)
            crc = int.from_bytes(data[4:8], byteorder='big')
            if (crc32_hash != crc):
                print("CRC is not OK")
                packet = creatPacket(data, idx)
                client.sendto(packet, SEND)
                time.sleep(0.001)
            else:
                answer = answer.decode(FORMAT).rstrip()
                if (answer == "OK"):
                    print("CRC is OK")
                    break
                else:
                    print("answer is not OK")
                    packet = creatPacket(data, idx)
                    client.sendto(packet, SEND)
                    time.sleep(0.001)
        except socket.timeout:
            packet = creatPacket(data, idx)
            client.sendto(packet, SEND)
            time.sleep(0.001)

def crc32(data):
    return zlib.crc32(data)

def sha256(data):
    return hashlib.sha256(data).digest()

def creatPacket(fdata, idx):
    crc = crc32(fdata)
    return idx.to_bytes(4, byteorder='big') + crc.to_bytes(4, byteorder='big') + fdata
    
def sendHASH(fname):
    f = open(fname, "rb")
    data = f.read()
    sha256_hash = sha256(data)
    print(sha256_hash)
    packet = creatPacket(sha256_hash, 1)
    client.sendto(packet, (IP, SEND_PORT))
    
    
if __name__ == "__main__":
    main() 