// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from wechat_interfaces:srv/WeChatMessage.idl
// generated code does not contain a copyright notice

#ifndef WECHAT_INTERFACES__SRV__DETAIL__WE_CHAT_MESSAGE__STRUCT_HPP_
#define WECHAT_INTERFACES__SRV__DETAIL__WE_CHAT_MESSAGE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__wechat_interfaces__srv__WeChatMessage_Request __attribute__((deprecated))
#else
# define DEPRECATED__wechat_interfaces__srv__WeChatMessage_Request __declspec(deprecated)
#endif

namespace wechat_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct WeChatMessage_Request_
{
  using Type = WeChatMessage_Request_<ContainerAllocator>;

  explicit WeChatMessage_Request_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->content = "";
      this->msg_type = "";
    }
  }

  explicit WeChatMessage_Request_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : content(_alloc),
    msg_type(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->content = "";
      this->msg_type = "";
    }
  }

  // field types and members
  using _content_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _content_type content;
  using _msg_type_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _msg_type_type msg_type;

  // setters for named parameter idiom
  Type & set__content(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->content = _arg;
    return *this;
  }
  Type & set__msg_type(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->msg_type = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    wechat_interfaces::srv::WeChatMessage_Request_<ContainerAllocator> *;
  using ConstRawPtr =
    const wechat_interfaces::srv::WeChatMessage_Request_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<wechat_interfaces::srv::WeChatMessage_Request_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<wechat_interfaces::srv::WeChatMessage_Request_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      wechat_interfaces::srv::WeChatMessage_Request_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<wechat_interfaces::srv::WeChatMessage_Request_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      wechat_interfaces::srv::WeChatMessage_Request_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<wechat_interfaces::srv::WeChatMessage_Request_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<wechat_interfaces::srv::WeChatMessage_Request_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<wechat_interfaces::srv::WeChatMessage_Request_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__wechat_interfaces__srv__WeChatMessage_Request
    std::shared_ptr<wechat_interfaces::srv::WeChatMessage_Request_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__wechat_interfaces__srv__WeChatMessage_Request
    std::shared_ptr<wechat_interfaces::srv::WeChatMessage_Request_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const WeChatMessage_Request_ & other) const
  {
    if (this->content != other.content) {
      return false;
    }
    if (this->msg_type != other.msg_type) {
      return false;
    }
    return true;
  }
  bool operator!=(const WeChatMessage_Request_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct WeChatMessage_Request_

// alias to use template instance with default allocator
using WeChatMessage_Request =
  wechat_interfaces::srv::WeChatMessage_Request_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace wechat_interfaces


#ifndef _WIN32
# define DEPRECATED__wechat_interfaces__srv__WeChatMessage_Response __attribute__((deprecated))
#else
# define DEPRECATED__wechat_interfaces__srv__WeChatMessage_Response __declspec(deprecated)
#endif

namespace wechat_interfaces
{

namespace srv
{

// message struct
template<class ContainerAllocator>
struct WeChatMessage_Response_
{
  using Type = WeChatMessage_Response_<ContainerAllocator>;

  explicit WeChatMessage_Response_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  explicit WeChatMessage_Response_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : message(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->success = false;
      this->message = "";
    }
  }

  // field types and members
  using _success_type =
    bool;
  _success_type success;
  using _message_type =
    std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>>;
  _message_type message;

  // setters for named parameter idiom
  Type & set__success(
    const bool & _arg)
  {
    this->success = _arg;
    return *this;
  }
  Type & set__message(
    const std::basic_string<char, std::char_traits<char>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<char>> & _arg)
  {
    this->message = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    wechat_interfaces::srv::WeChatMessage_Response_<ContainerAllocator> *;
  using ConstRawPtr =
    const wechat_interfaces::srv::WeChatMessage_Response_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<wechat_interfaces::srv::WeChatMessage_Response_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<wechat_interfaces::srv::WeChatMessage_Response_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      wechat_interfaces::srv::WeChatMessage_Response_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<wechat_interfaces::srv::WeChatMessage_Response_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      wechat_interfaces::srv::WeChatMessage_Response_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<wechat_interfaces::srv::WeChatMessage_Response_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<wechat_interfaces::srv::WeChatMessage_Response_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<wechat_interfaces::srv::WeChatMessage_Response_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__wechat_interfaces__srv__WeChatMessage_Response
    std::shared_ptr<wechat_interfaces::srv::WeChatMessage_Response_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__wechat_interfaces__srv__WeChatMessage_Response
    std::shared_ptr<wechat_interfaces::srv::WeChatMessage_Response_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const WeChatMessage_Response_ & other) const
  {
    if (this->success != other.success) {
      return false;
    }
    if (this->message != other.message) {
      return false;
    }
    return true;
  }
  bool operator!=(const WeChatMessage_Response_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct WeChatMessage_Response_

// alias to use template instance with default allocator
using WeChatMessage_Response =
  wechat_interfaces::srv::WeChatMessage_Response_<std::allocator<void>>;

// constant definitions

}  // namespace srv

}  // namespace wechat_interfaces

namespace wechat_interfaces
{

namespace srv
{

struct WeChatMessage
{
  using Request = wechat_interfaces::srv::WeChatMessage_Request;
  using Response = wechat_interfaces::srv::WeChatMessage_Response;
};

}  // namespace srv

}  // namespace wechat_interfaces

#endif  // WECHAT_INTERFACES__SRV__DETAIL__WE_CHAT_MESSAGE__STRUCT_HPP_
