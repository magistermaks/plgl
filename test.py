import math

def sides(l, r):
    return math.pi / math.atan(l / 2 * r)

def ctg(x):
	return math.cos(x) / math.sin(x)

l = float(input('Enter length: '))
r = float(input('Enter radius: '))
n = float(input('Enter sides: '))

p1 = (n / 4.0) * ctg(math.pi / n) * (l * l)
p2 = l * n * r / 2

print(f'Sides l={l} and r={r} is {sides(l, r)}')
print(f'For sides {n}, P1={p1} P2={p2}')

