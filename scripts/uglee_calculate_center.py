import sys
from rdkit import Chem

def get_first_atom_coordinates(filename):
    # Attempt to read the file as an SDF by default; adjust this according to your file format
    suppl = Chem.SDMolSupplier(filename)
    for mol in suppl:
        if mol is None:
            continue  # Skip molecules that couldn't be read
        conf = mol.GetConformer()
        if conf.GetNumAtoms() > 0:
            first_atom = conf.GetAtomPosition(0)
            # Print coordinates separated by spaces
            print(f"{first_atom.x} {first_atom.y} {first_atom.z}")
            return  # Stop after processing the first valid molecule
    raise ValueError("No valid molecules found in the file or file could not be read.")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        # Optionally, provide an error message in a way that fits your use case
        print("Error: Incorrect usage. Please provide a filename.", file=sys.stderr)
        sys.exit(1)
    
    filename = sys.argv[1]
    try:
        get_first_atom_coordinates(filename)
    except Exception as e:
        # Optionally print the error to stderr if needed for debugging
        print(str(e), file=sys.stderr)
        sys.exit(1)

