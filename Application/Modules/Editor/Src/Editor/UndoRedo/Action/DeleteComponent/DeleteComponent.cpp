#include "DeleteComponent.h"
namespace UndoRedoAction {
	DeleteComponent::DeleteComponent(std::shared_ptr<IComponent> _cmp)
		: component(_cmp)
		, Base(_cmp.get())
	{
		auto ptr = _cmp.get();

		auto& list = _cmp->GetOwner().lock()->GetComponentList();
		auto it = std::find(list.begin(), list.end(), component);
		index = std::distance(list.begin(), it);
	}
	void DeleteComponent::Undo()
	{
		auto& list = component->GetOwner().lock()->WorkComponentList();
		list.insert(std::next(list.begin(), index), component);
	}
	void DeleteComponent::Redo()
	{
		auto& list = component->GetOwner().lock()->WorkComponentList();
		list.erase(std::next(list.begin(), index));
	}
	void DeleteComponent::Init()
	{
		auto& list = component->GetOwner().lock()->WorkComponentList();
		list.erase(std::next(list.begin(), index));
	}
}