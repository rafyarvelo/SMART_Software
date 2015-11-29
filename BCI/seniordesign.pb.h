// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: seniordesign.proto

#ifndef PROTOBUF_seniordesign_2eproto__INCLUDED
#define PROTOBUF_seniordesign_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace seniordesign {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_seniordesign_2eproto();
void protobuf_AssignDesc_seniordesign_2eproto();
void protobuf_ShutdownFile_seniordesign_2eproto();

class ProcessingResults;
class ProcessingAck;

enum Direction {
  SD_NEUTRAL = 0,
  SD_FORWARD = 1,
  SD_BACKWARD = 2,
  SD_LEFT = 3,
  SD_RIGHT = 4
};
bool Direction_IsValid(int value);
const Direction Direction_MIN = SD_NEUTRAL;
const Direction Direction_MAX = SD_RIGHT;
const int Direction_ARRAYSIZE = Direction_MAX + 1;

const ::google::protobuf::EnumDescriptor* Direction_descriptor();
inline const ::std::string& Direction_Name(Direction value) {
  return ::google::protobuf::internal::NameOfEnum(
    Direction_descriptor(), value);
}
inline bool Direction_Parse(
    const ::std::string& name, Direction* value) {
  return ::google::protobuf::internal::ParseNamedEnum<Direction>(
    Direction_descriptor(), name, value);
}
// ===================================================================

class ProcessingResults : public ::google::protobuf::Message {
 public:
  ProcessingResults();
  virtual ~ProcessingResults();

  ProcessingResults(const ProcessingResults& from);

  inline ProcessingResults& operator=(const ProcessingResults& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ProcessingResults& default_instance();

  void Swap(ProcessingResults* other);

  // implements Message ----------------------------------------------

  ProcessingResults* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ProcessingResults& from);
  void MergeFrom(const ProcessingResults& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required .seniordesign.Direction direction = 1;
  inline bool has_direction() const;
  inline void clear_direction();
  static const int kDirectionFieldNumber = 1;
  inline ::seniordesign::Direction direction() const;
  inline void set_direction(::seniordesign::Direction value);

  // required double confidence = 2;
  inline bool has_confidence() const;
  inline void clear_confidence();
  static const int kConfidenceFieldNumber = 2;
  inline double confidence() const;
  inline void set_confidence(double value);

  // @@protoc_insertion_point(class_scope:seniordesign.ProcessingResults)
 private:
  inline void set_has_direction();
  inline void clear_has_direction();
  inline void set_has_confidence();
  inline void clear_has_confidence();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  double confidence_;
  int direction_;
  friend void  protobuf_AddDesc_seniordesign_2eproto();
  friend void protobuf_AssignDesc_seniordesign_2eproto();
  friend void protobuf_ShutdownFile_seniordesign_2eproto();

  void InitAsDefaultInstance();
  static ProcessingResults* default_instance_;
};
// -------------------------------------------------------------------

class ProcessingAck : public ::google::protobuf::Message {
 public:
  ProcessingAck();
  virtual ~ProcessingAck();

  ProcessingAck(const ProcessingAck& from);

  inline ProcessingAck& operator=(const ProcessingAck& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const ProcessingAck& default_instance();

  void Swap(ProcessingAck* other);

  // implements Message ----------------------------------------------

  ProcessingAck* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ProcessingAck& from);
  void MergeFrom(const ProcessingAck& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required uint64 timestamp = 1;
  inline bool has_timestamp() const;
  inline void clear_timestamp();
  static const int kTimestampFieldNumber = 1;
  inline ::google::protobuf::uint64 timestamp() const;
  inline void set_timestamp(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:seniordesign.ProcessingAck)
 private:
  inline void set_has_timestamp();
  inline void clear_has_timestamp();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::uint64 timestamp_;
  friend void  protobuf_AddDesc_seniordesign_2eproto();
  friend void protobuf_AssignDesc_seniordesign_2eproto();
  friend void protobuf_ShutdownFile_seniordesign_2eproto();

  void InitAsDefaultInstance();
  static ProcessingAck* default_instance_;
};
// ===================================================================


// ===================================================================

// ProcessingResults

// required .seniordesign.Direction direction = 1;
inline bool ProcessingResults::has_direction() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ProcessingResults::set_has_direction() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ProcessingResults::clear_has_direction() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ProcessingResults::clear_direction() {
  direction_ = 0;
  clear_has_direction();
}
inline ::seniordesign::Direction ProcessingResults::direction() const {
  // @@protoc_insertion_point(field_get:seniordesign.ProcessingResults.direction)
  return static_cast< ::seniordesign::Direction >(direction_);
}
inline void ProcessingResults::set_direction(::seniordesign::Direction value) {
  assert(::seniordesign::Direction_IsValid(value));
  set_has_direction();
  direction_ = value;
  // @@protoc_insertion_point(field_set:seniordesign.ProcessingResults.direction)
}

// required double confidence = 2;
inline bool ProcessingResults::has_confidence() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ProcessingResults::set_has_confidence() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ProcessingResults::clear_has_confidence() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ProcessingResults::clear_confidence() {
  confidence_ = 0;
  clear_has_confidence();
}
inline double ProcessingResults::confidence() const {
  // @@protoc_insertion_point(field_get:seniordesign.ProcessingResults.confidence)
  return confidence_;
}
inline void ProcessingResults::set_confidence(double value) {
  set_has_confidence();
  confidence_ = value;
  // @@protoc_insertion_point(field_set:seniordesign.ProcessingResults.confidence)
}

// -------------------------------------------------------------------

// ProcessingAck

// required uint64 timestamp = 1;
inline bool ProcessingAck::has_timestamp() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ProcessingAck::set_has_timestamp() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ProcessingAck::clear_has_timestamp() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ProcessingAck::clear_timestamp() {
  timestamp_ = GOOGLE_ULONGLONG(0);
  clear_has_timestamp();
}
inline ::google::protobuf::uint64 ProcessingAck::timestamp() const {
  // @@protoc_insertion_point(field_get:seniordesign.ProcessingAck.timestamp)
  return timestamp_;
}
inline void ProcessingAck::set_timestamp(::google::protobuf::uint64 value) {
  set_has_timestamp();
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:seniordesign.ProcessingAck.timestamp)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace seniordesign

#ifndef SWIG
namespace google {
namespace protobuf {

template <> struct is_proto_enum< ::seniordesign::Direction> : ::google::protobuf::internal::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::seniordesign::Direction>() {
  return ::seniordesign::Direction_descriptor();
}

}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_seniordesign_2eproto__INCLUDED
