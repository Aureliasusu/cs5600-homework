import re
import matplotlib.pyplot as plt

pages = []
times = []

with open("vm_output.txt", "r") as f:
    for line in f:
        match = re.search(
            r"pages=(\d+)\s+trials=\d+\s+time_per_access=([\d.]+)\s+ns",
            line
        )

        if match:
            pages.append(int(match.group(1)))
            times.append(float(match.group(2)))

plt.figure()

plt.plot(pages, times, marker="o")

plt.xscale("log", base=2)

plt.xlabel("Number of Pages")
plt.ylabel("Time per Access (ns)")
plt.title("TLB Access Time - Ubuntu VM (CPU 0)")

plt.grid(True)

plt.savefig("vm_tlb.png", dpi=200, bbox_inches="tight")
plt.show()
