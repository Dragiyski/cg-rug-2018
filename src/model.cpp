#include "model.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

// A Private Vertex class for vertex comparison
// DO NOT include "vertex.h" or something similar in this file
struct Vertex {
    QVector3D coord;
    QVector3D normal;
    QVector2D texCoord;

    Vertex()
        : coord()
        , normal()
        , texCoord()
    {}
    Vertex(QVector3D coords, QVector3D normal, QVector2D texc)
        : coord(coords)
        , normal(normal)
        , texCoord(texc)
    {}

    bool operator==(const Vertex &other) const
    {
        if (other.coord != coord)
            return false;
        if (other.normal != normal)
            return false;
        if (other.texCoord != texCoord)
            return false;
        return true;
    }
};

Model::Model(QString filename) {
    hNorms = false;
    hTexs = false;

    qDebug() << ":: Loading model:" << filename;
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);

        QString line;
        QStringList tokens;

        while (!in.atEnd()) {
            line = in.readLine().trimmed();
            if (line.length() <= 0) {
                continue;
            }

            if (line.startsWith("#"))
                continue; // skip comments

            tokens = line.split(" ", Qt::SkipEmptyParts);

            // Switch depending on first element
            if (tokens[0] == "v") {
                parseVertex(tokens);
            }

            if (tokens[0] == "vn") {
                parseNormal(tokens);
            }

            if (tokens[0] == "vt") {
                parseTexture(tokens);
            }

            if (tokens[0] == "f") {
                parseFace(tokens);
            }
        }

        file.close();

        // create an array version of the data
        unpackIndexes();

        // Allign all vertex indices with the right normal/texturecoord indices
        alignData();
    }
}

/**
 * @brief Model::unitze Not Implemented yet!
 *
 * Unitize the model by scaling so that it fits a box with sides 1
 * and origin at 0,0,0
 * Usefull for models with different scales
 *
 */
void Model::unitize() { qDebug() << "TODO: implement this yourself"; }

std::vector<QVector3D> Model::getVertices() { return vertices; }

std::vector<QVector3D> Model::getNormals() { return normals; }

std::vector<QVector2D> Model::getTextureCoords() { return textureCoords; }

std::vector<QVector3D> Model::getVertices_indexed() { return vertices_indexed; }

std::vector<QVector3D> Model::getNormals_indexed() { return normals_indexed; }

std::vector<QVector2D> Model::getTextureCoords_indexed() {
    return textureCoords_indexed;
}

std::vector<unsigned> Model::getIndices() { return indices; }

std::vector<float> Model::getVNInterleaved() {
    std::vector<float> buffer;

    for (std::size_t i = 0; i != vertices.size(); ++i) {
        QVector3D vertex = vertices.at(i);
        QVector3D normal = normals.at(i);
        buffer.push_back(vertex.x());
        buffer.push_back(vertex.y());
        buffer.push_back(vertex.z());
        buffer.push_back(normal.x());
        buffer.push_back(normal.y());
        buffer.push_back(normal.z());
    }

    return buffer;
}

std::vector<float> Model::getVNTInterleaved() {
    std::vector<float> buffer;

    for (std::size_t i = 0; i != vertices.size(); ++i) {
        QVector3D vertex = vertices.at(i);
        QVector3D normal = normals.at(i);
        QVector2D uv = textureCoords.at(i);
        buffer.push_back(vertex.x());
        buffer.push_back(vertex.y());
        buffer.push_back(vertex.z());
        buffer.push_back(normal.x());
        buffer.push_back(normal.y());
        buffer.push_back(normal.z());
        buffer.push_back(uv.x());
        buffer.push_back(uv.y());
    }

    return buffer;
}

std::vector<float> Model::getVNInterleaved_indexed() {
    std::vector<float> buffer;

    for (std::size_t i = 0; i != vertices_indexed.size(); ++i) {
        QVector3D vertex = vertices_indexed.at(i);
        QVector3D normal = normals_indexed.at(i);
        buffer.push_back(vertex.x());
        buffer.push_back(vertex.y());
        buffer.push_back(vertex.z());
        buffer.push_back(normal.x());
        buffer.push_back(normal.y());
        buffer.push_back(normal.z());
    }

    return buffer;
}

std::vector<float> Model::getVNTInterleaved_indexed() {
    std::vector<float> buffer;

    for (std::size_t i = 0; i != vertices_indexed.size(); ++i) {
        QVector3D vertex = vertices_indexed.at(i);
        QVector3D normal = normals_indexed.at(i);
        QVector2D uv = textureCoords_indexed.at(i);
        buffer.push_back(vertex.x());
        buffer.push_back(vertex.y());
        buffer.push_back(vertex.z());
        buffer.push_back(normal.x());
        buffer.push_back(normal.y());
        buffer.push_back(normal.z());
        buffer.push_back(uv.x());
        buffer.push_back(uv.y());
    }

    return buffer;
}

/**
 * @brief Model::getNumTriangles
 *
 * Gets the number of triangles in the model
 *
 * @return number of triangles
 */
int Model::getNumTriangles() { return vertices.size() / 3; }

void Model::parseVertex(QStringList tokens) {
    float x, y, z;
    x = tokens[1].toFloat();
    y = tokens[2].toFloat();
    z = tokens[3].toFloat();
    vertices_indexed.push_back(QVector3D(x, y, z));
}

void Model::parseNormal(QStringList tokens) {
    hNorms = true;
    float x, y, z;
    x = tokens[1].toFloat();
    y = tokens[2].toFloat();
    z = tokens[3].toFloat();
    norm.push_back(QVector3D(x, y, z));
}

void Model::parseTexture(QStringList tokens) {
    hTexs = true;
    float u, v;
    u = tokens[1].toFloat();
    v = tokens[2].toFloat();
    tex.push_back(QVector2D(u, v));
}

void Model::parseFace(QStringList tokens) {
    QStringList elements;

    for (int i = 1; i != tokens.size(); ++i) {
        elements = tokens[i].split("/");
        // -1 since .obj count from 1
        indices.push_back(elements[0].toInt() - 1);

        if (elements.size() > 1 && !elements[1].isEmpty()) {
            texcoord_indices.push_back(elements[1].toInt() - 1);
        }

        if (elements.size() > 2 && !elements[2].isEmpty()) {
            normal_indices.push_back(elements[2].toInt() - 1);
        }
    }
}

/**
 * @brief Model::alignData
 *
 * Make sure that the indices from the vertices align with those
 * of the normals and the texture coordinates, create extra vertices
 * if vertex has multiple normals or texturecoords
 */
void Model::alignData() {
    std::vector<QVector3D> verts = std::vector<QVector3D>();
    verts.reserve(vertices_indexed.size());
    std::vector<QVector3D> norms = std::vector<QVector3D>();
    norms.reserve(vertices_indexed.size());
    std::vector<QVector2D> texcs = std::vector<QVector2D>();
    texcs.reserve(vertices_indexed.size());
    std::vector<Vertex> vs = std::vector<Vertex>();

    std::vector<unsigned> ind = std::vector<unsigned>();
    ind.reserve(indices.size());

    unsigned currentIndex = 0;

    for (std::size_t i = 0; i != indices.size(); ++i) {
        QVector3D v = vertices_indexed[indices[i]];

        QVector3D n = QVector3D(0, 0, 0);
        if (hNorms) {
            n = norm[normal_indices[i]];
        }

        QVector2D t = QVector2D(0, 0);
        if (hTexs) {
            t = tex[texcoord_indices[i]];
        }

        Vertex k = Vertex(v, n, t);
        std::vector<Vertex>::const_iterator ik = std::find(vs.begin(), vs.end(), k);
        if (ik != vs.end()) {
            // Vertex already exists, use that index
            ind.push_back(ik - vs.begin());
        } else {
            // Create a new vertex
            verts.push_back(v);
            norms.push_back(n);
            texcs.push_back(t);
            vs.push_back(k);
            ind.push_back(currentIndex);
            ++currentIndex;
        }
    }
    // Remove old data
    vertices_indexed.clear();
    normals_indexed.clear();
    textureCoords_indexed.clear();
    indices.clear();

    // Set the new data
    vertices_indexed = verts;
    normals_indexed = norms;
    textureCoords_indexed = texcs;
    indices = ind;
}

/**
 * @brief Model::unpackIndexes
 *
 * Unpack indices so that they are available for glDrawArrays()
 *
 */
void Model::unpackIndexes() {
    vertices.clear();
    normals.clear();
    textureCoords.clear();
    for (std::size_t i = 0; i != indices.size(); ++i) {
        vertices.push_back(vertices_indexed[indices[i]]);

        if (hNorms) {
            normals.push_back(norm[normal_indices[i]]);
        }

        if (hTexs) {
            textureCoords.push_back(tex[texcoord_indices[i]]);
        }
    }
}
