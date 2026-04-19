// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from wechat_interfaces:srv/WeChatMessage.idl
// generated code does not contain a copyright notice

#ifndef WECHAT_INTERFACES__SRV__DETAIL__WE_CHAT_MESSAGE__BUILDER_HPP_
#define WECHAT_INTERFACES__SRV__DETAIL__WE_CHAT_MESSAGE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "wechat_interfaces/srv/detail/we_chat_message__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace wechat_interfaces
{

namespace srv
{

namespace builder
{

class Init_WeChatMessage_Request_msg_type
{
public:
  explicit Init_WeChatMessage_Request_msg_type(::wechat_interfaces::srv::WeChatMessage_Request & msg)
  : msg_(msg)
  {}
  ::wechat_interfaces::srv::WeChatMessage_Request msg_type(::wechat_interfaces::srv::WeChatMessage_Request::_msg_type_type arg)
  {
    msg_.msg_type = std::move(arg);
    return std::move(msg_);
  }

private:
  ::wechat_interfaces::srv::WeChatMessage_Request msg_;
};

class Init_WeChatMessage_Request_content
{
public:
  Init_WeChatMessage_Request_content()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_WeChatMessage_Request_msg_type content(::wechat_interfaces::srv::WeChatMessage_Request::_content_type arg)
  {
    msg_.content = std::move(arg);
    return Init_WeChatMessage_Request_msg_type(msg_);
  }

private:
  ::wechat_interfaces::srv::WeChatMessage_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::wechat_interfaces::srv::WeChatMessage_Request>()
{
  return wechat_interfaces::srv::builder::Init_WeChatMessage_Request_content();
}

}  // namespace wechat_interfaces


namespace wechat_interfaces
{

namespace srv
{

namespace builder
{

class Init_WeChatMessage_Response_message
{
public:
  explicit Init_WeChatMessage_Response_message(::wechat_interfaces::srv::WeChatMessage_Response & msg)
  : msg_(msg)
  {}
  ::wechat_interfaces::srv::WeChatMessage_Response message(::wechat_interfaces::srv::WeChatMessage_Response::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::wechat_interfaces::srv::WeChatMessage_Response msg_;
};

class Init_WeChatMessage_Response_success
{
public:
  Init_WeChatMessage_Response_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_WeChatMessage_Response_message success(::wechat_interfaces::srv::WeChatMessage_Response::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_WeChatMessage_Response_message(msg_);
  }

private:
  ::wechat_interfaces::srv::WeChatMessage_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::wechat_interfaces::srv::WeChatMessage_Response>()
{
  return wechat_interfaces::srv::builder::Init_WeChatMessage_Response_success();
}

}  // namespace wechat_interfaces

#endif  // WECHAT_INTERFACES__SRV__DETAIL__WE_CHAT_MESSAGE__BUILDER_HPP_
