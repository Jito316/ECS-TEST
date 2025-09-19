#pragma once
class IGameObject;
class IComponent;

class IGameObjectManager
{
public:
	IGameObjectManager() = default;
	 virtual ~IGameObjectManager() = default;

	virtual void Start() = 0;
	// 更新系
	virtual void PreUpdate() = 0;
	virtual void Update() = 0;
	virtual void PostUpdate() = 0;
	virtual void RenderUpdate() = 0;

	// 初期化・解放
	virtual void Load(std::string _path) = 0;
	virtual void Release(std::string _path, bool _enableSave) = 0;

	virtual const std::list<std::shared_ptr<IGameObject>>& GetObjectList() const = 0;
	virtual std::list<std::shared_ptr<IGameObject>>& WorkObjectList() = 0;
};
