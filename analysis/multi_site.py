import uproot
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns

file = uproot.open("/N/project/phonon/PHONON_Bkgnds/job_submission/multisim/7395575/sum_neutron_planarsource_wafers.root")
tree = file["Primary"]
tree5 = file["Hits"]

df = tree.arrays(library="pd")
df5 = tree5.arrays(library="pd")

merged_df5 = df.merge(df5, on=["EventID"], how="inner")

plt.figure(figsize=(10, 6))
plt.hist(df5['CopyNumber'], bins=10, color='blue', alpha=0.7)
plt.title('Wafer Hits')
plt.xlabel('Hit Wafers')
plt.ylabel('Frequency')
plt.grid()
plt.savefig("wafer_hits.pdf")
plt.show()

#look at number of unique wafers hit in the event 
df5['numberWafers'] = df5.groupby('EventID')['CopyNumber'].transform('nunique')
print(df5)

df = df.merge(df5.groupby('EventID')['CopyNumber'].nunique()
                .reset_index(name='numberWafers'), on='EventID', how='left')
df['numberWafers'] = df['numberWafers'].fillna(0).astype(int)
print(df)

plt.figure(figsize=(10, 6))
plt.hist(df5['numberWafers'], color='blue', alpha=0.7)
plt.title('Total Number of Wafers hit')
plt.xlabel('Number of Hit Wafers')
plt.ylabel('Frequency')
plt.grid()
plt.yscale('log')
plt.savefig("total_wafers_hit.pdf")
plt.show()

mask = df['numberWafers'] > 0 
masked_df = df[mask]

plt.figure(figsize=(10, 6))
plt.hist(masked_df['numberWafers'], bins=10, range=(0,10), color='blue', alpha=0.7)
plt.title('Total Number of Wafers hit per event')
plt.xlabel('Number of Hit Wafers')
plt.ylabel('Frequency')
plt.grid()
plt.yscale('log')
plt.savefig("total_wafers_hit_eventlevel.pdf")
plt.show()
