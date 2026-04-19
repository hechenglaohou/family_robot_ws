// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from wechat_interfaces:srv/WeChatMessage.idl
// generated code does not contain a copyright notice
#include "wechat_interfaces/srv/detail/we_chat_message__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

// Include directives for member types
// Member `content`
// Member `msg_type`
#include "rosidl_runtime_c/string_functions.h"

bool
wechat_interfaces__srv__WeChatMessage_Request__init(wechat_interfaces__srv__WeChatMessage_Request * msg)
{
  if (!msg) {
    return false;
  }
  // content
  if (!rosidl_runtime_c__String__init(&msg->content)) {
    wechat_interfaces__srv__WeChatMessage_Request__fini(msg);
    return false;
  }
  // msg_type
  if (!rosidl_runtime_c__String__init(&msg->msg_type)) {
    wechat_interfaces__srv__WeChatMessage_Request__fini(msg);
    return false;
  }
  return true;
}

void
wechat_interfaces__srv__WeChatMessage_Request__fini(wechat_interfaces__srv__WeChatMessage_Request * msg)
{
  if (!msg) {
    return;
  }
  // content
  rosidl_runtime_c__String__fini(&msg->content);
  // msg_type
  rosidl_runtime_c__String__fini(&msg->msg_type);
}

bool
wechat_interfaces__srv__WeChatMessage_Request__are_equal(const wechat_interfaces__srv__WeChatMessage_Request * lhs, const wechat_interfaces__srv__WeChatMessage_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // content
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->content), &(rhs->content)))
  {
    return false;
  }
  // msg_type
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->msg_type), &(rhs->msg_type)))
  {
    return false;
  }
  return true;
}

bool
wechat_interfaces__srv__WeChatMessage_Request__copy(
  const wechat_interfaces__srv__WeChatMessage_Request * input,
  wechat_interfaces__srv__WeChatMessage_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // content
  if (!rosidl_runtime_c__String__copy(
      &(input->content), &(output->content)))
  {
    return false;
  }
  // msg_type
  if (!rosidl_runtime_c__String__copy(
      &(input->msg_type), &(output->msg_type)))
  {
    return false;
  }
  return true;
}

wechat_interfaces__srv__WeChatMessage_Request *
wechat_interfaces__srv__WeChatMessage_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wechat_interfaces__srv__WeChatMessage_Request * msg = (wechat_interfaces__srv__WeChatMessage_Request *)allocator.allocate(sizeof(wechat_interfaces__srv__WeChatMessage_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(wechat_interfaces__srv__WeChatMessage_Request));
  bool success = wechat_interfaces__srv__WeChatMessage_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
wechat_interfaces__srv__WeChatMessage_Request__destroy(wechat_interfaces__srv__WeChatMessage_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    wechat_interfaces__srv__WeChatMessage_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
wechat_interfaces__srv__WeChatMessage_Request__Sequence__init(wechat_interfaces__srv__WeChatMessage_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wechat_interfaces__srv__WeChatMessage_Request * data = NULL;

  if (size) {
    data = (wechat_interfaces__srv__WeChatMessage_Request *)allocator.zero_allocate(size, sizeof(wechat_interfaces__srv__WeChatMessage_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = wechat_interfaces__srv__WeChatMessage_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        wechat_interfaces__srv__WeChatMessage_Request__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
wechat_interfaces__srv__WeChatMessage_Request__Sequence__fini(wechat_interfaces__srv__WeChatMessage_Request__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      wechat_interfaces__srv__WeChatMessage_Request__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

wechat_interfaces__srv__WeChatMessage_Request__Sequence *
wechat_interfaces__srv__WeChatMessage_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wechat_interfaces__srv__WeChatMessage_Request__Sequence * array = (wechat_interfaces__srv__WeChatMessage_Request__Sequence *)allocator.allocate(sizeof(wechat_interfaces__srv__WeChatMessage_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = wechat_interfaces__srv__WeChatMessage_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
wechat_interfaces__srv__WeChatMessage_Request__Sequence__destroy(wechat_interfaces__srv__WeChatMessage_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    wechat_interfaces__srv__WeChatMessage_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
wechat_interfaces__srv__WeChatMessage_Request__Sequence__are_equal(const wechat_interfaces__srv__WeChatMessage_Request__Sequence * lhs, const wechat_interfaces__srv__WeChatMessage_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!wechat_interfaces__srv__WeChatMessage_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
wechat_interfaces__srv__WeChatMessage_Request__Sequence__copy(
  const wechat_interfaces__srv__WeChatMessage_Request__Sequence * input,
  wechat_interfaces__srv__WeChatMessage_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(wechat_interfaces__srv__WeChatMessage_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    wechat_interfaces__srv__WeChatMessage_Request * data =
      (wechat_interfaces__srv__WeChatMessage_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!wechat_interfaces__srv__WeChatMessage_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          wechat_interfaces__srv__WeChatMessage_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!wechat_interfaces__srv__WeChatMessage_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `message`
// already included above
// #include "rosidl_runtime_c/string_functions.h"

bool
wechat_interfaces__srv__WeChatMessage_Response__init(wechat_interfaces__srv__WeChatMessage_Response * msg)
{
  if (!msg) {
    return false;
  }
  // success
  // message
  if (!rosidl_runtime_c__String__init(&msg->message)) {
    wechat_interfaces__srv__WeChatMessage_Response__fini(msg);
    return false;
  }
  return true;
}

void
wechat_interfaces__srv__WeChatMessage_Response__fini(wechat_interfaces__srv__WeChatMessage_Response * msg)
{
  if (!msg) {
    return;
  }
  // success
  // message
  rosidl_runtime_c__String__fini(&msg->message);
}

bool
wechat_interfaces__srv__WeChatMessage_Response__are_equal(const wechat_interfaces__srv__WeChatMessage_Response * lhs, const wechat_interfaces__srv__WeChatMessage_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // success
  if (lhs->success != rhs->success) {
    return false;
  }
  // message
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->message), &(rhs->message)))
  {
    return false;
  }
  return true;
}

bool
wechat_interfaces__srv__WeChatMessage_Response__copy(
  const wechat_interfaces__srv__WeChatMessage_Response * input,
  wechat_interfaces__srv__WeChatMessage_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // success
  output->success = input->success;
  // message
  if (!rosidl_runtime_c__String__copy(
      &(input->message), &(output->message)))
  {
    return false;
  }
  return true;
}

wechat_interfaces__srv__WeChatMessage_Response *
wechat_interfaces__srv__WeChatMessage_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wechat_interfaces__srv__WeChatMessage_Response * msg = (wechat_interfaces__srv__WeChatMessage_Response *)allocator.allocate(sizeof(wechat_interfaces__srv__WeChatMessage_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(wechat_interfaces__srv__WeChatMessage_Response));
  bool success = wechat_interfaces__srv__WeChatMessage_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
wechat_interfaces__srv__WeChatMessage_Response__destroy(wechat_interfaces__srv__WeChatMessage_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    wechat_interfaces__srv__WeChatMessage_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
wechat_interfaces__srv__WeChatMessage_Response__Sequence__init(wechat_interfaces__srv__WeChatMessage_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wechat_interfaces__srv__WeChatMessage_Response * data = NULL;

  if (size) {
    data = (wechat_interfaces__srv__WeChatMessage_Response *)allocator.zero_allocate(size, sizeof(wechat_interfaces__srv__WeChatMessage_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = wechat_interfaces__srv__WeChatMessage_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        wechat_interfaces__srv__WeChatMessage_Response__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
wechat_interfaces__srv__WeChatMessage_Response__Sequence__fini(wechat_interfaces__srv__WeChatMessage_Response__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      wechat_interfaces__srv__WeChatMessage_Response__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

wechat_interfaces__srv__WeChatMessage_Response__Sequence *
wechat_interfaces__srv__WeChatMessage_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wechat_interfaces__srv__WeChatMessage_Response__Sequence * array = (wechat_interfaces__srv__WeChatMessage_Response__Sequence *)allocator.allocate(sizeof(wechat_interfaces__srv__WeChatMessage_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = wechat_interfaces__srv__WeChatMessage_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
wechat_interfaces__srv__WeChatMessage_Response__Sequence__destroy(wechat_interfaces__srv__WeChatMessage_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    wechat_interfaces__srv__WeChatMessage_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
wechat_interfaces__srv__WeChatMessage_Response__Sequence__are_equal(const wechat_interfaces__srv__WeChatMessage_Response__Sequence * lhs, const wechat_interfaces__srv__WeChatMessage_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!wechat_interfaces__srv__WeChatMessage_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
wechat_interfaces__srv__WeChatMessage_Response__Sequence__copy(
  const wechat_interfaces__srv__WeChatMessage_Response__Sequence * input,
  wechat_interfaces__srv__WeChatMessage_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(wechat_interfaces__srv__WeChatMessage_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    wechat_interfaces__srv__WeChatMessage_Response * data =
      (wechat_interfaces__srv__WeChatMessage_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!wechat_interfaces__srv__WeChatMessage_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          wechat_interfaces__srv__WeChatMessage_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!wechat_interfaces__srv__WeChatMessage_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
