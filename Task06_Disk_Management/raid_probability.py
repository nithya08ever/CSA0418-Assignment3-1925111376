p = 0.02
rebuild_days = 1
daily_failure = 1 - (1 - p) ** (1 / 365)

# RAID 1: one surviving copy; approximate probability that
# another of the five remaining disks fails during a one-day rebuild.
raid1_risk = 1 - (1 - daily_failure) ** 5

# RAID 5: after one disk fails, any of the five remaining disks
# failing during rebuild causes data loss.
raid5_risk = 1 - (1 - daily_failure) ** 5

print(f"Assumed annual disk failure probability: {p*100:.2f}%")
print(f"Assumed rebuild period: {rebuild_days} day")
print(f"Approximate RAID 1 rebuild-window risk: {raid1_risk*100:.6f}%")
print(f"Approximate RAID 5 rebuild-window risk: {raid5_risk*100:.6f}%")
print("Note: Simplified independent-failure model; not a full reliability model.")
