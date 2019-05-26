import random

random.seed()
count = int(input("Count: "))
output_file = input("File: ")

with open(output_file, "w") as f:
	f.write("SizeA= {}\n".format(count))

	for i in range(count):
		f.write("{}{:.2f}\n".format(random.choice(["", "-"]), random.randint(0, 999) / 100))
	f.write("\n")

	f.write("SizeB= {}\n".format(count))
	for i in range(count):
		f.write("{}{:.2f}\n".format(random.choice(["", "-"]), random.randint(0, 999) / 100))
	f.write("\n")

