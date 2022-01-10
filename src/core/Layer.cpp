#include "core/Layer.h"

LayerStack::~LayerStack()
{
	for (auto& layer : m_StackLayer)
	{
		layer->onDetach();
		delete layer;
	}
}


void LayerStack::PushLayer(Layer* layer)
{
	m_StackLayer.emplace(m_StackLayer.begin() + BottomLayerIndex, layer);
	layer->onAttach();
}


void LayerStack::PushTopLayer(Layer* layer)
{
	m_StackLayer.push_back(layer);
	layer->onAttach();
}


void LayerStack::PopLayer(Layer* layer)
{
	auto search = std::find(m_StackLayer.begin(), m_StackLayer.begin() + BottomLayerIndex, layer);
	if (search !=  m_StackLayer.begin() + BottomLayerIndex)
	{
		layer->onDetach();
		m_StackLayer.erase(search);
	}

}



void LayerStack::PopTopLayer(Layer* layer)
{
	auto search = std::find(m_StackLayer.begin() + BottomLayerIndex, m_StackLayer.end(), layer);
	if (search != m_StackLayer.end())
	{
		layer->onDetach();
		m_StackLayer.erase(search);
	}

}




