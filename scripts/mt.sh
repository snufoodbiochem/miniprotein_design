#!/bin/bash
export PATH="/home/hnc/.local/bin:$PATH"
echo "pydssp.sh [receptor+mini complex pdb] [round]"
grep ATOM $1 | grep " X "  >mini.pdb
grep ATOM $1 | grep -v " X " >receptor.pdb
pydssp -d cuda mini.pdb | awk '{print $1}' > ss
#number_ss=`cat ss | tr -d "-" | wc -m`
#total_number=`cat ss | wc -m`
#ss_ratio=`echo "$number_ss / $total_number" | bc -l`

pdb_tofasta mini.pdb | grep -v '>' | sed ':a;N;$!ba;s/\n//g' > fasta
