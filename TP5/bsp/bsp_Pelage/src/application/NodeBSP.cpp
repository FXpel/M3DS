#include "NodeBSP.h"
#include <string>

/**
* @author Fabrice Aubert
*/

using namespace std;
using namespace p3d;



/** ************************************************************** */

NodeBSP::~NodeBSP() {
  if (negative_) delete negative_;
  if (positive_) delete positive_;
}

NodeBSP::NodeBSP() {}

/// constructeur en affectant chacun des champs
NodeBSP::NodeBSP(const FaceBSP &f,NodeBSP *neg,NodeBSP *pos) : negative_{neg},positive_{pos},face_{f} {
}

/// affecte la facette pivot avec f
void NodeBSP::face(const FaceBSP &f) {
  face_=f;
}

/// affecte le noeud gauche
void NodeBSP::negative(NodeBSP *neg) {
  negative_=neg;
}

/// affecte le noeud droit
void NodeBSP::positive(NodeBSP *pos) {
  positive_=pos;
}

/// donne la facette du noeud
const FaceBSP &NodeBSP::face() const {
  return face_;
}

/// donne le sous arbre gauche
NodeBSP *NodeBSP::negative() const {
  return negative_;
}

/// donne le sous arbre droit
NodeBSP *NodeBSP::positive() const {
  return positive_;
}

/// nombre de faces
size_t NodeBSP::nbFace() {
  return 1+ (positive()?positive()->nbFace():0) + (negative()?negative()->nbFace():0);
}



