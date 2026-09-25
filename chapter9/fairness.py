import subprocess
import re
import matplotlib.pyplot as plt


def run_lottery(job_length, seed):
    command = [
        "python3",
        "/home/yaoqin/ostep-homework/cpu-sched-lottery/lottery.py",
        "-l",
        f"{job_length}:100,{job_length}:100",
        "-s",
        str(seed),
        "-c"
    ]

    result = subprocess.run(
        command,
        capture_output=True,
        text=True
    )

    return result.stdout


job_lengths = [1, 2, 5, 10, 20, 50, 100, 200, 500, 1000]
average_fairness_values = []

for job_length in job_lengths:
    fairness_values = []

    for seed in range(30):
        output = run_lottery(job_length, seed)

        times = re.findall(r"DONE at time (\d+)", output)

        first_finish = int(times[0])
        second_finish = int(times[1])

        fairness = first_finish / second_finish
        fairness_values.append(fairness)

    average_fairness = sum(fairness_values) / len(fairness_values)
    average_fairness_values.append(average_fairness)

    print(
        "Job length:",
        job_length,
        "Average fairness:",
        average_fairness
    )


plt.plot(job_lengths, average_fairness_values, marker="o")

plt.xscale("log")

plt.xlabel("Job Length")
plt.ylabel("Average Fairness")
plt.title("Lottery Scheduling Fairness")

plt.ylim(0, 1.05)

plt.grid(True)

plt.savefig("fairness.png")

plt.show()
 
