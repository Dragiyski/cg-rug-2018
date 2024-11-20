#ifndef MODEL_H
#define MODEL_H

#include <QString>
#include <QStringList>
#include <vector>
#include <QVector2D>
#include <QVector3D>

/**
 * @brief The Model class
 *
 * Loads all data from a Wavefront .obj file
 * IMPORTANT! Current only supports TRIANGLE meshes!
 *
 * Support for other meshes can be implemented by students
 *
 */
class Model
{
public:
    Model(QString filename);

    // Used for glDrawArrays()
    std::vector<QVector3D> getVertices();
    std::vector<QVector3D> getNormals();
    std::vector<QVector2D> getTextureCoords();

    // Used for interleaving into one buffer for glDrawArrays()
    std::vector<float> getVNInterleaved();
    std::vector<float> getVNTInterleaved();

    // Used for glDrawElements()
    std::vector<QVector3D> getVertices_indexed();
    std::vector<QVector3D> getNormals_indexed();
    std::vector<QVector2D> getTextureCoords_indexed();
    std::vector<unsigned>  getIndices();

    // Used for interleaving into one buffer for glDrawElements()
    std::vector<float> getVNInterleaved_indexed();
    std::vector<float> getVNTInterleaved_indexed();

    bool hasNormals();
    bool hasTextureCoords();
    int getNumTriangles();

    void unitize();

private:

    // OBJ parsing
    void parseVertex(QStringList tokens);
    void parseNormal(QStringList tokens);
    void parseTexture(QStringList tokens);
    void parseFace(QStringList tokens);

    // Alignment of data
    void alignData();
    void unpackIndexes();

    // Intermediate storage of values
    std::vector<QVector3D> vertices_indexed;
    std::vector<QVector3D> normals_indexed;
    std::vector<QVector2D> textureCoords_indexed;
    std::vector<unsigned> indices;

    std::vector<QVector3D> vertices;
    std::vector<QVector3D> normals;
    std::vector<QVector2D> textureCoords;

    // Utility storage
    std::vector<unsigned> normal_indices;
    std::vector<unsigned> texcoord_indices;
    std::vector<QVector3D> norm;
    std::vector<QVector2D> tex;

    bool hNorms;
    bool hTexs;
};

#endif // MODEL_H
