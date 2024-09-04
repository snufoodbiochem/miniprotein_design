from iotbx import file_reader

def extract_sequence(pdb_path):
    pdb_input = file_reader.any_file(pdb_path, force_type="pdb").file_object
    hierarchy = pdb_input.construct_hierarchy()
    for model in hierarchy.models():
        for chain in model.chains():
            is_protein = any([rg.atom_groups()[0].resname in ['ALA', 'CYS', 'ASP', 'GLU', 'PHE', 'GLY', 'HIS', 'ILE', 'LYS', 'LEU', 'MET', 'ASN', 'PRO', 'GLN', 'ARG', 'SER', 'THR', 'VAL', 'TRP', 'TYR'] for rg in chain.residue_groups()])
            if is_protein:
                print(">Chain%s" % chain.id)
                print(chain.as_padded_sequence())

if __name__ == "__main__":
    import sys
    pdb_path = sys.argv[1]
    extract_sequence(pdb_path)

