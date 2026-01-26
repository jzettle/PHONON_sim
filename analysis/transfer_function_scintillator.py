import uproot
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
from matplotlib.colors import LogNorm

file = uproot.open("/N/project/phonon/PHONON_Bkgnds/job_submission/multisim/7333557/sum_planarneutron_scintillator.root")
tree = file["Primary"]
#tree2 = file["WaterNeutrons"]
#tree3 = file["AirNeutrons"]
tree4 = file["DetectorNeutrons"]
#tree5 = file["Hits"]

print("Creating dataframes...")

df = tree.arrays(library="pd")
df4 = tree4.arrays(library="pd")
#df5 = tree5.arrays(library="pd")


#print(df)
#print(df4)
print("Merging dataframe...")
merged_df4 = df.merge(df4, on=["EventID"], how="inner")
#print(merged_df4)
print("Plotting...")
heatmap, xedges, yedges = np.histogram2d(merged_df4['PrimaryEnergy_x'], merged_df4['KineticEnergy'], bins=50)
#heatmap = heatmap / heatmap.sum()
heatmap = heatmap / len(df)
heatmap = np.ma.masked_where(heatmap == 0, heatmap) 
plt.figure(figsize=(10, 6))
#plt.scatter(merged_df_fill4['PrimaryEnergy'], merged_df_fill4['KineticEnergy'], alpha=0.5, s=1)
#plt.imshow(merged_df_fill4['PrimaryEnergy'], merged_df_fill4['KineticEnergy'])
plt.title('Neutron Energy Transfer through full shielding')
plt.pcolormesh(xedges,yedges, heatmap.T, norm=LogNorm(), cmap="viridis")
plt.colorbar(label="Fraction of Total Events")
plt.xlabel('Generated Neutron Energy (MeV)')
plt.ylabel('Neutron Energy at PHONON Detector (MeV)')
plt.grid()
plt.savefig("NeutronTransfer_FullScintillator_planarneutrons_FracColorBar.pdf")
plt.show()

print("Generating efficiency plot...")

#generate binning in same way as above plot for primary energy to setup looking for hits in the scintillator 
#do for both overall neutrons and neutrons that enter the detector
#Look for both hits in 0-150 keV individually and also total NR for LiNbO3 block events
bins_num = 50
Emin, Emax = df['PrimaryEnergy'].min(), df['PrimaryEnergy'].max()
counts_den, edges = np.histogram(df['PrimaryEnergy'], bins=bins_num, range=(Emin, Emax))
print(counts_den)
print(edges)

bin_centers = (edges[:-1] + edges[1:]) / 2.0

new_counts, _ = np.histogram(df4['PrimaryEnergy'], bins=bins_num, range=(Emin, Emax))
print(new_counts)

eff_vals = np.divide(new_counts, counts_den)
print(eff_vals)

plt.figure(figsize=(10,6))
plt.plot(bin_centers, eff_vals, '-')
plt.title("")
plt.xlabel("Neutron Energy (MeV)")
plt.ylabel("Efficiency to reach scintillator")
plt.grid()
plt.savefig("test_efficiency_scintillator.pdf")
plt.show()

