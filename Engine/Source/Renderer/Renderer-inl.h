#include "Renderer.h"

// libarary includes
#include <algorithm>

// engine includes
#include "Assert\Assert.h"
#include "Data\PooledString.h"
#include "Logger\Logger.h"

namespace engine {
namespace render {

inline Renderer* Renderer::Get()
{
	return Renderer::instance_;
}

inline engine::memory::SharedPointer<RenderableObject> Renderer::CreateRenderableObject(const engine::data::PooledString& i_file_name, const engine::memory::WeakPointer<engine::gameobject::GameObject>& i_game_object)
{
	// validate input
	ASSERT(i_file_name.GetLength() > 0);
	ASSERT(i_game_object);

	// create a sprite for the renderable
	auto sprite = CreateSprite(i_file_name);

	// create a new renderable
	engine::memory::SharedPointer<RenderableObject> renderable = RenderableObject::Create(sprite, i_game_object);

	// add it to the list
	renderables_.push_back(renderable);
	++num_renderables_;

	return renderable;
}

inline void Renderer::AddRenderableObject(const engine::memory::SharedPointer<RenderableObject>& i_renderable_object)
{
	// validate input
	ASSERT(i_renderable_object);

	std::lock_guard<std::mutex> lock(renderables_mutex_);

	// check if this object already exists
	if (std::find(renderables_.begin(), renderables_.end(), i_renderable_object) != renderables_.end())
	{
		LOG_ERROR("Renderer is already tracking this renderable!");
		return;
	}

	renderables_.push_back(i_renderable_object);
	++num_renderables_;
}

inline void Renderer::RemoveRenderableObject(const engine::memory::SharedPointer<RenderableObject>& i_renderable_object)
{
	// validate input
	ASSERT(i_renderable_object);
	// can't remove renderable if there are none
	ASSERT(num_renderables_ > 0);

	std::lock_guard<std::mutex> lock(renderables_mutex_);

	// check if this object exists
	auto it = std::find(renderables_.begin(), renderables_.end(), i_renderable_object);
	if (it == renderables_.end())
	{
		LOG_ERROR("Renderer could not find this renderable!");
		return;
	}

	renderables_.erase(it);
	--num_renderables_;
}

} // namespace render
} // namespace engine
