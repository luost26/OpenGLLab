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
        std::map<std::string, Texture *> texturesLoaded;
        
        MeshTextureNameGeneratorFactory * textureNameGeneratorFactory;
        
        AssimpModel() {
        }
        
        void loadFromFile(const std::string & path) {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        
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
            std::vector<MeshTexture> textures;
            
            for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
                MeshVertex vertex;
                glm::vec3 vector;
                
                // positions
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.position = vector;
                // normals
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;
                // texture coordinates
                if(mesh->mTextureCoords[0]) {
                    glm::vec2 vec;
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.textureCoordinate = vec;
                } else {
                    vertex.textureCoordinate = glm::vec2(0.0f, 0.0f);
                }
                // tangent
//                vector.x = mesh->mTangents[i].x;
//                vector.y = mesh->mTangents[i].y;
//                vector.z = mesh->mTangents[i].z;
//                vertex.tangent = vector;
//                // bitangent
//                vector.x = mesh->mBitangents[i].x;
//                vector.y = mesh->mBitangents[i].y;
//                vector.z = mesh->mBitangents[i].z;
//                vertex.bitangent = vector;
                vertices.push_back(vertex);
            }

            for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
                aiFace face = mesh->mFaces[i];
                for(unsigned int j = 0; j < face.mNumIndices; j++) {
                    indices.push_back(face.mIndices[j]);
                }
            }

            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            
            MeshTextureNameGenerator * name_gen = textureNameGeneratorFactory->generator();

            // 1. diffuse maps
            std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, name_gen->forType(MeshTextureNameGenerator::DIFFUSE));
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // 2. specular maps
            std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, name_gen->forType(MeshTextureNameGenerator::SPECULAR));
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            // 3. normal maps
            std::vector<MeshTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, name_gen->forType(MeshTextureNameGenerator::NORMAL));
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

            // 4. height maps
            std::vector<MeshTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, name_gen->forType(MeshTextureNameGenerator::HEIGHT));
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

            delete name_gen;
                        
            return new Mesh(vertices, indices, textures);
        }
        
        std::vector<MeshTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, MeshTextureNameGenerator * name_gen) {
            
            std::vector<MeshTexture> textures;
            for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
                aiString ai_filename;
                mat->GetTexture(type, i, &ai_filename);
                std::string filename = ai_filename.C_Str();
                
                if (texturesLoaded.count(filename))  {
                    textures.push_back(MeshTexture(name_gen->generate(), texturesLoaded[filename]));
                } else {
                    Texture2D * texture = new Texture2D();
                    std::string fullpath = textureDirectory + "/" + filename;
                    TextureImage * timg = TextureImage::fromPath(fullpath.c_str());
                    texture->bind()->wrapS(GL_REPEAT)->wrapT(GL_REPEAT)
                            ->minFilter(GL_LINEAR_MIPMAP_LINEAR)->magFilter(GL_LINEAR)
                            ->loadImage(timg)->generateMipmap();
                    delete timg;
                    textures.push_back(MeshTexture(name_gen->generate(), texture));
                }
            }
            
            return textures;
        }
        
    public:
        
        static AssimpModel * fromFile(const char * path, MeshTextureNameGeneratorFactory * tex_name_gen_factory) {
            AssimpModel * instance = new AssimpModel;
            instance->textureNameGeneratorFactory = tex_name_gen_factory;
            instance->loadFromFile(path);
            return instance;
        }
        
        ~AssimpModel() {
            for (Mesh * mesh : meshes) {
                delete mesh;
            }
        }
        
        void draw(Program * program) {
            for (Mesh * mesh : meshes) {
                mesh->setUp()->draw(program);
            }
        }
    };
}

#endif /* AssimpModel_hpp */
