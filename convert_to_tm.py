import argparse

parser = argparse.ArgumentParser()
parser.add_argument('-i', '--infile', help='', required=True)
parser.add_argument('-o', '--outfile', help='', required=True)
program_args = parser.parse_args()

with open(program_args.infile, 'rb') as f:
    code = f.read()

padding = bytearray(0x3000)

final = padding + code

with open(program_args.outfile, 'wb') as f:
    f.write(final)
