import sys

try:
    import pymol
    from pymol import cmd, stored
except ImportError as e:
    print("PyMOL module is not available. Ensure PyMOL is properly installed as a Python module.")
    sys.exit(1)

def detect_steric_clashes(pdb_file):
    pymol.pymol_argv = ['pymol', '-qc']  # Quiet and without GUI
    pymol.finish_launching()
    cmd.load(pdb_file, "molecule")
    cmd.remove("solvent")

    chains = cmd.get_chains("molecule")
    clash_detected = False

    for chain in chains:
        other_chains = [c for c in chains if c != chain]
        if not other_chains:  # Skip if no other chains to compare
            continue
        cmd.select(f"chain_{chain}", f"molecule and chain {chain}")
        for other_chain in other_chains:
            cmd.select(f"other_chain_{other_chain}", f"molecule and chain {other_chain}")
            overlap_count = cmd.count_atoms(f"chain_{chain} within 2 of other_chain_{other_chain}")
            if overlap_count > 0:
                print(f"Overlap detected between chain {chain} and chain {other_chain}: {overlap_count} atoms within 2A")
                clash_detected = True

    cmd.delete("molecule")
    return clash_detected

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <pdb_file_path>")
        sys.exit(1)

    pdb_file = sys.argv[1]

    if detect_steric_clashes(pdb_file):
        print("Yes, steric clashes detected.")
    else:
        print("No steric clashes detected.")

