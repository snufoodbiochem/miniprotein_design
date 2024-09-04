#!/bin/bash
# Here, we're designing binders to insulin receptor, without specifying the topology of the binder a prior
# We first provide the output path and input pdb of the target protein (insulin receptor)
# We then describe the protein we want with the contig input:
#   - residues 1-150 of the A chain of the target protein
#   - a chainbreak (as we don't want the binder fused to the target!)
#   - A 70-100 residue binder to be diffused (the exact length is sampled each iteration of diffusion)
# We tell diffusion to target three specific residues on the target, specifically residues 59, 83 and 91 of the A chain
# We make 10 designs, and reduce the noise added during inference to 0, to improve the quality of the designs
echo "Receptor pdb should have A in the chain name."
export PATH="/home/hnc/miniconda3/bin:/home/hnc/.local/bin:$PATH"
source activate SE3nv
mv  rfpdbs rfpdbs_old
mkdir rfpdbs
#aanumber_of_receptor="A1-150"
#aanumber_of="50-70"
receptor_pdb="receptor.pdb"
#contigs="'contigmap.contigs=[A1-150/0 50-70]'"
#hotspots="'ppi.hotspots_res=[A39,A65]'"
num_of_designed_binders=100

python3 /home/hnc/RFdiffusion/scripts/run_inference.py \
	inference.output_prefix=./rfpdbs/rf \
	inference.input_pdb=$receptor_pdb \
	'contigmap.contigs=[A1-168/0 50-60]'   \
	'ppi.hotspot_res=[A86,A100,A102,A141]'      \
	inference.num_designs=${num_of_designed_binders} \
	denoiser.noise_scale_ca=0 \
	denoiser.noise_scale_frame=0

cd rfpdbs
rm -f *.trb
rm -rf traj
ls *.pdb > pdblist

echo ../${receptor_pdb} > inputlist
grep -v " A " rf_0.pdb | grep ATOM  > polygly.pdb
echo polygly.pdb >> inputlist
mTM-align -i inputlist -o moved_receptor.pdb
cat mTM_result/moved_receptor.pdb | grep -v " A " | grep ATOM >receptor.pdb

for g in $(sed -n '=' pdblist)
do
        complexname=`head -$g pdblist | tail -1`
        echo $complexname
	grep " A " $complexname > rf.pdb_temp
       
	cat rf.pdb_temp | sed 's/ A / X /g' > X.pdb
	cat receptor.pdb >> X.pdb
	#rm rf.pdb_temp
	cp X.pdb $g_$complexname
	echo "$g_$complexname was created"
done

rm rf_*
rm X.pdb
rm polygly.pdb
mv receptor.pdb ..
rm *temp
rm -rf mTM_result
rm -rf traj
rm *list

cd ..
echo "rf_[rf number].pdb in rfpdbs"


