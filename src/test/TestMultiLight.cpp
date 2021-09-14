#include "TestMultiLight.h"
namespace test
{
    TestMultiLight::TestMultiLight(Camera* cam, GLFWwindow * window) :  camera(cam),
                                                                        n_pointlight(4) 
    {
        Timer timer("MultiLight");

        Texture texture[2];
        texture[0].Set("container2.png", GL_TEXTURE0);
        texture[1].Set("vc.png", GL_TEXTURE1);
        texture[0].active();
        texture[1].active();
        glEnable(GL_DEPTH_TEST);



        proj = core::proj3d(4/3);
        view = camera->view();

        cube = new Cube("fragmentMulti.txt");
        cube->SetPos(0,0,0);
        cube->SetScale(2);
        cube->SetDiffuseMap(0);
        cube->SetSpecularMap(1);
        cube->UpdateMVP(proj, view);
        cube->setUniModel();

        dirlight = new DirectionalLight;
        dirlight->SetDir(glm::vec3(1.0f));
        dirlight->UpdateMVP(proj,view);

        for(int i = 0; i<n_pointlight; i++)
        {
            pointlight.push_back(PointLight());
            pointlight.back().SetPos((i*2) - 4, 0, -7);
            pointlight.back().UpdateMVP(proj, view);
        }


        plane = new Plane;
        plane->SetPos(0,-3, 1);
        plane->UpdateMVP(proj,view);
        plane->setUniModel();
        plane->SetDiffuseMap(0);
        plane->SetSpecularMap(1);
    }

    void TestMultiLight::onUpdate(float deltatime) 
    {
        view = camera->view();
        cube->UpdateMVP(proj, view);
        cube->setUniDirLight(dirlight);
        // cube->setUniPointLight(pointlight);
        cube->setUniPointLightArray(pointlight.data(), n_pointlight);
        cube->setCameraPosition(camera);
        cube->setUniMaterial(dirlight->lightColor);

        for(int i = 0; i < pointlight.size(); i++)
        {
            pointlight[i].UpdateMVP(proj, view);
        }
    
        plane->UpdateMVP(proj,view);
        camera->statusController();
    }
    
    void TestMultiLight::onRender() 
    {
        glClearColor(0.3f,0.5f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Renderer* renderer;
        cube->draw(renderer);
        plane->draw(renderer);
        for(int i= 0; i <pointlight.size(); i++)
            pointlight[i].draw(renderer);
    }
    
    void TestMultiLight::onImGuiRender() 
    {
        ImGui::SliderFloat ("directional light ambient inf", &dirlight->ambientInf, 0.0f, 1.0f);
        ImGui::SliderFloat ("directional light diffuse inf", &dirlight->diffuseInf, 0.0f, 1.0f);
        ImGui::SliderFloat3("directional light direction",  &dirlight->direction.x, 0.0f, 1.0f);

        // ImGui::SliderFloat ("point light ambient inf", &pointlight->ambientInf, 0.0f, 1.0f);
        // ImGui::SliderFloat ("point light diffuse inf", &pointlight->diffuseInf, 0.0f, 1.0f);
        // ImGui::SliderFloat ("point light linear", &pointlight->linear, 0.0f, 0.1f);
        // ImGui::SliderFloat ("point light quadratic", &pointlight->quadratic, 0.0f, 0.1f);

        ImGui::SliderFloat ("Material shininess", &cube->material.shininess, 0.0f, 256.0f);
        ImGui::SliderFloat3("point light pos", vec, -4,4);
    }
}