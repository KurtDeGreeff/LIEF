/* Copyright 2017 R. Thomas
 * Copyright 2017 Quarkslab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <yaml-cpp/yaml.h>

#include <LIEF/ELF.hpp>

#include "utils.hpp"

extern const YAML::Node config = YAML::LoadFile(std::string(PATH_TO_CONFIG) + "/config.yaml");

using namespace LIEF::ELF;

TEST_CASE("Test parse", "[elf][builder]")
{

  using namespace Catch::Generators;
  std::vector<std::string> elf_files = Test::get_elf_files();

  // Get one
  std::vector<std::string>::iterator elf_file = between(
      std::begin(elf_files),
      std::prev(std::end(elf_files)));
  const std::string& elf_file_str = *elf_file;
  if (elf_file_str.find("binary_tiny") != std::string::npos) {
    INFO("Skip " << elf_file_str);
    return;
  }
  INFO("Binary used: " << elf_file_str);

  std::unique_ptr<Binary> binary_original;
  try {
     binary_original = std::unique_ptr<Binary>{Parser::parse(elf_file_str)};
  } catch (const LIEF::exception& e) {
    WARN("Can't parse: '" << elf_file_str << "' (" << e.what() << ")");
    return;
  }

  std::string output_name = binary_original->name() + "_built";
  binary_original->write(output_name);
  std::unique_ptr<Binary> binary_build{Parser::parse(output_name)};

  SECTION("Header") {
    REQUIRE(binary_original->get_header() == binary_build->get_header());
  }


  SECTION("Sections") {
    it_sections sections_lhs = binary_original->get_sections();
    it_sections sections_rhs = binary_build->get_sections();
    REQUIRE(sections_lhs.size() == sections_rhs.size());

    for (size_t i = 0; i < sections_lhs.size(); ++i) {
      REQUIRE(sections_lhs[i] == sections_rhs[i]);
    }
  }

  SECTION("Segments") {
    it_segments segment_lhs = binary_original->get_segments();
    it_segments segment_rhs = binary_build->get_segments();
    REQUIRE(segment_lhs.size() == segment_rhs.size());

    //TODO:
    //for (size_t i = 0; i < segment_rhs.size(); ++i) {
    //  REQUIRE(segment_lhs[i] == segment_rhs[i]);
    //}
  }

  SECTION("Dynamic symbols") {
    it_symbols symbols_lhs = binary_original->get_dynamic_symbols();
    it_symbols symbols_rhs = binary_build->get_dynamic_symbols();
    REQUIRE(symbols_lhs.size() == symbols_rhs.size());

    for (size_t i = 0; i < symbols_rhs.size(); ++i) {
      REQUIRE(symbols_lhs[i] == symbols_rhs[i]);
    }
  }

  SECTION("Static symbols") {
    it_symbols symbols_lhs = binary_original->get_static_symbols();
    it_symbols symbols_rhs = binary_build->get_static_symbols();
    REQUIRE(symbols_lhs.size() == symbols_rhs.size());

    for (size_t i = 0; i < symbols_rhs.size(); ++i) {
      REQUIRE(symbols_lhs[i] == symbols_rhs[i]);
    }
  }


  SECTION("Symbols version") {
    it_symbols_version symbols_version_lhs = binary_original->get_symbols_version();
    it_symbols_version symbols_version_rhs = binary_build->get_symbols_version();
    REQUIRE(symbols_version_lhs.size() == symbols_version_rhs.size());

    for (size_t i = 0; i < symbols_version_rhs.size(); ++i) {
      REQUIRE(symbols_version_lhs[i] == symbols_version_rhs[i]);
    }
  }


  SECTION("Symbols version definition") {
    it_symbols_version_definition symbols_version_definition_lhs = binary_original->get_symbols_version_definition();
    it_symbols_version_definition symbols_version_definition_rhs = binary_build->get_symbols_version_definition();
    REQUIRE(symbols_version_definition_lhs.size() == symbols_version_definition_rhs.size());

    for (size_t i = 0; i < symbols_version_definition_lhs.size(); ++i) {
      REQUIRE(symbols_version_definition_lhs[i] == symbols_version_definition_rhs[i]);
    }
  }


  SECTION("Symbols version requirement") {
    it_symbols_version_requirement symbols_version_requirement_lhs = binary_original->get_symbols_version_requirement();
    it_symbols_version_requirement symbols_version_requirement_rhs = binary_build->get_symbols_version_requirement();
    REQUIRE(symbols_version_requirement_lhs.size() == symbols_version_requirement_rhs.size());

    for (size_t i = 0; i < symbols_version_requirement_lhs.size(); ++i) {
      REQUIRE(symbols_version_requirement_lhs[i] == symbols_version_requirement_lhs[i]);
    }
  }

  SECTION("Dynamic relocations") {
    it_relocations dynamic_relocations_lhs = binary_original->get_dynamic_relocations();
    it_relocations dynamic_relocations_rhs = binary_build->get_dynamic_relocations();
    REQUIRE(dynamic_relocations_lhs.size() == dynamic_relocations_rhs.size());

    for (size_t i = 0; i < dynamic_relocations_lhs.size(); ++i) {
      REQUIRE(dynamic_relocations_lhs[i] == dynamic_relocations_rhs[i]);
    }
  }

  SECTION(".plt.got relocations") {
    it_relocations pltgot_relocations_lhs = binary_original->get_pltgot_relocations();
    it_relocations pltgot_relocations_rhs = binary_build->get_pltgot_relocations();
    REQUIRE(pltgot_relocations_lhs.size() == pltgot_relocations_rhs.size());

    for (size_t i = 0; i < pltgot_relocations_lhs.size(); ++i) {
      REQUIRE(pltgot_relocations_lhs[i] == pltgot_relocations_rhs[i]);
    }
  }



}
