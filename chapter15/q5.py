import subprocess
import matplotlib.pyplot as plt

seeds = [1, 2, 3]
limits = [128, 256, 384, 512, 640, 768, 896, 1024]
num_addresses = 100

for seed in seeds:
    fractions = []

    for limit in limits:
        result = subprocess.run(
            [
                "python3", "relocation.py",
                "-s", str(seed),
                "-n", str(num_addresses),
                "-a", "1k",
                "-l", str(limit),
                "-c"
            ],
            capture_output=True,
            text=True
        )

        valid_count = result.stdout.count("VALID:")
        fraction = valid_count / num_addresses
        fractions.append(fraction)

        print(
            f"seed={seed}, limit={limit}, "
            f"valid={valid_count}/{num_addresses}, fraction={fraction:.2f}"
        )

    plt.plot(limits, fractions, marker="o", label=f"Seed {seed}")

plt.xlabel("Bounds Register (Limit)")
plt.ylabel("Fraction of Valid Virtual Addresses")
plt.title("Fraction of Valid Addresses vs. Bounds Register")
plt.legend()
plt.grid()

plt.savefig("q5.png")
plt.show()
