#ifndef SCENE_PBR_TEST_H
#define SCENE_PBR_TEST_H

#include <cell/renderer/renderer.h>
#include <cell/shading/material.h>
#include <cell/shading/shader.h>

class ScenePbrTest 
{
private:
    Renderer *m_Renderer;
    Camera   *m_Camera;

    Material m_PBRMaterial;
    Material m_PBRIrradianceMaterial;
    Material m_PBRPrefilterMaterial;
    Material m_PBRBRDFIntegrateMaterial;
    Shader   *m_PbrShader;
    Shader   *m_PbrIrradianceShader;
    Shader   *m_PbrPrefilterShader;
    Shader   *m_PbrBRDFintegrateShader;

public:
    // NOTE(Joey): pass over relevant data per scene: dependancy injection
    ScenePbrTest(Renderer *renderer, Camera *camera);
    ~ScenePbrTest();

    void Init();

    void Update(float dt);
    void Render();
};

#endif