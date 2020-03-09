#pragma once

#include <vector>
#include <string>



/*!
*
* @file Simple OBJ Loader :
* all faces are in the array vertexIndexFace
* each face (type VertexFace) is an array of VertexAttrib (one VertexAttrib per vertex of the face).
* VertexAttrib contains 3 indices (position, normal, texCoord) that refers to the following arrays :
* - positionObj : array of all positions directly read from the file .obj (i.e. the v)
* - normalObj : array of all normals read from the file .obj (i.e. the vn)
* - texCoordObj : array of all texture coordinates from the file .obj (i.e. the t).
*
* @brief
* @author F. Aubert
*
*/

#include "Vector2.h"
#include "Vector3.h"

namespace p3d {
/*
 *
 * @class VertexAttrib contains all attributes of a vertex : each attribute is an index (refers to _positionMesh, _normalMesh and _texCoordMesh
 *
 */
class VertexAttrib {
public:
  size_t positionIndex_,normalIndex_,texCoordIndex_;
  VertexAttrib() : positionIndex_{0},normalIndex_{0},texCoordIndex_{0} {}
};

/*
 *
 * @class VertexAttribFace : each face contains the attributes of all vertices
 *
 */

typedef std::vector<VertexAttrib> VertexAttribFace;



class Mesh {
public:

    void read(const std::string &filename);
    void readInit(const std::string &resourceName, bool triangle=true);
    void scaleInBox(double left, double right, double top, double bottom,double znear,double zfar);
    void scaleInBoxRatio(double left, double right, double bottom, double top, double znear, double zfar);

    void check();
    void computeNormal();
    void computeTexCoord();
    void computeNormalFace();
    void computeNormalFace(size_t i);
    void triangulate();

    const Vector3 &positionMesh(size_t i) const;
    const Vector3 &normalMesh(size_t i) const;
    const Vector2 &texCoordMesh(size_t i) const;

    const Vector3 &normalFace(size_t i) const;

    const VertexAttrib &vertexAttrib(size_t i,int j) const;
    size_t indexPositionVertexFace(size_t i,int j) const;
    const Vector3 &positionVertexFace(size_t i,int j) const;
    const Vector3 &normalVertexFace(size_t i,int j) const;
    const Vector2 &texCoordVertexFace(size_t i,int j) const;

    size_t nbPosition() const;
    size_t nbNormal() const;
    size_t nbTexCoord() const;
    size_t nbVertexFace(size_t i) const;
    size_t nbFace() const;


    void rotateY(double angle);
    void addPositionMesh(const p3d::Vector3 &p);
    void addFaceMesh(const std::vector<int> &indexList);
    Mesh *clone();

protected:
    std::vector<p3d::Vector3> positionMesh_; //! x,y,z of a vertex (read from OBJ)
    std::vector<p3d::Vector3> normalMesh_; //! x,y,z of a normal  (read from OBJ)
    std::vector<p3d::Vector2> texCoordMesh_; //! s,t of a texCoord (read from OBJ)
    std::vector<p3d::Vector3> normalFace_; //! x,y,z of a normal (one normal per face)
    std::vector<VertexAttribFace> vertexAttrib_; //! _vertexAttrib[i][j] returns the VertexAttrib of the j-ieme vertex of the i-ieme face


};
}


