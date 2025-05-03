# Raspberry Pi - send_image.py
import socket
from picamera2 import Picamera2
import time

# Set PC IP address and port
PC_IP = "192.168.1.15" 
PC_PORT = 5001

# Setup camera
picam2 = Picamera2()
picam2.start()
time.sleep(2)

# Capture image
frame = picam2.capture_array()
picam2.close()

# Convert image to bytes (JPEG format)
from PIL import Image
import io

image = Image.fromarray(frame)
buffer = io.BytesIO()
image.save(buffer, format='JPEG')
image_bytes = buffer.getvalue()

# Send image over socket
sock = socket.socket()
sock.connect((PC_IP, PC_PORT))
sock.sendall(len(image_bytes).to_bytes(8, 'big'))  # Send length first
sock.sendall(image_bytes)
sock.close()

print("Image sent successfully.")
