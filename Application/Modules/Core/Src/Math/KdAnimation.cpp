#include "KdAnimation.h"
namespace JTN
{
	namespace Math
	{
		// 二分探索で、指定時間から次の配列要素のKeyIndexを求める関数
		// list		… キー配列
		// time		… 時間
		// 戻り値	… 次の配列要素のIndex
		template<class T>
		int BinarySearchNextAnimKey(const std::vector<T>& list, float time)
		{
			int low = 0;
			int high = (int)list.size();
			while (low < high)
			{
				int mid = (low + high) / 2;
				float midTime = list[mid].m_time;

				if (midTime <= time) low = mid + 1;
				else high = mid;
			}
			return low;
		}

		bool KdAnimationData::Node::InterpolateTranslations(Vector3& result, float time)
		{
			if (m_translations.size() == 0)return false;

			// キー位置検索
			UINT keyIdx = BinarySearchNextAnimKey(m_translations, time);

			// 先頭のキーなら、先頭のデータを返す
			if (keyIdx == 0) {
				result = m_translations.front().m_vec;
				return true;
			}
			// 配列外のキーなら、最後のデータを返す
			else if (keyIdx >= m_translations.size()) {
				result = m_translations.back().m_vec;
				return true;
			}
			// それ以外(中間の時間)なら、その時間の値を補間計算で求める
			else {
				auto& prev = m_translations[keyIdx - 1];	// 前のキー
				auto& next = m_translations[keyIdx];		// 次のキー
				// 前のキーと次のキーの時間から、0～1間の時間を求める
				float f = (time - prev.m_time) / (next.m_time - prev.m_time);
				// 補間
				result = DirectX::XMVectorLerp(
					prev.m_vec,
					next.m_vec,
					f
				);
			}

			return true;
		}

		bool KdAnimationData::Node::InterpolateRotations(Quaternion& result, float time)
		{
			if (m_rotations.size() == 0)return false;

			// キー位置検索
			UINT keyIdx = BinarySearchNextAnimKey(m_rotations, time);
			// 先頭のキーなら、先頭のデータを返す
			if (keyIdx == 0) {
				result = m_rotations.front().m_quat;
			}
			// 配列外のキーなら、最後のデータを返す
			else if (keyIdx >= m_rotations.size()) {
				result = m_rotations.back().m_quat;
			}
			// それ以外(中間の時間)なら、その時間の値を補間計算で求める
			else {
				auto& prev = m_rotations[keyIdx - 1];	// 前のキー
				auto& next = m_rotations[keyIdx];		// 次のキー
				// 前のキーと次のキーの時間から、0～1間の時間を求める
				float f = (time - prev.m_time) / (next.m_time - prev.m_time);
				// 補間
				result = DirectX::XMQuaternionSlerp(
					prev.m_quat,
					next.m_quat,
					f
				);
			}

			return true;
		}

		bool KdAnimationData::Node::InterpolateScales(Vector3& result, float time)
		{
			if (m_scales.size() == 0)return false;

			// キー位置検索
			UINT keyIdx = BinarySearchNextAnimKey(m_scales, time);

			// 先頭のキーなら、先頭のデータを返す
			if (keyIdx == 0) {
				result = m_scales.front().m_vec;
				return true;
			}
			// 配列外のキーなら、最後のデータを返す
			else if (keyIdx >= m_scales.size()) {
				result = m_scales.back().m_vec;
				return true;
			}
			// それ以外(中間の時間)なら、その時間の値を補間計算で求める
			else {
				auto& prev = m_scales[keyIdx - 1];	// 前のキー
				auto& next = m_scales[keyIdx];		// 次のキー
				// 前のキーと次のキーの時間から、0～1間の時間を求める
				float f = (time - prev.m_time) / (next.m_time - prev.m_time);
				// 補間
				result = DirectX::XMVectorLerp(
					prev.m_vec,
					next.m_vec,
					f
				);
			}

			return true;
		}

		void KdAnimationData::Node::Interpolate(Matrix& rDst, float time)
		{
			// ベクターによる拡縮補間
			bool isChange = false;
			Matrix scale;
			Vector3 resultVec;
			if (InterpolateScales(resultVec, time))
			{
				scale = scale.CreateScale(resultVec);
				isChange = true;
			}

			// クォタニオンによる回転補間
			Matrix rotate;
			Quaternion resultQuat;
			if (InterpolateRotations(resultQuat, time))
			{
				rotate = rotate.CreateFromQuaternion(resultQuat);
				isChange = true;
			}

			// ベクターによる座標補間
			Matrix trans;
			if (InterpolateTranslations(resultVec, time))
			{
				trans = trans.CreateTranslation(resultVec);
				isChange = true;
			}

			if (isChange)
			{
				rDst = scale * rotate * trans;
			}
		}

		/*
		void KdAnimator::AdvanceTime(std::vector<Module::Renderer::D3D11::IModelWork::Node>& rNodes, float speed)
		{
			if (!m_spAnimation) { return; }
			m_isLooped = false;
			// 全てのアニメーションノード（モデルの行列を補間する情報）の行列補間を実行する
			for (auto& rAnimNode : m_spAnimation->m_nodes)
			{
				// 対応するモデルノードのインデックス
				UINT idx = rAnimNode.m_nodeOffset;

				auto prev = rNodes[idx].m_localTransform;

				// アニメーションデータによる行列補間
				rAnimNode.Interpolate(rNodes[idx].m_localTransform, m_time);

				prev = rNodes[idx].m_localTransform;
			}

			// アニメーションのフレームを進める
			m_time += speed;

			// アニメーションデータの最後のフレームを超えたら
			if (m_time >= m_spAnimation->m_maxLength)
			{
				if (m_isLoop)
				{
					// アニメーションの最初に戻る（ループさせる
					m_isLooped = true;
					m_time = 0.0f;
				}
				else
				{
					m_time = m_spAnimation->m_maxLength;
				}
			}
		}
		*/

		void KdAnimator::CreateBonesTrans(std::vector<Vector3>& _poses, std::vector<Quaternion>& _quats, std::vector<Vector3>& _scales)
		{
			if (!m_spAnimation) { return; }
			_poses.clear(); _quats.clear(); _scales.clear();
			// 全てのアニメーションノード（モデルの行列を補間する情報）の行列補間を実行する
			// アニメーションデータによる行列補間
			for (auto& rAnimNode : m_spAnimation->m_nodes)
			{
				// ベクターによる拡縮補間
				DirectX::SimpleMath::Vector3 resultVec;
				if (rAnimNode.InterpolateScales(resultVec, m_time))
				{
					_scales.push_back(resultVec);
				}

				// クォタニオンによる回転補間
				DirectX::SimpleMath::Quaternion resultQuat;
				if (rAnimNode.InterpolateRotations(resultQuat, m_time))
				{
					_quats.push_back(resultQuat);
				}

				// ベクターによる座標補間
				if (rAnimNode.InterpolateTranslations(resultVec, m_time))
				{
					_poses.push_back(resultVec);
				}
			}
		}

		void KdAnimator::CreateBonesTrans(std::vector<Matrix>& _mats)
		{
			if (!m_spAnimation) { return; }
			_mats.clear();
			// 全てのアニメーションノード（モデルの行列を補間する情報）の行列補間を実行する
			for (auto& rAnimNode : m_spAnimation->m_nodes)
			{
				DirectX::SimpleMath::Matrix mat;
				// アニメーションデータによる行列補間
				rAnimNode.Interpolate(mat, m_time);
				_mats.push_back(mat);
			}
		}
	};
};