"""
This script concatenates .check files and prints the output to stdout. The .check file locations
are read from the arguments.

The reason it's required is that all #include directives need to be at the beginning of the file.
"""

import sys

def main():
    includes = set()
    lines = []

    for file_path in sys.argv[1:]:
        with open(file_path) as check_file:
            for line in check_file:
                if line.strip().startswith("#include"):
                    include = line.strip().replace("#include", "").strip()
                    includes.add(include)
                else:
                    lines.append(line.rstrip())

    for include in includes:
        print("#include {0}".format(include))
    for line in lines:
        print(line)

if __name__ == "__main__":
	main()

