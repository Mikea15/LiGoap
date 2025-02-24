// Michael Adaixo - 2025

#pragma once

#include <cassert>
#include <cstdint>
#include <string_view>

class StringHash
{
public:
	template <size_t N>
	constexpr StringHash(const char (&str)[N])
		: hash_value(HashStr(str, N - 1))
	{
		static_assert(N-1 <= MAX_SAFE_LENGTH, "String should be smaller than MAX_SAFE_LENGTH");
	}
	
	constexpr StringHash(std::string_view str)
		: hash_value(HashStr(str.data(), str.length()))
	{
		assert(str.length()-1 <= MAX_SAFE_LENGTH && "String should be smaller than MAX_SAFE_LENGTH");
	}
	
	constexpr operator uint64_t() const { return hash_value; }
	constexpr uint64_t value() const { return hash_value; }

	constexpr bool operator==(const StringHash& other) const { return hash_value == other.hash_value; }
	constexpr bool operator!=(const StringHash& other) const { return hash_value != other.hash_value; }

private:
	uint64_t hash_value;

	// Polynomial Rolling Hash
	static constexpr uint64_t MOD = 1000000007; // large prime to reduce chance of collision
	static constexpr uint64_t BASE = 31;

	// Could be as large as 1000, but any hard coded string longer than this need a different class tbh
	static constexpr size_t MAX_SAFE_LENGTH = 500; 

	// Helper for compile-time power calculation
	static constexpr uint64_t ComputePower(size_t exp)
	{
		uint64_t result = 1;
		uint64_t base = BASE;
		while (exp > 0)
		{
			if (exp & 1)
			{
				result = (result * base) % MOD;
			}
			base = (base * base) % MOD;
			exp >>= 1;
		}
		return result;
	}

	// Helper for compile-time hash calculation
	static constexpr uint64_t HashStr(const char* str, size_t length)
	{
		static_assert(std::is_same_v<uint64_t, unsigned long long>, "uint64_t must be unsigned long long");

		uint64_t hash = 0;
		for (size_t i = 0; i < length; ++i)
		{
			hash = (hash + (str[i] - 'a' + 1) * ComputePower(i)) % MOD;
		}

		return hash;
	}
};

// String literal operator (optional, allows for "hello"_hash syntax)
constexpr StringHash operator""_hash(const char* str, size_t len)
{
	return StringHash(std::string_view(str, len));
}
