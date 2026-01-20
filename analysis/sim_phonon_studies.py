import uproot
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
from matplotlib.colors import LogNorm

file = uproot.open("/N/project/phonon/PHONON_Bkgnds/job_submission/multisim/7401552/cevns_full_source_00.root")
tree = file["Primary"]
tree2 = file["PrimaryPhononTracks"]
tree3 = file["SecondaryPhononTracks"]

df = tree.arrays(library="pd")
df2 = tree2.arrays(library="pd")
df3 = tree3.arrays(library="pd")

file_mono = uproot.open("/N/project/phonon/PHONON_Bkgnds/job_submission/multisim/7401553/cevns_full_source_mono_00.root")
tree_mono = file_mono["Primary"]
tree2_mono = file_mono["PrimaryPhononTracks"]
tree3_mono = file_mono["SecondaryPhononTracks"]

df_mono = tree_mono.arrays(library="pd")
df2_mono = tree2_mono.arrays(library="pd")
df3_mono = tree3_mono.arrays(library="pd")

print("Plotting phonon number and energy spectra...")

plt.figure(figsize=(10, 6))
plt.hist(df['totalPhonons'], bins=200, color='blue', alpha=0.7)
plt.title('Total number of Phonons')
plt.xlabel('Phonon Number/1000')
plt.ylabel('Frequency')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_spectrum_phonon_number.pdf")
plt.show()

plt.figure(figsize=(10, 6))
plt.hist(df2['StartEnergy'], bins=200, color='blue', alpha=0.7)
plt.title('Total number of Primary Phonons')
plt.xlabel('Phonon Energy')
plt.ylabel('Number of Phonons/1000')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_spectrum_primary_phonon_energy.pdf")
plt.show()

plt.figure(figsize=(10, 6))
plt.hist(df3['StartEnergy'], bins=200, color='blue', alpha=0.7)
plt.title('Total number of Secondary Phonons')
plt.xlabel('Phonon Energy')
plt.ylabel('Number of Phonons/1000')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_spectrum_secondary_phonon_energy.pdf")
plt.show()

plt.figure(figsize=(10, 6))
plt.hist(df_mono['totalPhonons'], bins=200, color='blue', alpha=0.7)
plt.title('Total number of Phonons')
plt.xlabel('Phonon Number/1000')
plt.ylabel('Frequency')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_mono_phonon_number.pdf")
plt.show()

plt.figure(figsize=(10, 6))
plt.hist(df2_mono['StartEnergy'], color='blue', alpha=0.7)
plt.title('Total number of Primary Phonons')
plt.xlabel('Phonon Energy')
plt.ylabel('Number of Phonons/1000')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_mono_phonon_number.pdf")
plt.show()

plt.figure(figsize=(10, 6))
plt.hist(df3_mono['StartEnergy'], bins=200, color='blue', alpha=0.7)
plt.title('Total number of Secondary Phonons')
plt.xlabel('Phonon Energy')
plt.ylabel('Number of Phonons/1000')
plt.yscale('log')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_mono_phonon_number.pdf")
plt.show()

heatmap, xedges, yedges = np.histogram2d(df['PrimaryEnergy'], df['totalPhonons'], bins=50)
#heatmap = heatmap / heatmap.sum()
#heatmap = heatmap / 5000000.0
heatmap = np.ma.masked_where(heatmap == 0, heatmap) 

plt.figure(figsize=(10, 6))
plt.scatter(df['PrimaryEnergy'], df['totalPhonons'], alpha=0.5, s=1)
plt.title('Primary Phonons generated vs energy deposition')
plt.pcolormesh(xedges,yedges, heatmap.T, norm=LogNorm(), cmap="viridis")
plt.colorbar(label="Events")
plt.xlabel('Primary Nuclear Recoil Energy (keVnr)')
plt.ylabel('Total Number of Phonons/1000')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_spectrum_phononenergyvsnumber.pdf")
plt.show()

heatmap, xedges, yedges = np.histogram2d(df_mono['PrimaryEnergy'], df_mono['totalPhonons'], bins=50)
#heatmap = heatmap / heatmap.sum()
#heatmap = heatmap / 5000000.0
heatmap = np.ma.masked_where(heatmap == 0, heatmap) 

plt.figure(figsize=(10, 6))
plt.scatter(df_mono['PrimaryEnergy'], df_mono['totalPhonons'], alpha=0.5, s=1)
plt.title('Primary Phonons generated vs energy deposition')
plt.pcolormesh(xedges,yedges, heatmap.T, norm=LogNorm(), cmap="viridis")
plt.colorbar(label="Events")
plt.xlabel('Primary Nuclear Recoil Energy (keVnr)')
plt.ylabel('Total Number of Phonons/1000')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_mono_phononenergyvs_number.pdf")
plt.show()

#Add a column in the primary df counting the number of entries in each EventID in the primary/secondary counters
counts_prim = df2["EventID"].value_counts()
df["TotalPrimaryPhonons"] = df["EventID"].map(counts_prim).fillna(0).astype(int)
counts_sec = df3["EventID"].value_counts()
df["TotalSecondaryPhonons"] = df["EventID"].map(counts_sec).fillna(0).astype(int)

counts_prim_mono = df2_mono["EventID"].value_counts()
df_mono["TotalPrimaryPhonons"] = df_mono["EventID"].map(counts_prim_mono).fillna(0).astype(int)
counts_sec_mono = df3_mono["EventID"].value_counts()
df_mono["TotalSecondaryPhonons"] = df_mono["EventID"].map(counts_sec_mono).fillna(0).astype(int)

plt.figure(figsize=(10, 6))
plt.hist(df["TotalPrimaryPhonons"], bins=200, color='blue', alpha=0.7)
plt.title('Total number of Primary Phonons')
plt.xlabel('Primary Phonon Number')
plt.ylabel('Events')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_spectrum_primary_phonon_number.pdf")
plt.show()

plt.figure(figsize=(10, 6))
plt.hist(df['TotalSecondaryPhonons'], bins=200, color='blue', alpha=0.7)
plt.title('Total number of Secondary Phonons')
plt.xlabel('Secondary Phonon Number')
plt.ylabel('Events')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_spectrum_secondary_phonon_number.pdf")
plt.show()

plt.figure(figsize=(10, 6))
plt.hist(df_mono["TotalPrimaryPhonons"], bins=200, color='blue', alpha=0.7)
plt.title('Total number of Primary Phonons')
plt.xlabel('Primary Phonon Number')
plt.ylabel('Events')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_mono_primary_phonon_number.pdf")
plt.show()

plt.figure(figsize=(10, 6))
plt.hist(df_mono['TotalSecondaryPhonons'], bins=200, color='blue', alpha=0.7)
plt.title('Total number of Secondary Phonons')
plt.xlabel('Secondary Phonon Number')
plt.ylabel('Events')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_mono_secondary_phonon_number.pdf")
plt.show()

#make a few plots, calcs using the scaled phonon values (edep only reasonable here without downconversion/scattering)
phonon_scale = 1000
primary_esum = df2.groupby("EventID")["StartEnergy"].sum()*phonon_scale*1e3
df["PrimaryPhononEdep"] = df["EventID"].map(primary_esum).fillna(0).astype(float)
#slightly more complex for secondary, need to take into account the individual phonon energy as not identical
secondary_esum = df3.groupby("EventID")["StartEnergy"].sum()*phonon_scale*1e3 #sum energy and convert to keV
df["SecondaryPhononEdep"] = df["EventID"].map(secondary_esum).fillna(0).astype(float) 

primary_esum_mono = df2_mono.groupby("EventID")["StartEnergy"].sum()*phonon_scale*1e3
df_mono["PrimaryPhononEdep"] = df_mono["EventID"].map(primary_esum_mono).fillna(0).astype(float)
#slightly more complex for secondary, need to take into account the individual phonon energy as not identical
secondary_esum_mono = df3_mono.groupby("EventID")["StartEnergy"].sum()*phonon_scale*1e3 #sum and convert to keV
df_mono["SecondaryPhononEdep"] = df_mono["EventID"].map(secondary_esum_mono).fillna(0).astype(float) 

heatmap, xedges, yedges = np.histogram2d(df['PrimaryEnergy']*1000, df['PrimaryPhononEdep'], bins=50)
#heatmap = heatmap / heatmap.sum()
#heatmap = heatmap / 5000000.0
heatmap = np.ma.masked_where(heatmap == 0, heatmap)

plt.figure(figsize=(10, 6))
plt.scatter(df['PrimaryEnergy']*1000, df['PrimaryPhononEdep'], alpha=0.5, s=1)
plt.title('Primary Phonon Total Energy vs NR energy deposition')
plt.pcolormesh(xedges,yedges, heatmap.T, norm=LogNorm(), cmap="viridis")
plt.colorbar(label="Events")
plt.xlabel('Primary Nuclear Recoil Energy (keVnr)')
plt.ylabel('Primary Phonon Combined Energy (keV)')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_spectrum_primaryedepenergy.pdf")
plt.show()

heatmap, xedges, yedges = np.histogram2d(df_mono['PrimaryEnergy']*1000, df_mono['PrimaryPhononEdep'], bins=50)
#heatmap = heatmap / heatmap.sum()
#heatmap = heatmap / 5000000.0
heatmap = np.ma.masked_where(heatmap == 0, heatmap) 

plt.figure(figsize=(10, 6))
plt.scatter(df_mono['PrimaryEnergy']*1000, df_mono['PrimaryPhononEdep'], alpha=0.5, s=1)
plt.title('Primary Phonon Total Energy vs NR energy deposition')
plt.pcolormesh(xedges,yedges, heatmap.T, norm=LogNorm(), cmap="viridis")
plt.colorbar(label="Events")
plt.xlabel('Primary Nuclear Recoil Energy (keVnr)')
plt.ylabel('Primary Phonon Combined Energy (keV)')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_mono_primaryedepenergy.pdf")
plt.show()

heatmap, xedges, yedges = np.histogram2d(df['PrimaryEnergy']*1000, df['SecondaryPhononEdep'], bins=50)
#heatmap = heatmap / heatmap.sum()
#heatmap = heatmap / 5000000.0
heatmap = np.ma.masked_where(heatmap == 0, heatmap)

plt.figure(figsize=(10, 6))
plt.scatter(df['PrimaryEnergy']*1000, df['SecondaryPhononEdep'], alpha=0.5, s=1)
plt.title('Secondary Phonon Total Energy vs NR energy deposition')
plt.pcolormesh(xedges,yedges, heatmap.T, norm=LogNorm(), cmap="viridis")
plt.colorbar(label="Events")
plt.xlabel('Primary Nuclear Recoil Energy (keVnr)')
plt.ylabel('Secondary Phonon Combined Energy (keV)')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_spectrum_secondaryedepenergy.pdf")
plt.show()

heatmap, xedges, yedges = np.histogram2d(df_mono['PrimaryEnergy']*1000, df_mono['SecondaryPhononEdep'], bins=50)
#heatmap = heatmap / heatmap.sum()
#heatmap = heatmap / 5000000.0
heatmap = np.ma.masked_where(heatmap == 0, heatmap) 

plt.figure(figsize=(10, 6))
plt.scatter(df_mono['PrimaryEnergy']*1000, df_mono['SecondaryPhononEdep'], alpha=0.5, s=1)
plt.title('Secondary Phonon Total Energy vs NR energy deposition')
plt.pcolormesh(xedges,yedges, heatmap.T, norm=LogNorm(), cmap="viridis")
plt.colorbar(label="Events")
plt.xlabel('Primary Nuclear Recoil Energy (keVnr)')
plt.ylabel('Secondary Phonon Combined Energy (keV)')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_mono_secondaryedepenergy.pdf")
plt.show()

heatmap, xedges, yedges = np.histogram2d(df['PrimaryEnergy']*1000, df['PrimaryPhononEdep']/(df['PrimaryEnergy']*1000), bins=50)
#heatmap = heatmap / heatmap.sum()
#heatmap = heatmap / 5000000.0
heatmap = np.ma.masked_where(heatmap == 0, heatmap)

plt.figure(figsize=(10, 6))
plt.scatter(df['PrimaryEnergy']*1000, df['PrimaryPhononEdep']/(df['PrimaryEnergy']*1000), alpha=0.5, s=1)
plt.title('Primary Phonon Fractional Energy vs NR energy deposition')
plt.pcolormesh(xedges,yedges, heatmap.T, norm=LogNorm(), cmap="viridis")
plt.colorbar(label="Events")
plt.xlabel('Primary Nuclear Recoil Energy (keVnr)')
plt.ylabel('Fraction of total energy deposition')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_spectrum_fracprimaryphononenergy_vs_nredep.pdf")
plt.show()

heatmap, xedges, yedges = np.histogram2d(df_mono['PrimaryEnergy']*1000, df_mono['PrimaryPhononEdep']/(df_mono['PrimaryEnergy']*1000), bins=50)
#heatmap = heatmap / heatmap.sum()
#heatmap = heatmap / 5000000.0
heatmap = np.ma.masked_where(heatmap == 0, heatmap) 

plt.figure(figsize=(10, 6))
plt.scatter(df_mono['PrimaryEnergy']*1000, df_mono['PrimaryPhononEdep']/(df_mono['PrimaryEnergy']*1000), alpha=0.5, s=1)
plt.title('Primary Phonon Fractional Energy vs NR energy deposition')
plt.pcolormesh(xedges,yedges, heatmap.T, norm=LogNorm(), cmap="viridis")
plt.colorbar(label="Events")
plt.xlabel('Primary Nuclear Recoil Energy (keVnr)')
plt.ylabel('Fraction of total energy deposition')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_mono_fracprimaryphononedepenergy_vs_nredep.pdf")
plt.show()

print("Frequency of different phonon creation processes:")

df2['CreatorProcess'] = df2['CreatorProcess'].astype(str)
df3['CreatorProcess'] = df3['CreatorProcess'].astype(str)

mask_dc = df3['CreatorProcess'].str.contains('downConversion', case=False, na=False)
df3_full_dc = df3[mask_dc]

mask_luke = df3['CreatorProcess'].str.contains('LukeScattering', case=False, na=False)
df3_full_luke = df3[mask_luke]

GHz_to_meV = 4.13*1e-3

min = np.min(df3_full_dc['StartEnergy']*1e9/GHz_to_meV)
max = np.max(df3_full_dc['StartEnergy']*1e9/GHz_to_meV)
bins = np.logspace(np.log10(min), np.log10(max), num=50)

plt.figure(figsize=(10, 6))
plt.hist(df3_full_dc['StartEnergy']*1e9/GHz_to_meV, bins=bins, color='blue', alpha=0.7)
plt.title('Total number of Phonons produced through Downconversion')
plt.xlabel('Phonon Frequency (GHz)')
plt.ylabel('')
plt.yscale('log')
plt.xscale('log')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_spectrum_secondary_downconverted_phonon_frequency.pdf")
plt.show()

min = np.min(df3_full_luke['StartEnergy']*1e9/GHz_to_meV)
max = np.max(df3_full_luke['StartEnergy']*1e9/GHz_to_meV)
bins = np.logspace(np.log10(min), np.log10(max), num=50)

plt.figure(figsize=(10, 6))
plt.hist(df3_full_luke['StartEnergy']*1e9/GHz_to_meV, bins=bins, color='blue', alpha=0.7)
plt.title('Total number of Phonons produced through Luke Scattering')
plt.xlabel('Phonon Frequency (GHz)')
plt.ylabel('')
plt.yscale('log')
plt.xscale('log')
plt.grid()
plt.savefig("newLiNbO3params_CEvNS_spectrum_LukeScattering_phonon_frequency.pdf")
plt.show()

print("Done")



