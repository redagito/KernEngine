#include "gfx/Model.h"

#include <stdexcept>
#include <fmtlog/fmtlog.h>

#include "gfx/Texture.h"

Model::Model(const std::string& path) { loadModel(path); }

void Model::draw(Shader& shader) const
{
    // Draw each mesh
    // TODO Camera for culling?
    for (const auto& mesh : meshes)
    {
        mesh->draw(shader);
    }
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    // Check error
    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
    {
        throw std::runtime_error{importer.GetErrorString()};
    }

    // Directory of the loaded file
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // Process all meshes
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        // Meshes are stored in the scene, each node has a list
        // of mesh indices
        // TODO Range check
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // Process all children of the mesh
    // TODO Probably bad idea for meshes with large amount of children
    //      Change this to be non-recursive
    for (unsigned int i = 0; i < node->mNumChildren; ++i)
    {
        processNode(node->mChildren[i], scene);
    }
}

std::shared_ptr<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);

    std::vector<GLuint> indices;

    if (mesh->mNormals == nullptr)
        throw std::runtime_error{"Mesh is missing normal data"};

    // Per vertex data
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;
        vertex.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
        vertex.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};

        // Texture coordinates
        // Up to 8 different texture coordinates allowed per vertex
        // Only use the first
        if (mesh->mTextureCoords[0] != nullptr)
        {
            vertex.texCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        }
        else
        {
            vertex.texCoords = {0.f, 0.f};
        }

        vertices.push_back(vertex);
    }

    // Indices, stored per face
    for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
    {
        auto face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Material
    std::vector<std::shared_ptr<Texture>> diffuseTextures;
    std::vector<std::shared_ptr<Texture>> specularTextures;
    std::vector<std::shared_ptr<Texture>> normalTextures;
    if (mesh->mMaterialIndex >= 0)
    {
        auto material = scene->mMaterials[mesh->mMaterialIndex];
        diffuseTextures = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        specularTextures = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        normalTextures = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
    }

    return std::make_shared<Mesh>(vertices, indices, diffuseTextures, specularTextures, normalTextures);
}

std::vector<std::shared_ptr<Texture>> Model::loadMaterialTextures(aiMaterial* material, aiTextureType type,
                                                                  const std::string& typeName)
{
    std::vector<std::shared_ptr<Texture>> textures;
    logi("Loading {} material textures.", material->GetTextureCount(type));
    for (unsigned int i = 0; i < material->GetTextureCount(type); ++i)
    {
        // Path to the texture
        aiString fileName;
        material->GetTexture(type, i, &fileName);

        // Load texture
        auto texture = textureFromFile(fileName.C_Str(), directory, typeName);

        textures.push_back(texture);
    }

    return textures;
}