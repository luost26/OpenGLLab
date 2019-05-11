//
// Created by 罗 世通 on 2019-04-28.
//

#ifndef OPENGLLAB_MATERIAL_HPP
#define OPENGLLAB_MATERIAL_HPP

#include "../support.hpp"

namespace wglfw {

    class TexturePool {
    private:
        std::map<std::string, Texture*> textures;
    public:
        int has(std::string name) {
            return textures.count(name);
        }

        Texture * get(std::string name) {
            return textures[name];
        }

        void put(std::string name, Texture * tex) {
            textures[name] = tex;
        }
    };

    class AssimpMaterialLoader {
    private:

        std::string textureDirectory;

        TexturePool * texturePool;

        aiMaterial * assimpMaterial;


    public:
        AssimpMaterialLoader(aiMaterial * mat) {
            assimpMaterial = mat;
            texturePool = NULL;
        }

        AssimpMaterialLoader * setTextureDirectory(std::string dir) {
            textureDirectory = dir;
            return this;
        }

        AssimpMaterialLoader * setTexturePool(TexturePool * pool) {
            texturePool = pool;
            return this;
        }

        Texture * loadTexture(aiTextureType type) {
            if (assimpMaterial->GetTextureCount(type) == 0) {
                return NULL;
            }

            aiString ai_filename;
            assimpMaterial->GetTexture(type, 0, &ai_filename); // only load the first texture
            std::string filename = ai_filename.C_Str();

            if (texturePool->has(filename))
                return texturePool->get(filename);

            Texture2D * texture = new Texture2D();
            std::string full_path = textureDirectory + "/" + filename;
            TextureImage * tex_img = TextureImage::fromPath(full_path.c_str());

            texture->bind()->wrapS(GL_REPEAT)->wrapT(GL_REPEAT)
                    ->minFilter(GL_LINEAR_MIPMAP_LINEAR)->magFilter(GL_LINEAR)
                    ->loadImage(tex_img)->generateMipmap();
            delete tex_img;

            texturePool->put(filename, texture);

            return texture;
        }

        Texture * loadAmbientTexture() {
            return loadTexture(aiTextureType_AMBIENT);
        }

        Texture * loadDiffuseTexture() {
            return loadTexture(aiTextureType_DIFFUSE);
        }

        Texture * loadSpecularTexture() {
            return loadTexture(aiTextureType_SPECULAR);
        }

        Texture * loadNormalsTexture() {
            return loadTexture(aiTextureType_NORMALS);
        }

        Texture * loadHeightTexture() {
            return loadTexture(aiTextureType_HEIGHT);
        }

        glm::vec3 loadColor3D(const char * key, unsigned int type, unsigned int idx) {
            aiColor3D color(0.0f, 0.0f, 0.0f);
            assimpMaterial->Get(key, type, idx, color);
            return glm::vec3(color.r, color.g, color.b);
        }

        glm::vec3 loadAmbientColor3D() {
            return loadColor3D(AI_MATKEY_COLOR_AMBIENT);
        }

        glm::vec3 loadDiffuseColor3D() {
            return loadColor3D(AI_MATKEY_COLOR_DIFFUSE);
        }

        glm::vec3 loadSpecularColor3D() {
            return loadColor3D(AI_MATKEY_COLOR_SPECULAR);
        }

        float loadShininess() {
            float val = 0.0f;
            assimpMaterial->Get(AI_MATKEY_SHININESS, val);
            return val;
        }

		float loadOpacity() {
			float val = 1.0f;
			assimpMaterial->Get(AI_MATKEY_OPACITY, val);
  			return val;
		}
    };

    class Material {

    public:
        std::map<std::string, Texture *> textures;
        std::map<std::string, glm::vec3> vector3s;
        std::map<std::string, float> floats;

        Material() {
        }

        Material *loadFromAssimpMaterial(AssimpMaterialLoader * loader) {
            Texture * tex_ambient = loader->loadAmbientTexture();
            Texture * tex_diffuse = loader->loadDiffuseTexture();
            Texture * tex_specular = loader->loadSpecularTexture();
            Texture * tex_normals = loader->loadNormalsTexture();
            Texture * tex_height = loader->loadHeightTexture();

            floats["material.texture_ambient0_enabled"] = 0.0f;
            floats["material.texture_diffuse0_enabled"] = 0.0f;
            floats["material.texture_specular0_enabled"] = 0.0f;

            if (tex_ambient) {
                textures["material.texture_ambient0"] = tex_ambient;
                floats["material.texture_ambient0_enabled"] = 1.0f;
            } else vector3s["material.ambient"] = loader->loadAmbientColor3D();

            if (tex_diffuse) {
                textures["material.texture_diffuse0"] = tex_diffuse;
                floats["material.texture_diffuse0_enabled"] = 1.0f;
            } else vector3s["material.diffuse"] = loader->loadDiffuseColor3D();

            if (tex_specular) {
                textures["material.texture_specular0"] = tex_specular;
                floats["material.texture_specular0_enabled"] = 1.0f;
            } else vector3s["material.specular"] = loader->loadSpecularColor3D();

            if (tex_normals) textures["material.texture_normals0"] = tex_normals;

            if (tex_height) textures["material.texture_height0"] = tex_height;

            floats["material.shininess"] = loader->loadShininess();
			floats["material.opacity"] = loader->loadOpacity();

            return this;
        }

    };

}

#endif //OPENGLLAB_MATERIAL_HPP
