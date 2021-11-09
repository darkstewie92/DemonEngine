#include "include/TestTexture.h"
#include <core/Window.h>
#include <core/Events/Events.h>
#include <core/CoreFun.h>
#include <core/Input.h>

namespace  test
{

    TestTexture::TestTexture() 
    {
        camera = new Camera2d(16.f/9.f);
        CrossPlatformWindow::SetEventCallBack(BIND_EVENT(TestTexture::onEvent));
        vertices = nullptr;
        createVertices();
        core::CreateIndices(indices, 1);
        texture.Set("anim.png", GL_TEXTURE0);
        vao.bind();
        ebo.bind();
        vbo.bindDynamic(10);
        vao.newLayoutDynamic();
        ebo.set(indices.data(), sizeof(int)* indices.size());
        vbo.loadDynamic(0, sizeof(Vertex)* 4, vertices);
        vs.initShader(VERTEX_SHADER);
        fs.initShader(FRAGMENT_SHADER);
        vs.readSourceFile("VertexQuad.glsl");
        fs.readSourceFile("fragment.glsl");
        ps.compileShader(vs.id,fs.id);
        ps.use();
        ps.setUniMat4f("aMVP", camera->GetViewProjMatrix());
        ps.setUnifi("m_texture", 0);
    }

    void TestTexture::onUpdate(float deltatime) 
    {
        ps.setUniMat4f("aMVP", camera->GetViewProjMatrix());
        vbo.loadDynamic(0, sizeof(Vertex)* 4, vertices);
    }

    void TestTexture::onRender() 
    {
        Renderer::draw(vao, ebo, ps);
    }

    void TestTexture::onImGuiRender() 
    {

    }

    void TestTexture::onEvent(Event& e) 
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(TestTexture::onKeyPressed));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(TestTexture::onWindowResizedEvent));


    }


    bool TestTexture::onKeyPressed(KeyPressedEvent& e) 
    {
        std::cout<< e.toString() << "\n";
        if(animFrame == 0)
        {
            vertices[0].setTCor(0.5f, 0.0f);
            vertices[1].setTCor(1.f, 0.0f);
            vertices[2].setTCor(1.f, 1.0f);
            vertices[3].setTCor( 0.5f, 1.0f);
            animFrame = 1;
        }
        else
        {
            vertices[0].setTCor(0.0f, 0.0f);
            vertices[1].setTCor(0.5f, 0.0f);
            vertices[2].setTCor(0.5f, 1.0f);
            vertices[3].setTCor( 0.0f, 1.0f);
            animFrame = 0;
        }

        return true;
    }

   
    void TestTexture::createVertices() 
    {
        if(vertices !=  nullptr) 
            delete vertices;
        vertices = new Vertex[4];
        vertices[0].setPos(-0.5f, -0.5f);
        vertices[0].setTCor(0.0f, 0.0f);
        vertices[1].setPos( 0.5f, -0.5f);
        vertices[1].setTCor(0.5f, 0.0f);
        vertices[2].setPos( 0.5f, 0.5f);
        vertices[2].setTCor(0.5f, 1.0f);
        vertices[3].setPos( -0.5f, 0.5f);
        vertices[3].setTCor( 0.0f, 1.0f);
    }


    bool TestTexture::onWindowResizedEvent(WindowResizeEvent& e) 
    {
        camera->ResetProjMatrix(e.GetWidth(), e.GetHeight());
        return true;
    }

}