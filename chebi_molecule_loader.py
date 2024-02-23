import urllib.request
import gzip
import shutil
import os

#--- Fonctions ---#

def matrice_adjacence_triangulaire(mol) :
    """
    Génère la matrice d'adjacence triangulaire supérieure pour une molécule, 
    en ignorant les atomes d'hydrogènes.
    """

    atoms = [atom for atom in mol.GetAtoms() if atom.GetAtomicNum() != 1]  # Ignore les hydrogènes
    matrix_size = len(atoms)
    transition_matrix = [[0] * matrix_size for _ in range(matrix_size)]
    upper_triang_matrix = []

    atom_index = {atom.GetIdx(): i for i, atom in enumerate(atoms)}  # Fait correspondre atom index aux indexes réduits

    for bond in mol.GetBonds() :
        begin_atom = bond.GetBeginAtom()
        end_atom = bond.GetEndAtom()

        if begin_atom.GetAtomicNum() != 1 and end_atom.GetAtomicNum() != 1 :  # Ignore les liaisons avec des H
            begin_index = atom_index[begin_atom.GetIdx()]
            end_index = atom_index[end_atom.GetIdx()]
            transition_matrix[begin_index][end_index] = 1
            transition_matrix[end_index][begin_index] = 1

    for i in range(matrix_size) :
        row = [0] * (matrix_size - i)
        for j in range(matrix_size) :
            if j >= i :
                row[j - i] = transition_matrix[i][j]
        upper_triang_matrix.append(row)

    return upper_triang_matrix


def supprimer_atom_une_liaison(mol) :
    """
    Supprimer les atomes ayant moins de 2 liaisons.
    """

    atoms_to_remove = [1]
    while len(atoms_to_remove) > 0 :
        atoms_to_remove = []
        for atom in mol.GetAtoms() :
            if atom.GetDegree() <= 1 :  # Si l'atome a moins de 2 liaisons
                atoms_to_remove.append(atom.GetIdx())
        # Suppression des atomes de la molécule
        atoms_to_remove.sort(reverse=True)  # Trie de manière décroissante pour éviter les problèmes d'index
        # Convertir la molécule en EditableMol pour permettre des modifications
        editable_mol = Chem.EditableMol(mol)
        for atom_idx in atoms_to_remove:
            editable_mol.RemoveAtom(atom_idx)  # Supprimer l'atome de la molécule
        mol = editable_mol.GetMol()
    
    return mol

def non_connexe(mol) :

    frags = Chem.GetMolFrags(mol, asMols=True, sanitizeFrags=False)
    return len(frags) > 1

#--- Script ---#

# Vérifier si la bibliothèque RDKit est disponible
try:
    from rdkit import Chem

except ImportError :
    print("Téléchargement de la bibliothèque RDKit...")
    os.system("pip install rdkit")
    from rdkit import Chem

url = 'https://ftp.ebi.ac.uk/pub/databases/chebi/SDF/ChEBI_lite_3star.sdf.gz'
downloaded_file_name = 'ChEBI_lite_3star.sdf.gz'

urllib.request.urlretrieve(url, downloaded_file_name)
unzipped_file_name = 'ChEBI_lite_3star.sdf'

with gzip.open(downloaded_file_name, 'rb') as f_in, open(unzipped_file_name, 'wb') as f_out :
    shutil.copyfileobj(f_in, f_out)

os.remove(downloaded_file_name)

sdf_supplier = Chem.SDMolSupplier(unzipped_file_name)
output_directory = 'data'
os.makedirs(output_directory, exist_ok=True)

excluded = ['CHEBI:60153'] # Listes des molécules exclues car posent problèmes

for mol in sdf_supplier :

    if mol is not None :
        if mol.GetProp("ChEBI ID") in excluded :
                continue
        
        ring_info = mol.GetRingInfo()

        if ring_info.NumRings() > 0 :
            mol = supprimer_atom_une_liaison(mol)

            if non_connexe(mol):
                continue 

            transition_matrix = matrice_adjacence_triangulaire(mol)

            chebi_id = mol.GetProp("ChEBI ID")
            chebi_id = chebi_id.replace('CHEBI:', '')
            output_file_name = os.path.join(output_directory, f'{chebi_id}_{len(transition_matrix)}')

            with open(output_file_name, 'w') as output_file :

                # Taille de la molécule
                output_file.write(f"{len(transition_matrix)}\n")

                for row in transition_matrix:
                    output_file.write(' '.join(map(str, row)) + "\n")
                
                # Symboles des atomes
                atoms = mol.GetAtoms()
                output_file.write("\n")
                for atom in atoms:
                    atom_symbol = atom.GetSymbol()
                    output_file.write(f'{atom_symbol} ')

os.remove(unzipped_file_name)