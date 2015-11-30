// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: seniordesign.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "seniordesign.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace seniordesign {

namespace {

const ::google::protobuf::Descriptor* ProcessingResults_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ProcessingResults_reflection_ = NULL;
const ::google::protobuf::Descriptor* ProcessingAck_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ProcessingAck_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* Direction_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_seniordesign_2eproto() {
  protobuf_AddDesc_seniordesign_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "seniordesign.proto");
  GOOGLE_CHECK(file != NULL);
  ProcessingResults_descriptor_ = file->message_type(0);
  static const int ProcessingResults_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ProcessingResults, direction_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ProcessingResults, confidence_),
  };
  ProcessingResults_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ProcessingResults_descriptor_,
      ProcessingResults::default_instance_,
      ProcessingResults_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ProcessingResults, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ProcessingResults, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ProcessingResults));
  ProcessingAck_descriptor_ = file->message_type(1);
  static const int ProcessingAck_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ProcessingAck, timestamp_),
  };
  ProcessingAck_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ProcessingAck_descriptor_,
      ProcessingAck::default_instance_,
      ProcessingAck_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ProcessingAck, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ProcessingAck, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ProcessingAck));
  Direction_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_seniordesign_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ProcessingResults_descriptor_, &ProcessingResults::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ProcessingAck_descriptor_, &ProcessingAck::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_seniordesign_2eproto() {
  delete ProcessingResults::default_instance_;
  delete ProcessingResults_reflection_;
  delete ProcessingAck::default_instance_;
  delete ProcessingAck_reflection_;
}

void protobuf_AddDesc_seniordesign_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\022seniordesign.proto\022\014seniordesign\"S\n\021Pr"
    "ocessingResults\022*\n\tdirection\030\001 \002(\0162\027.sen"
    "iordesign.Direction\022\022\n\nconfidence\030\002 \002(\001\""
    "\"\n\rProcessingAck\022\021\n\ttimestamp\030\001 \002(\004*H\n\tD"
    "irection\022\013\n\007NEUTRAL\020\000\022\013\n\007FORWARD\020\001\022\014\n\010BA"
    "CKWARD\020\002\022\010\n\004LEFT\020\003\022\t\n\005RIGHT\020\004", 229);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "seniordesign.proto", &protobuf_RegisterTypes);
  ProcessingResults::default_instance_ = new ProcessingResults();
  ProcessingAck::default_instance_ = new ProcessingAck();
  ProcessingResults::default_instance_->InitAsDefaultInstance();
  ProcessingAck::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_seniordesign_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_seniordesign_2eproto {
  StaticDescriptorInitializer_seniordesign_2eproto() {
    protobuf_AddDesc_seniordesign_2eproto();
  }
} static_descriptor_initializer_seniordesign_2eproto_;
const ::google::protobuf::EnumDescriptor* Direction_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return Direction_descriptor_;
}
bool Direction_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
      return true;
    default:
      return false;
  }
}


// ===================================================================

#ifndef _MSC_VER
const int ProcessingResults::kDirectionFieldNumber;
const int ProcessingResults::kConfidenceFieldNumber;
#endif  // !_MSC_VER

ProcessingResults::ProcessingResults()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:seniordesign.ProcessingResults)
}

void ProcessingResults::InitAsDefaultInstance() {
}

ProcessingResults::ProcessingResults(const ProcessingResults& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:seniordesign.ProcessingResults)
}

void ProcessingResults::SharedCtor() {
  _cached_size_ = 0;
  direction_ = 0;
  confidence_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ProcessingResults::~ProcessingResults() {
  // @@protoc_insertion_point(destructor:seniordesign.ProcessingResults)
  SharedDtor();
}

void ProcessingResults::SharedDtor() {
  if (this != default_instance_) {
  }
}

void ProcessingResults::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ProcessingResults::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ProcessingResults_descriptor_;
}

const ProcessingResults& ProcessingResults::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_seniordesign_2eproto();
  return *default_instance_;
}

ProcessingResults* ProcessingResults::default_instance_ = NULL;

ProcessingResults* ProcessingResults::New() const {
  return new ProcessingResults;
}

void ProcessingResults::Clear() {
#define OFFSET_OF_FIELD_(f) (reinterpret_cast<char*>(      \
  &reinterpret_cast<ProcessingResults*>(16)->f) - \
   reinterpret_cast<char*>(16))

#define ZR_(first, last) do {                              \
    size_t f = OFFSET_OF_FIELD_(first);                    \
    size_t n = OFFSET_OF_FIELD_(last) - f + sizeof(last);  \
    ::memset(&first, 0, n);                                \
  } while (0)

  ZR_(confidence_, direction_);

#undef OFFSET_OF_FIELD_
#undef ZR_

  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ProcessingResults::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:seniordesign.ProcessingResults)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .seniordesign.Direction direction = 1;
      case 1: {
        if (tag == 8) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::seniordesign::Direction_IsValid(value)) {
            set_direction(static_cast< ::seniordesign::Direction >(value));
          } else {
            mutable_unknown_fields()->AddVarint(1, value);
          }
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(17)) goto parse_confidence;
        break;
      }

      // required double confidence = 2;
      case 2: {
        if (tag == 17) {
         parse_confidence:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   double, ::google::protobuf::internal::WireFormatLite::TYPE_DOUBLE>(
                 input, &confidence_)));
          set_has_confidence();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:seniordesign.ProcessingResults)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:seniordesign.ProcessingResults)
  return false;
#undef DO_
}

void ProcessingResults::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:seniordesign.ProcessingResults)
  // required .seniordesign.Direction direction = 1;
  if (has_direction()) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->direction(), output);
  }

  // required double confidence = 2;
  if (has_confidence()) {
    ::google::protobuf::internal::WireFormatLite::WriteDouble(2, this->confidence(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:seniordesign.ProcessingResults)
}

::google::protobuf::uint8* ProcessingResults::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:seniordesign.ProcessingResults)
  // required .seniordesign.Direction direction = 1;
  if (has_direction()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->direction(), target);
  }

  // required double confidence = 2;
  if (has_confidence()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteDoubleToArray(2, this->confidence(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:seniordesign.ProcessingResults)
  return target;
}

int ProcessingResults::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .seniordesign.Direction direction = 1;
    if (has_direction()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->direction());
    }

    // required double confidence = 2;
    if (has_confidence()) {
      total_size += 1 + 8;
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void ProcessingResults::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ProcessingResults* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ProcessingResults*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ProcessingResults::MergeFrom(const ProcessingResults& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_direction()) {
      set_direction(from.direction());
    }
    if (from.has_confidence()) {
      set_confidence(from.confidence());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ProcessingResults::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ProcessingResults::CopyFrom(const ProcessingResults& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ProcessingResults::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void ProcessingResults::Swap(ProcessingResults* other) {
  if (other != this) {
    std::swap(direction_, other->direction_);
    std::swap(confidence_, other->confidence_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ProcessingResults::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ProcessingResults_descriptor_;
  metadata.reflection = ProcessingResults_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int ProcessingAck::kTimestampFieldNumber;
#endif  // !_MSC_VER

ProcessingAck::ProcessingAck()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:seniordesign.ProcessingAck)
}

void ProcessingAck::InitAsDefaultInstance() {
}

ProcessingAck::ProcessingAck(const ProcessingAck& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:seniordesign.ProcessingAck)
}

void ProcessingAck::SharedCtor() {
  _cached_size_ = 0;
  timestamp_ = GOOGLE_ULONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ProcessingAck::~ProcessingAck() {
  // @@protoc_insertion_point(destructor:seniordesign.ProcessingAck)
  SharedDtor();
}

void ProcessingAck::SharedDtor() {
  if (this != default_instance_) {
  }
}

void ProcessingAck::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ProcessingAck::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ProcessingAck_descriptor_;
}

const ProcessingAck& ProcessingAck::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_seniordesign_2eproto();
  return *default_instance_;
}

ProcessingAck* ProcessingAck::default_instance_ = NULL;

ProcessingAck* ProcessingAck::New() const {
  return new ProcessingAck;
}

void ProcessingAck::Clear() {
  timestamp_ = GOOGLE_ULONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ProcessingAck::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:seniordesign.ProcessingAck)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required uint64 timestamp = 1;
      case 1: {
        if (tag == 8) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &timestamp_)));
          set_has_timestamp();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:seniordesign.ProcessingAck)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:seniordesign.ProcessingAck)
  return false;
#undef DO_
}

void ProcessingAck::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:seniordesign.ProcessingAck)
  // required uint64 timestamp = 1;
  if (has_timestamp()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(1, this->timestamp(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:seniordesign.ProcessingAck)
}

::google::protobuf::uint8* ProcessingAck::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:seniordesign.ProcessingAck)
  // required uint64 timestamp = 1;
  if (has_timestamp()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(1, this->timestamp(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:seniordesign.ProcessingAck)
  return target;
}

int ProcessingAck::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required uint64 timestamp = 1;
    if (has_timestamp()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt64Size(
          this->timestamp());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void ProcessingAck::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ProcessingAck* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ProcessingAck*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ProcessingAck::MergeFrom(const ProcessingAck& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_timestamp()) {
      set_timestamp(from.timestamp());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ProcessingAck::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ProcessingAck::CopyFrom(const ProcessingAck& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ProcessingAck::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void ProcessingAck::Swap(ProcessingAck* other) {
  if (other != this) {
    std::swap(timestamp_, other->timestamp_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ProcessingAck::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ProcessingAck_descriptor_;
  metadata.reflection = ProcessingAck_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace seniordesign

// @@protoc_insertion_point(global_scope)
