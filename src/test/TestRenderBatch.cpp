#include <TestRenderBatch.h>
#include "core/Random.h"
#include "core/Window.h"
#include "Renderer/renderer.h" 
#include "core/Input.h"
#include "core/Log.h"
#include <string>
#include "Renderer/Colors.h"




namespace test{


	TestRenderBatch::TestRenderBatch()
	{
		Random::init();
		Window::SetEventCallBack(BIND_EVENT(TestRenderBatch::onEvent));
		m_quad_data.position = { 0,0,0 };
		m_quad_data.color = { 1,0,0,1 };
		t_slot = 0;
		HappyFace = std::make_shared<Texture>("awesomeface.png");
		Wall = std::make_shared<Texture>("wall.jpg");
		Background = std::make_shared<Texture>("bg.jpg");
		anim = std::make_shared<Texture>("anim.png");
		container = std::make_shared<Texture>("container2.png");
		Tile=  std::make_shared<SubTexture>(anim, 0, 0, 200, 200);
		particles = std::make_shared<ParticleSystem>(Wall);
	}

	void TestRenderBatch::onUpdate(float deltatime)
	{
		Renderer::DrawTexturedQuad({0,0,-.9f}, Background, 100.0f, 0.0f, 1000.0f);
		camera.movement(deltatime);
		QuadData data;
		float x = 0, y = 0, z = 0;

		
		ParticleProps props;
		props.Position = { camera.GetMousePositionInWorldSpceX(Input::GetMousePosition().x), camera.GetMousePositionInWorldSpceY(Input::GetMousePosition().y), .6f };
		props.Shape = ParticleShape::Textured;
		props.Color = Color::White;
		props.ScalingStep = .1;
		for (size_t i = 0; i < n_particles * deltatime; i++)
		{
			props.RandomValues();
			props.Velocity *= .5f;
		//	particles->Emit(props);
		}

		Transform t(glm::vec3(0),glm::vec3(1.0f), 0);

		Renderer::DrawQuad(t, Color::Blue);


		
		
	}


	void TestRenderBatch::onRender()
	{
	//	particles->onRender();
	}


	void TestRenderBatch::onEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT(TestRenderBatch::onMouseScrolledEvent));
		dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT(TestRenderBatch::onMouseMovedEvent));
		dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(TestRenderBatch::onKeyPressedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT(TestRenderBatch::onWindowResizeEvent));
	}


	bool TestRenderBatch::onMouseScrolledEvent(MouseScrolledEvent& e)
	{
		camera.Zoom(e.GetYoffset());
		return false;
	}

	bool TestRenderBatch::onMouseMovedEvent(MouseMovedEvent& e)
	{
		glm::vec3 position = { camera.GetMousePositionInWorldSpceX(e.GetMouseX()), camera.GetMousePositionInWorldSpceY(e.GetMouseY()), 0.0f };
		return false;
	}


	bool TestRenderBatch::onKeyPressedEvent(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::V)
			Window::SetVSync(true);
		if (e.GetKeyCode() == Key::B)
			Window::SetVSync(false);

		return false;
	}


	void TestRenderBatch::onImGuiRender()
	{
		RendererStats stats = Renderer::GetStats();
		ImGui::Separator();
		ImGui::Text("DrawCalls  = %d", stats.DrawCalls);
		ImGui::Text("Quads		= %d", stats.QuadIndex/4);
		ImGui::Text("Circles	= %d", stats.CircleIndex/4);
		ImGui::Separator();
		ImGui::SliderInt("Particles number", &n_particles, 0, 1000);
		ImGui::SliderInt("TextureSlot", &t_slot, 0, 10000);

	}

	bool TestRenderBatch::onWindowResizeEvent(WindowResizeEvent& e)
	{
		return false;
	}
	

}