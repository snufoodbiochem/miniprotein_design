import sys
import os
from rdkit import Chem
from Bio.PDB import PDBParser, PDBIO
from Bio.PDB.StructureBuilder import PDBConstructionWarning
import warnings
import numpy as np

def main(pdb_path, sdf_path):
    # Suppress specific Biopython warnings about PDB construction issues
    warnings.simplefilter('ignore', PDBConstructionWarning)

    # Load the protein structure
    pdb_parser = PDBParser(QUIET=True)
    protein = pdb_parser.get_structure('Protein', pdb_path)

    # Load the ligand from SDF
    sdf_reader = Chem.SDMolSupplier(sdf_path, sanitize=True)
    ligand = sdf_reader[0]  # Assuming there's at least one ligand in the SDF

    # Compute centroid of the protein
    atom_coords = np.array([atom.get_coord() for atom in protein.get_atoms() if atom.get_name() == 'CA'])  # CA for simplicity
    centroid = atom_coords.mean(axis=0)

    # Compute centroid of the ligand (as a simple numpy array mean calculation)
    ligand_coords = np.array(ligand.GetConformer().GetPositions())
    ligand_centroid = ligand_coords.mean(axis=0)

    # Translate ligand to the protein's centroid
    translation_vector = centroid - ligand_centroid
    for i in range(ligand.GetNumAtoms()):
        x, y, z = ligand.GetConformer().GetAtomPosition(i)
        ligand.GetConformer().SetAtomPosition(i, (x + translation_vector[0], y + translation_vector[1], z + translation_vector[2]))

    # Temporarily save the protein structure
    temp_protein_file = 'complex_protein_only.pdb'
    io = PDBIO()
    io.set_structure(protein)
    io.save(temp_protein_file)

    # RDKit to PDB format (simple conversion, assumes no bonds broken in translation)
    ligand_pdbblock = Chem.MolToPDBBlock(ligand)
    output_file_name = f"{os.path.splitext(os.path.basename(pdb_path))[0]}_{os.path.splitext(os.path.basename(sdf_path))[0]}.pdb"
    
    with open(output_file_name, 'w') as f:
        with open(temp_protein_file, 'r') as prot_f:
            f.write(prot_f.read())
        f.write(ligand_pdbblock)

    # Remove the temporary protein file
    os.remove(temp_protein_file)

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print("Usage: python script.py <receptor_pdb> <ligand_sdf>")
        sys.exit(1)
    main(sys.argv[1], sys.argv[2])

