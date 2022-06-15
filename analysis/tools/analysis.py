import uproot
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

def clip(np_array, bins):
    clip_low = 0.5 * (bins[0] + bins[1])
    clip_high = 0.5 * (bins[-2] + bins[-1])
    return np.clip(np_array, clip_low, clip_high)

class PandasAnalysis:
    def __init__(self, sig_root_files, bkg_root_files, ttree_name="Events", 
                 weight_columns=[]):
        dfs = []
        # Load signal
        for root_file in sig_root_files:
            name = root_file.split("/")[-1].replace(".root", "")
            with uproot.open(root_file) as f:
                df = f.get(ttree_name).arrays(library="pd")
                df["name"] = name
                df["is_signal"] = True
                dfs.append(df)
        # Load background
        for root_file in bkg_root_files:
            name = root_file.split("/")[-1].replace(".root", "")
            with uproot.open(root_file) as f:
                df = f.get(ttree_name).arrays(library="pd")
                df["name"] = name
                df["is_signal"] = False
                dfs.append(df)

        self.df = pd.concat(dfs)
        self.df["name"] = self.df.name.astype("category")

        if weight_columns:
            self.df["event_weight"] = self.df[weight_columns[0]]
            for weight_col in weight_columns[1:]:
                self.df["event_weight"] *= self.df[weight_col]

    def make_selection(self, selection):
        if selection and type(selection) == str:
            selection = self.df.eval(selection)
        self.df = self.df[selection].copy()

    def sig_df(self, selection=None):
        if not selection:
            return self.df[self.df.is_signal]
        else:
            if selection and type(selection) == str:
                selection = self.df.eval(selection)
            return self.df[self.df.is_signal & selection]

    def bkg_df(self, selection=None):
        if not selection:
            return self.df[~self.df.is_signal]
        else:
            if selection and type(selection) == str:
                selection = self.df.eval(selection)
            return self.df[~self.df.is_signal & selection]

    def get_event_counts(self, selection=None, raw=False):
        bkg_df = self.bkg_df(selection=selection)
        sig_df = self.sig_df(selection=selection)
        if raw:
            return len(sig_df), len(bkg_df)
        else:
            return sig_df.event_weight.sum(), bkg_df.event_weight.sum()

    def set_split_column(self, ratio=0.5, name="split"):
        self.df[name] = (np.random.rand(len(self.df)) < ratio)
        self.df[f"{name}_weight"] = self.df.event_weight
        for sample_name in self.df.name.unique():
            split_left = ((self.df.name == sample_name) & ~self.df[name])
            split_right = ((self.df.name == sample_name) & self.df[name])
            orig_integral = self.df[split_left | split_right].event_weight.sum()
            split_left_integral = self.df[split_left].event_weight.sum()
            split_right_integral = self.df[split_right].event_weight.sum()
            if orig_integral*split_left_integral*split_right_integral == 0.:
                self.df.loc[(split_left | split_right), f"{name}_weight"] *= 0.
            else:
                self.df.loc[split_left, f"{name}_weight"] *= orig_integral/split_left_integral
                self.df.loc[split_right, f"{name}_weight"] *= orig_integral/split_right_integral

    def plot_sig_vs_bkg(self, column, bins, selection="", abs=False, raw=False, 
                        xlabel="", logy=False):

        fig, axes = plt.subplots(figsize=(12, 9))

        bkg_df = self.bkg_df(selection=selection)
        sig_df = self.sig_df(selection=selection)

        if raw:
            bkg_weights, sig_weights = np.ones(len(bkg_df)), np.ones(len(sig_df))
        else:
            bkg_weights, sig_weights = bkg_df.event_weight, sig_df.event_weight

        bkg_data = bkg_df[column] if not abs else np.abs(bkg_df[column])
        axes.hist(
            clip(bkg_data, bins),
            bins=bins,
            weights=bkg_weights,
            alpha=0.5,
            label=f"total background [{sum(bkg_weights):0.1f} events]",
            log=logy
        )

        sig_data = sig_df[column] if not abs else np.abs(sig_df[column])
        axes.hist(
            clip(sig_data, bins),
            bins=bins,
            weights=sig_weights,
            histtype="step",
            color="r",
            label=f"total signal [{sum(sig_weights):0.1f} events]",
            log=logy
        )
        
        axes.set_xlabel(xlabel, size=18)
        axes.set_ylabel("Events", size=18)
        axes.legend(fontsize=16)

        return axes

    def fom_scan(self, variable, working_points, abs=False, operator=">", 
                 base_selection="", fom=lambda s, b: s/np.sqrt(b)):
        if abs:
            variable = f"abs({variable})"

        print("sel,sig,bkg,fom")
        for wp in working_points:
            if base_selection:
                sel = f"{base_selection} and {variable} {operator} {wp}"
            else:
                sel = f"{variable} {operator} {wp}"
                
            sig, bkg = self.get_event_counts(selection=sel)
            print(f"{sel},{sig},{bkg},{fom(sig, bkg)}")