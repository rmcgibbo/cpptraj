#ifndef INC_ATOMMASK_H
#define INC_ATOMMASK_H
#include <string>
#include <vector>
#include "AmberParm.h"
// Class: AtomMask
/// Hold info on selected atoms based on mask expression.
/** AtomMask is used to hold an array of integers that represent atom numbers
  * of atoms selected based on a mask string. This class is actually an 
  * interface to the ptraj mask parser written by Viktor Hornak (PtrajMask.cpp).
  * Takes as input a string and an AmberParm class since the basic parser 
  * requires access to ipres, atom names, etc.
  * First the mask string is set via SetMaskString. Then the actual mask can
  * be set up in two ways.
  * 1) Integer mask
  *    Although an array of ints becomes larger than a simple character mask 
  *    once more than 25% of the system is selected, it tends to be faster 
  *    than the character array up until about 80% of the system is selected, 
  *    at which point the speed is comparable. This is the default way to use
  *    AtomMask and is how most of the routines in the Frame class have been
  *    written to use AtomMask.
  * 2) Character mask
  *    This is the original way to use the atom mask, useful e.g. when 
  *    you need to know what atoms are not selected as well as what atoms
  *    are selected. Unlike the integer mask, the character mask is not
  *    directly accessible by outside routines, only by the AtomInCharMask
  *    routine.
  */
// *** NOTE ***
// invertMask, AddAtom, AddAtoms, and AddAtomRange currently only apply
// to the Selected array.
class AtomMask {
    std::vector<char> CharMask; ///< Char array of atoms, T if selected, F if not.
    std::string Postfix;        ///< maskString tokenized and converted to RPN
    char maskChar;              ///< The character used to denote a selected atom (default 'T')
    std::string maskString;     ///< String specifying atom selection
  public:
    int Nselected;              ///< Number of selected atoms in mask
    std::vector<int> Selected;  ///< Int array of selected atom numbers, 1 for each selected atom
    std::vector<int> Selected3; ///< Int array of selected atom coordinate indices 

    const char *MaskString() { return maskString.c_str(); }

    AtomMask();
    ~AtomMask();
    AtomMask(const AtomMask &);
    AtomMask & operator=(const AtomMask&);

    void ResetMask();              ///< Reset atom mask
    void InvertMask();             ///< Switch char used to denote selected atoms (T->F, F->T)
    AtomMask *CopyMask();          ///< Return a copy of this mask

    void AddAtom(int);             ///< Add given atom to Selected array 
    void AddAtoms(std::vector<int>&); ///< Add a list of atoms to mask
    void AddAtomRange(int,int);    ///< Add minAtom <= atom < maxAtom to mask
    void PrintMaskAtoms(const char*); ///< Print all mask atoms in to a line
    bool None();                   ///< Return true if Nselected==0
    int SetMaskString(char*);      ///< Set the mask string. If NULL, set * (all)
    /// Set up Selected based on maskString and given parm
    int SetupMask(AmberParm*,double*,int);
    /// Set up CharMask based on maskString and given parm
    int SetupCharMask(AmberParm*,double*,int);
    /// True if given atom is T in CharMask
    bool AtomInCharMask(int atom);
 
};
#endif
