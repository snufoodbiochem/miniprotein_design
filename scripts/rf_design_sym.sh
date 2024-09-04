#!/bin/bash
# Here, we're designing binders to insulin receptor, without specifying the topology of the binder a prior
# We first provide the output path and input pdb of the target protein (insulin receptor)
# We then describe the protein we want with the contig input:
#   - residues 1-150 of the A chain of the target protein
#   - a chainbreak (as we don't want the binder fused to the target!)
#   - A 70-100 residue binder to be diffused (the exact length is sampled each iteration of diffusion)
# We tell diffusion to target three specific residues on the target, specifically residues 59, 83 and 91 of the A chain
# We make 10 designs, and reduce the noise added during inference to 0, to improve the quality of the designs
export PATH="/home/hnc/miniconda3/bin:/home/hnc/.local/bin:$PATH"
source activate SE3nv
mv  scaffolds scaffolds_old
mkdir scaffolds


echo "inference.symmetery="C2", "C3", "C4", "C5", "C6", ..., dihedral, or tetrahedral"
echo "The configmap.contigs length refers to the total length of your oligomer. Therefore, it must be divisible by n chains."


python3 /home/hnc/RFdiffusion/scripts/run_inference.py \
--config-name=symmetry \
inference.symmetry="C4" \
inference.num_designs=10 \
inference.output_prefix="scaffolds/C6" \
'potentials.guiding_potentials=["type:olig_contacts,weight_intra:1,weight_inter:0.1"]' \
potentials.olig_intra_all=True \
potentials.olig_inter_all=True \
potentials.guide_scale=2.0 \
potentials.guide_decay="quadratic" \
'contigmap.contigs=[360-360]'



