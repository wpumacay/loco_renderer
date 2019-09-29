#pragma once

#include <materials/CIMaterial.h>
#include <core/CTexture.h>

namespace engine
{

    class CPhongMaterial : public CIMaterial
    {

    public :

        CVec3   ambient;
        CVec3   diffuse;
        CVec3   specular;
        float32 shininess;

        CPhongMaterial( const std::string& name,
                        const CVec3& ambientColor,
                        const CVec3& diffuseColor,
                        const CVec3& specularColor,
                        float32 shininess,
                        std::shared_ptr< CTexture > albedoMap = nullptr,
                        std::shared_ptr< CTexture > specularMap = nullptr,
                        std::shared_ptr< CTexture > normalMap = nullptr );

        ~CPhongMaterial();

        void setAlbedoMap( std::shared_ptr< CTexture > albedoMap ) { m_albedoMap = albedoMap; }
        void setSpecularMap( std::shared_ptr< CTexture > specularMap ) { m_specularMap = specularMap; }
        void setNormalMap( std::shared_ptr< CTexture > normalMap ) { m_normalMap = normalMap; }

        void bind( std::shared_ptr< CShader > shader ) override;
        void unbind() override;

        std::shared_ptr< CTexture > albedoMap() const { return m_albedoMap; }
        std::shared_ptr< CTexture > specularMap() const { return m_specularMap; }
        std::shared_ptr< CTexture > normalMap() const { return m_normalMap; }

    protected :

        std::string _toStringInternal() override;

    private :

        std::shared_ptr< CTexture > m_albedoMap;
        std::shared_ptr< CTexture > m_specularMap;
        std::shared_ptr< CTexture > m_normalMap;
    };

}
