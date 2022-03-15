#pragma once

namespace ChrTrcProfiler
{
	namespace ChrTrcCompileTimeStr
	{
		/// @brief a simple class responsible for concatenating string literals at compile time.
		/// in order to quickly concatenate compiletime strings, see the operator+(CTStr, CTStr)
		/// @tparam the length of the string
		template<size_t N>
		class CTStr
		{
			char c_str[N] = {0};

		public:

			/// @brief constructor
			/// constructs an initial CTStr out of a string literal
			constexpr CTStr(const char(&lit)[N])
			{
				for (size_t i = 0; i < N; i++)
					c_str[i] = lit[i];
			}

			/// @brief concatenate constructor
			/// constructs an CTStr which represents the two passed CTStr's concatenated.
			template<size_t LEN_A, size_t LEN_B, std::enable_if_t<LEN_A + LEN_B - 1 == N, bool> = false>
			constexpr CTStr(CTStr<LEN_A> lit_a, CTStr<LEN_B> lit_b)
			{
				for (size_t i = 0; i < LEN_A - 1; i++)
					c_str[i] = lit_a.data()[i];

				for (size_t i = 0; i < LEN_B; i++)
					c_str[LEN_A - 1 + i] = lit_b.data()[i];
			}

			/// @return the CTStr's string data. 
			constexpr const char(&data() const)[N] { return c_str; }

		};

		/// @brief operator+ for CTStr + CTStr
		/// @return CTStr containing the two passed CTStr's concatenated.
		template<size_t LEN_A, size_t LEN_B>
		constexpr inline CTStr<LEN_A + LEN_B - 1> operator+(CTStr<LEN_A> lit_a, CTStr<LEN_B> lit_b)
		{
			return CTStr<LEN_A + LEN_B - 1>(lit_a, lit_b);
		}

		/// @brief operator+ for CTStr + c_str
		/// @return CTStr containing the two passed strings concatenated.
		template<size_t LEN_A, size_t LEN_B>
		constexpr inline CTStr<LEN_A + LEN_B - 1> operator+(CTStr<LEN_A> lit_a, const char(&lit_b)[LEN_B])
		{
			return lit_a + CTStr(lit_b);
		}

		/// @brief operator+ for c_str + CTStr
		/// @return CTStr containing the two passed strings concatenated.
		template<size_t LEN_A, size_t LEN_B>
		constexpr inline CTStr<LEN_A + LEN_B - 1> operator+(const char(&lit_a)[LEN_A], CTStr<LEN_B> lit_b)
		{
			return CTStr(lit_a) + lit_b;
		}
	}
}