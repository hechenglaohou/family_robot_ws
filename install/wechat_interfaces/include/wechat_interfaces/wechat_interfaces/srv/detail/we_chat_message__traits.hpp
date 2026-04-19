// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from wechat_interfaces:srv/WeChatMessage.idl
// generated code does not contain a copyright notice

#ifndef WECHAT_INTERFACES__SRV__DETAIL__WE_CHAT_MESSAGE__TRAITS_HPP_
#define WECHAT_INTERFACES__SRV__DETAIL__WE_CHAT_MESSAGE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "wechat_interfaces/srv/detail/we_chat_message__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace wechat_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const WeChatMessage_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: content
  {
    out << "content: ";
    rosidl_generator_traits::value_to_yaml(msg.content, out);
    out << ", ";
  }

  // member: msg_type
  {
    out << "msg_type: ";
    rosidl_generator_traits::value_to_yaml(msg.msg_type, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const WeChatMessage_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: content
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "content: ";
    rosidl_generator_traits::value_to_yaml(msg.content, out);
    out << "\n";
  }

  // member: msg_type
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "msg_type: ";
    rosidl_generator_traits::value_to_yaml(msg.msg_type, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const WeChatMessage_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace wechat_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use wechat_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const wechat_interfaces::srv::WeChatMessage_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  wechat_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use wechat_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const wechat_interfaces::srv::WeChatMessage_Request & msg)
{
  return wechat_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<wechat_interfaces::srv::WeChatMessage_Request>()
{
  return "wechat_interfaces::srv::WeChatMessage_Request";
}

template<>
inline const char * name<wechat_interfaces::srv::WeChatMessage_Request>()
{
  return "wechat_interfaces/srv/WeChatMessage_Request";
}

template<>
struct has_fixed_size<wechat_interfaces::srv::WeChatMessage_Request>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<wechat_interfaces::srv::WeChatMessage_Request>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<wechat_interfaces::srv::WeChatMessage_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace wechat_interfaces
{

namespace srv
{

inline void to_flow_style_yaml(
  const WeChatMessage_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << ", ";
  }

  // member: message
  {
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const WeChatMessage_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }

  // member: message
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const WeChatMessage_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace srv

}  // namespace wechat_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use wechat_interfaces::srv::to_block_style_yaml() instead")]]
inline void to_yaml(
  const wechat_interfaces::srv::WeChatMessage_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  wechat_interfaces::srv::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use wechat_interfaces::srv::to_yaml() instead")]]
inline std::string to_yaml(const wechat_interfaces::srv::WeChatMessage_Response & msg)
{
  return wechat_interfaces::srv::to_yaml(msg);
}

template<>
inline const char * data_type<wechat_interfaces::srv::WeChatMessage_Response>()
{
  return "wechat_interfaces::srv::WeChatMessage_Response";
}

template<>
inline const char * name<wechat_interfaces::srv::WeChatMessage_Response>()
{
  return "wechat_interfaces/srv/WeChatMessage_Response";
}

template<>
struct has_fixed_size<wechat_interfaces::srv::WeChatMessage_Response>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<wechat_interfaces::srv::WeChatMessage_Response>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<wechat_interfaces::srv::WeChatMessage_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<wechat_interfaces::srv::WeChatMessage>()
{
  return "wechat_interfaces::srv::WeChatMessage";
}

template<>
inline const char * name<wechat_interfaces::srv::WeChatMessage>()
{
  return "wechat_interfaces/srv/WeChatMessage";
}

template<>
struct has_fixed_size<wechat_interfaces::srv::WeChatMessage>
  : std::integral_constant<
    bool,
    has_fixed_size<wechat_interfaces::srv::WeChatMessage_Request>::value &&
    has_fixed_size<wechat_interfaces::srv::WeChatMessage_Response>::value
  >
{
};

template<>
struct has_bounded_size<wechat_interfaces::srv::WeChatMessage>
  : std::integral_constant<
    bool,
    has_bounded_size<wechat_interfaces::srv::WeChatMessage_Request>::value &&
    has_bounded_size<wechat_interfaces::srv::WeChatMessage_Response>::value
  >
{
};

template<>
struct is_service<wechat_interfaces::srv::WeChatMessage>
  : std::true_type
{
};

template<>
struct is_service_request<wechat_interfaces::srv::WeChatMessage_Request>
  : std::true_type
{
};

template<>
struct is_service_response<wechat_interfaces::srv::WeChatMessage_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

#endif  // WECHAT_INTERFACES__SRV__DETAIL__WE_CHAT_MESSAGE__TRAITS_HPP_
