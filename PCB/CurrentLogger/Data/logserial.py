##############
## Script listens to serial port and writes contents into a file
##############
## requires pySerial to be installed 
import serial
import datetime

serial_port = '/dev/ttyACM0';
baud_rate = 115200; #In arduino, Serial.begin(baud_rate)
write_to_file_path = "output.txt";

output_file = open(write_to_file_path, "w+");
ser = serial.Serial(serial_port, baud_rate)
while True:
    line = ser.readline();
    line = line.decode("utf-8") #ser.readline returns a binary, convert to string
    line = line[:-2] + ",\t"+str(datetime.datetime.now()) +"\n"
    print(line);
    output_file.write(line);
