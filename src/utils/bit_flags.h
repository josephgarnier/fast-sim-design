////////////////////////////////////////////////////////////
/// Copyright 2024-present, Zaven Muradyan. Adapted from this article:
/// https://voithos.io/articles/type-safe-enum-class-bit-flags/.
/// Copyright 2024-present, Joseph Garnier
/// All rights reserved.
///
/// This source code is licensed under the license found in the
/// LICENSE file in the root directory of this source tree.
///
////////////////////////////////////////////////////////////

#pragma once

#ifndef FAST_SIM_DESIGN_BIT_FLAGS_H
#define FAST_SIM_DESIGN_BIT_FLAGS_H

#include <bitset>
#include <ostream>
#include <type_traits>

namespace FastSimDesign {
////////////////////////////////////////////////////////////
///
/// Helper class for bitwise flag-like operations on scoped enums.
///
/// This class provides a way to represent combinations of enum values without
/// directly overloading operators on the enum type itself. This approach
/// avoids ambiguity in the type system and allows the enum type to continue
/// representing a single value, while the BitFlags can hold a combination
/// of enum values.
///
/// Example usage:
///
/// enum class MyEnum { FlagA = 1 << 0, FlagB = 1 << 1, FlagC = 1 << 2 };
///
/// BitFlags<MyEnum> flags = { MyEnum::FlagA, MyEnum::FlagC };
/// flags.Unset(MyEnum::FlagA);
/// if (flags.IsSet(MyEnum::FlagC)) {
///   // ...
/// }
///
/// flags |= MyEnum::FlagB;
/// BitFlags<MyEnum> new_flags = ~flags;
///
////////////////////////////////////////////////////////////
template<typename T, typename = std::enable_if_t<std::is_enum_v<T>>>
class BitFlags
{
public:
  using UnderlyingT = std::underlying_type_t<T>;

public:
  // Construct BitFlags from raw values.
  static constexpr BitFlags FromRaw(UnderlyingT flags) noexcept
  {
    return BitFlags(flags);
  }

private:
  static constexpr UnderlyingT ToUnderlying(T value) noexcept
  {
    return static_cast<UnderlyingT>(value);
  }

public:
  constexpr explicit BitFlags() noexcept
    : m_flags(static_cast<UnderlyingT>(0))
  {
  }
  constexpr explicit BitFlags(T value) noexcept
    : m_flags(ToUnderlying(value))
  {
  }
  constexpr explicit BitFlags(std::initializer_list<T> values) noexcept
    : BitFlags{}
  {
    for (T value : values)
    {
      m_flags |= ToUnderlying(value);
    }
  }
  BitFlags(BitFlags const&) = default;
  BitFlags(BitFlags&&) = default;
  BitFlags& operator=(BitFlags const&) = default;
  BitFlags& operator=(BitFlags&&) = default;
  virtual ~BitFlags() = default;

  // Checks if a specific flag is set.
  constexpr bool isSet(T value) const noexcept
  {
    return (m_flags & ToUnderlying(value)) == ToUnderlying(value);
  }

  // Sets a single flag value.
  constexpr void set(T value) noexcept { m_flags |= ToUnderlying(value); }

  // Unsets a single flag value.
  constexpr void unset(T value) noexcept { m_flags &= ~ToUnderlying(value); }

  // Clears all flag values.
  constexpr void clear() noexcept { m_flags = static_cast<UnderlyingT>(0); }

  // Retrieve the raw underlying flags.
  constexpr UnderlyingT toRaw() const noexcept { return m_flags; }

  constexpr T toUnderType() const noexcept { return m_flags; }

  inline constexpr operator bool() const noexcept
  {
    return m_flags != static_cast<UnderlyingT>(0);
  }

  template<typename U>
  friend inline constexpr std::ostream& operator<<(
      std::ostream& stream, BitFlags<U> const& left) noexcept;

  template<typename U>
  friend inline constexpr BitFlags<U> operator|(
      BitFlags<U> const& left, U const& right) noexcept;
  template<typename U>
  friend inline constexpr BitFlags<U> operator|(
      BitFlags<U> const& left, BitFlags<U> const& right) noexcept;

  template<typename U>
  friend inline constexpr BitFlags<U> operator&(
      BitFlags<U> const& left, U const& right) noexcept;
  template<typename U>
  friend inline constexpr BitFlags<U> operator&(
      BitFlags<U> const& left, BitFlags<U> const& right) noexcept;

  template<typename U>
  friend inline constexpr BitFlags<U> operator^(
      BitFlags<U> const& left, U const& right) noexcept;
  template<typename U>
  friend inline constexpr BitFlags<U> operator^(
      BitFlags<U> const& left, BitFlags<U> const& right) noexcept;

  template<typename U>
  friend inline constexpr BitFlags<U>& operator|=(
      BitFlags<U>& left, U const& right) noexcept;
  template<typename U>
  friend inline constexpr BitFlags<U>& operator|=(
      BitFlags<U>& left, BitFlags<U> const& right) noexcept;

  template<typename U>
  friend inline constexpr BitFlags<U>& operator&=(
      BitFlags<U>& left, U const& right) noexcept;
  template<typename U>
  friend inline constexpr BitFlags<U>& operator&=(
      BitFlags<U>& left, BitFlags<U> const& right) noexcept;

  template<typename U>
  friend inline constexpr BitFlags<U>& operator^=(
      BitFlags<U>& left, U const& right) noexcept;
  template<typename U>
  friend inline constexpr BitFlags<U>& operator^=(
      BitFlags<U>& left, BitFlags<U> const& right) noexcept;

  template<typename U>
  friend inline constexpr BitFlags<U> operator~(BitFlags<U> const& bf) noexcept;

  template<typename U>
  friend inline constexpr bool operator==(
      BitFlags<U> const& left, BitFlags<U> const& right) noexcept;
  template<typename U>
  friend inline constexpr bool operator!=(
      BitFlags<U> const& left, BitFlags<U> const& right) noexcept;

protected:
private:
  constexpr explicit BitFlags(UnderlyingT flags) noexcept
    : m_flags{flags}
  {
  }

public:
protected:
private:
  UnderlyingT m_flags;
};

////////////////////////////////////////////////////////////
/// Outside class declarations/definitions
////////////////////////////////////////////////////////////
template<typename T>
inline constexpr std::ostream& operator<<(
    std::ostream& stream, BitFlags<T> const& left) noexcept
{
  // Write out a bitset representation.
  stream << std::bitset<sizeof(BitFlags<T>::UnderlyingT) * 8>(left.m_flags);
  return stream;
}

template<typename T>
inline constexpr BitFlags<T> operator|(
    BitFlags<T> const& left, T const& right) noexcept
{
  return BitFlags<T>(left.m_flags | BitFlags<T>::ToUnderlying(right));
}
template<typename T>
inline constexpr BitFlags<T> operator|(
    BitFlags<T> const& left, BitFlags<T> const& right) noexcept
{
  return BitFlags<T>(left.m_flags | right.m_flags);
}

template<typename T>
inline constexpr BitFlags<T> operator&(
    BitFlags<T> const& left, T const& right) noexcept
{
  return BitFlags<T>(left.m_flags & BitFlags<T>::ToUnderlying(right));
}
template<typename T>
inline constexpr BitFlags<T> operator&(
    BitFlags<T> const& left, BitFlags<T> const& right) noexcept
{
  return BitFlags<T>(left.m_flags & right.m_flags);
}

template<typename T>
inline constexpr BitFlags<T> operator^(
    BitFlags<T> const& left, T const& right) noexcept
{
  return BitFlags<T>(left.m_flags ^ BitFlags<T>::ToUnderlying(right));
}
template<typename T>
inline constexpr BitFlags<T> operator^(
    BitFlags<T> const& left, BitFlags<T> const& right) noexcept
{
  return BitFlags<T>(left.m_flags ^ right.m_flags);
}

template<typename T>
inline constexpr BitFlags<T>& operator|=(
    BitFlags<T>& left, T const& right) noexcept
{
  left.m_flags |= BitFlags<T>::ToUnderlying(right);
  return left;
}
template<typename T>
inline constexpr BitFlags<T>& operator|=(
    BitFlags<T>& left, BitFlags<T> const& right) noexcept
{
  left.m_flags |= right.m_flags;
  return left;
}

template<typename T>
inline constexpr BitFlags<T>& operator&=(
    BitFlags<T>& left, T const& right) noexcept
{
  left.m_flags &= BitFlags<T>::ToUnderlying(right);
  return left;
}
template<typename T>
inline constexpr BitFlags<T>& operator&=(
    BitFlags<T>& left, BitFlags<T> const& right) noexcept
{
  left.m_flags &= right.m_flags;
  return left;
}

template<typename T>
inline constexpr BitFlags<T>& operator^=(
    BitFlags<T>& left, T const& right) noexcept
{
  left.m_flags ^= BitFlags<T>::ToUnderlying(right);
  return left;
}
template<typename T>
inline constexpr BitFlags<T>& operator^=(
    BitFlags<T>& left, BitFlags<T> const& right) noexcept
{
  left.m_flags ^= right.m_flags;
  return left;
}

template<typename T>
inline constexpr BitFlags<T> operator~(BitFlags<T> const& bf) noexcept
{
  return BitFlags<T>(~bf.m_flags);
}

template<typename T>
inline constexpr bool operator==(
    BitFlags<T> const& left, BitFlags<T> const& right) noexcept
{
  return left.m_flags == right.m_flags;
}
template<typename T>
inline constexpr bool operator!=(
    BitFlags<T> const& left, BitFlags<T> const& right) noexcept
{
  return left.m_flags != right.m_flags;
}
} // namespace FastSimDesign
#endif