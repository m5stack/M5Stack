#!/usr/bin/python3

'''
# Example:
> python bin2code.py m5_logo.jpg
> Out:m5_logo.jpg.c Done!
'''

import sys, os

in_name = sys.argv[1]
out_name = in_name + '.c'
file_size = os.path.getsize(in_name)

with open(in_name, 'rb') as infile:
  with open(out_name, 'wb+') as outfile:
    arrary_name = 'const unsigned char ' + out_name[0:out_name.find('.')] + '[' +str(file_size)+'] = {\n'
    outfile.write(arrary_name.encode('utf-8'))
    while True:
      data = infile.read(20)
      if len(data) > 0:
        # outfile.write('\t'.encode('utf-8'))
        for i in range(0, len(data)):
          d = "0x%02x," % data[i]
          outfile.write(d.encode('utf-8'))
        outfile.write('\n'.encode('utf-8'))
      else:
        outfile.write('};\n'.encode('utf-8'))
        break

print('Out:'+ out_name +' Done!')