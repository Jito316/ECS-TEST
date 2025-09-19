#pragma once
class ModelRendererComponent;

struct AnimatorData
{
	std::string animeName;

	float speed = 1.f;

	bool isLoop = false;

	bool isComplement = false;
	float complementSpeed = 1.f;
};

class ModelAnimatorComponent
	:public Component
{
public:
	/*
	* @fn
	* @brief アニメーションを変更します
	* @param _view			アニメーション名を入れてください
	* @param _isComplement	アニメーションを補間するか
	* @param _isLoop		繰り返し再生するか
	* @param _time			補間時間
	*/
	void ChangeAnime(std::string_view _view,const bool _isLoop = true, const float _speed = 1.f, const bool _isComplement = false, const float _time = 1.0f);
	void ChangeAnime(const AnimatorData& _pData);

	const std::shared_ptr<Math::KdAnimationData> GetAnime(std::string_view _view);

	bool is_End();
	bool is_Playing();
	bool is_looped() { return m_animator.IsLooped(); };

	float GetAnimePhase() { return m_animator.AnimePhase(); }
	float GetAnimeTime() { return m_animator.AnimeTime(); }
	float GetAnimeMaxTime() { return m_animator.AnimeMaxTime(); }

	std::string_view GetNowAnimeAnimeName() { return m_nowAnimatorData.animeName; }
	const Math::KdAnimator& GetAnimator() { return m_animator; }

	bool ImGuiAnimeSet(std::string_view _label, std::string& _str);
	bool ImGuiAnimeData(AnimatorData& _data);

	static void Serialize_AnimatorData(std::string_view _key, nlohmann::json& _json,const AnimatorData& _data);
	static void Deserialize_AnimatorData(std::string_view _key, const nlohmann::json& _json, AnimatorData& _data);
private:
	void Start()override;
	void PreUpdate()override;
	void RenderUpdate()override;

	void AdvanceTime();

	struct Nodes
	{
		int size() const
		{
			return pos.size();
		}

		std::vector<Math::Vector3> pos;
		std::vector<Math::Quaternion> rot;
		std::vector<Math::Vector3> scale;
	};

	Nodes m_nextNodes;
	Nodes m_postNodes;

	bool m_isChanging = false;	//アニメーションが切り替わったか
	float m_timeCnt = 0.0f;	//アニメーションの補完経過時間
	AnimatorData m_nowAnimatorData;

	std::weak_ptr<ModelRendererComponent> m_modelComponent;
	std::weak_ptr<IModelWork> m_wpModel;
	Math::KdAnimator m_animator;
};
