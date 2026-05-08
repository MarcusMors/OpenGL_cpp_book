#ifndef __SHADER_SOURCE_H__
#define __SHADER_SOURCE_H__

#include <string>
#include <string_view>

class Shader_Source
{
public:
  // Constructor from an initializer_list of unsigned char (the type produced by #embed).
  // This allows us to write: Shader_Source{ #embed "file.glsl" }
  Shader_Source(std::initializer_list<unsigned char> init) : m_source(init.begin(), init.end()) {}

  // Access the source code as a string view (non-owning).
  std::string_view view() const noexcept { return m_source; }

  // Implicit conversion to std::string_view for convenience.
  operator std::string_view() const noexcept { return view(); }

  bool empty() const noexcept { return m_source.empty(); }

private:
  std::string m_source;
};

#endif// __SHADER_SOURCE_H__
