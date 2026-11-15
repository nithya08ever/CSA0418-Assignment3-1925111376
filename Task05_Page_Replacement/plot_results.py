import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("fault_results.csv")

for workload in ["locality", "random"]:
    subset = data[data["workload"] == workload]

    for algorithm in ["FIFO", "LRU", "Optimal"]:
        plt.plot(subset["frames"], subset[algorithm],
                 marker="o", label=algorithm)

    plt.xlabel("Number of Frames")
    plt.ylabel("Page Faults")
    plt.title(f"Page Replacement - {workload.capitalize()} Workload")
    plt.xticks(range(3, 11))
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.savefig(f"{workload}_page_faults.png")
    plt.close()

print("Graphs saved: locality_page_faults.png and random_page_faults.png")
