// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef GOOGLE_PROTOBUF_COMPILER_CSHARP_FIELD_BASE_H__
#define GOOGLE_PROTOBUF_COMPILER_CSHARP_FIELD_BASE_H__

#include <string>

#include "google/protobuf/compiler/code_generator.h"
#include "y_absl/container/flat_hash_map.h"
#include "y_absl/strings/ascii.h"
#include "y_absl/strings/escaping.h"
#include "y_absl/strings/str_replace.h"
#include "y_absl/strings/str_split.h"
#include "google/protobuf/compiler/csharp/csharp_source_generator_base.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/io/printer.h"

namespace google {
namespace protobuf {
namespace compiler {
namespace csharp {

class FieldGeneratorBase : public SourceGeneratorBase {
 public:
  FieldGeneratorBase(const FieldDescriptor* descriptor,
                     int presenceIndex,
                     const Options* options);
  ~FieldGeneratorBase();

  FieldGeneratorBase(const FieldGeneratorBase&) = delete;
  FieldGeneratorBase& operator=(const FieldGeneratorBase&) = delete;

  virtual void GenerateCloningCode(io::Printer* printer) = 0;
  virtual void GenerateFreezingCode(io::Printer* printer);
  virtual void GenerateCodecCode(io::Printer* printer);
  virtual void GenerateExtensionCode(io::Printer* printer);
  virtual void GenerateMembers(io::Printer* printer) = 0;
  virtual void GenerateMergingCode(io::Printer* printer) = 0;
  virtual void GenerateParsingCode(io::Printer* printer) = 0;
  virtual void GenerateParsingCode(io::Printer* printer, bool use_parse_context);
  virtual void GenerateSerializationCode(io::Printer* printer) = 0;
  virtual void GenerateSerializationCode(io::Printer* printer, bool use_write_context);
  virtual void GenerateSerializedSizeCode(io::Printer* printer) = 0;

  virtual void WriteHash(io::Printer* printer) = 0;
  virtual void WriteEquals(io::Printer* printer) = 0;
  // Currently unused, as we use reflection to generate JSON
  virtual void WriteToString(io::Printer* printer) = 0;

 protected:
  const FieldDescriptor* descriptor_;
  const int presenceIndex_;
  y_absl::flat_hash_map<y_absl::string_view, TProtoStringType> variables_;

  void AddDeprecatedFlag(io::Printer* printer);
  void AddNullCheck(io::Printer* printer);
  void AddNullCheck(io::Printer* printer, const TProtoStringType& name);

  void AddPublicMemberAttributes(io::Printer* printer);
  void SetCommonOneofFieldVariables(
      y_absl::flat_hash_map<y_absl::string_view, TProtoStringType>* variables);

  TProtoStringType oneof_property_name();
  TProtoStringType oneof_case_name(); 
  TProtoStringType oneof_name();
  TProtoStringType property_name();
  TProtoStringType name();
  TProtoStringType type_name();
  TProtoStringType type_name(const FieldDescriptor* descriptor);
  bool has_default_value();
  TProtoStringType default_value();
  TProtoStringType default_value(const FieldDescriptor* descriptor);
  TProtoStringType number();
  TProtoStringType capitalized_type_name();

 private:
  void SetCommonFieldVariables(
      y_absl::flat_hash_map<y_absl::string_view, TProtoStringType>* variables);
  TProtoStringType GetStringDefaultValueInternal(const FieldDescriptor* descriptor);
  TProtoStringType GetBytesDefaultValueInternal(const FieldDescriptor* descriptor);
};

}  // namespace csharp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google

#endif  // GOOGLE_PROTOBUF_COMPILER_CSHARP_FIELD_BASE_H__
