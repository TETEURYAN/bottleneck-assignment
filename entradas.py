import numpy as np

def generate_bap_instance(n, filename, max_cost=1000):
    matrix = np.random.randint(1, max_cost + 1, size=(n, n))
    with open(filename, 'w') as f:
        f.write(f"{n}\n")
        for row in matrix:
            f.write(" ".join(map(str, row)) + "\n")

# Gerar instâncias:
generate_bap_instance(100, "./instances/bat2.in")
generate_bap_instance(500, "./instances/bat3.in")
generate_bap_instance(1000, "./instances/bat4.in", max_cost=10000)