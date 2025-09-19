#pragma once
#include "Interface/Object/GameObject/Transform/ITransform.h"
#include "Script/Object/Component/Component.h"

class TransformComponent
	: public ITransform
	, public Component
{
public:
	TransformComponent();
	virtual	~TransformComponent() override = default;

	//更新
	virtual void MatrixUpdate() override;
	virtual void ImGuiUpdate() override;

	//シリアライズ/デシリアライズ
	void Deserialize(const nlohmann::json& _json);
	void Serialize(nlohmann::json& _json);

	//座標
	virtual const Math::Vector3 GetWorldPosition()	const override;
	virtual const Math::Vector3& GetLocalPosition()	const override;
	virtual void SetLocalPosition(const Math::Vector3& _pos) override;

	//回転
	virtual const Math::Vector3& GetWorldEuler() const override;
	virtual const Math::Vector3& GetLocalEuler() const override;
	virtual const Math::Quaternion& GetLocalRotation()	const override;
	virtual void SetLocalRotation(const Math::Quaternion& _rota) override;
	virtual void SetLocalRotation(const Math::Vector3& _rota) override;

	//拡縮
	virtual const Math::Vector3& GetLocalScale()	const override;
	virtual void SetLocalScale(const Math::Vector3& _scale) override;

	//座標行列
	virtual const Math::Matrix& GetWorldMatrix() const override;
	virtual const Math::Matrix& GetLocalMatrix() const override;
	virtual void SetWorldMatrix(const Math::Matrix& _matrix) override;
	virtual void SetLocalMatrix(const Math::Matrix& _matrix) override;

	//計算順序
	virtual const std::string& GetMatTag() override;
	virtual void SetMatTag(std::string_view _tag) override;

	//追従
	virtual void UnFollow() override;
	virtual const bool is_Follow() const override;
	virtual void SetFollow(const bool _flg) override;

	//親子関係
	virtual void SetParent(std::weak_ptr<ITransform> _parent) ;

private:
	const Math::Matrix CreateLocalMatrix();
	const Math::Matrix GetTMat() const;
	const Math::Matrix GetSMat() const;
	const Math::Matrix GetRMat() const;
	const Math::Matrix GetRMat(UINT _shafts) const;

	// 座標
	Math::Vector3		m_localPosition = Math::Vector3::Zero;
	// 回転
	Math::Quaternion	m_localRotation = Math::Quaternion::Identity;
	Math::Vector3		m_worldEuler;
	Math::Vector3		m_localEuler;
	// 拡縮
	Math::Vector3		m_localScale = Math::Vector3::One;

	// 行列
	Math::Matrix		m_mWorld = Math::Matrix::Identity;
	Math::Matrix		m_mLocal = Math::Matrix::Identity;
	std::string			m_myMatTag = "SRT";
	std::unordered_map<char, std::function<Math::Matrix()>> m_getMatrix;

	// 親トランスフォーム関係
	std::weak_ptr<ITransform> m_parent;
	bool m_bFollow = true;

	// その他
	bool m_bDirty = true;
};
