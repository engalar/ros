/*
 * Copyright (C) 2009, Willow Garage, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the names of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ROSLIB_MESSAGE_TRAITS_H
#define ROSLIB_MESSAGE_TRAITS_H

#include <ros/time.h>

#include <string>
#include <boost/utility/enable_if.hpp>

#define ROS_FORWARD_DECLARE_MESSAGE_WITH_ALLOCATOR(ns, msg, new_name, alloc) \
  namespace ns \
  { \
    template<template <typename T> class Allocator > struct msg##_; \
    typedef msg##_<alloc> new_name; \
    typedef boost::shared_ptr<msg> new_name##Ptr; \
    typedef boost::shared_ptr<msg const> new_name##ConstPtr; \
  }

#define ROS_FORWARD_DECLARE_MESSAGE(ns, msg, new_name) ROS_FORWARD_DECLARE_MESSAGE_WITH_ALLOCATOR(ns, msg, new_name, std::allocator)

ROS_FORWARD_DECLARE_MESSAGE(roslib, Header, Header);

#define ROS_TYPEDEF_MESSAGE_WITH_ALLOCATOR(ns, base_name, new_name, allocator) \
  ROS_FORWARD_DECLARE_MESSAGE_WITH_ALLOCATOR(ns, base_name, new_name, allocator);

namespace ros
{
namespace message_traits
{

struct TrueType
{
  static const bool value = true;
  typedef TrueType type;
};

struct FalseType
{
  static const bool value = false;
  typedef FalseType type;
};

template<typename M> struct IsSimple : public FalseType {};
template<typename M> struct IsFixedSize : public FalseType {};
template<typename M> struct HasHeader : public FalseType {};

template<typename M>
struct MD5Sum
{
  static const char* value()
  {
    return M::__s_getMD5Sum().c_str();
  }

  static const char* value(const M& m)
  {
    return m.__getMD5Sum().c_str();
  }
};

template<typename M>
struct DataType
{
  static const char* value()
  {
    return M::__s_getDataType().c_str();
  }

  static const char* value(const M& m)
  {
    return m.__getDataType().c_str();
  }
};

template<typename M>
struct Definition
{
  static const char* value()
  {
    return M::__s_getMessageDefinition().c_str();
  }

  static const char* value(const M& m)
  {
    return m.__getMessageDefinition().c_str();
  }
};

template<typename M, typename Enable = void>
struct Header
{
  static roslib::Header* pointer(M& m) { return 0; }
  static roslib::Header const* pointer(const M& m) { return 0; }
};

template<typename M>
struct Header<M, typename boost::enable_if<HasHeader<M> >::type >
{
  static roslib::Header* pointer(M& m) { return &m.header; }
  static roslib::Header const* pointer(const M& m) { return &m.header; }
};

template<typename M, typename Enable = void>
struct FrameId
{
  static std::string* pointer(M& m) { return 0; }
  static std::string const* pointer(const M& m) { return 0; }
  static std::string value(const M& m) { return std::string(); }
};

template<typename M>
struct FrameId<M, typename boost::enable_if<HasHeader<M> >::type >
{
  static std::string* pointer(M& m) { return &m.header.frame_id; }
  static std::string const* pointer(const M& m) { return &m.header.frame_id; }
  static std::string value(const M& m) { return m.header.frame_id; }
};

template<typename M, typename Enable = void>
struct TimeStamp
{
  static ros::Time* pointer(M& m) { return 0; }
  static ros::Time const* pointer(const M& m) { return 0; }
  static ros::Time value(const M& m) { return ros::Time(); }
};

template<typename M>
struct TimeStamp<M, typename boost::enable_if<HasHeader<M> >::type >
{
  static ros::Time* pointer(M& m) { return &m.header.stamp; }
  static ros::Time const* pointer(const M& m) { return &m.header.stamp; }
  static ros::Time value(const M& m) { return m.header.stamp; }
};

template<typename M>
inline const char* md5sum()
{
  return MD5Sum<M>::value();
}

template<typename M>
inline const char* datatype()
{
  return DataType<M>::value();
}

template<typename M>
inline const char* definition()
{
  return Definition<M>::value();
}

template<typename M>
inline const char* md5sum(const M& m)
{
  return MD5Sum<M>::value(m);
}

template<typename M>
inline const char* datatype(const M& m)
{
  return DataType<M>::value(m);
}

template<typename M>
inline const char* definition(const M& m)
{
  return Definition<M>::value(m);
}

template<typename M>
inline roslib::Header* header(M& msg)
{
  return Header<M>::pointer(msg);
}

template<typename M>
inline roslib::Header const* header(const M& msg)
{
  return Header<M>::pointer(msg);
}

template<typename M>
inline std::string* frameId(M& msg)
{
  return FrameId<M>::pointer(msg);
}

template<typename M>
inline std::string const* frameId(const M& msg)
{
  return FrameId<M>::pointer(msg);
}

template<typename M>
inline ros::Time* timeStamp(M& msg)
{
  return TimeStamp<M>::pointer(msg);
}

template<typename M>
inline ros::Time const* timeStamp(const M& msg)
{
  return TimeStamp<M>::pointer(msg);
}

template<typename M>
inline bool isSimple()
{
  return IsSimple<M>::value;
}

template<typename M>
inline bool isFixedSize()
{
  return IsFixedSize<M>::value;
}

template<typename M>
inline bool hasHeader()
{
  return HasHeader<M>::value;
}

} // namespace message_traits
} // namespace ros

#endif // ROSLIB_MESSAGE_TRAITS_H
