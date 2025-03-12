import argparse


def bit_not(n, numbits=8):
    return (1 << numbits) - 1 - n

def swap_lines(addr, mask_1, mask_2):
    m1_val = mask_1 & addr
    m2_val = mask_2 & addr
    
    addr &= ~mask_1
    addr &= ~mask_2
    
    if m1_val:
        addr |=  mask_2
        
    if m2_val:
        addr |= mask_1
    
    return addr

parser = argparse.ArgumentParser("Convert to binary")
parser.add_argument("--input", required=True)
parser.add_argument("--output", required=True)
parser.add_argument("--swaplines", required=False)

args=parser.parse_args()

with open(args.input, "r") as infile:
	indata = infile.readlines()

stripped_indata = [i.strip() for i in  indata]

bindata = []
addresses = []
for d in stripped_indata:
	if len(d) <= 20:
		(addr, value)= d.split()
		hex_value = int(value, 16)
		assert hex_value <= 0xFF
		bindata.append(hex_value)
		addresses.append(int(addr, 16))
  
  


m, n = 15,16

mask_1 = 1<<m 
mask_2 = 1<<n 
swap_mapping = {}
if args.swaplines:
	print(f"Swapping lines {m} and {n}")  
	for a in addresses:
		swap_mapping[a] = swap_lines(a, mask_1, mask_2)
  
  
	resorted_mapping = {k: v for k, v in sorted(swap_mapping.items(), key=lambda item: item[1])}
	
	resorted_bindata = []
	for k in resorted_mapping.keys():
		resorted_bindata.append(bindata[k])
  

	with open(args.output, "wb") as outfile:
		outfile.write(bytes(resorted_bindata))

else:
	with open(args.output, "wb") as outfile:
		outfile.write(bytes(bindata))



