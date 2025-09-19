#pragma once


namespace Math
{
	template<class ArrayType, size_t N = 1>
	struct ArrayN
	{
		ArrayType v[N];

		ArrayN& operator+=(const ArrayN& _other)
		{
			for (size_t i = 0; i < N; ++i)
				v[i] += _other.v[i];
			return *this;
		}

		ArrayN& operator-=(const ArrayN& _other)
		{
			for (size_t i = 0; i < N; ++i)
				v[i] -= _other.v[i];
			return *this;
		}

		ArrayN& operator*=(const ArrayN& _other)
		{
			for (size_t i = 0; i < N; ++i)
				v[i] *= _other.v[i];
			return *this;
		}

	};

	using Vector4 = ArrayN<float, 4>;
	using Vector3 = ArrayN<float, 3>;
	using Vector2 = ArrayN<float, 2>;
};