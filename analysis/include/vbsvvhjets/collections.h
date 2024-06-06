#ifndef VBSVVHJETS_COLLECTIONS_H
#define VBSVVHJETS_COLLECTIONS_H

// RAPIDO
#include "arbol.h"
#include "looper.h"
#include "cutflow.h"
#include "utilities.h"          // Utilities::Variables
// VBS
#include "core/collections.h"   // Core::Core::Analysis, Core::Skimmer
#include "core/cuts.h"
#include "vbswh/collections.h"
#include "vbswh/cuts.h"
#include "vbsvvhjets/cuts.h"
#include "corrections/all.h"    // PileUpSFs, LeptonSFsTTH/PKU, BTagSFs, JetEnergyScales

namespace VBSVVHJets
{

struct Analysis : Core::Analysis
{
    JetEnergyScales* jes;
    LeptonSFsPKU* lep_sfs;
    HLT1LepSFs* hlt_sfs;
    BTagSFs* btag_sfs;
    PileUpSFs* pu_sfs;
    PileUpJetIDSFs* puid_sfs;
    VBSVVHXbbSFs* xbb_sfs;
    VBSVVHXWqqSFs* xwqq_sfs;
    TString file_name;
    bool all_corrections;

    Analysis(Arbol& arbol_ref, Nano& nt_ref, HEPCLI& cli_ref, Cutflow& cutflow_ref) 
    : Core::Analysis(arbol_ref, nt_ref, cli_ref, cutflow_ref)
    {
        // W/Z fat jet globals
        cutflow.globals.newVar<LorentzVector>("ld_vqqfatjet_p4");
        cutflow.globals.newVar<LorentzVector>("tr_vqqfatjet_p4");
        cutflow.globals.newVar<unsigned int>("ld_vqqfatjet_gidx", 999); // idx in 'good' fatjets global vector
        cutflow.globals.newVar<unsigned int>("tr_vqqfatjet_gidx", 999); // idx in 'good' fatjets global vector
        // W/Z AK4 jet globals
        cutflow.globals.newVar<LorentzVector>("ld_vqqjet_p4");
        cutflow.globals.newVar<LorentzVector>("tr_vqqjet_p4");
        // Hbb jet globals
        cutflow.globals.newVar<LorentzVector>("hbbfatjet_p4");
        cutflow.globals.newVar<unsigned int>("hbbfatjet_gidx", 999); // idx in 'good' fatjets global vector
        // Vqq jet globals
        cutflow.globals.newVar<unsigned int>("ld_vqqjet_gidx", 999);
        cutflow.globals.newVar<unsigned int>("tr_vqqjet_gidx", 999);

        // Scale factors
        jes = nullptr;
        lep_sfs = nullptr;
        hlt_sfs = nullptr;
        btag_sfs = nullptr;
        pu_sfs = nullptr;
        puid_sfs = nullptr;
        xbb_sfs = nullptr;
        xwqq_sfs = nullptr;
        all_corrections = false;
        file_name = cli.input_tchain->GetCurrentFile()->GetName();
    };

    virtual void initBranches()
    {
        Core::Analysis::initBranches();
        // W/Z fat jet branches
        arbol.newBranch<double>("ld_vqqfatjet_xwqq", -999);
        arbol.newBranch<double>("ld_vqqfatjet_xvqq", -999);
        arbol.newBranch<double>("ld_vqqfatjet_pt", -999);
        arbol.newBranch<double>("ld_vqqfatjet_eta", -999);
        arbol.newBranch<double>("ld_vqqfatjet_phi", -999);
        arbol.newBranch<double>("ld_vqqfatjet_mass", -999);
        arbol.newBranch<double>("ld_vqqfatjet_msoftdrop", -999);
        arbol.newBranch<double>("tr_vqqfatjet_xwqq", -999);
        arbol.newBranch<double>("tr_vqqfatjet_xvqq", -999);
        arbol.newBranch<double>("tr_vqqfatjet_pt", -999);
        arbol.newBranch<double>("tr_vqqfatjet_eta", -999);
        arbol.newBranch<double>("tr_vqqfatjet_phi", -999);
        arbol.newBranch<double>("tr_vqqfatjet_mass", -999);
        arbol.newBranch<double>("tr_vqqfatjet_msoftdrop", -999);
        // W/Z AK4 jet branches
        arbol.newBranch<double>("ld_vqqjet_qgl", -999);
        arbol.newBranch<double>("ld_vqqjet_pt", -999);
        arbol.newBranch<double>("ld_vqqjet_eta", -999);
        arbol.newBranch<double>("ld_vqqjet_phi", -999);
        arbol.newBranch<double>("ld_vqqjet_mass", -999);
        arbol.newBranch<double>("tr_vqqjet_qgl", -999);
        arbol.newBranch<double>("tr_vqqjet_pt", -999);
        arbol.newBranch<double>("tr_vqqjet_eta", -999);
        arbol.newBranch<double>("tr_vqqjet_phi", -999);
        arbol.newBranch<double>("tr_vqqjet_mass", -999);
        arbol.newBranch<double>("vqqjets_Mjj", -999);
        arbol.newBranch<double>("vqqjets_dR", -999);
        // Hbb fat jet branches
        arbol.newBranch<double>("hbbfatjet_xbb", -999);
        arbol.newBranch<double>("hbbfatjet_pt", -999);
        arbol.newBranch<double>("hbbfatjet_eta", -999);
        arbol.newBranch<double>("hbbfatjet_phi", -999);
        arbol.newBranch<double>("hbbfatjet_mass", -999);
        arbol.newBranch<double>("hbbfatjet_msoftdrop", -999);
        // Other branches
        arbol.newBranch<double>("ST", -999);
        arbol.newBranch<bool>("passes_bveto", false);
        arbol.newBranch<bool>("is_allmerged", false);
        arbol.newBranch<bool>("is_semimerged", false);
        arbol.newBranch<double>("M_VVH", -999);
        arbol.newBranch<double>("VVH_pt", -999);
        arbol.newBranch<double>("VVH_eta", -999);
        arbol.newBranch<double>("VVH_phi", -999);
        arbol.newBranch<double>("alphaS_up", -999);
        arbol.newBranch<double>("alphaS_dn", -999);
        arbol.newBranch<double>("xbb_sf", 1.);
        arbol.newBranch<double>("xbb_sf_up", 1.);
        arbol.newBranch<double>("xbb_sf_dn", 1.);
        arbol.newBranch<double>("xwqq_ld_vqq_sf", 1.);
        arbol.newBranch<double>("xwqq_ld_vqq_sf_up", 1.);
        arbol.newBranch<double>("xwqq_ld_vqq_sf_dn", 1.);
        arbol.newBranch<double>("xwqq_tr_vqq_sf", 1.);
        arbol.newBranch<double>("xwqq_tr_vqq_sf_up", 1.);
        arbol.newBranch<double>("xwqq_tr_vqq_sf_dn", 1.);
        arbol.newBranch<float>("abcdnet_score", -999);
    };

    virtual void initCorrections()
    {
        jes = new JetEnergyScales(cli.variation);
        pu_sfs = new PileUpSFs();
        puid_sfs = new PileUpJetIDSFs();
        xbb_sfs = new VBSVVHXbbSFs();
        xwqq_sfs = new VBSVVHXWqqSFs();
        all_corrections = true;
    };

    virtual void initCutflow()
    {
        // Bookkeeping
        Cut* bookkeeping = new Core::Bookkeeping("Bookkeeping", *this, pu_sfs);
        cutflow.setRoot(bookkeeping);

        // Cut to make jet selection consistent in cutflow tables...
        Cut* jet_pt = new LambdaCut(
            "Geq2JetsPtGt30_FatJetHLTPlataeuCut",
            [&]()
            {
                int n_jets = 0;
                for (auto jet_pt : nt.Jet_pt())
                {
                    if (jet_pt > 30) { n_jets++; }
                }

                double max_fatjet_pt = 0;
                for (auto fatjet_pt : nt.FatJet_pt())
                {
                    if (fatjet_pt > max_fatjet_pt) { max_fatjet_pt = fatjet_pt; }
                }

                return (n_jets >= 2 && max_fatjet_pt > 550);
            }
        );
        cutflow.insert(bookkeeping, jet_pt, Right);

        // Save LHE mu_R and mu_F scale weights
        Cut* save_lhe = new Core::SaveSystWeights("SaveSystWeights", *this);
        cutflow.insert(jet_pt, save_lhe, Right);

        // Event filters
        Cut* event_filters = new VBSWH::PassesEventFilters("PassesEventFilters", *this);
        cutflow.insert(save_lhe, event_filters, Right);

        // HT triggers
        Cut* ht_triggers = new PassesTriggers("PassesTriggers", *this);
        cutflow.insert(event_filters, ht_triggers, Right);

        // Lepton selection
        Cut* select_leps = new Core::SelectLeptons("SelectLeptons", *this);
        cutflow.insert(ht_triggers, select_leps, Right);

        // Lepton veto
        Cut* no_leps = new LambdaCut(
            "NoLeptons", 
            [&]() 
            { 
                return cutflow.globals.getVal<LorentzVectors>("veto_lep_p4s").size() == 0; 
            }
        );
        cutflow.insert(select_leps, no_leps, Right);

        // Fat jet selection
        Cut* select_fatjets = new Core::SelectFatJets("SelectFatJets", *this, jes);
        cutflow.insert(no_leps, select_fatjets, Right);

        Cut* trigger_plateau = new LambdaCut( // Delete if jet_pt Cut works
            "TriggerPlateauCuts",
            [&]()
            {
                LorentzVectors fatjet_p4s = cutflow.globals.getVal<LorentzVectors>("good_fatjet_p4s");
                double max_fatjet_pt = -999;
                for (auto fatjet_p4 : fatjet_p4s)
                {
                    if (fatjet_p4.pt() > max_fatjet_pt) { max_fatjet_pt = fatjet_p4.pt(); }
                }
                return max_fatjet_pt > 550;
            }
        );
        cutflow.insert(select_fatjets, trigger_plateau, Right); // Delete if jet_pt Cut works

        /* ------------------ 3 fatjet channel ------------------ */
        Cut* geq3_fatjets = new LambdaCut(
            "Geq3FatJets", [&]() { return arbol.getLeaf<int>("n_fatjets") >= 3; }
        );
        // cutflow.insert(select_fatjets, geq3_fatjets, Right);
        cutflow.insert(trigger_plateau, geq3_fatjets, Right); // Delete if jet_pt Cut works

        // VVH fat jet candidate selection
        Cut* allmerged_select_vvh = new SelectVVHFatJets("AllMerged_SelectVVHFatJets", *this, AllMerged);
        cutflow.insert(geq3_fatjets, allmerged_select_vvh, Right);

        // Jet selection
        Cut* allmerged_select_jets = new SelectJetsNoFatJetOverlap("AllMerged_SelectJets", *this, AllMerged, jes, btag_sfs, puid_sfs);
        cutflow.insert(allmerged_select_vvh, allmerged_select_jets, Right);

        // VBS jet selection
        Cut* allmerged_select_vbsjets = new SelectVBSJets("AllMerged_SelectVBSJets", *this, AllMerged);
        cutflow.insert(allmerged_select_jets, allmerged_select_vbsjets, Right);

        // Save analysis variables
        Cut* allmerged_save_vars = new SaveVariables("AllMerged_SaveVariables", *this, AllMerged, xbb_sfs, xwqq_sfs);
        cutflow.insert(allmerged_select_vbsjets, allmerged_save_vars, Right);

        // Preselection
        Cut* allmerged_presel = new LambdaCut(
            "AllMerged_Preselection", 
            [&]() 
            { 
                return (
                    arbol.getLeaf<double>("hbbfatjet_xbb") > 0.5
                    && arbol.getLeaf<double>("ld_vqqfatjet_xwqq") > 0.3
                    && arbol.getLeaf<double>("tr_vqqfatjet_xwqq") > 0.3
                );
            },
            [&]()
            {
                /* Because we do the ParticleNet "resampling" for QCD events, we would like to
                 * normalize the QCD MC integral to [data - (nonQCD MC)], but we can't calculate 
                 * this scale factor here, since this code runs file-by-file.
                 *
                 * This isn't super important, though, because we recompute this factor every time 
                 * that we run any of the analysis steps after the looper (e.g. plotting). Thus, 
                 * since this is only to make the cutflow match the numbers in the plots, we just 
                 * copy the number computed in e.g. the plot-making script here. That means that 
                 * this number can get outdated, so update it when necessary!
                 */
                return (file_name.Contains("QCD") ? 1.2289449051788426 : 1.0);
            }
        );
        cutflow.insert(allmerged_save_vars, allmerged_presel, Right);

        Cut* allmerged_save_abcdnet = new LambdaCut(
            "AllMerged_SaveABCDNetScore",
            [&]()
            {
                // Save ABCDNet score
                float score =  ABCDNetAllMerged::run(
                    arbol.getLeaf<double>("hbbfatjet_pt"),
                    arbol.getLeaf<double>("hbbfatjet_eta"),
                    arbol.getLeaf<double>("hbbfatjet_phi"),
                    arbol.getLeaf<double>("hbbfatjet_mass"),
                    arbol.getLeaf<double>("ld_vqqfatjet_pt"),
                    arbol.getLeaf<double>("ld_vqqfatjet_eta"),
                    arbol.getLeaf<double>("ld_vqqfatjet_phi"),
                    arbol.getLeaf<double>("ld_vqqfatjet_mass"),
                    arbol.getLeaf<double>("tr_vqqfatjet_pt"),
                    arbol.getLeaf<double>("tr_vqqfatjet_eta"),
                    arbol.getLeaf<double>("tr_vqqfatjet_phi"),
                    arbol.getLeaf<double>("tr_vqqfatjet_mass"),
                    arbol.getLeaf<double>("M_jj")
                );
                arbol.setLeaf<float>("abcdnet_score", score);
                return true;
            }
        );
        cutflow.insert(allmerged_presel, allmerged_save_abcdnet, Right);
        
        // Signal region ParticleNet cuts
        Cut* allmerged_xbb_cut = new LambdaCut(
            "AllMerged_XbbGt0p9", 
            [&]() { return arbol.getLeaf<double>("hbbfatjet_xbb") > 0.8; },
            [&]() { return arbol.getLeaf<double>("xbb_sf"); }
        );
        cutflow.insert(allmerged_save_abcdnet, allmerged_xbb_cut, Right);

        Cut* allmerged_xwqq_cuts = new LambdaCut(
            "AllMerged_XWqqCuts", 
            [&]() 
            { 
                return (
                    arbol.getLeaf<double>("ld_vqqfatjet_xwqq") > 0.8
                    && arbol.getLeaf<double>("tr_vqqfatjet_xwqq") > 0.7
                ); 
            },
            [&]() 
            { 
                return (
                    arbol.getLeaf<double>("xwqq_ld_vqq_sf")
                    *arbol.getLeaf<double>("xwqq_tr_vqq_sf")
                );
            }
        );
        cutflow.insert(allmerged_xbb_cut, allmerged_xwqq_cuts, Right);

        Cut* allmerged_A = new ABCDRegions("AllMerged_RegionA", *this, "A", AllMerged);
        cutflow.insert(allmerged_xwqq_cuts, allmerged_A, Right);

        Cut* allmerged_B = new ABCDRegions("AllMerged_RegionB", *this, "B", AllMerged);
        cutflow.insert(allmerged_A, allmerged_B, Left);

        Cut* allmerged_C = new ABCDRegions("AllMerged_RegionC", *this, "C", AllMerged);
        cutflow.insert(allmerged_B, allmerged_C, Left);

        Cut* allmerged_D = new ABCDRegions("AllMerged_RegionD", *this, "D", AllMerged);
        cutflow.insert(allmerged_C, allmerged_D, Left);
        /* ------------------------------------------------------ */

        /* ------------------ 2 fatjet channel ------------------ */
        Cut* exactly2_fatjets = new LambdaCut(
            "Exactly2FatJets", [&]() { return arbol.getLeaf<int>("n_fatjets") == 2; }
        );
        cutflow.insert(geq3_fatjets, exactly2_fatjets, Left);

        // VVH fat jet candidate selection
        Cut* semimerged_select_vvh = new SelectVVHFatJets("SemiMerged_SelectVVHFatJets", *this, SemiMerged);
        cutflow.insert(exactly2_fatjets, semimerged_select_vvh, Right);

        // Jet selection
        Cut* semimerged_select_jets = new SelectJetsNoFatJetOverlap("SemiMerged_SelectJets", *this, SemiMerged, jes, btag_sfs, puid_sfs);
        cutflow.insert(semimerged_select_vvh, semimerged_select_jets, Right);

        // N jets >= 4 (2 VBS + V --> qq)
        Cut* semimerged_geq4_jets = new LambdaCut(
            "SemiMerged_Geq4Jets", [&]() { return arbol.getLeaf<int>("n_jets") >= 4; }
        );
        cutflow.insert(semimerged_select_jets, semimerged_geq4_jets, Right);

        // V --> qq jet candidate selection
        Cut* semimerged_select_vjets = new SelectVJets("SemiMerged_SelectVJets", *this);
        cutflow.insert(semimerged_geq4_jets, semimerged_select_vjets, Right);

        // VBS jet selection
        Cut* semimerged_select_vbsjets = new SelectVBSJets("SemiMerged_SelectVBSJets", *this, SemiMerged);
        cutflow.insert(semimerged_select_vjets, semimerged_select_vbsjets, Right);

        // Save analysis variables
        Cut* semimerged_save_vars = new SaveVariables("SemiMerged_SaveVariables", *this, SemiMerged, xbb_sfs, xwqq_sfs);
        cutflow.insert(semimerged_select_vbsjets, semimerged_save_vars, Right);

        // Preselection
        Cut* semimerged_presel = new LambdaCut(
            "SemiMerged_Preselection", 
            [&]() 
            { 
                return (
                    arbol.getLeaf<double>("hbbfatjet_xbb") > 0.8
                    && arbol.getLeaf<double>("ld_vqqfatjet_xwqq") > 0.6
                    && arbol.getLeaf<double>("ld_vqqfatjet_xvqq") > 0.6
                );
            }
        );
        cutflow.insert(semimerged_save_vars, semimerged_presel, Right);

        Cut* semimerged_save_abcdnet = new LambdaCut(
            "SemiMerged_SaveABCDNetScore",
            [&]()
            {
                // Save ABCDNet score
                float score =  ABCDNetSemiMerged::run(
                    arbol.getLeaf<double>("hbbfatjet_pt"),
                    arbol.getLeaf<double>("hbbfatjet_eta"),
                    arbol.getLeaf<double>("hbbfatjet_phi"),
                    arbol.getLeaf<double>("hbbfatjet_msoftdrop"),
                    arbol.getLeaf<double>("ld_vqqfatjet_pt"),
                    arbol.getLeaf<double>("ld_vqqfatjet_eta"),
                    arbol.getLeaf<double>("ld_vqqfatjet_phi"),
                    arbol.getLeaf<double>("ld_vqqfatjet_msoftdrop"),
                    arbol.getLeaf<double>("ld_vqqjet_pt"),
                    arbol.getLeaf<double>("ld_vqqjet_eta"),
                    arbol.getLeaf<double>("ld_vqqjet_phi"),
                    arbol.getLeaf<double>("ld_vqqjet_mass"),
                    arbol.getLeaf<double>("tr_vqqjet_pt"),
                    arbol.getLeaf<double>("tr_vqqjet_eta"),
                    arbol.getLeaf<double>("tr_vqqjet_phi"),
                    arbol.getLeaf<double>("tr_vqqjet_mass"),
                    arbol.getLeaf<double>("ST"),
                    arbol.getLeaf<double>("vqqjets_Mjj"),
                    arbol.getLeaf<double>("HT"),
                    arbol.getLeaf<int>("n_jets"),
                    arbol.getLeaf<double>("ld_vqqfatjet_xwqq")
                );
                arbol.setLeaf<float>("abcdnet_score", score);
                return true;
            }
        );
        cutflow.insert(semimerged_presel, semimerged_save_abcdnet, Right);
        
        Cut* semimerged_A = new ABCDRegions("SemiMerged_RegionA", *this, "A", SemiMerged);
        cutflow.insert(semimerged_save_abcdnet, semimerged_A, Right);

        Cut* semimerged_B = new ABCDRegions("SemiMerged_RegionB", *this, "B", SemiMerged);
        cutflow.insert(semimerged_A, semimerged_B, Left);

        Cut* semimerged_C = new ABCDRegions("SemiMerged_RegionC", *this, "C", SemiMerged);
        cutflow.insert(semimerged_B, semimerged_C, Left);

        Cut* semimerged_D = new ABCDRegions("SemiMerged_RegionD", *this, "D", SemiMerged);
        cutflow.insert(semimerged_C, semimerged_D, Left);
        /* ------------------------------------------------------ */
    };

    virtual void init()
    {
        Core::Analysis::init();
        if (all_corrections)
        {
            jes->init(file_name);
            pu_sfs->init(file_name);
            puid_sfs->init(file_name);
            xbb_sfs->init(file_name);
            xwqq_sfs->init(file_name);
        }
    };
};

} // End namespace VBSVVHJets;

#endif
