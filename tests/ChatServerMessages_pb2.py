# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: ChatServerMessages.proto

from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)




DESCRIPTOR = _descriptor.FileDescriptor(
  name='ChatServerMessages.proto',
  package='com.avxer.chat',
  serialized_pb='\n\x18\x43hatServerMessages.proto\x12\x0e\x63om.avxer.chat\"<\n\x10UserLoginRequest\x12\x12\n\nlogin_name\x18\x01 \x02(\t\x12\x14\n\x0clogin_passwd\x18\x02 \x02(\t\"\'\n\x11UserLoginResponse\x12\x12\n\nerror_code\x18\x01 \x02(\x05\"\x13\n\x11UserLogoutRequest\"R\n\x10\x43reatRoomRequest\x12\x16\n\x0emax_user_count\x18\x01 \x02(\r\x12\x11\n\troom_name\x18\x02 \x01(\t\x12\x13\n\x0broom_passwd\x18\x03 \x01(\t\"8\n\x11\x43reatRoomResponse\x12\x12\n\nerror_code\x18\x01 \x02(\x05\x12\x0f\n\x07room_id\x18\x02 \x01(\r\"8\n\x10\x45nterRoomRequest\x12\x0f\n\x07room_id\x18\x01 \x02(\r\x12\x13\n\x0broom_passwd\x18\x02 \x01(\t\"\'\n\x11\x45nterRoomResponse\x12\x12\n\nerror_code\x18\x01 \x02(\x05\";\n\x0fRoomChatRequest\x12\x0f\n\x07room_id\x18\x01 \x02(\r\x12\x17\n\x0fmessage_content\x18\x02 \x02(\x0c\"Q\n\rGroupChatNtfy\x12\x0f\n\x07room_id\x18\x01 \x02(\r\x12\x17\n\x0fmessage_content\x18\x02 \x02(\x0c\x12\x16\n\x0esender_user_id\x18\x03 \x02(\t\"5\n\x0fUserEnteredNtfy\x12\x0f\n\x07user_id\x18\x01 \x02(\t\x12\x11\n\tnick_name\x18\x02 \x02(\t\"#\n\x10LeaveRoomRequest\x12\x0f\n\x07room_id\x18\x01 \x02(\r\"!\n\x0eUserLeavedNtfy\x12\x0f\n\x07user_id\x18\x01 \x02(\t\"\x1a\n\x04Ping\x12\x12\n\ntime_stamp\x18\x01 \x02(\x04\"\x12\n\x10KeepAliveRequest\"\x13\n\x11KeepAliveResponseB\x02H\x03')




_USERLOGINREQUEST = _descriptor.Descriptor(
  name='UserLoginRequest',
  full_name='com.avxer.chat.UserLoginRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='login_name', full_name='com.avxer.chat.UserLoginRequest.login_name', index=0,
      number=1, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=unicode("", "utf-8"),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='login_passwd', full_name='com.avxer.chat.UserLoginRequest.login_passwd', index=1,
      number=2, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=unicode("", "utf-8"),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=44,
  serialized_end=104,
)


_USERLOGINRESPONSE = _descriptor.Descriptor(
  name='UserLoginResponse',
  full_name='com.avxer.chat.UserLoginResponse',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='error_code', full_name='com.avxer.chat.UserLoginResponse.error_code', index=0,
      number=1, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=106,
  serialized_end=145,
)


_USERLOGOUTREQUEST = _descriptor.Descriptor(
  name='UserLogoutRequest',
  full_name='com.avxer.chat.UserLogoutRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=147,
  serialized_end=166,
)


_CREATROOMREQUEST = _descriptor.Descriptor(
  name='CreatRoomRequest',
  full_name='com.avxer.chat.CreatRoomRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='max_user_count', full_name='com.avxer.chat.CreatRoomRequest.max_user_count', index=0,
      number=1, type=13, cpp_type=3, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='room_name', full_name='com.avxer.chat.CreatRoomRequest.room_name', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=unicode("", "utf-8"),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='room_passwd', full_name='com.avxer.chat.CreatRoomRequest.room_passwd', index=2,
      number=3, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=unicode("", "utf-8"),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=168,
  serialized_end=250,
)


_CREATROOMRESPONSE = _descriptor.Descriptor(
  name='CreatRoomResponse',
  full_name='com.avxer.chat.CreatRoomResponse',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='error_code', full_name='com.avxer.chat.CreatRoomResponse.error_code', index=0,
      number=1, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='room_id', full_name='com.avxer.chat.CreatRoomResponse.room_id', index=1,
      number=2, type=13, cpp_type=3, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=252,
  serialized_end=308,
)


_ENTERROOMREQUEST = _descriptor.Descriptor(
  name='EnterRoomRequest',
  full_name='com.avxer.chat.EnterRoomRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='room_id', full_name='com.avxer.chat.EnterRoomRequest.room_id', index=0,
      number=1, type=13, cpp_type=3, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='room_passwd', full_name='com.avxer.chat.EnterRoomRequest.room_passwd', index=1,
      number=2, type=9, cpp_type=9, label=1,
      has_default_value=False, default_value=unicode("", "utf-8"),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=310,
  serialized_end=366,
)


_ENTERROOMRESPONSE = _descriptor.Descriptor(
  name='EnterRoomResponse',
  full_name='com.avxer.chat.EnterRoomResponse',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='error_code', full_name='com.avxer.chat.EnterRoomResponse.error_code', index=0,
      number=1, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=368,
  serialized_end=407,
)


_ROOMCHATREQUEST = _descriptor.Descriptor(
  name='RoomChatRequest',
  full_name='com.avxer.chat.RoomChatRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='room_id', full_name='com.avxer.chat.RoomChatRequest.room_id', index=0,
      number=1, type=13, cpp_type=3, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='message_content', full_name='com.avxer.chat.RoomChatRequest.message_content', index=1,
      number=2, type=12, cpp_type=9, label=2,
      has_default_value=False, default_value="",
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=409,
  serialized_end=468,
)


_GROUPCHATNTFY = _descriptor.Descriptor(
  name='GroupChatNtfy',
  full_name='com.avxer.chat.GroupChatNtfy',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='room_id', full_name='com.avxer.chat.GroupChatNtfy.room_id', index=0,
      number=1, type=13, cpp_type=3, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='message_content', full_name='com.avxer.chat.GroupChatNtfy.message_content', index=1,
      number=2, type=12, cpp_type=9, label=2,
      has_default_value=False, default_value="",
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='sender_user_id', full_name='com.avxer.chat.GroupChatNtfy.sender_user_id', index=2,
      number=3, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=unicode("", "utf-8"),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=470,
  serialized_end=551,
)


_USERENTEREDNTFY = _descriptor.Descriptor(
  name='UserEnteredNtfy',
  full_name='com.avxer.chat.UserEnteredNtfy',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='user_id', full_name='com.avxer.chat.UserEnteredNtfy.user_id', index=0,
      number=1, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=unicode("", "utf-8"),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='nick_name', full_name='com.avxer.chat.UserEnteredNtfy.nick_name', index=1,
      number=2, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=unicode("", "utf-8"),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=553,
  serialized_end=606,
)


_LEAVEROOMREQUEST = _descriptor.Descriptor(
  name='LeaveRoomRequest',
  full_name='com.avxer.chat.LeaveRoomRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='room_id', full_name='com.avxer.chat.LeaveRoomRequest.room_id', index=0,
      number=1, type=13, cpp_type=3, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=608,
  serialized_end=643,
)


_USERLEAVEDNTFY = _descriptor.Descriptor(
  name='UserLeavedNtfy',
  full_name='com.avxer.chat.UserLeavedNtfy',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='user_id', full_name='com.avxer.chat.UserLeavedNtfy.user_id', index=0,
      number=1, type=9, cpp_type=9, label=2,
      has_default_value=False, default_value=unicode("", "utf-8"),
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=645,
  serialized_end=678,
)


_PING = _descriptor.Descriptor(
  name='Ping',
  full_name='com.avxer.chat.Ping',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='time_stamp', full_name='com.avxer.chat.Ping.time_stamp', index=0,
      number=1, type=4, cpp_type=4, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=680,
  serialized_end=706,
)


_KEEPALIVEREQUEST = _descriptor.Descriptor(
  name='KeepAliveRequest',
  full_name='com.avxer.chat.KeepAliveRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=708,
  serialized_end=726,
)


_KEEPALIVERESPONSE = _descriptor.Descriptor(
  name='KeepAliveResponse',
  full_name='com.avxer.chat.KeepAliveResponse',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  serialized_start=728,
  serialized_end=747,
)

DESCRIPTOR.message_types_by_name['UserLoginRequest'] = _USERLOGINREQUEST
DESCRIPTOR.message_types_by_name['UserLoginResponse'] = _USERLOGINRESPONSE
DESCRIPTOR.message_types_by_name['UserLogoutRequest'] = _USERLOGOUTREQUEST
DESCRIPTOR.message_types_by_name['CreatRoomRequest'] = _CREATROOMREQUEST
DESCRIPTOR.message_types_by_name['CreatRoomResponse'] = _CREATROOMRESPONSE
DESCRIPTOR.message_types_by_name['EnterRoomRequest'] = _ENTERROOMREQUEST
DESCRIPTOR.message_types_by_name['EnterRoomResponse'] = _ENTERROOMRESPONSE
DESCRIPTOR.message_types_by_name['RoomChatRequest'] = _ROOMCHATREQUEST
DESCRIPTOR.message_types_by_name['GroupChatNtfy'] = _GROUPCHATNTFY
DESCRIPTOR.message_types_by_name['UserEnteredNtfy'] = _USERENTEREDNTFY
DESCRIPTOR.message_types_by_name['LeaveRoomRequest'] = _LEAVEROOMREQUEST
DESCRIPTOR.message_types_by_name['UserLeavedNtfy'] = _USERLEAVEDNTFY
DESCRIPTOR.message_types_by_name['Ping'] = _PING
DESCRIPTOR.message_types_by_name['KeepAliveRequest'] = _KEEPALIVEREQUEST
DESCRIPTOR.message_types_by_name['KeepAliveResponse'] = _KEEPALIVERESPONSE

class UserLoginRequest(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _USERLOGINREQUEST

  # @@protoc_insertion_point(class_scope:com.avxer.chat.UserLoginRequest)

class UserLoginResponse(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _USERLOGINRESPONSE

  # @@protoc_insertion_point(class_scope:com.avxer.chat.UserLoginResponse)

class UserLogoutRequest(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _USERLOGOUTREQUEST

  # @@protoc_insertion_point(class_scope:com.avxer.chat.UserLogoutRequest)

class CreatRoomRequest(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _CREATROOMREQUEST

  # @@protoc_insertion_point(class_scope:com.avxer.chat.CreatRoomRequest)

class CreatRoomResponse(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _CREATROOMRESPONSE

  # @@protoc_insertion_point(class_scope:com.avxer.chat.CreatRoomResponse)

class EnterRoomRequest(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _ENTERROOMREQUEST

  # @@protoc_insertion_point(class_scope:com.avxer.chat.EnterRoomRequest)

class EnterRoomResponse(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _ENTERROOMRESPONSE

  # @@protoc_insertion_point(class_scope:com.avxer.chat.EnterRoomResponse)

class RoomChatRequest(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _ROOMCHATREQUEST

  # @@protoc_insertion_point(class_scope:com.avxer.chat.RoomChatRequest)

class GroupChatNtfy(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _GROUPCHATNTFY

  # @@protoc_insertion_point(class_scope:com.avxer.chat.GroupChatNtfy)

class UserEnteredNtfy(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _USERENTEREDNTFY

  # @@protoc_insertion_point(class_scope:com.avxer.chat.UserEnteredNtfy)

class LeaveRoomRequest(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _LEAVEROOMREQUEST

  # @@protoc_insertion_point(class_scope:com.avxer.chat.LeaveRoomRequest)

class UserLeavedNtfy(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _USERLEAVEDNTFY

  # @@protoc_insertion_point(class_scope:com.avxer.chat.UserLeavedNtfy)

class Ping(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _PING

  # @@protoc_insertion_point(class_scope:com.avxer.chat.Ping)

class KeepAliveRequest(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _KEEPALIVEREQUEST

  # @@protoc_insertion_point(class_scope:com.avxer.chat.KeepAliveRequest)

class KeepAliveResponse(_message.Message):
  __metaclass__ = _reflection.GeneratedProtocolMessageType
  DESCRIPTOR = _KEEPALIVERESPONSE

  # @@protoc_insertion_point(class_scope:com.avxer.chat.KeepAliveResponse)


DESCRIPTOR.has_options = True
DESCRIPTOR._options = _descriptor._ParseOptions(descriptor_pb2.FileOptions(), 'H\003')
# @@protoc_insertion_point(module_scope)
