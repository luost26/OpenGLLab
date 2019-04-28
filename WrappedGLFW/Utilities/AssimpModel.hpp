//
//  AssimpModel.hpp
//
//  Created by 罗 世通 on 2019/3/30.
//

#ifndef AssimpModel_hpp
#define AssimpModel_hpp

#include "../support.hpp"
#include "../Exception.hpp"
#include "../Program.hpp"
#include "Mesh.hpp"
#include "MeshTextureNameGenerator.hpp"
#include "Material.hpp"

namespace wglfw {
    class AssimpModelLoadFailureException : public Exception {
    private:
        std::string info;
    public:
        AssimpModelLoadFailureException(const std::string & i): info(i) {}
        const char * what() throw () {
            return info.c_str();
        }
    };
    
    
    
    class AssimpModel {
    private:
        std::vector<Mesh *> meshes;
        std::string textureDirectory;
        TexturePool * texturePool;

        AssimpModel() {
            texturePool = new TexturePool();
        }
        
        void loadFromFile(const std::string & path) {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path,
                    aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenNormals
                    );
			// aiProcess_FlipUVs Disabled
        
            if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                throw AssimpModelLoadFailureException(importer.GetErrorString());
            }
            
            textureDirectory = path.substr(0, path.find_last_of('/'));
            processNode(scene->mRootNode, scene);
        }
        
        void processNode(aiNode *node, const aiScene *scene) {

            for(unsigned int i = 0; i < node->mNumMeshes; i++) {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
            }

            for(unsigned int i = 0; i < node->mNumChildren; i++) {
                processNode(node->mChildren[i], scene);
            }
            
        }
        
        Mesh * processMesh(aiMesh *mesh, const aiScene *scene) {
            std::vector<MeshVertex> vertices;
            std::vector<unsigned int> indices;
            
            for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
                MeshVertex vertex;
                glm::vec3 vector;
                
                // positions
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.position = vector;
                // normals
                if (mesh->mNormals) {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.normal = vector;
                }
                // texture coordinates
                if(mesh->mTextureCoords[0]) {
                    glm::vec2 vec;
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.textureCoordinate = vec;

                    // tangent
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.tangent = vector;
                    // bitangent
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.bitangent = vector;

                } else {
                    vertex.textureCoordinate = glm::vec2(0.0f, 0.0f);
                }

                vertices.push_back(vertex);
            }

            for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
                aiFace face = mesh->mFaces[i];
                for(unsigned int j = 0; j < face.mNumIndices; j++) {
                    indices.push_back(face.mIndices[j]);
                }
            }

            aiMaterial * assimp_material = scene->mMaterials[mesh->mMaterialIndex];

            AssimpMaterialLoader * loader = new AssimpMaterialLoader(assimp_material);
            loader->setTextureDirectory(textureDirectory)->setTexturePool(texturePool);

            Material * material = new Material;
            material->loadFromAssimpMaterial(loader);

            return new Mesh(vertices, indices, material);
        }

        
    public:
        
        static AssimpModel * fromFile(const char * path) {
            AssimpModel * instance = new AssimpModel;
            instance->loadFromFile(path);
            return instance;
        }
        
        ~AssimpModel() {
            for (Mesh * mesh : meshes) {
                delete mesh;
            }
            delete texturePool;
        }
        
        void draw(Program * program) {
            for (Mesh * mesh : meshes) {
                mesh->setUp()->draw(program);
            }
        }
    };
}

#endif /* AssimpModel_hpp */
