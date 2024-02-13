import urllib.request
import gzip
import shutil
import os

# Vérifier si la bibliothèque RDKit est disponible
try:
    from rdkit import Chem
except ImportError:
    # Si la bibliothèque n'est pas disponible, télécharger et installer
    print("Téléchargement de la bibliothèque RDKit...")
    os.system("pip install rdkit")
    from rdkit import Chem

def get_transition_matrix(mol):
    """Generate the transition matrix for a molecule, ignoring hydrogen atoms."""
    atoms = [atom for atom in mol.GetAtoms() if atom.GetAtomicNum() != 1]  # Exclude hydrogens
    matrix_size = len(atoms)
    transition_matrix = [[0] * matrix_size for _ in range(matrix_size)]

    atom_index = {atom.GetIdx(): i for i, atom in enumerate(atoms)}  # Map from atom index to reduced index

    for bond in mol.GetBonds():
        begin_atom = bond.GetBeginAtom()
        end_atom = bond.GetEndAtom()

        if begin_atom.GetAtomicNum() != 1 and end_atom.GetAtomicNum() != 1:  # Exclude bonds involving hydrogen
            begin_index = atom_index[begin_atom.GetIdx()]
            end_index = atom_index[end_atom.GetIdx()]
            transition_matrix[begin_index][end_index] = 1
            transition_matrix[end_index][begin_index] = 1  # Assuming undirected graph for simplicity

    return transition_matrix

url = 'https://ftp.ebi.ac.uk/pub/databases/chebi/SDF/ChEBI_lite_3star.sdf.gz'
downloaded_file_name = 'ChEBI_lite_3star.sdf.gz'
urllib.request.urlretrieve(url, downloaded_file_name)
unzipped_file_name = 'ChEBI_lite_3star.sdf'

with gzip.open(downloaded_file_name, 'rb') as f_in, open(unzipped_file_name, 'wb') as f_out:
    shutil.copyfileobj(f_in, f_out)

os.remove(downloaded_file_name)

sdf_supplier = Chem.SDMolSupplier(unzipped_file_name)
output_directory = 'data'
os.makedirs(output_directory, exist_ok=True)

# Fonction pour supprimer les atomes ayant moins de 2 liaisons
def remove_single_bond_atoms(mol):
    atoms_to_remove = [1]
    while len(atoms_to_remove) > 0:
        atoms_to_remove = []
        for atom in mol.GetAtoms():
            if atom.GetDegree() <= 1:  # Si l'atome a moins de 2 liaisons
                atoms_to_remove.append(atom.GetIdx())
        # Suppression des atomes de la molécule
        atoms_to_remove.sort(reverse=True)  # Trie de manière décroissante pour éviter les problèmes d'index
        # Convertir la molécule en EditableMol pour permettre des modifications
        editable_mol = Chem.EditableMol(mol)
        for atom_idx in atoms_to_remove:
            editable_mol.RemoveAtom(atom_idx)  # Supprimer l'atome de la molécule
        mol = editable_mol.GetMol()
    
    return mol

for i, mol in enumerate(sdf_supplier):
    if mol is not None:
        ring_info = mol.GetRingInfo()
        
        if ring_info.NumRings() > 0:

            # Suppression des atomes ayant moins de 2 liaisons
            mol = remove_single_bond_atoms(mol)

            #if(mol.GetNumAtoms()>6):
            chebi_id = mol.GetProp("ChEBI ID")
            chebi_id = chebi_id.replace(':', '_')
            output_file_name = os.path.join(output_directory, f'{chebi_id}.txt')

            with open(output_file_name, 'w') as output_file:

                chebi_name = mol.GetProp("ChEBI Name")
                output_file.write(f'Molecule Name: {chebi_name}\n\n')
                # Generate the transition matrix for the molecule
                transition_matrix = get_transition_matrix(mol) #Chem.rdmolops.GetAdjacencyMatrix(mol)
                # Write the matrix to the file
                output_file.write(f"{len(transition_matrix)}\n")  # Write the size of the matrix
                for row in transition_matrix:
                    output_file.write(' '.join(map(str, row)) + "\n")

os.remove(unzipped_file_name)
