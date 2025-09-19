#pragma once
class IGameObject;

class SCRIPT_API ITransform
{
public:
	enum Shaft
		: UINT
	{
		X = 1 << 0,
		Y = 1 << 1,
		Z = 1 << 2
	};

	ITransform() = default;
	virtual ~ITransform() = default;

	// 更新
	virtual void MatrixUpdate() = 0;
	virtual void ImGuiUpdate() = 0;

	// 座標
	virtual const Math::Vector3 GetWorldPosition()	const = 0;
	virtual const Math::Vector3& GetLocalPosition()	const = 0;
	virtual void SetLocalPosition(const Math::Vector3& _pos) = 0;

	// 回転
	virtual const Math::Vector3& GetWorldEuler() const = 0;
	virtual const Math::Vector3& GetLocalEuler() const = 0;
	virtual const Math::Quaternion& GetLocalRotation()	const = 0;
	virtual void SetLocalRotation(const Math::Quaternion& _rota) = 0;
	virtual void SetLocalRotation(const Math::Vector3& _rota) = 0;

	// 拡縮
	virtual const Math::Vector3& GetLocalScale() const = 0;
	virtual void SetLocalScale(const Math::Vector3& _scale) = 0;

	// 座標行列
	virtual const Math::Matrix& GetWorldMatrix() const = 0;
	virtual const Math::Matrix& GetLocalMatrix() const = 0;
	virtual void SetWorldMatrix(const Math::Matrix& _matrix) = 0;
	virtual void SetLocalMatrix(const Math::Matrix& _matrix) = 0;

	// 計算順序
	virtual const std::string& GetMatTag() = 0;
	virtual void SetMatTag(std::string_view _tag) = 0;

	// 追従
	virtual void UnFollow() = 0;
	virtual const bool is_Follow() const = 0;
	virtual void SetFollow(const bool _flg) = 0;
};