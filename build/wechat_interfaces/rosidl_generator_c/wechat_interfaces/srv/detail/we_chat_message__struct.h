// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from wechat_interfaces:srv/WeChatMessage.idl
// generated code does not contain a copyright notice

#ifndef WECHAT_INTERFACES__SRV__DETAIL__WE_CHAT_MESSAGE__STRUCT_H_
#define WECHAT_INTERFACES__SRV__DETAIL__WE_CHAT_MESSAGE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'content'
// Member 'msg_type'
#include "rosidl_runtime_c/string.h"

/// Struct defined in srv/WeChatMessage in the package wechat_interfaces.
typedef struct wechat_interfaces__srv__WeChatMessage_Request
{
  /// 消息内容
  rosidl_runtime_c__String content;
  /// 消息类型: text/markdown
  rosidl_runtime_c__String msg_type;
} wechat_interfaces__srv__WeChatMessage_Request;

// Struct for a sequence of wechat_interfaces__srv__WeChatMessage_Request.
typedef struct wechat_interfaces__srv__WeChatMessage_Request__Sequence
{
  wechat_interfaces__srv__WeChatMessage_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} wechat_interfaces__srv__WeChatMessage_Request__Sequence;


// Constants defined in the message

// Include directives for member types
// Member 'message'
// already included above
// #include "rosidl_runtime_c/string.h"

/// Struct defined in srv/WeChatMessage in the package wechat_interfaces.
typedef struct wechat_interfaces__srv__WeChatMessage_Response
{
  /// 是否发送成功
  bool success;
  /// 提示信息
  rosidl_runtime_c__String message;
} wechat_interfaces__srv__WeChatMessage_Response;

// Struct for a sequence of wechat_interfaces__srv__WeChatMessage_Response.
typedef struct wechat_interfaces__srv__WeChatMessage_Response__Sequence
{
  wechat_interfaces__srv__WeChatMessage_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} wechat_interfaces__srv__WeChatMessage_Response__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // WECHAT_INTERFACES__SRV__DETAIL__WE_CHAT_MESSAGE__STRUCT_H_
