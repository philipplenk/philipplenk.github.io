#include <array>
#include <type_traits>

#include <cstddef>
#include <cstdint>

class byte_proxy
{
	public:
	constexpr byte_proxy(std::uint16_t& target, unsigned shift) noexcept:
		target_{target},
		shift_{shift}
	{}
	
	constexpr operator std::uint8_t() const noexcept { return (target_>>shift_)&0xff; }
	
	constexpr byte_proxy& operator=(std::uint8_t val) noexcept
	{
		target_&=~((0xff)<<shift_);
		target_|=(std::uint16_t{val}<<shift_);
		return *this;
	}
	
	private:
	std::uint16_t& target_;
	unsigned shift_;
};

template <typename T, typename enum_type, ::std::size_t number_of_values>
class enum_map 
{
    public:
	using underlying=std::underlying_type_t<enum_type>;
	
	constexpr auto& operator[](enum_type v) noexcept
	{
		return data[static_cast<underlying>(v)];
	}
	
	constexpr const auto& operator[](enum_type v) const noexcept
	{
		return data[static_cast<underlying>(v)];
	}
	
	constexpr auto size() const { return number_of_values; }
	
    private:
	std::array<T,number_of_values> data{};
};

template <typename T, typename enum_type>
using enum_map_with_num=enum_map<T,enum_type,static_cast<std::underlying_type_t<enum_type>>(enum_type::num)>;

enum class register8
{
	C,B,E,D,L,H,F,A, num
};
	
enum class register16
{
	BC, DE, HL, AF, SP, PC, num
};

class simple_cpustate_enum_proxy
{
	public:
	constexpr std::uint16_t& operator[](register16 id) noexcept
	{
		return registers_[id];
	}

	constexpr byte_proxy operator[](register8 id) noexcept
	{
		const auto idx=static_cast<::std::underlying_type_t<register8>>(id);
		return {registers_[register16{idx/2}],(idx%2)*8u};
	}
	
	private:
	enum_map_with_num<std::uint16_t, register16> registers_;
};

union register_union
{
    std::uint16_t combined; std::uint8_t parts[2];
};

struct simple_cpustate_union
{
    register_union BC, DE, HL, AF, SP, PC;
};

simple_cpustate_enum_proxy magic_enum();
simple_cpustate_union magic_union();

int test_enum()
{
    auto state=magic_enum();
    return state[register8::A]+state[register16::SP];
}

int test_union()
{
    auto state=magic_union();
    return state.AF.parts[1]+state.SP.combined;
}

int test_enum2()
{
    auto state=magic_enum();
    return state[register8::F]+state[register16::SP];
}

int test_union2()
{
    auto state=magic_union();
    return state.AF.parts[0]+state.SP.combined;
}

simple_cpustate_enum_proxy& magic_enum_ref();
simple_cpustate_union& magic_union_ref();

void test_enum3(int v)
{
    auto& state=magic_enum_ref();
    state[register8::F]=v;
}

void test_union3(int v)
{
    auto& state=magic_union_ref();
    state.AF.parts[0]=v;
}

void test_enum4(int v)
{
    auto& state=magic_enum_ref();
    state[register8::A]=v;
}

void test_union4(int v)
{
    auto& state=magic_union_ref();
    state.AF.parts[1]=v;
}
